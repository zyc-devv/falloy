# 规则&预期

## 1
对标准库都模块化，遇到包含时跳过，插入待包含表，遇到需要时再解析包含的文件
## 2
1.常量直接在处理模块时内联
2.尽量对模块实例化
3.尽量不要改头文件，保证模块稳定
4.尽量不要写宏
5.将编译器和链接器合并
## 3
标准库元数据不直接加载，而是内存映射会怎样，把模板库和标准库尽量精简和分离模块，按照热点模块加载到内存
## 4
允许循环依赖，但不允许两个函数循环依赖，且标准库需要简化成DAG
## 5
对于动态库
- 如果是标准库，按要求DAG，可以激进优化
- 否则让编译器不管，自己维护循环调用问题，提供动态链接抽象层
## 6
加入现代化包管理器。默认AOT编译，但加入llvm和jit（支持REPL）。
## 7
简化类型（不推荐使用指针（会要求在unsafe块内，否则编译失败），保留智能指针和引用（增加弱引用），不会引入GC，会引入RC+预处理作用域，回收失败概率大且未手动处理会警告），增加提前推导，迭代器期间不允许操作，增加无锁安全和互斥规则，如果可以不复制类型，尽量不复制。
## 8
- 兼容一部分c++标准语法，方便迁移代码，但不会受到其他任何语言标准的限制。
- 让错误提示更友好，并使用强类型。
- 没有类似c++的标准版本（允许使用所有稳定的新增功能，稳定版中用 `feature_flag` 开启不稳定功能，尽量前向兼容，测试分支包含更多不稳定的功能）。
- 增加资源受限模式（尽量优化CPU和内存占用，适合CI/CD/OJ/嵌入/教育，且可以设置占用上限，超过先尝试清理缓存，失败自动终止，自动使用cgroups/工作对象，如果没有自动降级成自己维护占用）
- 增加原生IDE联动，文件默认 utf-8
- 分离守护进程 falloyd，提供索引，AST，补全，预加载模块，内存映射，增量更新变化检测
- const表示尽量编译器预处理，如果失败变成运行时常量
- 对象有公开和私有，分完全私有 `private`（支持类似标签，也可以直接加在函数前，表示仅当前文件可用），建议私有（名字加上_），命名私有（加上 `__`，名字实际会变成`_+类名+__函数名字`，类似），去除结构体，class默认公开
- 默认开启IO优化
- 显示声明类型，但auto会结合上下文推导，any 表示动态类型。（示例：`let int age = 15`）
- 顶部用package实现包命名
- 用import实现包管理，没有#include，项目建议有 `__init__` ，没有也允许
- import支持加上[]，表示传递，`[(allow_none),allow_prefix(,dep=1#表示最长前缀传递深度),allow_std,allow_all,allow[list]]`，模块默认 `allow_none`
## 9
采用静态类型
整数类型定义：
int:32位整数
long:64位整数

浮点数：
float:32位
double:64位

字符串：
string（表示字符串）
char表示单个字符
不推荐使用char[]或char*

字节：tytes
布尔：bool

高级类型（不是基础类型）：
unique_long:无长度限制的长整数（要用SIMD，乘法根据大小采用朴素、Karatsuba / Toom-Cook、FFT；其余运算同理。若太小自动降级）
decimal/unique_long_double:128位浮点
fraction:分数
fixed_point:定点数
vector 边长数组（不是基础类型）
map 自适应映射（支持哈希，红黑树，开放哈希，链表，哈希+分块数组）（不是基础类型）
unique_long，decimal/unique_long_double，fraction，fixed_point在std.math中，
string在std.types.string，map在std.types.map，vector在std.types.vector


IO:
输出支持std.io.write/print(xxx)
或者cout=std.io.console.out()兼容c++
输入支持std.io.read/input(xxx)
或者cin=std.io.console.in()兼容c++


（预期：融合c++的性能，go/llvm的编译速度，python的方便）

我们优先维护中文和英文文档，部分文档由机器翻译，当然你可以参与维护任何语言的文档。

名称:Fast&Future&Fusion Alloy，后缀falloy
