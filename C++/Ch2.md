### 顶层 const 
    * `顶层const` ：指针本身是个常量。
    * `底层const`：指针指向的对象是个常量。拷贝时严格要求相同的底层 `const` 资格。
    我的理解：
    * 被修饰的变量本身无法改变的 `const` 是顶层 `const`；
    * 通过指针或引用等间接途径来限制目标内容不可变的 `const` 是底层 `const`。

### constexpr 
    `constexpr` 把它所定义的对象置位了顶层 `const`。
    ```C++
        const int *p = nullptr ; // p 是一个指向整型常量的指针
        constexptr int *q = nullptr ; // q 是一个指向整数的常量指针
    ```
### typedef 
    如果某个类型别名指代的是复合类型或常量，那么把它用到声明语句里就会产生意向不到的后果。
    ```C++
        typedef char *pstring ; 
        const pstring cstr = 0 ; // cstr 是指向 char 的常量指针 而 非指向常量字符的指针
    ```

### decltype 
    ```C++
    int i = 42 , *p = &i , &r = i ;
    decltype(r + 0) b ; // 正确：加法的结果是 int ， 因此 b 是一个（未初始化的) int
    decltype(*p) c ; // 错误: c 是 int& , 必须初始化。
    记：如果表达式的内容是解引用操作，则 decltype 将得到引用类型。

    decltype((i)) d ; // 错误： d 是 int& ,必须初始化
    decltype(i) e ; // 正确： e 是一个(未初始化的) int
    记：decltype((variable)) 的结果永远是引用，而 decltype(variable) 结果只有当 variable 本身就是一个引用时才是引用
    ```
    `decltype` 处理 `顶层const` 和 `引用` 的方式与 `auto` 不同，`decltype` 会将 `顶层const` 和 `引用` 保留起来 , 而 `auto` 不会。

### 预处理器概述：

    * 预处理器：确保头文件多次包含仍能安全工作。
    * 当预处理器看到 `#include` 标记时，会用指定的头文件内容代替`#include` 。
    * 头文件保护符：头文件保护符依赖于预处理变量的状态：已定义和未定义。`#define` 指令把一个名字设定为`预处理变量`，另外两个指令则分别检查某个指定的预处理变量是否已经定义：`#ifndef` 当且仅当变量未定义时为真，一旦检查结果为真，则执行后续操作直至遇到 `#endif` 指令为止。
    
    ```C++
    #ifndef SALES_DATA_H
    #define SALES_DATA_H
    #include <string>
    struct Sales_data{
        std::string bookNo ;
        unsigned units_sold = 0 ; 
    };
    #endif
    
    第一次包含 Sales_data.h 时，#ifndef 的检查结果为真，预处理器将顺序执行后面的操作直到遇到 #endif 为止。此时，预处理变量 SALES_DATA_H 的值将变为已定义，而且 Sales_data.h 也会被拷贝到我们的程序中来。后面如果再一次包含 Sales_data.h ，则 #ifndef 的检查结果将为假，编译器将忽略 #ifndef 到 #endif 之间的部分。
    ```