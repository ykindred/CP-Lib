# 数学补充模板索引

这里收录了 24 份待审核的下划线头文件：新建 21 份，更新原有参考模板 3 份。它们仍保留待审核标记。正式模板没有改动。

每份头文件都可以在 C++17 下单独包含。只需包含实际要用的文件，再按接口传入数据。参数类型 `Z` 通常使用现有的 `mint`；需要指定其他模数时，可以使用 `ModInt<P>`。大部分函数可以从实参推导元素类型。

以下索引用于查找入口。每个函数的用途、参数、返回值和条件，均在对应头文件中说明。推导与常见用法见 [中文知识点](_notes.md)。

## 代数、数值积分与博弈

| 文件 | 主要接口与用法 |
|---|---|
| [矩阵](Alg/_Matrix.hpp) | 用 `Matrix<mint> a(n, m)` 建立矩阵，`a[i][j]` 访问元素；支持加减、矩阵乘法、乘列向量，以及 `a.pow(k)`。幂运算要求方阵且 $k\ge0$。 |
| [异或线性基](Alg/_linear_basis.hpp) | 用 `LinearBasis<> b` 建立空线性基；`insert` 插入，`contains` 判断能否表示，`max_xor`、`min_xor` 查询最值，`merge` 合并。`kth` 从零编号，越界返回 `nullopt`。 |
| [行列式](Alg/_determinant.hpp) | `determinant(a)` 返回方阵的行列式；空矩阵返回一。通常使用质数模 `mint`。 |
| [矩阵求逆](Alg/_matrix_inverse.hpp) | `auto [ok, b] = matrix_inv(a)`。`ok` 为真时，`b` 为逆矩阵；否则 `b` 为空。通常使用质数模 `mint`。 |
| [异或高斯消元](Alg/_gauss_xor.hpp) | `gauss_xor(a, m)` 接收 `vector<bitset<M>>`。前 `m` 位为变量系数，第 `m` 位为常数，返回秩与一个特解；无解时秩为 `-1`。 |
| [矩阵树定理](Alg/_kirchhoff.hpp) | `spanning_trees(n, edges)` 用于无向图；`arborescences(n, edges, root)` 用于每个点最终走向根的有向树。点从零编号，每条边写作 `{u, v, w}`。 |
| [自适应 Simpson](Alg/_integrate.hpp) | `integrate(f, l, r, eps, depth)` 返回积分估计值。参数与返回值使用 `long double`，最后两个参数可省略。 |
| [Nim 与 SG](Alg/_sg.hpp) | `nim_win(a)` 判断普通 Nim；`misere_nim_win(a)` 判断拿走最后一颗者输的 Nim。`sg_dag(g)` 返回无环局面图各点的 SG 值。 |

## 数论

| 文件 | 主要接口与用法 |
|---|---|
| [Pollard-Rho](NT/_pollard_rho.hpp) | `rho::prime_factor(x)` 返回按质数递增排列的分解，每项为 `{质数, 指数}`；`rho::factor(x)` 返回递增排列的所有正约数。输入必须为正整数。 |
| [由分解生成约数](NT/_divisors.hpp) | `divisors(fac)` 接收质因数分解并返回递增约数；可以使用现有试除分解的结果。 |
| [莫比乌斯函数与欧拉函数筛](NT/_mobius.hpp) | `MobiusSieve s(n)` 预处理到正上限 `n`。`primes` 存质数，`minp` 存最小质因子，`mu`、`phi` 存函数值，`smu`、`sphi` 存对应前缀和。 |
| [杜教筛](NT/_dujiao_sieve.hpp) | `DuJiao d(B)` 先预筛到 `B`。`d.getsmu(n)` 返回莫比乌斯函数前缀和，`d.getsphi(n)` 返回欧拉函数前缀和，后者使用 `i128`。 |
| [数论分块](NT/_quotient_blocks.hpp) | `quotient_blocks(n, work)` 对每段调用 `work(l, r, q)`。`quotient_sum(n, m, prefix)` 返回带权的两个整数商乘积之和，结果类型与前缀和元素类型相同。 |
| [整除关系变换](NT/_divisor_transform.hpp) | `divisor_zeta(a, invert)` 求约数和变换，`multiple_zeta(a, invert)` 求倍数和变换；`invert` 为真时求逆。`convolution_gcd`、`convolution_lcm` 返回对应卷积。数组下标从一开始。 |
| [模平方根](NT/_mod_sqrt.hpp) | `mod_sqrt(a, p)` 要求 `p` 为质数，返回较小的非负根，无解返回 `-1`。 |
| [exLucas](NT/_exlucas.hpp) | `ExLucas f(mod)` 指定正 `int` 模数，`f.nCr(n, r)` 查询组合数。会预处理完整素数幂表，使用前应确认内存足够。 |

## 多项式、插值与递推

