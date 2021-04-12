## OSI 网络七层模型

    OSI 模型把网络通信的工作分为 7 层，从下到上分别是物理层、数据链路层、网络层、传输层、会话层、表示层和应用层。

![OSI 七层网络模型和 TCP/IP 四层网络模型的对比](http://c.biancheng.net/uploads/allimg/190124/1-1Z1241445324H.jpg)

## TCP 协议的粘包问题（数据的无边界性）

* **发送方产生粘包** : 采用 TCP 协议传输数据的客户端与服务器经常是保持一个长连接的状态（一次连接发一次数据不存在粘包），双方在连接不断开的情况下，可以一直传输数据。但当发送的数据包过于的小时，那么 TCP 协议默认的会启用 Nagle 算法，将这些较小的数据包进行合并发送（缓冲区数据发送是一个堆压的过程）；这个合并过程就是在发送缓冲区中进行的，也就是说数据发送出来它已经是粘包的状态了。`当发送的的数据过于小时，TCP 协议就会默认将较小的数据包合并再发送，也就是说数据发送出来时它已经时粘包状态了`

* **接收方产生粘包**：接收方采用 TCP 协议接收数据时的过程是这样的：数据到接收方，从网络模型的下方传递至传输层，传输层的 TCP 协议处理是将其放置接收缓冲区，然后由应用层来主动获取（C 语言用 recv、read 等函数）；这时会出现一个问题，就是我们在程序中调用的读取数据函数不能及时的把缓冲区中的数据拿出来，而下一个数据又到来并有一部分放入的缓冲区末尾，等我们读取数据时就是一个粘包。（放数据的速度 > 应用层拿数据速度）`接收方没有及时把缓冲区的数据拿出来导致下一个数据到来并有部分数据放入了缓冲区，等我们再读取数据时就会出现粘包`

* **怎么解决拆包和粘包？**：跟数据链路层的帧定界一样，可以采取：1.特殊字符控制；2.在包头首都添加数据包的长度。

* **粘包例子**：write()/send() 重复执行三次，每次都发送字符串"abc"，那么目标机器上的 read()/recv() 可能分三次接收，每次都接收"abc"；也可能分两次接收，第一次接收"abcab"，第二次接收"cabc"；也可能一次就接收到字符串"abcabcabc"。 假设我们希望客户端每次发送一位学生的学号，让服务器端返回该学生的姓名、住址、成绩等信息，这时候可能就会出现问题，服务器端不能区分学生的学号。例如第一次发送 1，第二次发送 3，服务器可能当成 13 来处理，返回的信息显然是错误的。

## TCP 三次握手

* 一次握手：服务器 B ，我们建立连接吧；二次握手：好的，我们可以建立连接的； 三次：谢谢你接受我的连接 
![](https://user-gold-cdn.xitu.io/2019/9/24/16d61cbcef672eb5?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

* **为什么两次握手不可以呢?** 为了防止已经失效的连接请求报文段突然又传送到了 B，因而产生错误。比如下面这种情况：A 发出的第一个连接请求报文段并没有丢失，而是在网路结点长时间滞留了，以致于延误到连接释放以后的某个时间段才到达 B。本来这是一个早已失效的报文段。但是 B 收到此失效的链接请求报文段后，就误认为 A 又发出一次新的连接请求。于是就向 A 发出确认报文段，同意建立连接。对于上面这种情况，如果不进行第三次握手，B 发出确认后就认为新的运输连接已经建立了，并一直等待 A 发来数据。B 的许多资源就这样白白浪费了。如果采用了三次握手，由于 A 实际上并没有发出建立连接请求，所以不会理睬 B 的确认，也不会向 B 发送数据。B 由于收不到确认，就知道 A 并没有要求建立连接。`为了防止已经失效的连接请求报文段突然又传送到了 B，因而产生错误,并一直等待 A 发来数据。B 的许多资源就这样白白浪费了`

* **为什么不需要四次握手?** 有人可能会说 A 发出第三次握手的信息后在没有接收到 B 的请求就已经进入了连接状态，那如果 A 的这个确认包丢失或者滞留了怎么办？我们需要明白一点，完全可靠的通信协议是不存在的。`在经过三次握手之后，客户端和服务端已经可以确认之前的通信状况，都收到了确认信息。所以即便再增加握手次数也不能保证后面的通信完全可靠，所以是没有必要的。`

* **Server 端收到 Client 端的 SYN 后，为什么还要传回 SYN？** 接收端传回发送端所发送的 SYN 是为了告诉发送端，我接收到的信息确实就是你所发送的请求连接信号了。SYN 是 TCP / IP 建立连接时使用的握手信号。

## 详细说下 TCP 四次挥手的过程？

* 一次交互：我的数据发送完了，我要断开连接了；二次：好的，我准备下；三次：我准备好了，可以断开连接了；四次：好的，谢谢合作。
![](https://user-gold-cdn.xitu.io/2019/9/24/16d61cb0506aea3f?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

* **为什么 TIME-WAIT 状态必须等待 2MSL 的时间呢？**   
    `1. 为了保证 A 发送的最后一个 ACK 报文段能够到达 B。`这个 ACK 报文段有可能丢失，因而使处在 LAST-ACK 状态的 B 收不到对已发送的 FIN + ACK 报文段的确认。B 会超时重传这个 FIN+ACK 报文段，而 A 就能在 2MSL 时间内（超时 + 1MSL 传输）收到这个重传的 FIN+ACK 报文段。接着 A 重传一次确认，重新启动 2MSL 计时器。最后，A 和 B 都正常进入到 CLOSED 状态。如果 A 在 TIME-WAIT 状态不等待一段时间，而是在发送完 ACK 报文段后立即释放连接，那么就无法收到 B 重传的 FIN + ACK 报文段，因而也不会再发送一次确认报文段，这样，B 就无法按照正常步骤进入 CLOSED 状态。`2. 防止已失效的连接请求报文段出现在本连接中。`A 在发送完最后一个 ACK 报文段后，再经过时间 2MSL`(最大报文生存时间 MSL , 是一个数据报在网络中单向发出到认定丢失的时间)`，就可以使本连接持续的时间内所产生的所有报文段都从网络中消失。这样就可以使下一个连接中不会出现这种旧的连接请求报文段。

* **TIME_WAIT状态会导致什么问题，怎么解决?** 在某些场景下，60 秒的等待销毁时间确实是难以接受的，例如：高并发的压力测试。当我们通过并发请求测试远程服务的吞吐量和延迟时，本地就可能产生大量处于 TIME_WAIT 状态的 TCP 连接。 一个TCP连接是由源地址、源端口、目的地址以及目的端口来唯一确定的。 这里能打开的连接数更可能是受限于服务器上对同时打开的文件描述符数目的限制。1）缩短 time-wait 时间 2）修改Linux内核中的参数，使得新的TCP连接可以重用处于TIME_WAIT的连接 3）增加机器
    ```C++
    1. netstat 用于查看网络连接的各种信息，一般看到的连接状态会有：ESTABLISHED、SYN_RCVED、CLOSE_WAIT、TIME_WAIT;

    2. 如果是你在部署某一个程序服务的时候，看到大量的TIME_WAIT连接是怎么回事？ 分析了TIME_WAIT是主动关闭一方，所以服务器应该是有大量短连接请求 ; 

    3. 那么大量的TIME_WAIT会导致什么问题呢？从端口占用 2MSL 不能使用，可能导致后续没有端口建立新连接。

    4. 那我们怎么去避免这个问题，避免大量TIME_WAIT？ 讲了设置socket选项SO_LINGER（直接终止连接）或者设置SO_REUSEADDR让端口和地址重用（允许内核重用处于 TIME_WAIT 状态的 TCP 连接）
    ```

## TCP 协议是如何保证可靠传输的？

1) 数据包校验; 2) 应答机制；3)超时重发；4)流量控制(滑动窗口协议)； 5) 对失序数据包重排序； 6) 丢弃重复数据； 7) 拥塞控制

