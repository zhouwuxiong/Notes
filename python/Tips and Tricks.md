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
datas=[[KF, F]]
datav=[[KF, F]]
在datas和datasv中查找F相同的KF
```python
datas[np.where(np.in1d(datas[:, 1], datav[:, 1]))][:, 0]
```

## 获取变量名
```python
import inspect

def retrieve_name(var):
    callers_local_vars = inspect.currentframe().f_back.f_locals.items()
    return [var_name for var_name, var_val in callers_local_vars if var_val is var]

name, address, age, gender = "bob", "hangzhou", 21, "man"
person = {}

for i in [name, address, age, gender]:
    person[retrieve_name(i)[0]] = i

print person
```

## python设置输出编码
```python
import sys
import io

sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')
```