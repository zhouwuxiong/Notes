## 解析导数

## 自动求导

## 数值导数
### 前向差分与中心差分
前向差分： D(f) = (f(x+h)-f(x))/h
中心差分： D(f) = (f(x+h)-f(x-h))/2h
h 是数值上的极小数

随着步长(h)的减小，中心差分公式收敛得更快，以获得更准确的导数估计值.（中心差分在 h 较大时取得的效果比前向差分更好，详见ceres链接中的图例）
### Ridders’ Method
Ridders’ Method 比中心差分和前向差分更好，适用于更大的步长h。（原理比较复杂，建议直接看官网）
### 参考
数字导数就是在 x 上添加一个很小的扰动，然后通过求极限的方式获取导数值。
[ceres: Numeric derivatives¶](ceres-solver.org/numerical_derivatives.html#forward-differences)

## Automatic Derivatives
自动求导虽然很方便，但是也会遇到一些问题。如：
```text
y = 1 - sqrt(x0^2 + x1^2)
y_x0 = -x0/sqrt(x0^2 + x1^2)
y_x1 = -x1/sqrt(x0^2 + x1^2)
```
当 x0 = x1 = 0 时，倒数是不存在的。为了解决这个问题需要明确的推算出这些点并进行替换或则使用正则化去除这些点。

## Modeling Non-linear Least Squares
### CostFunction
CostFunction 主要用于计算残差向量和雅克比矩阵。
```C++
class CostFunction {
 public:
    // function used Compute the residual vector and the Jacobian matrices.
  virtual bool Evaluate(double const* const* parameters,    // 输入 参数块
                        double* residuals,                  // 输出 残差块
                        double** jacobians) const = 0;      //【输出】 雅克比
  const vector<int32>& parameter_block_sizes();
  int num_residuals() const;

 protected:
  vector<int32>* mutable_parameter_block_sizes(); 
  void set_num_residuals(int num_residuals);    
};
```
如果你继承了 CostFunction ，那么你要设置 `CostFunction::parameter_block_sizes_` 和  `CostFunction::num_residuals_ ` 。这两个信息会在 Problem::AddResidualBlock() 中被校验

### SizedCostFunction
如果参数块和残差块的大小在编译期间就已经知道，那么可以使 SizedCostFunction，size 通过模板参数指定，这是你只需要实现 Evaluate() 函数。
```
template<int kNumResiduals, int... Ns>
class SizedCostFunction : public CostFunction {
 public:
  virtual bool Evaluate(double const* const* parameters,
                        double* residuals,
                        double** jacobians) const = 0;
};

```

### AutoDiffCostFunction

```C++
template <typename CostFunctor,
       int kNumResiduals,  // Number of residuals, or ceres::DYNAMIC.
       int... Ns>          // Size of each parameter block
class AutoDiffCostFunction : public
SizedCostFunction<kNumResiduals, Ns> {
 public:
  AutoDiffCostFunction(CostFunctor* functor, ownership = TAKE_OWNERSHIP);
  // Ignore the template parameter kNumResiduals and use
  // num_residuals instead.
  AutoDiffCostFunction(CostFunctor* functor,
                       int num_residuals,
                       ownership = TAKE_OWNERSHIP);
};
```
如果要是用自动求导，你必须定义一个实现了`opreate()`的函数
```C++
class MyScalarCostFunctor {
  MyScalarCostFunctor(double k): k_(k) {}

  template <typename T>
  bool operator()(const T* const x , const T* const y, T* e) const {    // 如果有更多输入参数，可以在y后继续添加参数
    e[0] = k_ - x[0] * y[0] - x[1] * y[1];
    return true;
  }

 private:
  double k_;
};
```

```C++
CostFunction* cost_function
    = new AutoDiffCostFunction<MyScalarCostFunctor, 1, 2, 2>(
        new MyScalarCostFunctor(1.0));              ^  ^  ^
                                                    |  |  |
                        Dimension of residual ------+  |  |
                        Dimension of x ----------------+  |
                        Dimension of y -------------------+
```
AutoDiffCostFunction 默认会获取 CostFunctor 对象的所有权，这样在 AutoDiffCostFunction 被 delete 时，会自动调用 CostFunctor 的析构函数。

## ceres::LocalParameterization

[深入探索ceres::Problem](https://blog.csdn.net/jdy_lyy/article/details/119360492)