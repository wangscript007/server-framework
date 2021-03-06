# server framework
# 跨平台底层库 https://github.com/hujianzhe/util, 下载后置于BootServer目录下  

简介：本代码只实现节点启动自举与集群节点之间的消息联动，不包含任何业务代码  
节点进程通过加载你指定的动态链接库来执行你的业务代码  
节点进程内部由多个线程处理io，另有一个工作线程处理内部消息和收到的网络消息并派遣到你的业务代码逻辑中  
各节点通过读取配置文件加载或更新你自定义的各种服务集群的节点信息  
支持按照[一致性hash环，hash取余数，依次轮询，权重值，最小连接数]选取目标集群节点  
集群节点之间，按照你的业务代码逻辑与选取的节点建立长连接(连接不存在则发起异步连接)  

不太常见的一些特性:  
1、纯C编写实现，去除C++干扰，跨平台，不依赖任何第三方框架  
2、支持异步回调、协程、消息3种RPC调度机制  
3、网络通信支持TCP和可靠UDP  

TODO:  
1、一个好的说明文档，实在没时间写  
2、请求处理的统计  
3、对脚本语言编写业务逻辑提供支持(如果节点是无状态的,似乎不是做了也意义不大)  
4、有状态服务节点的扩容与缩容  

模块介绍:  
1、BootServer：服务节点启动的必备初始化和操作  
2、ServiceTemplate：服务节点代码模板，用来写你的业务逻辑  
3、SoTest：测试节点，用于测试功能  

编译:  
windows直接VS编译，然后把需要的dll拷贝到对应服务目录下  
linux下使用make debug 或 make release  
mac笔记本下未解决dylib的问题，需要修改业务目录下的makefile，定义-DUSE_STATIC_MODULE，并将业务代码连同BootServer代码一起编译  

启动:  
编辑好服务节点启动需要的配置文件(具体格式参看附带的配置文件模板)，给每个节点一个配置文件和唯一日志标识名，注意IP和端口号  
windows直接VS打开，工程配置好启动参数  <配置文件>  
linux编译后，sh run.sh <配置文件>  
mac需要自行启动编译后的可执行文件，不使用run.sh脚本启动，例如./SoTest/SoTest.exe <配置文件>  