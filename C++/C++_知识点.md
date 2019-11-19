## C++ 个人知识欠缺清单
针对 《C++ Primer》 总结出自己对 C++ 知识技能欠缺的地方。

### 第一章：开始
* **程序块**： 零条或多条语句的序列，用花括号包围;  
* 附：
	* [Sale_item.h](./Sale_item.h)

### 第二章：变量和基本类型

**何为对象？**：通常情况下，对象是指一块能存储数据并具有某种类型的内存空间。
**关键字 extern**: 如果想声明一个变量而非定义它，就在变量名前添加关键字 `extern` ，而且不要显式地初始化变量;在函数体内部，如果试图初始化一个由 `extern` 关键字标记的变量，将引发错误 。 
```C++
extern int i ; // 声明 i 而非定义 i
int j ;       // 声明并定义 j 
extern double pi = 3.1416; //定义
```
* #### 引用
    * **引用** 为对象起了另外一个名字，引用类型引用另外一种类型。通过将声明符写成 `&d` 的形式来定义引用类型，其中 d 声明的变量名。
    ```
    int ival = 1024 ; 
    int &refVal = ival ; // refVal 指向 ival (是 ival 的另一个名字) 
    int &refVal2 ;  // 报错： 引用必须被初始化
    ```
    * 程序把引用和它的初始值**绑定**在一起，而不是将初始值拷贝给引用。一旦初始化完成，引用将和它的初始值对象一直绑定在一起。因为无法令引用重新绑定到另外一个对象，因此引用必须初始化。  
    * **引用即别名，引用并非对象，相反的，它只是为一个已经存在的对象所起的另外一个名字** 因为引用的本身不是一个对象，所以不能定义引用的引用。
* #### 指针
    * 指针和引用相比有很多不同点。
        1. 指针本身就是一个对象，允许对指针赋值和拷贝，而且在指针的生命周期内它可以先后指向几个不同的对象。
        2.  指针无须在定义时赋初值。
        3.  和其他内置类型一样，在块作用域内定义的指针如果没有被初始化，也将拥有一个不确定的值。
    * 如果指针指向了一个对象，则允许使用**解引用符**（操作符 * ）来访问该对象
    ```C++
    int *ip1 , *ip2 ; // ip1 和 ip2 都是指向 int 型对象的指针
    double dp , *dp2 ; // dp2 是指向 double 型对象的指针 , dp 是 double 型对象
    int ival = 42 ; 
    int *p = &ival ; //  p 存放变量 ival 的地址，或者说 p 是指向变量 ival 的指针
    *p = 0 ; // 由符号 * 得到指针 p 所指的对象，即可经由 p 为变量 ival 赋值

    double dval ; 
    double *pd = &dval ;  //正确 ：初始值是 double 型对象的地址
    double *pd2 = pd ;    //正确：初始值是指向 double 对象的指针

    int *pi = nullptr ; // 空指针
    pi = &ival // pi 的值被改变，现在 pi 指向了 ival 
    *pi = 0 ; //ival 的值被改变，指针 pi 并没有改变
    
    int i = 1024 , *p = &i , &r = i ; // i 是一个 int 型的数， p 是一个 int 型指针, r 是一个 int 型引用
    int *p1 , p2 ; // p1 是指向 int 的指针 , p2 是 int 
  	
    // 指向指针的引用
    int i= 42 ; 
    int *p ; // p 是一个 int 型指针
    int *&r = p ; // r 是一个对指针 p 的引用
    r = &i ; // r 引用了一个指针，因此给 r 赋值 &i 就是令 p 指向 i ;
    *r = 0 ; // 解引用 r 得到 i , 也就是 p 指向的对象 , 将 i 的值改为０；
    ```
    * 要理解 r 的类型到底是什么，最简单的办法是从右向左阅读 r 的定义。离变量名最近的符号（此例中是 &r 的符号 &） 对变量的类型有最直接的影响，因此 r 是一个引用。声明符的其余部分用以确定 r 引用的类型是什么，此例中的符号 * 说明 r 引用的是一个指针。最后，声明的基本数据类型部分指出 r 引用的是一个 int 指针.

