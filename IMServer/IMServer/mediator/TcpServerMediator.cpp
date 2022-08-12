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


//������
bool TcpServerMediator::OpenNet()
{
	if(!m_pNet->InitNet()){
		return false;
	}
	return true;
}
//��������
bool TcpServerMediator::SendData(long ISendIp,char* buf,int nLen)
{
		if(!m_pNet->SendData(ISendIp,buf,nLen)){
		return false;
	}
	return true;
}
//�ر�����
void TcpServerMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//��������
bool TcpServerMediator::DealData(long ISendIp,char* buf,int nLen)
{
	//TODO�����ݷ������Ĵ�����
	cout<<buf<<endl;
	CKernel::pKernel->dealData(ISendIp, buf,nLen);
	return true;
}