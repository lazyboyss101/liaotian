#include"TcpClientNet.h"
#include"TcpClientMediator.h"

TcpClientMediator::TcpClientMediator()
{
	m_pNet = new TcpClientNet(this);
}
TcpClientMediator::~TcpClientMediator()
{
	if(m_pNet){
		delete m_pNet;
		m_pNet = NULL;
	}
}


//打开网络
bool TcpClientMediator::OpenNet()
{
	if(!m_pNet->InitNet()){
		return false;
	}
	return true;
}
//发送数据
bool TcpClientMediator::SendData(long ISendIp,char* buf,int nLen)
{
		if(!m_pNet->SendData(ISendIp,buf,nLen)){
		return false;
	}
	return true;
}
//关闭网络
void TcpClientMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//处理数据
bool TcpClientMediator::DealData(long ISendIp,char* buf,int nLen)
{
	//TODO发数据发给核心处理类
	cout<<buf<<endl;
	return true;
}