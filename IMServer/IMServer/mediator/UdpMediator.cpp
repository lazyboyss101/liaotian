#include"UdpNet.h"
#include"UdpMediator.h"

UdpMedator::UdpMedator()
{
	m_pNet = new UdpNet(this);
}
UdpMedator::~UdpMedator()
{
	if(m_pNet){
		delete m_pNet;
		m_pNet = NULL;
	}
}


//打开网络
bool UdpMedator::OpenNet()
{
	if(!m_pNet->InitNet()){
		return false;
	}
	return true;
}
//发送数据
bool UdpMedator::SendData(long ISendIp,char* buf,int nLen)
{
		if(!m_pNet->SendData(ISendIp,buf,nLen)){
		return false;
	}
	return true;
}
//关闭网络
void UdpMedator::CloseNet()
{
	m_pNet->UninitNet();
}
//处理数据
bool UdpMedator::DealData(long ISendIp,char* buf,int nLen)
{
	//TODO发数据发给核心处理类
	cout<<buf<<endl;
	return true;
}