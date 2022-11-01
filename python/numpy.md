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