## 数据链路层的滑动窗口的分类

数据链路层的滑动窗口协议中 ，窗口的大小在传输过程中是固定的（注意与传输层的滑动窗口协议的区别）。

1. 停止等待协议：发送窗口大小 = 1， 接收窗口大小= 1
2. 后退 N 桢协议：发送窗口大小 > 1，接收窗口大小 = 1
3. 选择重传协议 ： 发送窗口大小 > 1， 接收窗口大小 > 1

## 拥塞控制(网络全局观)

拥塞控制和流量控制不同，前者是一个全局性的过程，而后者指点对点通信量的控制。在某段时间，若对网络中某一资源的需求超过了该资源所能提供的可用部分，网络的性能就要变坏。这种情况就叫拥塞。

TCP 的拥塞控制采用了四种算法，即：慢开始、拥塞避免、快重传和快恢复。

## TCP和UDP区别

* TCP面向连接（三次握手），通信前需要先建立连接；UDP面向无连接，通信前不需要连接。
* TCP通过序号、重传、流量控制、拥塞控制实现可靠传输；UDP不保障可靠传输，尽最大努力交付。
* TCP面向字节流传输，因此可以被分割并在接收端重组；UDP面向数据报传输。
* TCP 开销大建立时间长；UDP 开销小，适用于即时通讯。

