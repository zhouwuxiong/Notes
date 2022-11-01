## 简介
numpy.linalg模块包含线性代数的函数。使用这个模块，可以计算逆矩阵、求特征值、解线性方程组以及求解行列式等。

  NumPy linear algebra 依赖于 BLAS 和 LAPACK 提供高效的线性代数算法的底层实现。numpy中默认包含了这些算法的c版本实现，高度优化的库可以更好的利用专业处理器的优势，例如： OpenBLAS, MKL (TM), and ATLAS.等，因为这些库是多线程的。

  SciPy.linalg 模块和 Numpy.linalg 包含了一些重复的函数，但是 SciPy.linalg 模块中也包含了一些 Numpy.linalg 中的函数，例如：LU分解、Schur分解、多种伪逆计算方法、matrix transcendentals such as the matrix logarithm

  [numpy.linalg Tutorial](https://numpy.org/doc/stable/reference/routines.linalg.html)