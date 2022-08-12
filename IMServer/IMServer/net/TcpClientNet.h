#pragma once
#include"INet.h"

class TcpClientNet:public INet
{
public:
	TcpClientNet(INetMediator* pMediator);
	~TcpClientNet();				//ʹ��ʱ����ָ��ָ�����������Ҫʹ��������
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