* #### const 限定符
    * **const 限定符：** const 对象一旦创建后其值不能再改变，所以 const  对象必须初始化。
    ```C++
    const int bufSize = 512; // 输入缓冲区大小
    bufSize = 512 ; // 错误； 试图向 const 对象写值
    const int i = get_size() ; // 正确: 运行时初始化
    const int j = 42 ; // 正确：编译时初始化
    sonst int k ; //错误：k 是一个未经初始化的常量
    ```
    * 如果想在多个文件之间**共享 `const` 对象**，必须在变量的定义之前添加 `extern` 关键字。 `file_1.cc` 定义并初始化了 `bufSize` ，因为这条语句包含了初始值，所以它（显然）是一次定义，然而，因为 bufSize 是一个常量，必须用 extern 加以限定使其被其他文件使用。
    ```C++
    // file_1.cc 定义并初始化了一个常量，该常量能被其他文件访问
    extern const int bufSize = fcn() ; 
    // file_1.h 头文件
    extern const int bufSize; // 与 file_1.cc 中定义的 bufSize 是同一个
    ```
    * **const 的引用**
    ```C++
    const int ci = 1024 ; 
    const int &r1 = ci ; // 正确：引用及其对应的对象都是常量
    r1 = 42 ; // 错误： r1 是对常量的引用
    int &r2 = ci ; //错误： 试图让一个非常量引用指向一个常量对象
    ```
    * 允许一个常量引用绑定非常量的对象、字面值，甚至是个一般表达式
    ```C++
    int i = 42 ; 
    const int &r1 = i ; //允许将 const int& 绑定到一个普通 int 对象上
    const int &r2 = 42 ; // 正确： r2 是一个常量引用
    const int &r3 = r1 * 2 ; // 正确：r3 是一个常量引用
    int &r4 = r1 * 2 ; // 错误： r4 是一个普通的非常量引用
    ```
    * **指针和 const** ， 和常量引用一样，指向常量的指针也没有规定其所指的对象必须是一个常量。所谓指向常量的指针仅仅要求不能通过该指针改变对象的值，而没有规定那个对象的值不能通过其他其他途径改变。*所谓指向常量的指针或引用，不过是指针或引用“自以为是”罢了，它们觉得自己指向了常量，所以自觉地不去改变所指向对象的值*
    ```C++
    const double pi = 3.14; // pi 是个常量，它的值不能改变
    double *ptr = &pi ; // 错误： ptr 是一个普通指针
    const double *cptr = &pi ; // 正确： cptr 可以指向一个双精度常量
    *cptr = 42; // 错误：不能给 *cptr 赋值
    
    double  dval = 3.14; // dval 是一个双精度浮点数，它的值可以改变
    cptr = &dval ; // 正确：但是不能通过 cptr 改变 dval 的值
    ```
    * **const 指针** , 指针是对象而引用不是，因此就像其他对象类型一样，允许把指针本身定为常量。**常量指针**必须初始化，而且一旦初始化完成，则它的值（也就是存放在指针中的那个地址）就不能再改变了。把 * 放在 const 关键字之前用以说明指针是一个常量，即不变的是指针本身的值而非指向的那个值。
    ```C++
    int errNumb = 0 ; 
    int *const curErr = &errNumb ; // curErr 将一直指向 errNumb 
    const double pi = 3.14159 ; 
    const double *const pip = &pi ; // pip 是一个指向常量对象的常量指针
    ```
* #### 类型别名
	* **类型别名** 是一个名字，它是某种类型的同义词
	```C++
    typedef double wages ; //wages 是 double 的同义词
    typedef wages base , *p ; // base 是 double 的同义词  ， P 是 double * 的同义词
    
    using SI = Sales_item ; // SI 是 Sales_item 的同义词
    ```
        * **重点提醒：**
        ```C++
        typedef char *pastring ; 
        const pastring cstr = 0 ; // cstr 是指向 char 的常量指针
        const char *cstr = 0; //指向常量 char 的指针
        const pstring *ps ; // ps 是一个指针，它的对象是指向 char 的常量指针
        ```
        遇到一条使用了类型别名的声明语句时，人们往往会错误地尝试把类型别名替换成它本来的样子，以理解语句的含义：
        ```
        const char *cstr = 0 ; // 是对　const pstring cstr 的错误理解
        ```  
        因为声明语句中用到 pstring 时，其基本数据类型是指针。 （上面的语句）用 char * 重写了声明语句后，数据类型就变成了 char , * 成为了声明符的一部分。这样改写的结果是，const char 成了基本数据类型。前后两种声明含义截然不同，前者声明了一个指向 char 的常量指针，改写后的形式则声明了一个指向 const char 的指针。
        
