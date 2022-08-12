#pragma once

#define _DEF_UDP_PORT (12345)
#define _DEF_BUFFER_SIZE (4096)
#define _DEF_NAME_SIZE  (60)
#define _DEF_CONTENT_SIZE  (1024)
#define _DEF_SERVER_IP   ("10.101.206.65")
#define _DEF_TCP_PORT	(54321)
#define _DEF_MAX_SIZE	(100)
#define _DEF_PROTOCAL_COUNT (100)

//Э��ͷ
#define _DEF_PROTOCAL_BASE (1000)
//UDPЭ��ͷ
//����
#define _DEF_PROTOCAL_ONLINE_RQ (_DEF_PROTOCAL_BASE + 1)
#define _DEF_PROTOCAL_ONLINE_RS (_DEF_PROTOCAL_BASE + 2)
//����
#define _DEF_PROTOCAL_OFFLINE_RQ (_DEF_PROTOCAL_BASE + 3)
//����
#define _DEF_PROTOCAL_CHAT_RQ	(_DEF_PROTOCAL_BASE + 4)
//���ļ�
#define _DEF_PROTOCAL_FILEINFO_RQ (_DEF_PROTOCAL_BASE + 5)

//TCPЭ��ͷ
//ע��
#define _DEF_PACK_REGISTER_RQ	(_DEF_PROTOCAL_BASE+10)
#define _DEF_PACK_REGISTER_RS	(_DEF_PROTOCAL_BASE+11)

//��¼
#define _DEF_PACK_LOGIN_RQ		(_DEF_PROTOCAL_BASE+12)
#define _DEF_PACK_LOGIN_RS		(_DEF_PROTOCAL_BASE+13)
//������Ϣ
#define _DEF_PACK_FRIEND_INFO	(_DEF_PROTOCAL_BASE+14)
//����
#define _DEF_PACK_CHAT_RQ		(_DEF_PROTOCAL_BASE+15)
#define _DEF_PACK_CHAT_RS		(_DEF_PROTOCAL_BASE+16)
//��Ӻ���
#define _DEF_PACK_ADDFRIEND_RQ	(_DEF_PROTOCAL_BASE+17)
#define _DEF_PACK_ADDFRIEND_RS	(_DEF_PROTOCAL_BASE+18)
//����
#define _DEF_PACK_OFFLINE_RQ	(_DEF_PROTOCAL_BASE+19)

//tcp���ؽ��
//ע������Ľ��
#define user_is_exist		(0)
#define register_success	(1)
//��¼����Ľ��
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)
//��Ӻ�������Ľ��
#define no_this_user		(0)
#define user_refuse			(1)
#define user_offline		(2)
#define add_success			(3)
//����״̬
#define status_online		(0)
#define status_offline		(1)
//������Ϣ���ͽ��
#define send_error			(0)
#define send_success		(1)
#define user_offline		(2)




//����ṹ��
//UDPЭ��

//����֪ͨ���Լ���ip, �ҵ�������
//���߻ظ����Լ���ip���ҵ�������

struct STRU_ONLINE{
	STRU_ONLINE():nType(_DEF_PROTOCAL_ONLINE_RQ){
		memset(szName, 0 ,_DEF_NAME_SIZE);
	}
	int nType;
	//long ip;
	char szName[_DEF_NAME_SIZE];
};

//����֪ͨ���Լ���ip
struct STRU_OFFONLINE_RQ{
	STRU_OFFONLINE_RQ():nType(_DEF_PROTOCAL_ONLINE_RQ){

	}
	int nType;
};
//���������������ݡ��Լ���ip
struct STRU_CHAT_RQ{
	STRU_CHAT_RQ():nType(){
	memset(szContent, 0 ,_DEF_CONTENT_SIZE);
	}
	int nType;
	char szContent[_DEF_CONTENT_SIZE];
};


