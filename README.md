# simplex_algorithm
solve linear program problem by  simplex algorithm



##### 一.约束的规范化

线性规划的约束的标准形式定义为：

maxmize:
  (c1\*x1+...cn\*xn)
constraints:
  a11\*x1+...+a1n\*xn<=b1
  .
  .
  .
  am1\*x1+...+amn\*xn<=bm


将线性规划的约束变为标准形式：
  缺少非负约束的变量xj的处理:
xj=xj1-xj2 , xj1,xj2>0
对于等式约束a11\*x1+...+a1n\*xn==b1的处理：a11\*x1+...+a1n\*xn<=b1 和 -(a11\*x1+...+a1n\*xn)<=-b1


##### 二.输入数据格式

代码从文件（in.txt）读入数据：

第一行有两个数m和n，表示约束个数与变量个数

之后m行，每行n+1个数，第i行前n个数为变量前的系数，最后一个为bi

最后一行n个数，代表目标函数中各变量前的系数


 4 4 //m,n
 -1 -1 1 1 1 //-1*x1-x2+x3+x4<=1,
 1 1 -1 -1 1
 1 1 1 1 1
 -1 -1 -1 -1 1//
 0.5 0.5 1 1//目标函数=0.5\*x1+0.5\*x2+x3+x4





