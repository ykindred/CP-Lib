# 计算几何常用结论

来源：`CG.md` 的「常用结论」一节（平面几何结论归档 / 立体几何结论归档）。
这些是文字性结论与公式，不是代码，故单独记在这里，不写成 `.cpp`。

## 平面几何结论归档

- `hypot` 函数可以直接计算直角三角形的斜边长；
- **边心距**是指正多边形的外接圆圆心到正多边形某一边的距离，边长为 $s$ 的正 $n$ 角形的边心距公式为 $\displaystyle a=\frac{t}{2\cdot\tan \frac{\pi}{n}}$ ，外接圆半径为 $R$ 的正 $n$ 角形的边心距公式为 $a=R\cdot \cos \dfrac{\pi}{n}$ ；
- **三角形外接圆半径**为 $\dfrac{a}{2\sin A}=\dfrac{abc}{4S}$ ，其中 $S$ 为三角形面积，内切圆半径为 $\dfrac{2S}{a+b+c}$；
- 由小正三角形拼成的大正三角形，耗费的小三角形数量即为构成一条边的小三角形数量的平方。如下图，总数量即为 $4^2$ [See](https://codeforces.com/problemset/problem/559/A)。

  <img src="https://s2.loli.net/2023/08/17/p7kRACD4cTf3YxK.png" alt="91044c3ef9c959aae5be2e7d53c13dd0.png" style="zoom:30%;" />
- 正 $n$ 边形圆心角为 $\dfrac{360^{\circ}}{n}$ ，圆周角为 $\dfrac{180^{\circ}}{n}$ 。定义正 $n$ 边形上的三个顶点 $A,B$ 和 $C$（可以不相邻），使得 $\angle ABC=\theta$ ，当 $n\le 360$ 时，$\theta$ 可以取 $1^{\circ}$ 到 $179^{\circ}$ 间的任何一个整数 [See](https://codeforces.com/problemset/problem/1096/C)。
- 某一点 $B$ 到直线 $AC$ 的距离公式为 $\dfrac{|\vec{BA}\times \vec{BC}|}{|AC|}$ ，等价于 $\dfrac{|aX+bY+c|}{\sqrt{a^2+b^2}}$。
- `atan(y / x)` 函数仅用于计算第一、四象限的值，而 `atan2(y, x)` 则允许计算所有四个象限的正反切，在使用这个函数时，需要尽量保证 $x$ 和 $y$ 的类型为整数型，如果使用浮点数，实测会慢十倍。
- 在平面上有奇数个点 $A_0,A_1,\dots,A_n$ 以及一个点 $X_0$ ，构造 $X_1$ 使得 $X_0,X_1$ 关于 $A_0$ 对称、构造 $X_2$ 使得 $X_1,X_2$ 关于 $A_1$ 对称、……、构造 $X_j$ 使得 $X_{j-1},X_j$ 关于 $A_{(j-1)\mod n}$ 对称。那么周期为 $2n$ ，即 $A_0$ 与 $A_{2n}$ 共点、$A_1$ 与 $A_{2n+1}$ 共点 [See](https://codeforces.com/contest/24/problem/C) 。
- 已知 $A\ (x_A, y_A)$ 和 $X\ (x_X,y_X)$ 两点及这两点的坐标，构造 $Y$ 使得 $X,Y$ 关于 $A$ 对称，那么 $Y$ 的坐标为 $(2\cdot x_A-x_X,2\cdot y_A-y_X)$ 。
- **海伦公式**：已知三角形三边长 $a,b$ 和 $c$ ，定义 $p=\dfrac{a+b+c}{2}$ ，则 $S_{\triangle}=\sqrt{p(p-a)(p-b)(p-c)}$ ，在使用时需要注意越界问题，本质是铅锤定理，一般多使用叉乘计算三角形面积而不使用该公式。
- 棱台体积 $V=\frac{1}{3}(S_1+S_2+\sqrt{S_1S_2})\cdot h$，其中 $S_1,S_2$ 为上下底面积。
- 正棱台侧面积 $\frac{1}{2}(C_1+C_2)\cdot L$，其中 $C_1,C_2$ 为上下底周长，$L$ 为斜高（上下底对应的平行边的距离）。
- 球面积 $4\pi r^2$，体积 $\frac{4}{3}\pi r^3$。
- 正三角形面积 $\dfrac{\sqrt 3 a^2}{4}$，正四面体面积 $\dfrac{\sqrt 2 a^3}{12}$。
- 设扇形对应的圆心角弧度为 $\theta$ ，则面积为 $S=\frac{\theta}{2}\cdot R^2$ 。

## 立体几何结论归档

- 已知向量 $\vec{r}=\{x,y,z\}$ ，则该向量的三个方向余弦为 $\cos \alpha =\dfrac{x}{|\vec r|}=\dfrac{x}{\sqrt{x^2+y^2+z^2}}; \ \cos \beta = \dfrac{y}{|\vec r|};\ \cos \gamma =\dfrac{z}{|\vec r|}$ 。其中 $\alpha,\beta,\gamma\in [0,\pi]$ ，$\cos^2\alpha+\cos^2\beta+\cos^2\gamma=1$ 。
