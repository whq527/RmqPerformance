taskkill /F /IM RmqPerformanceCustomer.exe
taskkill /F /IM RmqPerformanceProduct.exe

rem customer key �û� ���� ip exchange type ������ �������� 1ѹ��0��ѹ��
rem product key ���� �������� ����ѭ���ӳ�(ms) �û� ���� ip exchange type 1ѹ��0��ѹ��
rem for (��ʼ,����,����)

set ip=10.10.101.35
set exg=test

rem for /l %%i in (1,1,2) do (start %~dp0\RmqPerformanceCustomer.exe  # client client %ip% %exg% topic "" "")
start %~dp0\RmqPerformanceProduct.exe one 10000 100000 1000 client client %ip% %exg% topic 1
