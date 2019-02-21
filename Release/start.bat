taskkill /F /IM RmqPerformanceCustomer.exe
taskkill /F /IM RmqPerformanceProduct.exe

//1 SocketThread接口 
//customer key ip exchange type
//product key 总数 发送循环延迟(ms) ip exchange type
//2 新接口
//customer key 用户 密码 ip exchange type 队列名 消费者名 1压缩0不压缩
//product key 总数 主动限流 发送循环延迟(ms) 用户 密码 ip exchange type 1压缩0不压缩

set ip=10.10.101.35
set exg=test

start %~dp0\RmqPerformanceCustomer.exe 1 # %ip% %exg% topic
start %~dp0\RmqPerformanceProduct.exe 1 a.f 10000000 0 %ip% %exg% topic

start %~dp0\RmqPerformanceCustomer.exe 2 # client client %ip% %exg% topic queue-1 c-recv-1
start %~dp0\RmqPerformanceProduct.exe 2 a.f 10000000 100000 0 client client %ip% %exg% topic 1