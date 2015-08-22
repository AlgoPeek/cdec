cdec
====

Decorated C++(简称CDEC)的设计思路是在效率不敏感的代码域中，通过改变代码风格，辅以运行库支持，屏蔽平台相关性，使得应用代码更加关注实现而不是语法细节，提高使用C++开发功能代码的效率。

## Getting Started
cdec包括以下组件：

* cdecegg: 库的基础，主要包含一些跨平台定义
* cdeccore：库的核心，包含常用库，如容器，字符串，IO, 编码解码，XML, JSON, 加密解密等
* stavlcommon: 工具库，包含日志，线程，时间，命令参数解等。
* mysqlcdec：基于cdec框架封装的mysqlconn库
* httpserv：基于cdec框架封装的microhttpd库
* curlcdec：基于cdec框架封装的curl库
* lzmacdec：基于cdec框架封装的lzma库
* zlibcdec：基于cdec框架封装的zlib库
* TestRunnerC：一个简单的单元测试框架，用于整个cdec库的测试

依赖关系：

* TestRunnerC不依赖于任何工程
* cdeccore依赖于cdecegg
* 其它工程都依赖于cdecegg和cdeccore

Linux 编译

* 可以对每个工程单独编译，编译时注意工程间的依赖关系
* cmake CMakeList.txt
* make
* 可以使用脚本buildall.sh对整个工程进行编译，debug版本编译后的库文件放入product/linuxd下

Windows 编译

* VS2010打开工程文件cdec.sln
* 在VS下对每个工程进行编译，编译时注意工程间的依赖关系
* debug版本编译后的文件放入product/win32d下

## 概要
设计宗旨

* 在开发速度和执行速度上取得双赢
  - 代码风格类似于Java(开发效率)
  - 可以无缝集成标准C++(热点函数执行效率)
* 相对于多语言联合的优势
  - 没有Invoke调用过程
  - 无缝集成标准C++

主要组成

* Essentail
  - 引用类与GC
  - 跨模块使用
  - 标准化异常
  - 互操作
   
* 支持的运行库
  - 标准容器类：stringx、Array、Vector、Map
  - IO：文件、目录操作；文件流和文本文件支持
  - XML: SAX和DOM
  - JSON: JsonParser和JsonWriter
  - Text: Encoding
  - Crypto: MD5、SHA1、AES、Base64
  - 工具类：Converter、Math、Datetime
  - 适配第三方库：microhttpd、curl、mysqlconn、lzma、zlib
  - 更多运行库支持不断添加中
  
## 框架体系

* 值类型 & 引用类型
  值类型
    - 标准类型：整数、浮点数、布尔等
    - 指向非cdec类的指针
    - stringx
    - struct定义的结构体
    - 总是从栈上创建，不允许动态分配
    - 不允许传递值类型变量的指针，不过可以通过装箱操作来传递
    - 允许在函数参数中使用引用来对外传值
    - 使用专用的值类型容器，如ArrayV, VectorV等
  引用类型
    - 使用class定义的类或者interface定义的接口
    - 总是直接或间接继承自Object, 接口只能继承接口或者Object
    - 不允许继承多个类，但可以继承或实现多个接口
    - 不允许定义拷贝构造函数，应该定义显式的Clone或类似方法来完成实例的复制

  值类型示例
```C++
struct RECT { int Width, Height };
```

  引用类型示例
```C++
class Region : public Object
{
  DECLARE_REF_CLASS(Region)
  
  Region(int left, int top, int width, int hegith);
  ...
};

ref<Region> Union(ref<Region> a, ref<Region> b);

ref<Region> a = gc_new<Region>(0, 0, 4, 4);
ref<Region> b = gc_new<Region>(2, 2, 3, 3);
ref<Region> c = Union(a, b);
```

  接口类型示例
```C++
interface IA : public Object
{
  DECLARE_REF_CLASS(IA)
  virtual void FooA() = 0;
};

interface IB : public Object
{
  DECLARE_REF_CLASS(IB)
  vitual void FooB() = 0;
};

class MyClass : public IA, public IB
{
public:
  void FooA() {...}
  void FooB() {...}
};
```

  引用对象与智能指针
```C++
ref<CdecClass> CreateCdecInstance()
{
  return gc_new<CdecClass>();
}
ref<CdecClass> a = CreateCdecInstance();

shared_ptr<CppClass> CreateCppInstance()
{
  return shared_ptr<CppClass>(new CppClass());
}
shared_ptr<CppClass> b = CreateCppInstance();
```

* GC
  GC类型
    - 简单GC, 使用reference counting控制生命周期，无法回收循环引用，附加消耗小，在无循环引用的情况下立即回收
    - 完整GC，使用reference tracking控制生命周期，可以回收循环引用，附加消耗大，部分对象需要延迟回收
  cdec的GC机制
    - 采用简单GC，附加简单代码风格约定，可以获得较好的性能
    - 值类型完全在栈上存在
    - 引用类型采用GC进行内存管理
    - 非cdec对象按照标准C++规则，自行处理
  引用和弱引用
    - 引用： ref<ClassName>
    - 弱引用：weak_ref<ClassName>
    - 空引用常量也用NULL
  对象创建与类型转换
    - ref<ClassType> instance = gc_new<ClassType>(arguments)
    - 不要在cdec对象上使用new操作
  循环引用
    - 对存在直接或间接控制本对象生命周期(管理或引用)对象的引用，采用weak_ref
    - 简单泛化，引用关系全部采用weak_ref, 实例传递全部用ref

* 字符串
  - 完全按照值类型使用
  - 采用Copy-On-Write优化
  - 字符串对象StringBuilder
    - 引用类型，与Java、C#等价类作用相同
    - 包含了字符串缓冲区，可以直接操作
    - 克隆对象会复制所有有效字符串

* foreach
  语法：foreach(Type, value, enumerator)
  使用条件：实现ref<IEnumerator<_Ty> > GetEnumerator()函数

示例1
```C++
ref<ArrayV<int> > arr = ...;
foreach(int, v, arr)
{
  printf("%d\n", v);
}
```
示例2
```C++
ref<Array<MyClass> > arr = ...;
foreach(ref<MyClass>, o, arr)
{
  o->DoSomthing();
}
```

## 基础库