* #### 自定义数据结构
	* **#ifdef** 当且仅当变量已定义时为真，**#ifndef** 当且仅当变量未定义时为真，一旦检查结果为真，则执行后续操作直至遇到**#endif**指令为止。  


### 第三章：变量和基本类型

**命名空间的 `using` 声明**:  有了 `using `声明就无须专门的前缀（形如命名空间：：）也能使用所需的名字了。  
**作用于操作符（::）**： 编译器应从操作符左侧名字所示的作用域中寻找右侧那个名字。因此，`std::cin` 的意思就是要使用命名空间 `std` 中的名字 `cin` ;   
**string::size_type**: 对于 `size` 函数来说，返回的是一个`string::size_type` 类型的值，由于`size`函数返回的是一个无符号整形数，因此切记，如果在表达式中混用了带符号数和无符号数将可能产生意象不到的后果。例如：假设 n 是一个具有负值的 `int` ，则表达式 `s.size() < n ` 的判断结果几乎肯定是 `true` ，这是因为负值 n 会自动地转换成一个比较大的无符号数。*(被坑过)*
* **vector**  
	* `vector` 是模版而非类型，由 `vector`生成的类型必须包含`vector`中元素的类型，例如：
    ```C++
    vector<int> ivec ;  // ivec 保存 int 类型的对象
    vector<Sales_item> Sales_vec ; // 保存 Sales_item 类型的对象
    vector<vector<string>> file ; // 该向量的元素是 vector 对象
    ```
    **C++11标准**之前，处理元素为`vector` 的`vector` 对象 ， 必须在外层 `vector`对象的右尖括号和其元素类型之间添加一个空格，如应该写成`vector<vector<int> >` 而非 `vector<vector<int>>`  *（被坑过）* 。
    
