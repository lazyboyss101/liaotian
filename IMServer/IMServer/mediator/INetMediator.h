#pragma once

class INet;
class INetMediator
{
public:
	INetMediator(){};
	virtual~INetMediator(){};	//使用时，父类指针指向子类对象，需要使用虚析构 
	//打开网络
	virtual bool OpenNet()=0;
	//发送数据
	virtual bool SendData(long ISendIp,char* buf,int nLen)=0;
	//关闭网络
	virtual void CloseNet()=0;
	//处理数据
	virtual bool DealData(long ISendIp,char* buf,int nLen)=0;
protected:
	
	INet* m_pNet;
};