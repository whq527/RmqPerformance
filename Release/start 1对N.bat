taskkill /F /IM RmqPerformanceCustomer.exe
taskkill /F /IM RmqPerformanceProduct.exe

rem 1 SocketThread�ӿ� 
rem customer key ip exchange type
rem product key ���� ����ѭ���ӳ�(ms) ip exchange type
rem 2 �½ӿ�
rem customer key �û� ���� ip exchange type ������ �������� 1ѹ��0��ѹ��
rem product key ���� �������� ����ѭ���ӳ�(ms) �û� ���� ip exchange type 1ѹ��0��ѹ��
rem for (��ʼ,����,����)

set ip=10.10.101.35
set exg=test

rem for /l %%i in (1,1,5) do (start %~dp0\RmqPerformanceCustomer.exe 1 # %ip% %exg% topic)
start %~dp0\RmqPerformanceProduct.exe 1 one 10000 1000 %ip% %exg% topic

rem for /l %%i in (1,1,2) do (start %~dp0\RmqPerformanceCustomer.exe 2 # client client %ip% %exg% topic "" "")
start %~dp0\RmqPerformanceProduct.exe 2 one 10000 100000 1000 client client %ip% %exg% topic 1