* **数组**
    * **定义和初始化内置数组**：编译的时候维度应该是已知的，所以维度必须是一个**常量表达式**。  
    * **常量表达式**：是指值不会改变并且在编译过程就能得到计算结果的表达式。
    ```C++
    const int max_files = 20 ; // max_files 是常量表达式
    const int limit = max_files + 1 ; // limit 是常量表达式
    int staff_size = 27 // staff_size 不是常量表达式
    constexpr int mf = 20 // 20 是常量表达式
    constexpr int limit = mf + 1 // mf + 1 是常量表达式
    
    unsigned cnt = 42 ; // 不是常量表达式
    constexpr unsigned sz = 42 ; // 常量表达式
    string bad[cnt] ; // 错误，cnt 不是常量表达式
    ```
    * **不允许拷贝和赋值**
    ```C++
    int a[] = {0,1,2} ; // 含有 3 个整数的数组 
    int a2[] = a ;  // 错误 ， 不允许使用一个数组初始化另一个数组
    a2 = a ; // 错误，不能把一个数组直接赋值给另一个数组 
    ```
    * **理解复杂的数组声明**
    ```C++
    int *ptrs[10] ; // ptrs 是含有 10 个整型指针的数组
    int &refs[10] = /*?*/ ; // 错误：不存在引用的数组 , 数组元素是对象
    int (*Parray)[10] = &arr ; // Parray 指向一个含有 10 个整数的数组
    int (&arrRef)[10] = arr ; // arrRef 引用一个含有 10 个整数的数组
    int *(&arry)[10] = ptrs ; // arry 是数组的引用，该数组含有 10 个指针
    ```
    　　由内而外的顺序可帮助我们更好地理解 Parray 的含义：首先是圆括号括起来的部分，`*Parray` 意味着Parray是个指针，接下来观察右边，可知道Parray是一个指向大小为 10 的数组指针，最后观察左边，知道数组中的元素是 int 。这样最终的含义就明白无误了，Parray是一个指针，它指向一个 int 数组，数组中包含 10 个元素。  
    　　同理，（&arrRef）表示 arrRef 是一个引用，它引用的对象是一个大小为 10 的数组，数组中元素的类型是 int 。  
    　　同理 知道 array 是一个引用，然后观察右边知道 arry 引用的对象是一个大小为 10 的数组，最后观察左边知道，数组的元素类型是指向 int 的指针。这样， arry 就是一个含有 10 个 int 型指针的数组的引用。  
    　　**要想理解数组声明的含义，最后的办法是从数组的名字开始安装由内向外的、由右向左的顺序阅读**
      
    * **C 风格字符串**：C风格字符串不是一种类型，而是为了表达和使用字符串而形成的一种约定俗成的写法，按此习惯书写的字符串存放在字符数组并以**空字符结束**。
    ```C++
    strlen(p) 、 strcmp(p1,p2) , strcat(p1,p2) , strcpy(p1,p2)  等C风格字符串的函数
    ```
    但是对大多数应用来说，使用标准库 string 要比使用 C 风格字符串更安全、更高效*(被坑过)*
    * **多维数组的下标引用**：
    ```C++
    int (&row)[4] = ia[1]; // 把 row 绑定到 ia 的第二个 4 元素数组上
    
    constexpr size_t rowCnt = 3 , colCnt = 4 ; // size_t 是 cstddef 头文件定义的一种与机器实现有关的无符号整数类型，它的空间足够大，能够表示任意数组的大小。
    int ia[rowCnt][colCnt] ; // 12 个未初始化的元素
    
    size_t cnt = 0 ; 
    for (auto &row :ia ) // 对于外层数组的每一个元素
    	for(auto &col : row ）{ // 对于内层数组的每一个元素
        	 col = cnt ;      // 将下一个值赋给该元素
             ++cnt ;  // 将 cnt 加 1 
    }
    
    for （auto row : ia)
    	for(auto col : row)
    ```
    注意最下面这个循环无法通过编译，这是因为，像之前一样第一个循环遍历 ia 的所有元素，注意这些元素实际上是大小为 4 的数组。因为 row 不是引用类型，所以编译器初始化 row 时会自动将这些数组形式的元素转换成指向该数组内首元素的指针，这样得到的 row 类型就是 int * ,显然内层的循环就不合法了，编译器将试图在一个 int * 内遍历，这显然和程序的初衷相去甚远了。**要使用范围 for 语句处理多维数组，除了最内层的循环外，其他所有循环的控制变量都应该是引用类型。**
    * **指针和多维数组**
    ```C++
    int ia[3][4] ;  // 大小为 3 的数组，每个元素是含有 4 个整数的数组
    int (*p)[4] = ia ;  // p 指向含有 4 个整数的数组
    int *ip[4] ; // 整形指针的数组
    p = &ia[2] ; // p 指向 ia 的尾元素
    
    // 输出 ia 中每个元素的值 ， 每个内层数组各占一行
    // p 指向含有 4 个整数的数组
    for(auto p = ia , p != ia + 3 ; ++p)
    	// q 指向4 个整数数组的首元素，也就是说， q 指向一个整数
        for(auto q = *p ; q != *p +4 ; ++q)
        	cout << *q <<' ' ; 
        cout << endl ; 
    
    // p 指向 ia 的第一个数组
    for(auto p = begin(ia) ; p != end(ia) ; ++p) {
    	// q 指向内层数组的首元素
        for(auto q = begin(*p) ; q != end(*p) ; ++q)
        	cout<< *q << ' ' ;  // 输出 q 所指的整形值
        cout<<endl ; 
    }
    
    using int_array = int[4] ; 
    for(int_array *p = ia ; p != ia + 3 ; ++p ){
    	for(int *q = *p ; q != *p + 4 ; ++1 )
        	cout<< *q << ' ' ; 
        cout<<endl ;
    }
    ```
    
### 第四章：表达式
**一元运算符**：作用于一个运算对象的运算符是一元运算符，如取地址符`（&）`和解引用符`（ * ）`；
**二元运算符**：作用于两个运算对象的运算符是二元运算符，如相等运算符`（==）`和乘法运算符`（ * ）`；

