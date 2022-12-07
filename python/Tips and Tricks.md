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

## python 类静态变量
Python中没有static关键词，所以无法直接显式的表明函数的静态变量。
考虑到Python中的类具有自己的类变量，而这个与类的实例区分开来，具有一定的持久性，所以可以这么做：
```python
class get_static:
    static_var = np.array([1,2])

get_static.static_var[0] += 1
```

##  计算字典中每个Key对应的平均值
```python
# 1.
np.array(list(dict.values())).mean(axis=1)
# 2. 
# sum( Dict.values() )/len( Dict )
```

## 捕获多个异常
```python
try:
    # smtpObj = smtplib.SMTP('localhost')

    smtpObj = smtplib.SMTP()  # 创建一个连接
    smtpObj.ehlo()
    smtpObj.starttls()
    smtpObj.connect('smtp.qq.com')  # 连接发送邮件的服务器
    # smtp.login(username, password)
    smtpObj.sendmail(sender, receivers, message.as_string())
    print("邮件发送成功")
except smtplib.SMTPException as e:
    print(type(e), '::', e)
    print("Error send fail")
except Exception as e:
    print(str(e.errno), str(e.strerror))
    print("Error send fail")
```

## 数组A按照数组B顺序排序
### 1. 使用zip压缩
```python
    mileage_, KD_ = zip(*sorted(zip(mileage, KD)))
```
### 2. argsoort
```python
np.array(A)[np.array(B).argsoort(,axis=0)]
```
