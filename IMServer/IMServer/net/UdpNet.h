#pragma once
#include"INet.h"

class UdpNet:public INet
{
public:
	UdpNet(INetMediator* pMediator);
	~UdpNet();				//ʹ��ʱ����ָ��ָ�����������Ҫʹ��������
	//��ʼ������
	bool InitNet();
	//��������
	bool SendData(long ISendIp,char*buf,int nLen);
	//�ر�����
	void UninitNet();
protected:
	static unsigned int _stdcall RecvThread(void* IpVoid);
	//��������
	bool RecvData();
	SOCKET m_sock;
	HANDLE m_handle;
	bool m_isStop;
	//INetMediator* pMediator

};