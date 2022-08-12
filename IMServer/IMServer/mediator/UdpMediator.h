#pragma once
#include "INetMediator.h"


class UdpMedator : public INetMediator
{
public:
	UdpMedator();
	~UdpMedator();	//使用时，父类指针指向子类对象，需要使用虚析构 
	//打开网络
	bool OpenNet();
	//发送数据
	bool SendData(long ISendIp,char* buf,int nLen);
	//关闭网络
	void CloseNet();
	//处理数据
	bool DealData(long ISendIp,char* buf,int nLen);
protected:
	INet* m_pNet;
};