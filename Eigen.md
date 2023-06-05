# Matrix class

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
-`*` 在 Matrix 中是矩阵相乘,而在 Array 中是对应元素相乘

## Block operations
```c++
matrix.block(i,j,p,q);
matrix.topLeftCorner(p,q);
matrix.topRows(q);
matrix.middleCols(i,q);
```

## Basic slicing
```C++
seq(firstIdx,lastIdx) 
seq(firstIdx,lastIdx,incr) // incr 步长
seqN(firstIdx,size)   // size 元素个数
seqN(firstIdx,size,incr) 
```

## Advanced initialization  
```C++
Array33f a1 = Array33f::Zero();
ArrayXf a2 = ArrayXf::Zero(3);
ArrayXXf a3 = ArrayXXf::Zero(3, 4);

ArrayXXf table(10, 4);
table.col(0) = ArrayXf::LinSpaced(10, 0, 90);
table.col(1) = M_PI / 180 * table.col(0);
table.col(2) = table.col(1).sin();
table.col(3) = table.col(1).cos();

// Usage as temporary objects
MatrixXd m = MatrixXd::Random(3,3);
m = (m + MatrixXd::Constant(3,3,1.2)) * 50;

mat = (MatrixXf(2,2) << 0, 1, 1, 0).finished() * mat;
```

## Reductions, visitors and broadcasting
### reductions
```C++
mat.sum()
mat.prod()  // 所有元素乘积
mat.mean()
mat.minCoeff()  // 最小元素
mat.maxCoeff()
mat.trace() // 主对角线元素和
```

```C++
v.squaredNorm() // 各元素平方和
v.norm()  // sqrt(squaredNorm())
v.lpNorm<p>() // 求lp范数,例如:L0 -> 非0元素个数, L1 元素绝对值和, L2 -> 元素平方和的平方根,等价于 Norm() 
// 无穷范数：正无穷范数：所有元素中绝对值最小的。负无穷范数：所有元素中绝对值最大的。
m.lpNorm<Eigen::Infinity>() // 负无穷范数
``` 
```C++
  // Boolean reductions
  std::cout << "(a > 0).all()   = " << (a > 0).all() << std::endl;
  std::cout << "(a > 0).any()   = " << (a > 0).any() << std::endl;
  std::cout << "(a > 0).count() = " << (a > 0).count() << std::endl;
```
###  Visitors
```C++
  //get location of maximum
  Eigen::Index maxRow, maxCol;
  float max = m.maxCoeff(&maxRow, &maxCol);
 
  //get location of minimum
  Eigen::Index minRow, minCol;
  float min = m.minCoeff(&minRow, &minCol);
```
###  Partial reductions
```C++
  std::cout << "Column's maximum: " << std::endl
   << mat.colwise().maxCoeff() << std::endl;

  float maxNorm = mat.colwise().sum().maxCoeff(&maxIndex);  // 寻找元素和最大的列
  std::cout << mat.col( maxIndex ) << std::endl;
```
###  Broadcasting
```C++
  Eigen::MatrixXf mat(2,4);
  Eigen::VectorXf v(4);
  
  mat << 1, 2, 6, 9,
         3, 1, 7, 2;
         
  v << 0,1,2,3;
       
  //add v to each row of m
  mat.rowwise() += v.transpose();
```

```C++
  Eigen::MatrixXf m(2,4);
  Eigen::VectorXf v(2);
  
  m << 1, 23, 6, 9,
       3, 11, 7, 2;
       
  v << 2,
       3;
 
  Eigen::Index index;
  // find nearest neighbour
  (m.colwise() - v).colwise().squaredNorm().minCoeff(&index);
```

## Reshape
Eigen::Reshaped() 默认为列优先,即先排完第一列再排第二列...
m.reshaped<RowMajor>() 以行优先排列,先排第一行再排第二行...
*Reshaped() 可以指定形状,如: Reshaped(2,3),不给参数时所有元素变为1列*