* **sizeof 运算符：** sizeof 运算符返回一条表达式或一个类型名字所占的字符数。sizeof 运算符满足右结合律，其所得的值是一个` size_t `类型。
    ```C++
    Sales_data data , *p ; 
    sizeof(Sales_data) ; // 存储 Sales_data 类型的对象所占的空间大小
    sizeof data ; // data 的类型的大小，即 sizeof(Sales_data) 
    sizeof p ; // 指针所占的空间大小
    sizeof *p == sizeof(*p); // p 所指类型的空间大小，即 sizeof(Sales_data) 
    sizeof data.revenue ; //Sales_data 的 revenue 成员对应类型的大小
    sizeof Sales_data::revenue; // 另一种获取 revenue 大小的方式
    ```
    sizeof 运算符的结果部分地依赖于其作用的类型
    * 对 char 或者类型为 char 的表达式执行 sizeof 类型，结果得 1 。
    * 对引用类型执行 sizeof 运算得到被引用对象所占空间的大小 。
    * 对指针执行 sizeof 运算得到指针本身所占空间的大小 。 
    * 对解引用指针执行 sizeof 运算得到指针指向的对象所占空间的大小，指针不需有效 。 
    * 对数组执行 sizeof 运算得到整个数组所占空间的大小，等价于对数组中所有的元素各执行一次 sizeof 运算并将所得结果求和。 注意， sizeof 运算不会把数组转换成指针来处理。
    * 对 string 对象或 vector 对象执行 sizeof 运算只返回该类型固定部分的大小，不会计算对象中元素的个数。
    ```C++
    constexpr size_t sz = sizeof(ia)/sizeof(*ia) ; // 返回 ia 的元素数量 
    int arr2[sz]; // 正确：sizeof 返回一个常量表达式。
    ```
* **显式转换：** 有时我们希望显式地将对象强制转换成另外一种类型。
	* **static_cast：**对于有明确定义的类型转换，只要不包含底层 const ，都可以使用 static_cast 。
	```C++
    double slope = static_cast<double>(j) / i ; // 进行强制类型转换以便执行浮点数除法
    void *p = &d ; // 正确：任何非常量对象的地址都能存入 void*
    double *dp = static_cast<double *>(p) ; // 正确：将 void* 转换回初始的指针类型
    ```
    * **const_cast:** 只能改变运算对象的底层 const 
    ```C++
    const char *pc ; 
    char *p = const_char<char*> (pc) ;// 正确，但是通过 p 写值是未定义的行为
    ```
    * **reinterpret_cast：**通常为运算对象的位模式提供较低层次上的重新解释。  
     
 **强制类型转换干扰了正常的类型检查，因此我们强烈建议程序员避免使用强制类型转换**
 
### 第五章：语句

简单语句我基本上都掌握了，所以这章就直接跳过了。
    * while 、for 和 do while 语句，执行迭代操作。
    * if 和 switch 语句，提供条件分支结构
    * continue 语句，终止循环的当前一次迭代
    * break 语句，退出循环或者 switch 语句
    * goto 语句，将控制权转移到一条带标签的语句
    * try 和 catch , 将一段可能抛出异常的语句序列括在花括号里构成 try 语句块 。 catch 子句负责处理代码抛出的异常。
    * throw 表达式语句，存在于代码块中，将控制权转移到相关的 catch 子句 。
    * return 语句，终止函数的执行。

### 第六章：函数