//TCPЭ��
typedef int PackType;
//ע��
typedef struct STRU_REGISTER_RQ{
	STRU_REGISTER_RQ():type(_DEF_PACK_REGISTER_RQ)
	{
		memset(tel, 0, _DEF_MAX_SIZE);
		memset(name, 0, _DEF_MAX_SIZE);
		memset(password, 0, _DEF_MAX_SIZE); 
	}
	//Э��ͷ���ֻ��š��ǳơ�����
	PackType type;
	char tel[_DEF_MAX_SIZE];
	char name[_DEF_MAX_SIZE];
	char password[_DEF_MAX_SIZE];
}STRU_REGISTER_RQ;
typedef struct STRU_REGISTER_RS{
	STRU_REGISTER_RS():type(_DEF_PACK_REGISTER_RS),result(register_success){

	}
	//Э��ͷ��ע����
	PackType type;
	int result;
}STRU_REGISTER_RS;
//��¼
typedef struct STRU_LOGIN_RQ{
	STRU_LOGIN_RQ():type(_DEF_PACK_LOGIN_RQ){
		memset(tel, 0, _DEF_MAX_SIZE);
		memset(password, 0, _DEF_MAX_SIZE); 
	}
	//Э��ͷ���ֻ��š�����
	PackType type;
	char tel[_DEF_MAX_SIZE];
	char password[_DEF_MAX_SIZE];
}STRU_LOGIN_RQ;

typedef struct STRU_LOGIN_RS{
	STRU_LOGIN_RS():type(_DEF_PACK_LOGIN_RS),result(login_success),userid(0){
	}
	//Э��ͷ���ֻ��š�����
	PackType type;
	int result;
	int userid;
}STRU_LOGIN_RS;



//������Ϣ
typedef struct STRU_FRIEND_INFO{
	STRU_FRIEND_INFO():type(_DEF_PACK_FRIEND_INFO),userId(0),icon(0),state(status_online){
		memset(tel, 0, _DEF_MAX_SIZE);
		memset(name, 0, _DEF_MAX_SIZE);
		memset(feeling, 0, _DEF_MAX_SIZE); 
	}
	//Э��ͷ���û�id���ֻ��š��ǳơ�ǩ����״̬��ͷ��
	PackType type;
	int userId;
	int icon;
	int state;
	char tel[_DEF_MAX_SIZE];
	char name[_DEF_MAX_SIZE];
	char feeling[_DEF_MAX_SIZE];
}STRU_FRIEND_INFO;

//����
typedef struct STRU_TCP_CHAT_RQ{
	STRU_TCP_CHAT_RQ():type(_DEF_PACK_CHAT_RQ),userId(0),friendId(0){
		memset(content, 0, _DEF_CONTENT_SIZE);
	}
	//Э��ͷ���������ݡ����ͷ�id�����շ�id
	PackType type;
	int userId;
	int friendId;
	char content[_DEF_CONTENT_SIZE];
}STRU_TCP_CHAT_RQ;

typedef struct STRU_TCP_CHAT_RS{
	STRU_TCP_CHAT_RS():type(_DEF_PACK_CHAT_RS),userId(0),friendId(0),result(send_success){
	}
	//Э��ͷ�����ͷ�id�����շ�id������������
	PackType type;
	int userId;
	int friendId;
	int result;
}STRU_TCP_CHAT_RS;

//��Ӻ���
typedef struct STRU_ADD_FRIEND_RQ{
	STRU_ADD_FRIEND_RQ():type(_DEF_PACK_ADDFRIEND_RQ),userId(0){
		memset(userName, 0, _DEF_MAX_SIZE);
		memset(friendName, 0, _DEF_MAX_SIZE);

	}
	//Э��ͷ���û�1���û�1���ǳơ��û�2���ǳ�
	PackType type;
	int userId;
	char userName[_DEF_MAX_SIZE];
	char friendName[_DEF_MAX_SIZE];
}STRU_ADD_FRIEND_RQ;

typedef struct STRU_ADD_FRIEND_RS{
	STRU_ADD_FRIEND_RS():type(_DEF_PACK_ADDFRIEND_RS),userId(0),friendId(0),result(add_success){
		memset(friendName, 0, _DEF_MAX_SIZE);
	}
	//Э��ͷ���û�1���û�2��id���û�2�ǳ�
	PackType type;
	int userId;
	int friendId;
	char friendName[_DEF_MAX_SIZE];
	int result;
}STRU_ADD_FRIEND_RS;

//����
typedef struct STRU_TCP_OFFLINE_RQ{
	STRU_TCP_OFFLINE_RQ():type(_DEF_PACK_OFFLINE_RQ),userId(0){
	}
	//Э��ͷ���û�id
	PackType type;
	int userId;
}STRU_TCP_OFFLINE_RQ;