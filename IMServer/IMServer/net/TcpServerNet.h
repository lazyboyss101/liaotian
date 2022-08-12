#pragma once
#include"INet.h"
#include<list>
#include<map>
class TcpServerNet:public INet
{
public:
	TcpServerNet(INetMediator* pMediator);
	~TcpServerNet();				//使用时父类指针指向子类对象，需要使用虚析构
	//初始化网络
	bool InitNet();
	//发送数据
	bool SendData(long ISendIp,char*buf,int nLen);
	//关闭网络
	void UninitNet();
protected:
	static unsigned int _stdcall AcceptThread(void* IpVoid);
	static unsigned int _stdcall RecvThread(void* IpVoid);
	//接收数据
	bool RecvData();
	SOCKET m_sock;	//用于接受连接的socket
	//保存线程句柄
	list<HANDLE>m_hThreadHandleList; //保存线程句柄
	//保存客户端和对应sock的映射关系
	map<unsigned int, SOCKET> m_mapThreadIdToSocket;
	bool m_isStop;
	//INetMediator* pMediator

};