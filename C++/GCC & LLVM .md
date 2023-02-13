## 简介
### 传统的静态编译器
传统编译器的工作原理基本上都是三段式的，可以分为前端（Frontend）、优化器（Optimizer）、后端（Backend）。前端负责解析源代码，检查语法错误，并将其翻译为抽象的语法树（Abstract Syntax Tree）。优化器对这一中间代码进行优化，试图使代码更高效。后端则负责将优化器优化后的中间代码转换为目标机器的代码，这一过程后端会最大化的利用目标机器的特殊指令，以提高代码的性能。

传统编译器的致命缺陷是，目标文件是一个完整的可执行文件，没有给其它语言的开发者提供代码重用的接口。即使GCC是开源的，但是源代码重用的难度也比较大。
### LLVM， Low Level Virtual Machine
为了解决编译器代码重用的问题

### Apple LLVM compiler 4.2和LLVM GCC 4.2
Apple LLVM compiler 4.2是一个真正的LLVM编译器，前端使用的是Clang，基于最新的LLVM 3.2编译的。LLVM GCC 4.2编译器的核心仍然是LLVM，但是前端使用的是GCC 4.2编译器。从LLVM的下载页面可以看出，LLVM从1.0到2.5使用的都是GCC作为前端，直到2.6开始才提供了Clang前

[LLVM和GCC的区别](https://www.cnblogs.com/zuopeng/p/4141467.html)
[LLVM](http://www.aosabook.org/en/llvm.html)
[（译）开源软件架构之 LLVM(The Architecture of Open Source Applications LLVM)](https://juejin.cn/post/6844904034134917134)