## http？无状态？无状态怎么实现用户登录？

* `HTTP `本身是一个无状态的连接协议，为了实现客户端与服务器之间的交互的协议

* `Cookie` 实际上是一小段的文本信息。客户端请求服务器，如果服务器需要记录该用户状态，就使用`response`向客户端浏览器颁发一个`Cookie`。客户端浏览器会把`Cookie`保存起来。当浏览器再请求该网站时，浏览器把请求的网址连同该`Cookie`一同提交给服务器。服务器检查该`Cookie`，以此来辨认用户状态。服务器还可以根据需要修改`Cookie`的内容。

* 浏览器第一次访问服务器，服务器会创建一个`session`，然后同时为该`session`生成一个唯一的会话的`key`,也就是`sessionid`，然后，将`sessionid`及对应的`session`分别作为`key`和`value`保存到缓存中，也可以持久化到数据库中，然后服务器再把`sessionid`，以`cookie`的形式发送给客户端。这样浏览器下次再访问时，会直接带着`cookie`中的`sessionid`。然后服务器根据`sessionid`找到对应的`session`进行匹配；



## Socket 编程

1. 什么是 Socket ？ 

    socket 的原意是“插座”，在计算机通信领域，socket 被翻译为“套接字”，它是计算机之间进行通信的一种约定或一种方式。通过 socket 这种约定，一台计算机可以接收其他计算机的数据，也可以向其他计算机发送数据。

2. 套接字有哪些类型

    套接字分为，流格式套接字（Stream Sockets）就是“面向连接的套接字”，它基于 TCP 协议；数据报格式套接字（Datagram Sockets）就是“无连接的套接字”，它基于 UDP 协议。

3. server 服务器函数作用、
    ```C++
    1. socket() : socket 也是一个文件，也有文件描述符。使用 socket() 函数创建套接字以后，返回值就是一个 int 类型的文件描述符。
    2. bind() 函数将套接字与特定的 IP 地址和端口绑定起来，只有这样，流经该 IP 地址和端口的数据才能交给套接字处理。
    3. listen(int sockfd , int backlog): 最多允许 backlog 个客户端处于待连接状态，并不是最大支持的连接数量；socket()函数创建的socket默认是一个主动类型的，listen函数将socket变为被动类型的，等待客户的连接请求；让套接字进入监听状态，并没有真正接收客户端请求，listen() 后面的代码会继续执行，直到遇到 accept()。
    4. accept() 会阻塞程序执行（后面代码不能被执行），直到有新的请求到来
    5. send() , recv() 
    6. close() 
    ```
4. client 函数作用
    ```C++
    1. socket() 
    2. connect() 
    3. send() , recv() 
    4. close() 
    ```

## IO 多路复用

* `I/O`多路复用的本质是使用`select`,`poll`或者`epoll`函数，挂起进程，当一个或者多个`I/O`事件发生之后，将控制返回给用户进程。因为在单一的进程上下文当中，所以不会有多进程多线程模型的切换开销。`IO多路复用是指内核一旦发现进程指定的一个或者多个IO条件准备读取，它就通知该进程。`
* `非阻塞IO`: 进程发起IO系统调用后，如果内核缓冲区没有数据，需要到IO设备中读取，进程返回一个错误而不会被阻塞；进程发起IO系统调用后，如果内核缓冲区有数据，内核就会把数据返回进程。

#### Select 
    1. 内核对被监控的文件描述符集合大小做了限制，并且这个是通过宏控制的，大小不可改变(限制为1024)。
    2. 每次调用 `select` 都需要把监听的文件描述符集合`fd_set`从用户态拷贝到内核态，
    3. 每次调用`select` 调用返回之后都需要遍历所有文件描述符，判断哪些文件描述符有读写事件发生，这是O(n)的时间开销。