https://eigen.tuxfamily.org/dox/group__TutorialReshape.html

## STL iterators and algorithms
```C++
Array4i v = Array4i::Random().abs();
cout << "Here is the initial vector v:\n" << v.transpose() << "\n";
std::sort(v.begin(), v.end());


Matrix2i A = Matrix2i::Random();
cout << "Here are the coeffs of the 2x2 matrix A:\n";
for(auto x : A.reshaped())
  cout << x << " ";

ArrayXXi A = ArrayXXi::Random(4,4).abs();
cout << "Here is the initial matrix A:\n" << A << "\n";
for(auto row : A.rowwise())
  std::sort(row.begin(), row.end());
```

```C++
int data[] = {1,2,3,4,5,6,7,8,9};
Map<RowVectorXi> v(data,4);
cout << "The mapped vector v is: " << v << "\n";
new (&v) Map<RowVectorXi>(data+4,5);  // 这里的new 不会进行内存(allocator),应为new给定了一个存在的地址
cout << "Now v is: " << v << "\n";

```

## Map
有时候你希望定预定义一个数组,然后在 Eigen 的 vector 和 Matrix 中使用它,一种简单的方法是元素复制.但是通常你希望 Eigen 可以重复使用预定义的内存,这时 Eigen::Map 可以很容易实现, Eigen::Map 可以将一个数组和一个现有的Eigen对象组织成一个 Map 对象, Map 对象与原始数据占用一块内存,因此对 Map 对象的数据修改会直接作用于原始数据.或将顺序容器中的元素（或者说是一段连续内存）表达成eigen中矩阵类型如Matrix或者Vector

