#pragma once
#include"INet.h"
#include<list>
#include<map>
class TcpServerNet:public INet
{
public:
	TcpServerNet(INetMediator* pMediator);
	~TcpServerNet();				//ʹ��ʱ����ָ��ָ�����������Ҫʹ��������
	//��ʼ������
	bool InitNet();
	//��������
	bool SendData(long ISendIp,char*buf,int nLen);
	//�ر�����
	void UninitNet();
protected:
	static unsigned int _stdcall AcceptThread(void* IpVoid);
	static unsigned int _stdcall RecvThread(void* IpVoid);
	//��������
	bool RecvData();
	SOCKET m_sock;	//���ڽ������ӵ�socket
	//�����߳̾��
	list<HANDLE>m_hThreadHandleList; //�����߳̾��
	//����ͻ��˺Ͷ�Ӧsock��ӳ���ϵ
	map<unsigned int, SOCKET> m_mapThreadIdToSocket;
	bool m_isStop;
	//INetMediator* pMediator

};