#pragma once
#include<winsock2.h>
#include<iostream>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

class INetMediator;  //生命有一个INetMediator的类，不需要包含头文件 
class INet
{
public:
	INet(){};
	virtual~INet(){};	//使用时，父类指针指向子类对象，需要使用虚析构 
	//初始化网络
	virtual bool InitNet()=0;
	//发送数据
	virtual bool SendData(long ISendIp,char*buf,int nLen)=0;
	//关闭网络
	virtual void UninitNet()=0;
protected:
	//接收数据
	virtual bool RecvData()=0;
	INetMediator* m_pMediator;
};