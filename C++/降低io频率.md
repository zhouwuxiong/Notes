# 怎样降低磁盘 io 频率？
在使用 fstream 时，如果频繁的进行磁盘读写，可能会导致程序崩溃。

## fstream 缓冲区
C++ 有两种主要的缓冲区类型， filebuf 和 streambuf 分别作为文件流和字符流的缓冲区。filebuf 可以使用 setbuf() 和 pubsetbuf() 设置自定义的缓冲区。fstream 的默认缓冲区大小和最大缓冲区大小与文件系统和c++标准相关

- setbuf() 函数用于设置标准 I/O 流的缓冲区，例如 FILE* 类型的 stdin，stdout 和 stderr 流。
- pubsetbuf() 函数用于设置任何类型的流的缓冲区，例如 std::stringstream，std::fstream，std::ofstream 等.

## linux 缓冲区
linux 有三种缓类型：全缓冲、无缓冲、行缓冲。
行缓冲的大小为 1KB