#### Poll 
    1. poll改变了文件描述符集合的描述方式，通过一个pollfd数组向内核传递需要关注的事件，没有描述符个数的限制.
    2. 也不用每次都重新拷贝 fd_set ，只需要将结构体中的 revent 置零，但是用户空间和内核空间重复拷贝需要。
    3. 但是依旧没有解决，调用 `Poll` 返回之后都需要遍历所有文件描述符，判断哪些文件描述符有读写事件发生，这是O(n)的时间开销。

#### Epoll
    1. `epoll`使用一个文件描述符管理多个描述符，将用户关心的文件描述符的事件存放到内核的一个事件表中，这样在用户空间和内核空间只需一次拷贝，不用每次调用都重复传入文件描述符集合。
    2. 在`epoll_wait`函数返回时，无须遍历整个被侦听的描述符集，只要遍历那些被内核`IO`事件异步唤醒而加入就绪队列的描述符集合就行，这样就避免了无效的遍历。
    
    **epoll的水平触发和边沿触发有什么区别？**
        答：水平触发(Level Trigger)：是eopll的默认工作模式，当epoll_wait检测到某文件描述符上有事件发生，并通知应用程序之后，应用程序可以不立即处理该事件，当下次调用epoll_wait时，epoll_wait还会再次向应用程序通知此事件，直到该事件被应用程序处理。
            边缘触发（Edge Trigger）： 当epoll_wait检测到某文件描述符上有事件发生，并通知应用程序之后，应用程序必须立即处理该事件。如果应用程序不处理，下次调用epoll_wait时，epoll_wait不会再次通知此事件。
       ET模式很大程度上减少了epoll事件被重复触发的次数，因此效率比LT模式下高。




## Get 和 Post 的区别

1. GET和POST本质上就是TCP链接，并无差别。但是由于HTTP的规定和浏览器/服务器的限制，导致他们在应用过程中体现出一些不同；
2. 就安全性而言，POST 的安全性要比 GET 的安全性高，因为 GET 请求提交的数据将明文出现在 URL 上，而且 POST 请求参数则被包装到请求体中，相对更安全；
3. 从请求的大小看，GET 请求的长度受限于浏览器或服务器对 URL 长度的限制，允许发送的数据量比较小，而 POST 请求则是没有大小限制的。
4. 对于GET方式的请求，浏览器会把http header和data一并发送出去，服务器响应200（返回数据）；而对于POST，浏览器先发送header，服务器响应100 continue，浏览器再发送data，服务器响应200 ok（返回数据）。 

## 在浏览器中输入 URL 地址到显示主页的过程？

1. 浏览器解析 `URL` 生成 `Http` 请求信息  ； 
2. 应用层 `DNS` 协议，解析出域名对应的 `IP` 地址，有迭代查询和递归查询两种； 递归查询主要是在问缓存和本地域名服务器，然后 本地域名服务器帮忙迭代去问其他域名服务器（根、顶级等等）。
3. 传输层 `TCP` 协议，浏览器获得域名对应的 `IP` 地址以后，浏览器向服务器请求建立链接，发起三次握手；`ICMP` 用于告知网络包传送过程中产生的错误以及各种控制信息。
4. 网络层 `IP` 、 `ICMP` 、`ARP` 协议 ; `ICMP` 用于告知网络包传送过程中产生的错误以及各种控制信息。 `ARP ` 用于根据 IP 地址查询相应的以太网 MAC 地址。
5. 发送 `HTTP` 请求：`TCP` 连接建立起来后，浏览器向服务器发送 `HTTP` 请求；
6. 服务器处理请求并返回 `HTTP` 报文：服务器接收到这个请求，并根据路径参数映射到特定的请求处理器进行处理，并将处理结果及相应的视图返回给浏览器；
7. 浏览器解析渲染页面：浏览器解析并渲染视图，若遇到对 `js` 文件、`css` 文件及图片等静态资源的引用，则重复上述步骤并向服务器请求这些资源；浏览器根据其请求到的资源、数据渲染页面，最终向用户呈现一个完整的页面。

`浏览器解析 URL 生成 Http 请求信息 , DNS 根据域名解析出IP，然后发起 TCP 三次握手建立连接，其中会牵扯到网络层的 IP 协议，和 ICMP 协议用于告知网络包传送过程中产生的错误以及各种控制信息，ARP 协议用于获得对应 IP 的 MAC 地址，网卡会将包转为电信号，通过网线发送出去，交换机会根据 MAC 地址表查找 MAC 地址，然后将信号发送到相应的端口，没找到则交换机会将包发送到除源端口之外的所有端口，再到路由表查询转发目标`