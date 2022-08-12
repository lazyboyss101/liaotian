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


//������
bool UdpMedator::OpenNet()
{
	if(!m_pNet->InitNet()){
		return false;
	}
	return true;
}
//��������
bool UdpMedator::SendData(long ISendIp,char* buf,int nLen)
{
		if(!m_pNet->SendData(ISendIp,buf,nLen)){
		return false;
	}
	return true;
}
//�ر�����
void UdpMedator::CloseNet()
{
	m_pNet->UninitNet();
}
//��������
bool UdpMedator::DealData(long ISendIp,char* buf,int nLen)
{
	//TODO�����ݷ������Ĵ�����
	cout<<buf<<endl;
	return true;
}