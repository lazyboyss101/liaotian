#include"TcpServerNet.h"
#include"TcpServerMediator.h"
#include"../Kernel.h"

TcpServerMediator::TcpServerMediator()
{
	m_pNet = new TcpServerNet(this);
}
TcpServerMediator::~TcpServerMediator()
{
	if(m_pNet){
		delete m_pNet;
		m_pNet = NULL;
	}
}


//打开网络
bool TcpServerMediator::OpenNet()
{
	if(!m_pNet->InitNet()){
		return false;
	}
	return true;
}
//发送数据
bool TcpServerMediator::SendData(long ISendIp,char* buf,int nLen)
{
		if(!m_pNet->SendData(ISendIp,buf,nLen)){
		return false;
	}
	return true;
}
//关闭网络
void TcpServerMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//处理数据
bool TcpServerMediator::DealData(long ISendIp,char* buf,int nLen)
{
	//TODO发数据发给核心处理类
	cout<<buf<<endl;
	CKernel::pKernel->dealData(ISendIp, buf,nLen);
	return true;
}