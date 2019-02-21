taskkill /F /IM RmqPerformanceCustomer.exe
taskkill /F /IM RmqPerformanceProduct.exe

rem 1 SocketThread接口 
rem customer key ip exchange type
rem product key 总数 发送循环延迟(ms) ip exchange type
rem 2 新接口
rem customer key 用户 密码 ip exchange type 队列名 消费者名 1压缩0不压缩
rem product key 总数 主动限流 发送循环延迟(ms) 用户 密码 ip exchange type 1压缩0不压缩
rem for (起始,步进,结束)

set ip=10.10.101.35
set exg=test

rem for /l %%i in (1,1,5) do (start %~dp0\RmqPerformanceCustomer.exe 1 # %ip% %exg% topic)
start %~dp0\RmqPerformanceProduct.exe 1 one 10000 1000 %ip% %exg% topic

rem for /l %%i in (1,1,2) do (start %~dp0\RmqPerformanceCustomer.exe 2 # client client %ip% %exg% topic "" "")
start %~dp0\RmqPerformanceProduct.exe 2 one 10000 100000 1000 client client %ip% %exg% topic 1
