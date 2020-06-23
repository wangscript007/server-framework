# server framework

简介:  
1、底层库https://github.com/hujianzhe/util, 下载后置于BootServer目录下  
2、纯C实现，跨平台，不依赖任何第三方框架，通过加载动态链接库实现不同的业务逻辑  
3、支持异步回调、协程、消息3种RPC调度机制  
4、网络通信支持TCP和可靠UDP，支持自定义消息编解码  
5、自带http(支持GET/POST)和websocket协议接入层(你只需编写解析具体业务的包体格式)  
6、各节点通过读取配置文件获取你自定义的各种服务集群的节点信息  
7、按你的业务需要只在特定节点间维持长连接(连接不存在则发起异步连接)  
8、支持按照[一致性hash环，hash取余数，依次轮询，权重值，最小连接数]选取目标集群节点  

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

启动:  
编辑好服务节点启动需要的配置文件(具体格式参看附带的配置文件模板)，给每个节点一个配置文件和唯一日志标识名  
windows直接VS打开，工程配置好启动参数  <配置文件> <加载模块路径>  
linux下进入BootServer目录，sh run.sh <配置文件> <加载模块路径>  
