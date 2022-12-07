## np 文件
### np.savetxt保存时数据不使用科学计数法形式
```python
np.set_printoptions(suppress=True)
np.set_printoptions(precision=4)   #设精度
np.savetxt('data_name‘, data.view(-1, 1), fmt='%.04f')   #保留4位小数
```
## 输出
1. numpy在print时会有...省略掉中间部分，如果希望显示处完整数组：
``` python
np.set_printoptions(threshold=np.inf)
```
2. np 调整精度
```
A = np.around(A,decimals=3)
```

## np 矩阵乘法
```python
# 叉乘、外积
tf.matmul(A,C)=np.dot(A,C)= A@C
# 点乘、内积
tf.multiply(A,C)= A*C=A∙C
```
![numpy矩阵乘法](./assets/np%20matrix%20multiply.png)

## 元素查找
### np.in1d
测试一维数组的每个元素是否也存在于第二个数组中。
返回一个长度相同的布尔数组ar1这是 True 其中一个元素ar1在ar2否则为 False。
#### 参数
numpy.in1d(ar1, ar2, assume_unique=False, invert=False)

assume_unique： 布尔型，可选
如果为 True，则假定输入数组都是唯一的，这样可以加快计算速度。默认为假。
invert： 布尔型，可选
如果为 True，则返回数组中的值被反转(即，False，其中一个元素ar1在ar2否则为真)。默认为假。np.in1d(a, b, invert=True)相当于(但比)快np.invert(in1d(a, b)).
### np.where
np.where有两种用法：
1.np.where(condition,x,y) 当where内有三个参数时，第一个参数表示条件，当条件成立时where方法返回x，当条件不成立时where返回y
2.np.where(condition) 当where内只有一个参数时，那个参数表示条件，当条件成立时，where返回的是每个符合condition条件元素的坐标,返回的是以元组的形式
### 示例
1. np.in1d() 查找datas[:,1]与datavv[:,1]中的相同元素
2. np.where() 获取相同元素的在datas中的索引
3. datas[*] 获取相同的datas[0,:]
```python
datas[np.where(np.in1d(datas[:, 1], datav[:, 1]))[0]][:, 0]
```
