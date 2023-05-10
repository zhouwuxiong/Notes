## Matrix class

## 矩阵和向量运算
```auto

```
** Eigen 中变量的类型和长度是在编译期间就已经确定的，因此运算速度更快 **
** Eigen 表达式的计算不是立即进行的，而是等到执行写操作时（例如：=）才进行计算，这样可以允许编辑器做更多的优化**
** 由于 Eigen 是复制时计算,因此 `a=a.tanspose()` 的写法是错误的，会产生 `alias issue` 问题，正确的写法是使用`InPlace`函数，例如：`a=a.transposeInPlace()` **

类别|头文件|运算|函数|描诉|示例
-|-|-|-|-|-
基本运算| <Eigen/Core> |+ - * / | + - * /|+ - 可以用于矩阵，* / 只能用与常数和矩阵 |
变换| -|<Eigen/Core> | transpose(), conjugate()|-|
矩阵运算|<Eigen/Dense>| -|v.dot(w)，v.cross(w)，adjoint()|-|
reduction operations| <Eigen/Core> |-|sum() ,prod(),mean()，minCoeff()，maxCoeff()，trace() |-|auto min = a.minCoeff(i,j);返回a的最小元素赋值给min，并将最小元素所在行号、列号赋值给i、j

## Array class and coefficient-wise operations
Matrix 主要用于线性代数相关的矩阵运算，Arrays 主要用于系数运算。例如：
```C++
  Eigen::ArrayXXf a(3,3);
  Eigen::ArrayXXf b(3,3);
  a * b  // 对应元素相乘

  Eigen::MatrixXXf a(3,3);
  Eigen::MatrixXXf b(3,3);
  a * b // 矩阵相乘
  ```
### Converting between array and matrix expressions
- .array() and .matrix() 可以进行 Array 和 Matrix 之间的相互转换，this doesn't have any runtime cost。 
- Both .array() and .matrix() can be used as rvalues and as lvalues.
- Mixing matrices and arrays in an expression is forbidden with Eigen.

## Block operations

```C++
// 1. 基本运算 + - * /
// 2. transpose and inverse
// 2. 点乘

```