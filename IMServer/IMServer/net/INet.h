#pragma once
#include<winsock2.h>
#include<iostream>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

class INetMediator;  //������һ��INetMediator���࣬����Ҫ����ͷ�ļ� 
class INet
{
public:
	INet(){};
	virtual~INet(){};	//ʹ��ʱ������ָ��ָ�����������Ҫʹ�������� 
	//��ʼ������
	virtual bool InitNet()=0;
	//��������
	virtual bool SendData(long ISendIp,char*buf,int nLen)=0;
	//�ر�����
	virtual void UninitNet()=0;
protected:
	//��������
	virtual bool RecvData()=0;
	INetMediator* m_pMediator;
};