taskkill /F /IM RmqPerformanceCustomer.exe
taskkill /F /IM RmqPerformanceProduct.exe

//1 SocketThread�ӿ� 
//customer key ip exchange type
//product key ���� ����ѭ���ӳ�(ms) ip exchange type
//2 �½ӿ�
//customer key �û� ���� ip exchange type ������ �������� 1ѹ��0��ѹ��
//product key ���� �������� ����ѭ���ӳ�(ms) �û� ���� ip exchange type 1ѹ��0��ѹ��

set ip=10.10.101.35
set exg=test

start %~dp0\RmqPerformanceCustomer.exe 1 # %ip% %exg% topic
start %~dp0\RmqPerformanceProduct.exe 1 a.f 10000000 0 %ip% %exg% topic

start %~dp0\RmqPerformanceCustomer.exe 2 # client client %ip% %exg% topic queue-1 c-recv-1
start %~dp0\RmqPerformanceProduct.exe 2 a.f 10000000 100000 0 client client %ip% %exg% topic 1