```C++
int array[8];
for(int i = 0; i < 8; ++i) array[i] = i;
cout << "Column-major:\n" << Map<Matrix<int,2,4> >(array) << endl;
cout << "Row-major:\n" << Map<Matrix<int,2,4,RowMajor> >(array) << endl;
cout << "Row-major using stride:\n" <<
  Map<Matrix<int,2,4>, Unaligned, Stride<1,4> >(array) << endl;


int array[8];
for(int i = 0; i < 8; ++i) array[i] = i;
cout << "Column-major:\n" << Map<Matrix<int,2,4> >(array) << endl;
cout << "Row-major:\n" << Map<Matrix<int,2,4,RowMajor> >(array) << endl;
cout << "Row-major using stride:\n" <<
  Map<Matrix<int,2,4>, Unaligned, Stride<1,4> >(array) << endl;
```
[Eigen::Map使用](https://blog.csdn.net/qq_35590091/article/details/97135512)

## Aliasing
Eigen 的别名是指同一个 Matrix 或 Vector 同时出现在赋值语句的左边和右边. 例如: mat = 2 * mat; or mat = mat.transpose().这回使 Eigen 的计算产生混叠,因为 Eigen 使用了惰性计算的策略,只有在写数据时才会产生计算,而只有在计算产生时才会去读数据,而当混叠产生时 Eigen 计算读取的数据可能已经被上一次计算更改,从而导致产生与预期不一致的结果,甚至产生程序崩溃.例如: `mat = mat.transpose()`.

Eigen 可以在编译时检测大部分 Alias 问题,但是有少部分的 Alias 问题会在 runtiem assert 中检测到.用户可以通过定义`EIGEN_NO_DEBUG`宏来关闭 runtime assert

###  Resolving aliasing issues
为了解决 alias 问题,最简单的方式就是先缓存右边的计算结果,然后再赋值. Eigen 中的 eval() 可以很容易实现这个功能.例如:
```C++
mat.bottomRightCorner(2,2) = mat.topLeftCorner(2,2).eval();
mat = mat.transpose().eval()
```
eval() 主要用于缓存一个表达式的结果,从而达到延迟计算减少不必要计算量的目的.

需要注意的是 eval() 总用于普通的 Matix 或 Vector 时只会返回原对象的引用,以避免无效的数据拷贝.而且尽量不要将 eval() 与 C++ 的 auto 关键字一起使用.参考 [ Compilation error with template methods](https://eigen.tuxfamily.org/dox/TopicPitfalls.html#TopicPitfalls_auto_keyword).

简而言之.就是 auto 关键字变量中存储的是一个计算表达式而不是结算结果.这导致每次在使用 auto 关键字时都需要对表达式进行计算.例如:
```C++
MatrixXd A, B;
auto C = A*B;
for(...) { ... w = C * v;  ...}
```
更严重的是,如果表达式中存储了临时变量,临时变量的生存空间为当前代码行,因此如果 auto 的表达式使用了临时变量,则在后续的计算中会导致引用无效内存.如:
```C++
auto C = ((A+B).eval()).transpose();  //eval() 生成了临时变量, transpose() 引用了临时变量
// do something with C

// 解决方法
auto C = (A+B).transpose().eval();
```
### Aliasing and component-wise operations
虽然 Alias 问题可能产生严重的程序错误,但是对于某些操作却是安全的.例如:矩阵加法、矩阵与标量的乘法、Array的乘法（对应元素相乘）等。因为这些元素的的第（i,j）元素的结果只与输入中的第（i,j）个元素有关。

###  Aliasing and matrix multiplication
矩阵乘法是唯一一个 Eigen 默认认为产生别名的运算符。例如：
```C++
matA = matA * matA；
```
但是这会产生一个性能问题，因为 Eigen 会先将计算存储到一个临时变量，然后再将临时变量赋值给 matA.而且当存储结果的矩阵不是 matA 时也会执行这个操作。例如：
```C++
matB = matA * matA；
```
这显然是没有必要的，你可以通过 noalias() 函数显示的表明这里不会产生别名问题。例如：
```C++
matB.noalias() = matA * matA
```
这样 Eigen 会将结果直接存入 matB。

Moreover, starting in Eigen 3.3, aliasing is not assumed if the destination matrix is resized and the product is not directly assigned to the destination. Therefore, the following example is also wrong:
```C++
MatrixXf A(2,2), B(3,2);
B << 2, 0,  0, 3, 1, 1;
A << 2, 0, 0, -2;

A = (B * A).cwiseAbs(); // wrong
A = (B * A).eval().cwiseAbs(); // right
cout << A;
```
## Storage orders
### Column-major and row-major storage
Eigen 进行矩阵存储时，有行优先和列优先两种方式，其决定了矩阵在内存中铺设的方式。Eigen 默认是行优先铺设的方式。可以通过 Options 模板参数进行更改.两种排列方式在 Eigen 可以混合使用。
```C++
Matrix<Scalar, RowsAtCompileTime and ColsAtCompileTime,Options, MaxRowsAtCompileTime and MaxColsAtCompileTime>

Matrix<int, 3, 4, ColMajor> Acolmajor;
```
###  Which storage order to choose?
- 如果你使用的其它库对排列方式有特殊要求
- 当你的算法需要逐行遍历时，行优先的排列方式读取更快
- Eigen 虽然提供了两种可选的排列方式，但是所有基于 Eigen 的开发工作可能都是基于列优先的。

## Alignment issues


[**Quick reference guide**](https://eigen.tuxfamily.org/dox/group__QuickRefPage.html)


# Dense linear problems and decompositions
## Linear algebra and decompositions
###  Least squares solving

###  Checking if a matrix is singular

###  Computing eigenvalues and eigenvectors
计算特征值分解时需要保证矩阵为自伴随矩阵。自伴随矩阵主**对角线**上的元素都是实数的，其**特征值**也是实数。如果 D 是**特征值**在**对角线**上的对角矩阵， V 是以**特征向量为列的矩阵**，则 a=V D V^{-1} （对于自伴矩阵，矩阵 V 总是可逆的），这称为**特征分解**。
特征值和特征向量的计算并不一定收敛，但是不收敛的情况是很罕见的。你可以通过 info() 来判断这种情况发生。

[](https://zhuanlan.zhihu.com/p/550779050)
```C++
#include <iostream>
#include <Eigen/Dense>
 
int main()
{
   Eigen::Matrix2f A;
   A << 1, 2, 2, 3;
   std::cout << "Here is the matrix A:\n" << A << std::endl;
   Eigen::SelfAdjointEigenSolver<Eigen::Matrix2f> eigensolver(A);
   if (eigensolver.info() != Eigen::Success) abort();
   std::cout << "The eigenvalues of A are:\n" << eigensolver.eigenvalues() << std::endl;
   std::cout << "Here's a matrix whose columns are eigenvectors of A \n"
        << "corresponding to these eigenvalues:\n"
        << eigensolver.eigenvectors() << std::endl;
}
```

###  Computing inverse and determinant
逆矩阵和行列式虽然是基本的，但是在数值线性代数中其作用不大。*逆计算通常有利地由solve（）操作代替，并且行列式通常不是检查矩阵是否可逆的好方法*。

###  Separating the computation from the construction
矩阵分解通常在构造时就进行了计算，但是在有些情况下你希望将构造和计算分开。例如：在构造时你并不清楚需要分解的矩阵（矩阵中没有填值），或者你想重复使用已有的  decomposition object.
```C++
   Eigen::Matrix2f A, b;
   Eigen::LLT<Eigen::Matrix2f> llt; // 默认构造函数
   A << 2, -1, -1, 3;
   b << 1, 2, 3, 1;
   std::cout << "Here is the matrix A:\n" << A << std::endl;
   std::cout << "Here is the right hand side b:\n" << b << std::endl;
   std::cout << "Computing LLT decomposition..." << std::endl;
   llt.compute(A);  //计算
   std::cout << "The solution is:\n" << llt.solve(b) << std::endl;
   A(1,1)++;
   std::cout << "The matrix A is now:\n" << A << std::endl;
   std::cout << "Computing LLT decomposition..." << std::endl;
   llt.compute(A);
   std::cout << "The solution is now:\n" << llt.solve(b) << std::endl;
```
同时，你也可以在分解器构造时指定矩阵的大小，这样在计算时就不会再进行动态内存分配。（of course, if you are using fixed-size matrices, no dynamic memory allocation happens at all）
```C++
HouseholderQR<MatrixXf> qr(50,50);
MatrixXf A = MatrixXf::Random(50,50);
qr.compute(A); // no dynamic memory allocation
```

###  Rank-revealing decompositions
有些分解器在分解时，可以计算出矩阵的秩。这些分解器更适用于分解不满秩矩阵。同时 Rank-revealing decompositions 会提供一些便利的函数。如：是否可逆、求零空间、求列空间等。
```C++
#include <iostream>
#include <Eigen/Dense>
 
int main()
{
   Eigen::Matrix3f A;
   A << 1, 2, 5,
        2, 1, 4,
        3, 0, 3;
   std::cout << "Here is the matrix A:\n" << A << std::endl;
   Eigen::FullPivLU<Eigen::Matrix3f> lu_decomp(A);
   std::cout << "The rank of A is " << lu_decomp.rank() << std::endl;
   std::cout << "Here is a matrix whose columns form a basis of the null-space of A:\n"
        << lu_decomp.kernel() << std::endl;
   std::cout << "Here is a matrix whose columns form a basis of the column-space of A:\n"
        << lu_decomp.image(A) << std::endl; // yes, have to pass the original A
}
```
需要注意的是，矩阵的秩会受到阈值的影响，浮点数矩阵并没有绝对秩亏的（0通常表示为极小数），Eigen 会根据分解器和对角线元素*一个极小数来选取一个默认阈值。同时你也可以通过 setThreshold() 来设定合理的阈值。阈值的设定不会影响 compute() 的结果，不需要再重新计算分解。
```C++
#include <iostream>
#include <Eigen/Dense>
 
int main()
{
   Eigen::Matrix2d A;
   A << 2, 1,
        2, 0.9999999999;
   Eigen::FullPivLU<Eigen::Matrix2d> lu(A);
   std::cout << "By default, the rank of A is found to be " << lu.rank() << std::endl;
   lu.setThreshold(1e-5);
   std::cout << "With threshold 1e-5, the rank of A is found to be " << lu.rank() << std::endl;
}
```
## Catalogue of dense decompositions
[Catalogue of dense decompositions](https://eigen.tuxfamily.org/dox/group__TopicLinearAlgebraDecompositions.html)

## Solving linear least squares systems
Eigen 提供了三种方式来求解一个线性最小二乘法问题，分别是 SVD 分解、QR 分解和正规方程（normal equations）。the SVD decomposition is generally the most accurate but the slowest, normal equations is the fastest but least accurate, and the QR decomposition is in between.
###  Using the SVD decomposition
```C++
#include <iostream>
#include <Eigen/Dense>
 
int main()
{
   Eigen::MatrixXf A = Eigen::MatrixXf::Random(3, 2);
   std::cout << "Here is the matrix A:\n" << A << std::endl;
   Eigen::VectorXf b = Eigen::VectorXf::Random(3);
   std::cout << "Here is the right hand side b:\n" << b << std::endl;
   std::cout << "The least-squares solution is:\n"
        << A.template bdcSvd<Eigen::ComputeThinU | Eigen::ComputeThinV>().solve(b) << std::endl;
}
```
这是线性代数和分解的例子。如果您只需要解决最小二乘问题，但对SVD本身不感兴趣，则更快的替代方法是CompleteOrthogonalDecomposition。
###  Using the QR decomposition
QR分解类中的solve（）方法也计算最小二乘解。有三个QR分解类：HouseholderQR（无旋转，快速但不稳定，如果你的矩阵不是rull rank），ColPivHouseholderQR（列旋转，因此有点慢，但更稳定）和FullPivHouseholderQR（全旋转，所以最慢，比ColPivHouseholderQR稍微稳定）。下面是一个列旋转的例子：
```C++
MatrixXf A = MatrixXf::Random(3, 2);
VectorXf b = VectorXf::Random(3);
cout << "The solution using the QR decomposition is:\n"
     << A.colPivHouseholderQr().solve(b) << endl;
```
###  Using normal equations
Finding the least squares solution of Ax = b is equivalent to solving the normal equation ATAx = ATb. This leads to the following code
```C++
MatrixXf A = MatrixXf::Random(3, 2);
VectorXf b = VectorXf::Random(3);
cout << "The solution using normal equations is:\n"
     << (A.transpose() * A).ldlt().solve(A.transpose() * b) << endl;
```
这种方法通常是最快的，尤其是当A“又高又瘦”的时候。然而，如果矩阵A甚至是轻度病态的，这不是一个好方法，因为ATA的条件数是A的条件数的平方。这意味着与上面提到的更稳定的方法相比，使用法方程会损失大约两倍的精度。


*矩阵的条件数可用于描述线性系统对扰动的敏感性，从而能衡量线性系统的稳定性。如果线性系统的条件数较大，则线性系统变量b即使仅发生微小变化，都会给线性系统的解带来剧烈变化，对于任意矩阵A，其条件数为: $cond(A)=||A|||A^{-1}||$*，其中 ||A|| 为L1范数。
[代数基础｜矩阵的条件数](https://zhuanlan.zhihu.com/p/48580830)

条件数是区分矩阵“病态”的指标之一，条件数在1附近的问题称为“良态”问题，条件数高的问题称为“病态”问题，条件数趋于无穷大的问题为“奇异”问题。
[矩阵奇异性和矩阵‘病态’问题详细描述](https://zhuanlan.zhihu.com/p/360222065)

## Inplace matrix decompositions
Eigen 的某些分解器可以将进行原地分解操作，其分解的结果存储在输入矩阵的内存中，这在处理非常巨大的矩阵和内存紧缺的环境中非常有用。To this end，你需要在构建分解器时传入输入矩阵作为参数。需要注意的是，由于分解器的结果与输入共用内存，对输入矩阵的修改会导致分解的结果失效。

[支持原地分解的分解器](https://eigen.tuxfamily.org/dox/group__InplaceDecomposition.html)

## Benchmark of dense decompositions
[Eigen 分解器的效率对比](https://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html)

# Sparse linear algebra
###  Sparse matrix manipulations
在实际应用中，矩阵往往只包含很少的非0元素。为了节省存储空间，Eigen 为系数矩阵提供了特殊的存储方式`SparseMatrix `。主要包含4个部分：
- Values ：以列优先存储非0值
- InnerIndices ：values 中的非0值对应的行数
- OuterStarts ： 矩阵的每一列的第一个非0值元素在values中的索引
- InnerNNZs ： 矩阵的每一列包含的非0元素的个数

`SparseMatrix`会在 values 和 InnerIndices 保留一些空位，以方便在插入新元素时避免重新进行内存分配。你可以使用` SparseMatrix::makeCompressed() `对这些空位进行压缩。再压缩模式下 InnerNNZs 会被释放掉，因为它和 OuterStarts 之间是冗余的。
Eigen 的运算产生的稀疏矩阵总是压缩的，但是如果向压缩的稀疏矩阵中插入了新元素会将其转换为非压缩模式。

## Solving Sparse Linear Systems
###  List of sparse solvers
[List of sparse solvers](https://eigen.tuxfamily.org/dox/group__TopicSparseSystems.html)

###  Sparse solver concept
```C++
#include <Eigen/RequiredModuleName>
// ...
SparseMatrix<double> A;
// fill A
VectorXd b, x;
// fill b
// solve Ax = b
SolverClassName<SparseMatrix<double> > solver;
solver.compute(A);
if(solver.info()!=Success) {
  // decomposition failed
  return;
}
x = solver.solve(b);
if(solver.info()!=Success) {
  // solving failed
  return;
}
// solve for another right hand side:
x1 = solver.solve(b1);
```
 ### The Compute Step
 Compute() 函数用于计算矩阵的因式分解，其主要包含两个步骤：
 1. analyzePattern（）用于对矩阵中的非0元素进行重新排序，已在分解时做更少的填充。该函数只是利用了矩阵的结构，其结果可以用于其它相似结构的线性系统（类似于 Ax=b1 和 Ax= b2 ???），但是有些外部求解起（例如：superLU） 会在改步修改矩阵的值。Eigen 提供了一些内置的和外部的方法重新排列矩阵，你可以通过模板参数指定。
 ```C++
 IterativeSolverClassName<SparseMatrix<double> PreconditionerName<SparseMatrix<double> > solver; 
```
###  The Solve step
 solve() 用于求解线性系统，其参数为方程右边的值，可以是一个 Vector 或 Matrix。直接使用 solve() 会计算一个机器精度的解，但是如果你对精度的要求不高，可以在求解步骤之前使用setTolerance（）设置所需的精度。

### BenchmarkRoutine 
在大多数情况下，我们往往是根据求解时间来选择最合适的求解器，Eigen 提供了一个基准程序来达到这个目的。它非常容易使用。在build目录中，导航到bench/spbench，并通过键入make spbenchsolver编译例程。使用--help选项运行它以获取所有可用选项的列表。基本上，要测试的矩阵应该是MatrixMarket Coordinate格式的，并且例程返回来自Eigen中所有可用求解器的统计数据。

## Matrix-free solvers
诸如ConjugateGradient和BiCGSTAB之类的迭代求解器可以在无矩阵环境中使用。为此，用户必须提供一个继承EigenBase<>并实现以下方法的包装器类：
- Index rows() and Index cols(): returns number of rows and columns respectively
- operator* with your type and an Eigen dense column vector (its actual implementation goes in a specialization of the internal::generic_product_impl class)

#  geometry
## Space transformations
模块提供两种不同类型的几何变换：
- Abstract transformations, such as rotations (represented by angle and axis or by a quaternion), translations, scalings. These transformations are NOT represented as matrices, but you can nevertheless mix them with matrices and vectors in expressions, and convert them to matrices if you wish.
- Projective or affine transformation matrices: see the Transform class. These are really matrices.
你可以通过如下方式构造一个旋转。
```C++
Transform t(AngleAxis(angle,axis));

Transform t;
t = AngleAxis(angle,axis);

// 这个构造是不允许的因为 Eigen 没有提供 AngleAxis 到 Transform 的隐式转换函数
Transform t = AngleAxis(angle,axis); // wrong
```
###  Transformation types
要变换多个矢量，首选的表示是旋转矩阵，而对于其他用途，四元数是首选的表示，因为它们紧凑，快速和稳定。最后， Rotation2D 和 AngleAxis 是创建其他旋转对象的主要方便类型。
上述任何转换类型都可以转换为具有相同性质的任何其他类型，或者转换为更通用的类型。以下是一些额外的示例：
```C++
Rotation2Df r;  r  = Matrix2f(..);       // assumes a pure rotation matrix
AngleAxisf aa;  aa = Quaternionf(..);
AngleAxisf aa;  aa = Matrix3f(..);       // assumes a pure rotation matrix
Matrix2f m;     m  = Rotation2Df(..);
Matrix3f m;     m  = Quaternionf(..);       Matrix3f m;   m = Scaling(..);
Affine3f m;     m  = AngleAxis3f(..);       Affine3f m;   m = Scaling(..);
Affine3f m;     m  = Translation3f(..);     Affine3f m;   m = Matrix3f(..);
```
###  Affine transformations
仿射变幻通过内部的 TransForm 类表示，其内部是一个(Dim+1)^2 的矩阵（齐次矩阵）。Eigen 没有对点和向量进行区分，但是在应用变换时，是有区别的。
```C++
// Apply the transformation to a point 
VectorNf p1, p2;
p2 = t * p1;

// Apply the transformation to a vector
VectorNf vec1, vec2;
vec2 = t.linear() * vec1;

```

#  Make Eigen run in parallel
一些 Eigen 算法可以使用多线程，你需要  enable OpenMP on your compiler。 for instance:
- GCC: -fopenmp
- ICC: -openmp
- MSVC: check the respective option in the build properties.
您可以使用以下优先级 控制OpenMP API或Eigen API使用的线程数：
```text
OMP_NUM_THREADS=n ./my_program
omp_set_num_threads(n);
Eigen::setNbThreads(n);
```

通过定义EIGEN_DONT_PARALLELIZE预处理器标记，可以在编译时禁用Eigen的多线程。

*Eigen 是完全优化的，会占用100%的单核CPU，因此线程数量最好不超过物理核数量一致，否者高速缓存污染和其它开销反而会导致性能下降*

###  Using Eigen in a multi-threaded application
如果您自己的应用程序是多线程的，并且多个线程调用Eigen，那么您必须在创建线程之前通过调用以下例程来初始化Eigen：
```C++
#include <Eigen/Core>
 
int main(int argc, char** argv)
{
  Eigen::initParallel();
  
  ...
}

```
使用Eigen 3.3和完全兼容C++11的编译器（即，线程安全的静态局部变量初始化），然后调用initParallel（）是可选的。

请注意，所有生成随机矩阵的函数都不是可重入的，也不是线程安全的。其中包括DenseBase：：Random（）和DenseBase：：setRandom（），尽管调用了Eigen：：initParallel（）。这是因为这些函数基于std：：rand，而std：：rand是不可重入的。对于线程安全的随机生成器，我们推荐使用c++11随机生成器（示例）或boost：：random。

# Assertions
eigen_assert和eigen_plain_assert都在Macros. h中定义。间接定义eigen_assert给了您一个改变其行为的机会。如果你想做一些其他的事情，比如抛出一个异常，你可以重新定义这个宏，然后使用eigen_plain_assert返回到它的默认行为。下面的代码告诉Eigen抛出一个std：：runtime_error：
```C++
#include <stdexcept>
#undef eigen_assert
#define eigen_assert(x) \
  if (!(x)) { throw (std::runtime_error("Put your message here")); }
```
##  Static assertions


Eigen 的速度快于 OpenCV 的原因如下：

    编译时优化：Eigen 通过编译时优化生成高效的代码。Eigen 根据程序中使用的表达式，预先分析表达式的形状，生成高度优化的代码，并且依赖模板元编程和 constexpr 计算等技术实现的编译时计算，以进一步增加代码的可读性和减少计算量。相比较而言，OpenCV 中的一些计算是使用宏定义实现的，开销相对较大，不太适合大规模并行计算。

    SIMD 优化：Eigen 可以使用 SIMD (Single Instruction Multiple Data) 指令，这意味着在一次计算中使用向量化指令对多个数据进行操作。这种向量化指令可以在 CPU 上在单次计算中同时处理多个数值。OpenCV也支持向量化指令，但是使用 Eigen 可以更好地利用 SIMD，从而提高计算速度。

    内存管理策略：Eigen 在内部实现中有自己的内存管理策略，可以减少内存分配的次数，从而避免内存分配中的开销。其次，Eigen的矩阵内存分布是在堆上进行的，这大大提高了存储和访问的效率；而OpenCV 的 cv::Mat 默认是在栈上分配内存，虽然也可以通过 allocator 修改内存布局，但是代价很高，影响了速度。

    更少的函数调用和数组复制：Eigen 通过密集级联计算出结果，从而减少了复制数组和多余函数调用的开销，而 OpenCV 的操作常常包含中间变量，这些变量需要消耗额外的时间和内存。

    专注于线性代数运算：Eigen 的核心是矩阵代数、线性代数、无线代数和几何学，因此，它可以为这些领域提供更高效的计算。OpenCV 除了线性代数外，还有大量的图像处理和计算机视觉算法，因此它需要更好的平衡，这意味着它在某些方面会牺牲一点运算速度。

    **通过将表达式树嵌入 C++ 类型体系中，允许编译器生成高有效的代码。**

## 为什么 Eigen 比 CV::Mat 块
1. Eigen 使用延迟计算，Eigen 在计算时，会记录的表达式，而不是计算中间结果，只有在需要写数据或赋值时才进行最终的计算，这样减少了中间变量的生成。Eigen 的延迟计算可能会导致别名问题，即读取的数据可能被上一次更改，例如：A=A^T
2. Eigen 可以使用静态矩阵，静态矩阵的内存分配是在编译期间进行的，分配在栈区（动态矩阵内存分配在堆区，在运行时分配），因此可以避免运行时的内存分配。但是如果创建的静态矩阵过大，可能导致栈溢出。栈空间大小一般只有几十M到几百M（可以通过 `ulimit -a`查看）。
3. Eigen 时纯头文件，头文件的代码是直接粘贴到cpp文件中编译的，因此编译器可以做更多的优化。（非头文件库将每个cpp会编译为库文件然后进行链接）。

4. **针对矩阵运算，将大型矩阵切分成小块，并在运算过程中保留这些块。这使得优化的缓存使用并减少了缓存错误**
5. **通过将表达式树嵌入 C++ 类型体系中，允许编译器生成高有效的代码。**
