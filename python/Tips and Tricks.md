## numpy
### 相同元素查找
1. 使用 set查找相同元素
```python
set(datas[:, 1]) & set(datav[:, 1])
```
2. 使用 np.in1d(A, B) 查找相同元素的索引

```python
# 返回一个 bool 数组，指示 A 的每个值是否在 B 中找到
np.in1d(A, B) 

# 返回相同元素的在A中的索引
np.where(np.in1d(A, B))[0]

# 返回A中的相同元素
A[np.where(np.in1d(A, B))[0]]
```
示例：
查找A=[[KF, F]]
```python
datas[np.where(np.in1d(datas[:, 1], datav[:, 1]))][:, 0]
```

