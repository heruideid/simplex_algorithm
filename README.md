# simplex_algorithm
solve linear program problem by  simplex algorithm



##### 一.约束的规范化

线性规划的约束的标准形式定义为：
$$
maxmize \sum_{j=1}^nc_jx_j\\
\sum_{j=1}^na_{ij}x_j<=b_i\ \ for\ i=1,2,..,m\\
x_j>=0\ \ for\ j=1,,,,n
$$


将线性规划的约束变为标准形式：
$$
缺少非负约束的变量x_j的处理:
x_j=x_{j1}-x_{j2},x_{j1},x_{j2}>0\\
对于等式约束\sum_{j=1}^na_{ij}x_j=b_i的处理：\sum_{j=1}^na_{ij}x_j<=b_i，-\sum_{j=1}^na_{ij}x_j<=-b_i\\
$$

##### 二.输入数据格式

代码从文件（in.txt）读入数据：

第一行有两个数m和n，表示约束个数与变量个数

之后m行，每行n+1个数，第i行前n个数为变量前的系数，最后一个为bi

最后一行n个数，代表目标函数中各变量前的系数

> 4 4 //m,n
> -1 -1 1 1 1 //-1*x1-x2+x3+x4<=1,
> 1 1 -1 -1 1
> 1 1 1 1 1
> -1 -1 -1 -1 1//
> 0.5 0.5 1 1//目标函数=0.5\*x1+0.5\*x2+x3+x4





