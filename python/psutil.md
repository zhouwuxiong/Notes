psutil (python系统和过程工具)是一个跨平台的库,用于检索进程信息和系统利用率(CPU、内存、磁盘、网络、传感器)在python中。

## 获取进程信息
```python
import psutil
import sys
import io

sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

if __name__ == '__main__':
    # 主要信息：进程名，状态，创建时间，CPU内存使用情况，线程数
    p = psutil.Process(17500)
    print('进程名称:', p.name())  # 进程名称
    print('运行状态:', p.status())  # 当前状态
    print('创建时间:', p.create_time())  # 创建时间
    print('CPU信息:',  p.cpu_times())  # 进程的cpu时间信息,主要：user,system运行时间
    print('内存信息:', p.memory_percent())  # 进程内存利用率
    print('内存使用:', p.memory_info())  # 进程内存使用详情
    print('IO信息:', p.io_counters())  # 进程的IO信息,包括读写IO数字及参数
    print('线程数：', p.num_threads())  # 进程开启的线程数

```