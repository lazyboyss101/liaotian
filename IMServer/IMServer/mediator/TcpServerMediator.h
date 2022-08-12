#pragma once
#include "INetMediator.h"


class TcpServerMediator : public INetMediator
{
public:
	TcpServerMediator();
	~TcpServerMediator();	//ʹ��ʱ������ָ��ָ�����������Ҫʹ�������� 
	//������
	bool OpenNet();
	//��������
	bool SendData(long ISendIp,char* buf,int nLen);
	//�ر�����
	void CloseNet();
	//��������
	bool DealData(long ISendIp,char* buf,int nLen);
protected:

	INet* m_pNet;
};