"""验证数学打印清单、正文转换，以及失败时不覆盖已有章节。"""
import contextlib
import io
import json
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.dont_write_bytecode = True
import gen_print as gen

REPO = Path(__file__).resolve().parent.parent


class PrintManifestTest(unittest.TestCase):
    """用最小仓库测试配置错误，用真实仓库测试收录和正文转换。"""

    def fixture(self, root):
        """传入临时目录，建立最小数学仓库，返回可修改的清单对象。"""
        files = {"Math/head.hpp": "#pragma once\n",
                 "Math/NT/_sample.hpp": "int sample();\n",
                 "Math/CG/base.hpp": "struct Point {};\n",
                 "Math/_notes.md": "# 知识点\n\n## 普通标题\n\n$x^2$\n"}
        for rel, text in files.items():
            path = root / rel
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text, encoding="utf-8")
        (root / "docs").mkdir()
        return {"schema_version": 1, "path_base": "repository_root",
                "code_entries": [
                    {"chapter": "数学", "path": "Math/head.hpp", "title": "公共头", "reference": False},
                    {"chapter": "数学", "path": "Math/NT/_sample.hpp", "title": "参考函数", "reference": True},
                    {"chapter": "计算几何", "path": "Math/CG/base.hpp", "title": "几何基础", "reference": False}],
                "prose_entries": [{"chapter": "数学", "path": "Math/_notes.md", "title": "知识点",
                                   "mode": "full_document", "heading_shift": 1}]}

    def write_manifest(self, root, manifest):
        """接收临时仓库和清单，写入生成器使用的 JSON 文件。"""
        (root / "docs/math-print-manifest.json").write_text(
            json.dumps(manifest, ensure_ascii=False), encoding="utf-8")

    def test_real_inventory(self):
        """核对真实仓库的全部数学头文件、参考标题与几何条目，不恢复旧目录。"""
        gen.configure_chapters()
        configured = [(title, rel) for chapter, folder, entries in gen.CHAPTERS
                      for title, rel in entries if folder == "." and rel.startswith("Math/")]
        expected = {p.relative_to(REPO).as_posix() for p in (REPO / "Math").rglob("*.hpp")}
        self.assertEqual({rel for _, rel in configured}, expected)
        self.assertEqual(len(configured), len(expected))
        for title, rel in configured:
            if Path(rel).name.startswith("_"):
                self.assertIn("（参考）", title)
        geometry = next(entries for title, _, entries in gen.CHAPTERS if title == "计算几何")
        self.assertEqual({rel for _, rel in geometry}, {"Math/CG/base.hpp", "Math/CG/polar_sort.hpp"})
        self.assertFalse(gen.missing_sources())
        self.assertFalse(gen.unlisted_sources())

    def test_reference_title_and_idempotence(self):
        """清单省略参考后缀时自动补齐，多次加载不会重复增加条目。"""
        with tempfile.TemporaryDirectory() as temp, patch.object(gen, "ROOT", Path(temp)):
            root = Path(temp)
            manifest = self.fixture(root)
            self.write_manifest(root, manifest)
            gen.configure_chapters()
            first = gen.CHAPTERS.copy()
            gen.configure_chapters()
            self.assertEqual(first, gen.CHAPTERS)
            self.assertIn("参考函数（参考）", str(gen.CHAPTERS))

    def test_invalid_manifests(self):
        """检查重复、缺失、越界、身份不符、遗漏正文等错误都能被明确拒绝。"""
        cases = ["duplicate", "missing", "escape", "reference", "omitted", "empty",
                 "no_notes", "duplicate_notes", "mode", "heading", "wrong_chapter", "title"]
        for case in cases:
            with self.subTest(case=case), tempfile.TemporaryDirectory() as temp, patch.object(gen, "ROOT", Path(temp)):
                root = Path(temp)
                m = self.fixture(root)
                if case == "duplicate": m["code_entries"].append(m["code_entries"][0].copy())
                if case == "missing": (root / "Math/head.hpp").unlink()
                if case == "escape": m["code_entries"][0]["path"] = "../escape.hpp"
                if case == "reference": m["code_entries"][1]["reference"] = False
                if case == "omitted": m["code_entries"].pop()
                if case == "empty": (root / "Math/head.hpp").write_text("")
                if case == "no_notes": m["prose_entries"] = []
                if case == "duplicate_notes": m["prose_entries"] *= 2
                if case == "mode": m["prose_entries"][0]["mode"] = "harvest"
                if case == "heading": m["prose_entries"][0]["heading_shift"] = -1
                if case == "wrong_chapter": m["prose_entries"][0]["chapter"] = "计算几何"
                if case == "title": m["prose_entries"][0]["title"] = ""
                self.write_manifest(root, m)
                with self.assertRaises(ValueError): gen.configure_chapters()

    @unittest.skipUnless(shutil.which("pandoc"), "需要 Pandoc 验证正文转换")
    def test_full_notes(self):
        """确认普通标题、列表、行内代码、独立公式和公式环境全部保留。"""
        with tempfile.TemporaryDirectory() as temp, patch.object(gen, "ROOT", Path(temp)):
            root = Path(temp)
            m = self.fixture(root)
            notes = "# 源标题\n\n## 普通标题\n\n- 项目含 `a_b` 和 $x^2$。\n\n$$\n\\begin{cases}x,&x>0\\\\0,&x\\le0\\end{cases}\n$$\n"
            (root / "Math/_notes.md").write_text(notes, encoding="utf-8")
            tex = "\n".join(gen.emit_full_text(m["prose_entries"][0]))
            self.assertIn("普通标题", tex)
            self.assertIn("\\subsection{知识点}", tex)
            self.assertNotIn("\\section{", tex)
            self.assertIn("\\begin{cases}", tex)
            self.assertIn("\\begin{itemize}", tex)
            self.assertIn(r"\passthrough{\lstinline!a\_b!}", tex)
            self.assertIn(r"\providecommand{\passthrough}[1]{#1}", tex)
            self.assertIn(r"\providecommand{\tightlist}", tex)
            self.assertNotIn(r"\$", tex)

    def test_missing_pandoc(self):
        """缺少 Pandoc 时明确失败，不能生成缺少知识点的章节。"""
        with patch.object(gen.shutil, "which", return_value=None):
            with self.assertRaisesRegex(RuntimeError, "pandoc"):
                gen.emit_full_text({"path": "Math/_notes.md"})

    def test_failure_keeps_existing_outputs(self):
        """正文转换失败时，原章节不被覆盖，也不产生部分新章节。"""
        with tempfile.TemporaryDirectory() as temp:
            out = Path(temp)
            target = out / "04-math.tex"
            target.write_text("keep existing")
            with patch.object(gen, "OUTDIR", out), patch.object(gen, "emit_full_text", side_effect=RuntimeError("test failure")), patch.object(sys, "argv", ["gen_print.py"]), contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(gen.main(), 1)
            self.assertEqual(target.read_text(), "keep existing")
            self.assertEqual(list(out.iterdir()), [target])

    def test_check_writes_nothing(self):
        """检查模式返回成功，且不创建输出目录。"""
        with tempfile.TemporaryDirectory() as temp:
            out = Path(temp) / "chapters"
            with patch.object(gen, "OUTDIR", out), patch.object(sys, "argv", ["gen_print.py", "--check"]), contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(gen.main(), 0)
            self.assertFalse(out.exists())


    @unittest.skipUnless(shutil.which("pandoc"), "需要 Pandoc 验证新增正文")
    def test_geometry_and_stress(self):
        """几何参考和对拍说明全文收录，代码使用 listings，脚本按 Python 打印。"""
        gen.configure_chapters()
        self.assertEqual({e["path"] for e in gen.FULL_TEXT_SOURCES},
                         {"Math/_notes.md", "Math/CG/CG.md", "Miscellaneous/stress.md"})
        for entry in gen.FULL_TEXT_SOURCES:
            if entry["path"] == "Math/_notes.md":
                continue
            tex = "\n".join(gen.emit_full_text(entry))
            self.assertIn("\\subsection{" + entry["title"] + "}", tex)
            self.assertNotIn("\\section{", tex)
            self.assertNotIn("\\begin{Shaded}", tex)
            self.assertIn("\\begin{lstlisting}", tex)
            if entry["chapter"] == "计算几何":
                self.assertIn("判断一个凸包是否完全在另一个凸包内", tex)
                self.assertEqual(tex.count("\\includegraphics"), 5)
                self.assertNotIn("\\includegraphics{https://", tex)
            else:
                self.assertIn("python3 stress.py time 2", tex)
                self.assertIn("// gen.cpp", tex)
        misc = next(c for c in gen.CHAPTERS if c[0] == "杂项")
        self.assertNotIn("Miscellaneous/hashtable.hpp", str(misc))
        body, missing, _ = gen.emit_chapter(*misc)
        self.assertFalse(missing)
        tex = "\n".join(body)
        self.assertIn("\\begin{lstlisting}[language=Python]", tex)
        self.assertIn((REPO / "Miscellaneous/stress.py").read_text().rstrip("\n"), tex)

    def test_missing_image_cache(self):
        """远程图片没有本地缓存时拒绝生成，避免产生缺图的 LaTeX。"""
        with tempfile.TemporaryDirectory() as temp, patch.object(gen, "ROOT", Path(temp)):
            with self.assertRaisesRegex(RuntimeError, "图片缓存"):
                gen.prepare_images("![](https://example.org/missing.png)")


if __name__ == "__main__":
    unittest.main()
