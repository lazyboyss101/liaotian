#pragma once
#include"INet.h"

class TcpClientNet:public INet
{
public:
	TcpClientNet(INetMediator* pMediator);
	~TcpClientNet();				//使用时父类指针指向子类对象，需要使用虚析构
	//初始化网络
	bool InitNet();
	//发送数据
	bool SendData(long ISendIp,char*buf,int nLen);
	//关闭网络
	void UninitNet();
protected:
	static unsigned int _stdcall RecvThread(void* IpVoid);
	//接收数据
	bool RecvData();
	SOCKET m_sock;
	HANDLE m_handle;
	bool m_isStop;
	//INetMediator* pMediator

};