| 文件 | 主要接口与用法 |
|---|---|
| [BM 与 Kitamasa](Poly/_linear_recurrence.hpp) | `berlekamp_massey(a)` 恢复递推系数；`linear_nth(a, c, n)` 根据已知递推求项；`linear_guess(a, n)` 将两者组合。猜出的递推是否对后续成立，需要另外保证。 |
| [Bostan-Mori](Poly/_bostan_mori.hpp) | `bostan_mori(p, q, n)` 返回 $[x^n]P(x)/Q(x)$；`linear_nth_ntt(a, c, n)` 使用同样方法加速线性递推。 |
| [连续点插值与幂和](Poly/_lagrange.hpp) | `lagrange(y, x)` 接收 $f(0),\ldots,f(k)$，返回 $f(x)$。`power_sum<mint>(n, k)` 返回前 `n` 个正整数的 `k` 次幂之和。 |
| [Taylor shift](Poly/_taylor_shift.hpp) | `taylor_shift(a, mint(c))` 接收 $f(x)$ 的升幂系数，返回 $f(x+c)$ 的升幂系数。 |
| [子集卷积](Poly/_subset_convolution.hpp) | `subset_convolution(a, b)` 接收等长的集合数组，长度为 $2^k$；返回只合并不相交集合的卷积。时间为 $O(k^2 2^k)$，使用前应确认内存足够。 |

递推系数统一为

$$
a_i=\sum_{j=0}^{k-1}c_j a_{i-j-1}.
$$

也就是 `c[0]` 对应最近一项，项编号从零开始。Kitamasa 只需加减乘，可以用于合数模。BM 需要域上的除法。Bostan-Mori 和 Taylor shift 使用现有 NTT，必须满足对应的模数、原根和长度条件。

## 组合计数

| 文件 | 主要接口与用法 |
|---|---|
| [常见计数](Comb/_numbers.hpp) | `derangements`、`partition_numbers`、`bell_numbers` 分别返回错排数、整数拆分数、Bell 数的前缀数组。`catalan`、`ballot`、`multinomial` 和 `tree_degree_count` 接收现有 `Factorial` 对象；`cayley` 计算标号树总数。 |
| [Burnside、项链与手链](Comb/_burnside.hpp) | `burnside(group, mint(q))` 接收完整置换群；`necklaces(n, mint(q))` 计算项链，`bracelets(n, mint(q))` 计算手链。平均公式的分母必须在模意义下可逆。 |
| [Prüfer 编解码](Comb/_prufer.hpp) | `prufer_encode(g)` 接收树的邻接表；`prufer_decode(a)` 返回无向边列表。点从零编号，至少两个点，空序列对应两个点的树。 |

## 调用示例

下面两种方法计算同一个 Fibonacci 项：

```cpp
#include "NT/modint.hpp"
#include "Poly/_linear_recurrence.hpp"
#include "Poly/_bostan_mori.hpp"

int main() {
    vector<mint> a{0, 1}, c{1, 1};
    auto f1 = linear_nth(a, c, 1'000'000'000'000LL);
    auto f2 = linear_nth_ntt(a, c, 1'000'000'000'000LL);
    assert(f1 == f2);
}
```

下面使用莫比乌斯函数前缀和，统计两个区间中的互质有序数对：

```cpp
#include "NT/_mobius.hpp"
#include "NT/_quotient_blocks.hpp"

int main() {
    MobiusSieve s(1'000'000);
    vector<i128> prefix(s.smu.begin(), s.smu.end());
    i128 pairs = quotient_sum(12345, 67890, prefix);
    cout << (ll)pairs << '\n';
}
```

## 实现选择与验证

实现时对比了 KACTL、Nyaan、ei1333 和 ACL 的相关代码，参考链接见知识点文档末节。接口使用独立函数和必要的预处理结构体，并复用现有 `ModInt` 与 `convolution`。

这些模板已完成以下本地验证：

- 全部 24 份头文件分别通过 C++17 编译，也通过混合、重复包含检查。
- 使用固定随机种子 `1026` 运行对拍；优化编译版本、地址检查和未定义行为检查版本均通过。
- 行列式对照排列展开；逆矩阵检查左右乘积；异或消元枚举小规模解；线性基枚举所有不同异或值，并检查完整 64 位线性基的查询边界。
- 无向和有向矩阵树定理对照小图枚举；Prüfer 编解码检查往返结果。
- 莫比乌斯函数、欧拉函数和杜教筛对照直接计算或筛表；数论分块对照直接求和；两种整除卷积对照双重枚举。
- exLucas 检查模数从一到二百、上限不超过一百的全部组合数，并用独立约分方法检查很大上限、小选取数的情况。模平方根检查小素数的全部剩余类和接近 `ll` 上界的素数。Pollard-Rho 检查小整数、大半素数、素数幂和 `LLONG_MAX`。
- 三种递推算法对照直接递推；插值对照 Horner 求值；Taylor shift 检查代入恒等式；子集卷积对照枚举子集。
- 各类计数对照排列枚举、路径动态规划、背包或独立递推；项链和手链对照染色等价类枚举。Nim 对照状态搜索，SG 检查递推定义。积分检查多项式、三角函数、指数函数、尖峰函数、反向区间和零长区间。

本轮只调整文档与注释。验证记录不等同于在线评测通过记录；随机算法的耗时、数值积分的精度仍取决于输入。
