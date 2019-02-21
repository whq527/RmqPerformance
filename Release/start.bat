taskkill /F /IM RmqPerformanceCustomer.exe
taskkill /F /IM RmqPerformanceProduct.exe

//customer key 用户 密码 ip exchange type 队列名 消费者名 1压缩0不压缩
//product key 总数 主动限流 发送循环延迟(ms) 用户 密码 ip exchange type 1压缩0不压缩

set ip=192.168.3.200
set exg=test

start %~dp0\RmqPerformanceCustomer.exe # client client %ip% %exg% topic queue-1 c-recv-1
start %~dp0\RmqPerformanceProduct.exe a.f 100000 1000000 0 client client %ip% %exg% topic 1