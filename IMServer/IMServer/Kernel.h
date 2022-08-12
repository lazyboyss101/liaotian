#pragma once
#include "CMySql.h"
#include "INetMediator.h"
#include<iostream>
#include"PacDef.h"
#include<map>

using namespace std;

//����ָ��
class CKernel;
typedef void (CKernel::*PFUN)(long ISendIp,char* buf,int nLen);


class CKernel
{
public:
	CKernel(void);
	~CKernel(void);
	//��������
	bool startServer();
	//�رշ���
	void clossServer();
	//�������ݵĺ���
	void dealData(long ISendIp,char* buf,int nLen);
private:
	//��ʼ��Э��ӳ���
	 void setProtocalMap();
	 //��ȡ�����б������Լ�
	 void getUserList(int id);
	 //����id���û���Ϣ
	 void getUserInfoById(int id,STRU_FRIEND_INFO* info);
	//����ע������
	void dealRegisterRq(long ISendIp,char* buf,int nLen);
	//�����¼����
	void dealLoginRq(long ISendIp,char* buf,int nLen);
	//������������
	void dealChatRq(long ISendIp,char* buf,int nLen);
	//������Ӻ�������
	void dealAddFriendRq(long ISendIp,char* buf,int nLen);
	//������Ӻ��ѻظ�
	void dealAddFriendRs(long ISendIp,char* buf,int nLen);
	//������������
	void dealOfflineRq(long ISendIp,char* buf,int nLen);
public:
	static CKernel* pKernel;
private:
	CMySql m_sql;
	INetMediator* m_pServer;
//Э��ӳ�������ָ�����飩 Э��ͷ-base ->������
    PFUN m_netProtocalMap[_DEF_PROTOCAL_COUNT];
	//8������id��sock��ӳ���ϵ
	map< int,int>m_mapIdToSock;
};

