#pragma once
#include "CMySql.h"
#include "INetMediator.h"
#include<iostream>
#include"PacDef.h"
#include<map>

using namespace std;

//函数指针
class CKernel;
typedef void (CKernel::*PFUN)(long ISendIp,char* buf,int nLen);


class CKernel
{
public:
	CKernel(void);
	~CKernel(void);
	//开启服务
	bool startServer();
	//关闭服务
	void clossServer();
	//处理数据的函数
	void dealData(long ISendIp,char* buf,int nLen);
private:
	//初始化协议映射表
	 void setProtocalMap();
	 //获取好友列表，包括自己
	 void getUserList(int id);
	 //根据id查用户信息
	 void getUserInfoById(int id,STRU_FRIEND_INFO* info);
	//处理注册请求
	void dealRegisterRq(long ISendIp,char* buf,int nLen);
	//处理登录请求
	void dealLoginRq(long ISendIp,char* buf,int nLen);
	//处理聊天请求
	void dealChatRq(long ISendIp,char* buf,int nLen);
	//处理添加好友请求
	void dealAddFriendRq(long ISendIp,char* buf,int nLen);
	//处理添加好友回复
	void dealAddFriendRs(long ISendIp,char* buf,int nLen);
	//处理下线请求
	void dealOfflineRq(long ISendIp,char* buf,int nLen);
public:
	static CKernel* pKernel;
private:
	CMySql m_sql;
	INetMediator* m_pServer;
//协议映射表（函数指针数组） 协议头-base ->处理函数
    PFUN m_netProtocalMap[_DEF_PROTOCAL_COUNT];
	//8、保存id和sock的映射关系
	map< int,int>m_mapIdToSock;
};

