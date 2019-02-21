// RmqPerformance.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <windows.h>
#include <iostream>
#include <mutex>
#include <map>
#include <time.h>
#include <iomanip>
#include "cpack.h"
#include "ClibRmq.h"

using namespace std;

struct st_info
{
	string key = "";
	string exchange = "";
	long max_num = 0;//总数
	long count = 0;
	time_t start_time;//最初发送的时间
	time_t last_time;//最后发送的时间
	time_t recv_time;//最后处理的时间
	string recv_msg = "";
	double rate = 0;
	ULONGLONG tk = 0;//第一次的计数器
	ULONGLONG tk_use = 0;//第一次的计数器
};

mutex g_mtx;
std::condition_variable g_cv;
deque<st_cpack> g_recv_list;
map<string, st_info> g_recv_info;

void __stdcall OnRMQData(st_rmq_msg* msg)
{
	st_cpack one = { 0 };
	memcpy(&one, msg->content.bytes, msg->content.len);

	strcpy_s(one.head.hook.hostname, msg->routekey.c_str());//订阅的key
	//one.pack.lvol0 = msg->timestamp;//包发送的时间
	one.pack.lvol0 = one.head.userdata;//包放入队列的时间
	strcpy_s(one.pack.vsvarstr3, msg->exchange.c_str());//exchange
	unique_lock<mutex> ulk(g_mtx);
	g_recv_list.push_back(one);
	g_cv.notify_all();
}

ClibRmq g_rmq2(1);
void test_by_librmq(const char* _key, const char* _user, const char* _psw, const char* _ip, const char* _exchange, const char* _type, const char* _queue, const char* _consumer)
{
	if (g_rmq2.Init(_user, _psw, _ip, 5672, _exchange, _type, 1, 0, true, OnRMQData, _queue, _consumer))
	{
		printf("librmq connect %s ok\n", _ip);
		g_rmq2.Get_Bind(_key);
		//g_rmq2.Get_Bind(_key, "test2");
		g_rmq2.Start();
		printf("librmq customer ok\n");
	}
}

int main(int argc, char*argv[])
{
	if (argc == 9)
	{
		test_by_librmq(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
	}
	else
	{
		test_by_librmq("#", "client", "client", "192.168.3.200", "test", "topic", "queue-1", "c-recv");
	}

	ULONGLONG tk = GetTickCount64();
	size_t waitsize = 0;
	while (true)
	{
		bool work = false;
		st_cpack one_pack = { 0 };

		{
			unique_lock<mutex> ulk(g_mtx);
			if (g_recv_list.size() > 0)
			{
				memcpy(&one_pack, &g_recv_list.front(), sizeof(st_cpack));
				g_recv_list.pop_front();
				waitsize = g_recv_list.size();
				work = true;
			}
		}

		if (one_pack.head.RequestType > 0)
		{
			auto iter = g_recv_info.find(one_pack.head.hook.hostname);
			if (iter == g_recv_info.end())//new
			{
				st_info one;
				one.key = one_pack.head.hook.hostname;
				one.exchange = one_pack.pack.vsvarstr3;
				one.max_num = one_pack.head.retCode;
				one.start_time = one_pack.pack.lvol0;
				one.last_time = one_pack.pack.lvol0;
				one.recv_time = time(NULL);
				one.recv_msg = one_pack.pack.vsvarstr0;
				one.tk = GetTickCount64();
				one.count = one_pack.head.recCount;
				if (one.count >= one.max_num)
				{
					struct tm now_time;
					localtime_s(&now_time, &one.start_time);
					char start_time[256] = { 0 };
					strftime(start_time, 256, "%Y-%m-%d %H:%M:%S", &now_time);
					char last_send[256] = { 0 };
					localtime_s(&now_time, &one.last_time);
					strftime(last_send, 256, "%Y-%m-%d %H:%M:%S", &now_time);
					char recv_time[256] = { 0 };
					localtime_s(&now_time, &one.recv_time);
					strftime(recv_time, 256, "%Y-%m-%d %H:%M:%S", &now_time);
					cout << "开始发的时间 " << start_time << " 最后一笔发时间 " << last_send << " 最后一笔收时间 " << recv_time << " 时间差 " << std::fixed << setprecision(1) << difftime(one.last_time, one.recv_time) << "s " << endl
						<< "key " << one.key << " 收到数量 " << one.count << " 频率 " << std::fixed << setprecision(1) << one.rate << "/秒 收完耗时 " << std::fixed << setprecision(1) << (double)(one.tk_use) / 1000 << " 秒 总耗时 " << std::fixed << setprecision(1) << difftime(one.start_time, one.recv_time) << " 秒" << endl
						<< "消息内容 " << one.recv_msg << " key " << one.key << " exchange " << one.exchange << endl << endl;
				}
				else
				{
					g_recv_info.insert(pair<string, st_info>(one.key, one));
				}
			}
			else
			{
				iter->second.last_time = one_pack.pack.lvol0;
				iter->second.recv_time = time(NULL);
				iter->second.recv_msg = one_pack.pack.vsvarstr0;
				iter->second.count = one_pack.head.recCount;
				iter->second.tk_use = GetTickCount64() - iter->second.tk;
				if (iter->second.tk_use > 0)
					iter->second.rate = (double)iter->second.count / (double)iter->second.tk_use * 1000;
				if (iter->second.count >= iter->second.max_num)
				{
					struct tm now_time;
					localtime_s(&now_time, &iter->second.start_time);
					char start_time[256] = { 0 };
					strftime(start_time, 256, "%Y-%m-%d %H:%M:%S", &now_time);
					char last_send[256] = { 0 };
					localtime_s(&now_time, &iter->second.last_time);
					strftime(last_send, 256, "%Y-%m-%d %H:%M:%S", &now_time);
					char recv_time[256] = { 0 };
					localtime_s(&now_time, &iter->second.recv_time);
					strftime(recv_time, 256, "%Y-%m-%d %H:%M:%S", &now_time);
					cout << "开始发的时间 " << start_time << " 最后一笔发时间 " << last_send << " 最后一笔收时间 " << recv_time << " 时间差 " << std::fixed << setprecision(1) << difftime(iter->second.last_time, iter->second.recv_time) << "s " << endl
						<< "key " << iter->second.key << " 收到数量 " << iter->second.count << " 频率 " << std::fixed << setprecision(1) << iter->second.rate << "/秒 收完耗时 " << std::fixed << setprecision(1) << (double)(iter->second.tk_use) / 1000 << " 秒 总耗时 " << std::fixed << setprecision(1) << difftime(iter->second.start_time, iter->second.recv_time) << " 秒" << endl
						<< "消息内容 " << iter->second.recv_msg << " key " << iter->second.key << " exchange " << iter->second.exchange << endl << endl;
					g_recv_info.erase(iter);
				}
			}
		}

		if (!work)
		{
			unique_lock<mutex> ulk(g_mtx);
			g_cv.wait_for(ulk, std::chrono::milliseconds(100));
		}
	}
}