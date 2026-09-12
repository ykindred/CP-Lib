# latexmk 配置：CP-Lib 打印版
# 用法（仓库根目录）：latexmk print/main.tex

# 5 = 用 xelatex 生成 PDF（中文必需）
$pdf_mode = 5;

# 产物叫 CP-Lib.pdf 而不是 main.pdf
$jobname = 'CP-Lib';

# 强制切到源文件所在目录再编译，
# 这样 \input{preamble.tex} 和 assets/ 相对路径才能解析
$do_cd = 1;

$xelatex = 'xelatex -interaction=nonstopmode -file-line-error -synctex=1 %O %S';

# 目录/页码/引用需要多趟编译，latexmk 会自动判断跑几趟
$max_repeat = 5;

$clean_ext = 'aux log out toc lof lot fls fdb_latexmk synctex.gz bbl blg run.xml';
