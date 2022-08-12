#pragma once
#include "INetMediator.h"


class UdpMedator : public INetMediator
{
public:
	UdpMedator();
	~UdpMedator();	//ʹ��ʱ������ָ��ָ�����������Ҫʹ�������� 
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