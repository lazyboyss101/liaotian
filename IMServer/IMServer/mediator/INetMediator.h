#pragma once

class INet;
class INetMediator
{
public:
	INetMediator(){};
	virtual~INetMediator(){};	//ʹ��ʱ������ָ��ָ�����������Ҫʹ�������� 
	//������
	virtual bool OpenNet()=0;
	//��������
	virtual bool SendData(long ISendIp,char* buf,int nLen)=0;
	//�ر�����
	virtual void CloseNet()=0;
	//��������
	virtual bool DealData(long ISendIp,char* buf,int nLen)=0;
protected:
	
	INet* m_pNet;
};