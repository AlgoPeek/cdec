cdec
====

Decorated C++(简称CDEC)的设计思路是在效率不敏感的代码域中，通过改变代码风格，辅以运行库支持，屏蔽平台相关性，使得应用代码更加关注实现而不是语法细节，提高使用C++开发功能代码的效率。

## Getting Started
cdec包括以下组件：

* cdecegg: 库的基础，主要包含一些跨平台定义
* cdeccore：库的核心，包含常用库，如容器，字符串，IO, 编码解码，XML, JSON, 加密解密等
* stavlcommon: 工具库，包含日志，线程，时间，命令参数解等。
* mysqlcdec：基于cdec框架封装的mysql库
* httpserv：基于cdec框架封装的microhttpd库
* curlcdec：基于cdec框架封装的curl库
* lzmacdec：基于cdec框架封装的lzma库
* zlibcdec：基于cdec框架封装的zlib库
* TestRunnerC：一个简单的单元测试框架，用于整个cdec库的测试

依赖关系：

* TestRunnerC不依赖于任何工程
* cdeccore依赖于cdecegg
* 其它工程都依赖于cdecegg和cdeccore

CentOS 编译

* 可以对每个工程单独编译，编译时注意工程间的依赖关系
* cmake CMakeList.txt
* make
* 可以使用脚本buildall.sh对整个工程进行编译，debug版本编译后的库文件放入product/linuxd下

Windows 编译

* VS2010打开工程文件cdec.sln
* 在VS下对每个工程进行编译，编译时注意工程间的依赖关系
* debug版本编译后的文件放入product/win32d下
