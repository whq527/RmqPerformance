taskkill /F /IM RmqPerformanceCustomer.exe
taskkill /F /IM RmqPerformanceProduct.exe

//customer key �û� ���� ip exchange type ������ �������� 1ѹ��0��ѹ��
//product key ���� �������� ����ѭ���ӳ�(ms) �û� ���� ip exchange type 1ѹ��0��ѹ��

set ip=192.168.3.200
set exg=test

start %~dp0\RmqPerformanceCustomer.exe # client client %ip% %exg% topic queue-1 c-recv-1
start %~dp0\RmqPerformanceProduct.exe a.f 100000 1000000 0 client client %ip% %exg% topic 1