* **const** 形参和实参 ： 和其他初始化过程一样，当用实参初始化形参时会忽略掉顶层 const 。换句话说，形参的顶层 const 被忽略了，当形参有顶层 const 时，传给它常量对象或者非常量对象都是可以的。
```C++
const int ci = 42 // 不能改变 ci ,const 是顶层的
int i = ci ; // 正确，当拷贝 ci 时，忽略了它的顶层 const 
int * const p = &i ; // const 是顶层，不能给 p 给赋值
*p = 0 ;// 正确，通过 p 改变对象的内容是允许的，现在 i 变成了 0 
void fcn(const int i) {/* fcn 能够读取 i , 但是不能向 i 写值 */}
void fcn(int i){/**/} //  错误，重复定义了 fcn(int) ,顶层 const 被忽略掉了。
```
* **指针或引用形参与 const** : 形参的初始化方式和变量的初始化方式是一样的，我们可以使用非常量初始化一个底层 const 对象，但是返过来不行，同时一个普通的引用必须用同类型的对象初始化。
```C++
int i = 42 ; 
const int *cp = &i ; // 正确，但是 cp 不能改变 i ;
const int &r = i ; // 正确，但是 r 不能改变 i ; 
const int &r2 = 42 ; // 正确
int *p = cp ; // 错误，p 的类型和 cp 的类型不匹配
int &r3 = r ; // 错误，r3 的类型和 r 的类型不匹配
int &r4 = 42 ; // 错误，不能用字母值初始化一个非常量的引用
将同样的初始化规则应用到参数传递上可得如下形似：
int i = 0 ;
const int ci = i ;
string::size_type str = 0 ; 
reset(&i) ; // 调用形参类型是 int* 的 reset 函数
reset(&ci) ; // 错误，不能用指向 const int 对象的指针初始化 int* 
reset(i) ; // 调用形参类型是 int & 的 reset 函数
reset(ci); // 错误，不能把普通引用绑定到 const 对象 ci 上
reset(42) ; // 错误，不能把普通引用绑定到字面值上
reset(str) ;// 错误，类型不匹配，str 是无符号类型
```
尽量使用常量引用：
```C++
find_char("Hello World" , 'o' , str) ; // 正确, find_char 的第一个形参是对常量的引用
string::size_type find_char(string &s , char c , string::size_type &occurs) ; // 不良设计，第一个形参的类型应该是 const string & ;
则只能将 find_char 函数作用于 string 对象，类似下面这个样的调用
find_char("Hello World",'o',str) 将在编译时发送错误
```
* **内联函数**：一般来说，内联机制用于优化规模较小、流程直接、频繁调用的函数。
```C++
cout << shorterString(s1,s2) << endl ;
将在编译过程中展开成类似于下面的形式
cout << (s1.size() < s2.size() ? s1 : s2 ) <<endl ;
从而消除了 shorterString 函数的运行时开销，在 shorterString 函数的返回类型前面加上关键字 inline ，这样就可以将它声明成内联函数了:
inline const string & shorterString(const string &s1 , const string &s2){
	return s1.size() <= s2.size() ? s1 : s2 ; 
}
```
* **constexpr 函数**：是指能用于常量表达式的函数，定义 constexpr 函数的方法与其他函数类似，不过要遵循几项约定:函数的返回类型及所有形参的类型都得是字面值类型，而且函数体中必须有且只有一条 return 语句;
```C++
constexpr int new_sz() { return 42 ; }
constexpr int foo = new_sz() ; // 正确： foo 是一个常量表达式
constexpr sieze_t scale(size_t cnt) { return new_sz() * cnt ; }
当 scale 的实参是常量表达式时，它的返回值也是常量表达式；反之则不然 ；
int arr[scale(2)] ; // 正确； scale(2) 是常量表达式
int i = 2 ; // i 不是常量表达式
int a2[scale(i)]; // 错误： scale(i) 不是常量表达式
```
*costexpr 函数不一定返回常量表达式* 
* **函数指针**：函数指针指向的是函数而非对象。要想声明一个可以指向该函数的指针，只需要用指针替换函数名即可 。
```C++
bool lengthCompare(const string & , const string &) ; // 比较两个 string 对象的长度
pf 指向一个函数 ， 该函数的参数是两个 const string 的引用，返回值是 bool 类型
bool (*pf)(const string & , const string &) ; // 未初始化
```
当我们把函数名作为一个值使用时，该函数自动地转换成指针。
```C++
pf = lengthCompare ; // pf 指向名为 lengthCompare 的函数
pf = &lengthCompare ; // 等价的赋值语句：取地址符是可选的
bool b1 = pf("hello" , "goodbye"); // 调用 lengthCompare 函数
bool b2 = (*pf)("hello" , "goodbye") ; // 一个等价的调用
bool b3 = lengthCompare("hello" , "goodbye"); //另一个等价的调用
string::size_type sumLength(const string& , const string&) ; 
bool cstringCompare(const char* , const char*) ; 
pf = 0 ; // 正确 , pf 不指向任何函数
pf = sumLength ; // 错误，返回类型不匹配
pf = cstringCompare ; // 错误，形参类型不匹配
pf = lenthCompare ; // 正确，函数和指针的类型精确匹配
```