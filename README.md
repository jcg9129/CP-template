# CP-template
C++ template files for competitive programming.

#### 1.设计原则：

1. C++ style, not C style。
2. 代码格式化:
    { BasedOnStyle: LLVM, UseTab: Never, IndentWidth: 4, TabWidth: 4, BreakBeforeBraces: Attach, AllowShortIfStatementsOnASingleLine: true, IndentCaseLabels: true, ColumnLimit: 0, AccessModifierOffset: -4, NamespaceIndentation: All, FixNamespaceComments: false ,AllowShortCaseLabelsOnASingleLine: true,AllowShortLoopsOnASingleLine: true,AllowShortBlocksOnASingleLine: true}
3. 文件宏命名为：双下划线+OY+双下划线+模板名全称大写+双下划线
    模板参数命名为：大写每个单词首字母；
    类命名为：大写每个单词首字母
    编译期常量命名为：全小写+下划线分割；
    成员变量命名为：m+下划线分割单词
    成员函数命名为：下划线分割单词
    静态成员变量命名为：s+下划线+下划线分割单词
    形式参数命名为：下划线分割单词
4. 必要时使用 explicit 修饰单参数构造；必要时使用 const ；必要时使用 constexpr 修饰编译期方法；必要时使用 size_t 类型。
5.  C 的函数不使用 std 前缀。
6. 对不保证隐式类型转换成功的场景，使用显式类型转换。

如果代码出现 bug 或者设计问题，欢迎指出

#### 2.力扣输入输出模板使用方法：

1. 目前支持的编译器有 clang/gcc/MSVC 。
2. 包含 LeetcodeIO.h 头文件；
3. 在包含头文件之前，加一行 #define OY_LOCAL 作为本地的标志；（也可以在编译参数里加 -DOY_LOCAL）
4. 在运行目录下放入 in.txt 和 out.txt 作为输入输出文件；如果找不到运行目录，可以随便输出一个字符串，看看 `out.txt` 生成到了哪里。
5. Solution 类的使用：需要在第二行注册要运行的成员方法名；
6. 自定义 Class 类的使用：需要在第一行注册类名+构造函数的所有参数类型；需要在第二行注册类名+所有要用到的成员方法名

使用时可以参考两张 `png` 图片示例。

#### 3.数据结构使用说明

1. 本模板库的数据结构，拥有极其优秀的运行速度。例如：

   截止 2023.08.30，最快的线段树 https://judge.yosupo.jp/problem/range_affine_range_sum

   截止 2023.08.30，最快的二维树状数组 https://loj.ac/p/134

   截止 2023.08.30，最快的笛卡尔树 https://www.luogu.com.cn/problem/P5854

   截止 2023.08.30，最快的李超线段树 https://www.luogu.com.cn/problem/P4097

   此外，一些数据结构单论速度也是最快，但因为最终用时大于一些别的算法（如离线处理）因而未列入。


2. 本模板现已放宽对语言环境的要求， `gcc` 和 `clang` 在 `C++11` 之后均可使用； `MSVC` 在 `C++14` 之后均可使用（暂无 `C++11` 测试环境）。

3. 为了使各种数据结构避免受到环境下内存分配速度的影响，所以数据结构大多使用了内存池，只要看到有 `s_buffer` 的存在，即表示有内存池。

4. 内存池往往作为类的静态变量存在，整个程序运行期间都不会发生分配和回收动作。由于 `C++` 中，平凡类型的全局变量、静态变量初始化不消耗运行时，所以当你的数据结构的结点类型为平凡类型时，即使开再大的内存池也不会产生一丁点的运行时间。反之，如果你给结点设置了构造、析构，或者给某个成员变量设置了初始值，那么内存池的初始化就会占用时间。

5. 一些平台，例如 `codeforces` 对生成的程序大小有限制，有时候 `MAX_NODE` 过大，会生成超出大小限制的程序而无法通过编译。此时问题很好解决，将 `s_buffer` 从结点数组类型修改为结点指针类型，然后将类外的 `s_buffer` 初始化改写为 `s_buffer = new 【结点类型】[MAX_NODE]` 即可。

   例如，如果 `ST` 表因为 `MAX_NODE` 过大而无法通过 `codeforces` 编译，则需要做如下修改：

   第 `46` 行修改为

   ```
   static node *s_buffer;
   ```

   第 `136` 行修改为

   ```
   typename Table<Node, MAX_NODE>::node *Table<Node, MAX_NODE>::s_buffer = new typename Table<Node, MAX_NODE>::node[MAX_NODE];
   ```

   此时即可通过 `codeforces` 编译。
