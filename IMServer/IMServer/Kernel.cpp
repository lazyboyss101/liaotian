#include "Kernel.h"
#include"TcpServerMediator.h"


//��̬��Ա�����������ʼ��
CKernel* CKernel::pKernel = NULL;
#define NetProtocalMap(a) m_netProtocalMap[a - _DEF_PROTOCAL_BASE-10]

//��ʼ��Э��ӳ���
void CKernel::setProtocalMap()
{
   memset(m_netProtocalMap, 0, _DEF_PROTOCAL_COUNT);
   NetProtocalMap(_DEF_PACK_REGISTER_RQ)    = &CKernel::dealRegisterRq;
   NetProtocalMap(_DEF_PACK_LOGIN_RQ)       = &CKernel::dealLoginRq;
   NetProtocalMap(_DEF_PACK_CHAT_RQ)		=&CKernel::dealChatRq;
   NetProtocalMap(_DEF_PACK_ADDFRIEND_RQ)	=&CKernel::dealAddFriendRq;
   NetProtocalMap(_DEF_PACK_ADDFRIEND_RS)	=&CKernel::dealAddFriendRs;
   NetProtocalMap(_DEF_PACK_OFFLINE_RQ)	=&CKernel::dealOfflineRq;
}

CKernel::CKernel(void):m_pServer(NULL)
{
	pKernel=this;
	setProtocalMap();
}


CKernel::~CKernel(void)
{
	clossServer();
}
//��������
bool CKernel::startServer()
{
	//1���������ݿ�
	if(!m_sql.ConnectMySql("127.0.0.1","root","root","0716im")){
		cout<<"�������ݿ�ʧ��"<<endl;
	}
	//2����������
	m_pServer = new TcpServerMediator;
	if(!m_pServer->OpenNet()){
		cout<<"������ʧ��"<<endl;
		return false;
	}
	return true;
}
//�رշ���
void CKernel::clossServer()
{
	//��Դ����
	//1���Ͽ������ݿ������
	m_sql.DisConnect();
	//2���ر�����
	if(m_pServer){
		m_pServer->CloseNet();
		delete m_pServer;
		m_pServer = NULL;
	}
}

//��������
void CKernel::dealData(long ISendIp,char* buf,int nLen)
{
	  //1����ȡЭ��ͷ
    int nType = *(int*)buf;
    //2���ж�Э��ͷ�Ƿ���ӳ�䷶Χ��
    if(nType >= (_DEF_PROTOCAL_BASE + 10)&&
       nType<=(_DEF_PROTOCAL_BASE + 10 +_DEF_PROTOCAL_COUNT)){
        //3�����������±꣬��ȡ��Ӧ����
        PFUN pf = NetProtocalMap(nType);
        //4�����ָ����ڣ����ö�Ӧ�Ĵ�����
        if(pf){
            (this->*pf)(ISendIp,buf,nLen);
        }
    }
    //5������buf
    delete[]buf;
}

//����ע������
void CKernel::dealRegisterRq(long ISendIp,char* buf,int nLen)
{
	cout<<"dealRegisterRq"<<endl;
	//1�����
	STRU_REGISTER_RQ* rq = (STRU_REGISTER_RQ*)buf;
	//2��ȡ��tel��passw��name���жϺϷ��ԣ�����tel���
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select tel from t_user where tel = %s;",rq->tel);
	if(!m_sql.SelectMySql(sqlBuf,1,resultList)){
		cout<<"SelectMysql error:"<<sqlBuf<<endl;
		return;
	}
	//3���ж�tel�Ƿ��Ѿ���ע��
	STRU_REGISTER_RS rs;
	if(resultList.size()  > 0){
		//�绰�ű�ע��,�ظ��û��Ѵ���
		rs.result = user_is_exist;
	}else{
		//4�����û�б�ע�ᣬ�����ݿ���д��ע������
		rs.result = register_success;
		sprintf(sqlBuf,"insert into t_user(tel,password,name,icon,feeling) values ('%s','%s','%s',1,'�Ƚ�����ʲôҲûд')",
			rq->tel,rq->password,rq->name);
		if(!m_sql.UpdateMySql(sqlBuf)){
			cout<<"UpdateMysql error :"<<sqlBuf <<endl;
			return;
		}
	}
	//5�����ͻ��˷���ע����
	m_pServer->SendData(ISendIp, (char*)&rs,sizeof(rs));
}

//�����¼����
void CKernel::dealLoginRq(long ISendIp,char* buf,int nLen){
	cout<<"dealLoginRq"<<endl;
	//1�������ȡ��tel��password
	STRU_LOGIN_RQ* rq = (STRU_LOGIN_RQ*)buf;
	//2������tel�鿴�Ƿ�������û�
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select id, password from t_user where tel = %s;", rq->tel);
	if(!m_sql.SelectMySql(sqlBuf,2,resultList)){
		cout<<"SelectMysql error :"<<sqlBuf<<endl;
		return;
	}
	//3��û������û���ֱ�ӷ��ص�¼ʧ��
	STRU_LOGIN_RS rs;
	int nId = 0;
	string strPass;
	if(0 == resultList.size()){
		rs.result = user_not_exist;
		//���ͻ��˻ظ���¼���
		m_pServer->SendData(ISendIp,(char*)&rs,sizeof(rs));
	}else{
		//4�����������û�����ȡ����û���id������
		nId = atoi(resultList.front().c_str());
		resultList.pop_front();

		strPass = resultList.front();
		resultList.pop_front();

		//5���ж������Ƿ�һ��
		if(0 != strcmp(strPass.c_str(),rq->password)){
			//6�����벻һ�£�ֱ�ӷ��ص�¼ʧ��
			rs.result = password_error;
			//���ͻ��˻ظ���¼���
			m_pServer->SendData(ISendIp,(char*)&rs,sizeof(rs));
		}else{
			//7������һ�£����ص�½�ɹ����û�id
			rs.result = login_success;
			rs.userid = nId;
				//���ͻ��˻ظ���¼���
	m_pServer->SendData(ISendIp,(char*)&rs,sizeof(rs));

			//8������id��sock��ӳ���ϵ
			m_mapIdToSock[nId] = ISendIp;

			//9����ȡ�����б�
			getUserList(nId);
		}
	}
}

//��ȡ�����б������Լ�
void CKernel::getUserList(int id){
	//1����ѯ�Լ�����Ϣ
	STRU_FRIEND_INFO loginInfo;
	getUserInfoById(id, &loginInfo);
	//2���û����׽��֣�����id��map��ȡ��
	if(m_mapIdToSock.find(id) == m_mapIdToSock.end()){
		cout<< "user"<< id << " is not login." <<endl;
		return;
	}
	SOCKET loginSock = m_mapIdToSock[id];
	//3�����Լ�����Ϣ���͸��ͻ���
	m_pServer->SendData(loginSock, (char*)&loginInfo, sizeof(loginInfo));
	//4�������û�id�������еĺ���id
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select idB from t_friend where idA = %d;",id);
	if(!m_sql.SelectMySql(sqlBuf,1,resultList)){
		cout<<"SelectMysql error :"<<sqlBuf<<endl;
		return;
	}
	//5���������ҽ��
	int friendId = 0;
	STRU_FRIEND_INFO friendInfo;
	while(resultList.size() > 0){
		//6��ȡ������id�����ݺ���id���Һ�����Ϣ
		friendId = atoi(resultList.front().c_str());
		resultList.pop_front();
		getUserInfoById(friendId, &friendInfo);
		//7���Ѻ�����Ϣ�����ͻ���
		m_pServer->SendData(loginSock, (char*)&friendInfo, sizeof(friendInfo));
		//8������������ߣ�֪ͨ���ѵ�¼��������
		if(m_mapIdToSock.find(friendId) == m_mapIdToSock.end()){
			continue;	//������Ѳ����ߣ� ������һ��ѭ��
		}
		//9������������ߣ�ȡ�����ѵ��׽��֣������ѷ��͵�¼�˵�����֪ͨ
		SOCKET friendSock = m_mapIdToSock[friendId];
		m_pServer->SendData(friendSock, (char*)&friendInfo, sizeof(friendInfo));
	
	}
}
//����id���û���Ϣ
void CKernel::getUserInfoById(int id, STRU_FRIEND_INFO* info){
	//1������id
	info->userId = id;
	//2������id��ѯ�û���Ϣ
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select name, icon, feeling from t_user where id = %d;",id);
	if(!m_sql.SelectMySql(sqlBuf,3,resultList)){
		cout<<"SelectMysql error :"<<sqlBuf<<endl;
		return;
	}
	if(3 == resultList.size()){
		strcpy(info->name, resultList.front().c_str());
		resultList.pop_front();

		info->icon = atoi(resultList.front().c_str());
		resultList.pop_front();

		strcpy(info->feeling, resultList.front().c_str());
		resultList.pop_front();
	}
	
	//3���ж��û��Ƿ�����
	if(m_mapIdToSock.find(id) != m_mapIdToSock.end()){
		info->state = 1; //�ҵ�������
	
	}else{
		info->state = 0;
	}

}

//������������
void CKernel::dealChatRq(long isendip,char* buf,int nlen){
	//1�����
	STRU_TCP_CHAT_RQ* rq = (STRU_TCP_CHAT_RQ*)buf;
	//2�������շ��Ƿ�����
	if (m_mapIdToSock.find(rq->friendId) != m_mapIdToSock.end()){
		//2.1������Է����ߣ���ȡ�Է��׽��֣�ת������
		SOCKET sock = m_mapIdToSock[rq->friendId];
		m_pServer->SendData(sock, buf, nlen);
	}else{
		//2.2������Է������ߣ��ظ��ͻ��˺��Ѳ�����
		STRU_TCP_CHAT_RS rs;
		rs.result = user_offline;
		rs.userId = rq->userId;
		rs.friendId = rq->friendId;
		m_pServer->SendData(isendip, (char*)&rs, sizeof(rs));
	}
}

//������Ӻ�������
void CKernel::dealAddFriendRq(long ISendIp,char* buf,int nLen){
	//1�����
	STRU_ADD_FRIEND_RQ* rq = (STRU_ADD_FRIEND_RQ*)buf;
	//2���鿴����Ӻ����Ƿ����
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select id from t_user where name = '%s';",rq->friendName);
	if(!m_sql.SelectMySql(sqlBuf,1,resultList)){
		cout<<"SelectMysql error :"<<sqlBuf<<endl;
		return;
	}
	//3���������Ӻ��Ѵ��ڣ�ȡ��id
	if (resultList.size() > 0){
		int friendId = atoi(resultList.front().c_str());
		resultList.pop_front();
	
	//4��������Ӻ����Ƿ�����
		if(m_mapIdToSock.count(friendId) > 0){
			//4.1��������ߣ�ȡ��sock��������Ӻ�������
			SOCKET sock = m_mapIdToSock[friendId];
			m_pServer->SendData(sock,buf,nLen);
		}else{
			//4.2����������ߣ��ظ��ͻ��˲�����
			STRU_ADD_FRIEND_RS rs;
			rs.result = user_offline;
			rs.friendId = friendId;
			rs.userId = rq->userId;
			strcpy(rs.friendName, rq->friendName);
			m_pServer->SendData(ISendIp, (char*)&rs, sizeof(rs));
		}
	}else{
		//5���������Ӻ��Ѳ����ڣ� �ظ��ͻ��˺��Ѳ�����
			STRU_ADD_FRIEND_RS rs;
			rs.result = no_this_user;
			rs.friendId = 0;
			rs.userId = rq->userId;
			strcpy(rs.friendName, rq->friendName);
			m_pServer->SendData(ISendIp, (char*)&rs, sizeof(rs));
	}
}

//������Ӻ��ѻظ�
void CKernel::dealAddFriendRs(long ISendIp,char* buf,int nLen){
	//1�����
	STRU_ADD_FRIEND_RS* rs = (STRU_ADD_FRIEND_RS*)buf;
	//2���ж϶Է��Ƿ�ͬ��
	if(add_success == rs->result){
		//3�����ͬ�⣬�����ݿ�����Ӻ��ѹ�ϵ
		char sqlBuf[1024] = "";
		sprintf(sqlBuf, "insert into t_friend values(%d, %d);",rs->friendId, rs->userId);
		if(!m_sql.UpdateMySql(sqlBuf)){
		cout<<"UpdateMySql error :"<<sqlBuf<<endl;
	}
		sprintf(sqlBuf, "insert into t_friend values(%d, %d);",rs->userId, rs->friendId);
		if(!m_sql.UpdateMySql(sqlBuf)){
		cout<<"UpdateMySql error :"<<sqlBuf<<endl;
		//4�����º����б�
		getUserList(rs->friendId);
	}

	//5�������Ƿ�ͬ�⣬��Ҫת���ظ�
		if(m_mapIdToSock.count(rs->userId)>0){
			SOCKET sock = m_mapIdToSock[rs->userId];
			m_pServer->SendData(sock, buf, nLen);
		}
}
}
//������������
void CKernel::dealOfflineRq(long ISendIp,char* buf,int nLen){
	//1�����
	STRU_TCP_OFFLINE_RQ* rq = (STRU_TCP_OFFLINE_RQ*)buf;
	//2����ȡ�����û���id
	int userId = rq->userId;
	//3�����������û���id��ѯ���û��ĺ���id
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select idB from t_friend where idA = %d;",userId);
	if(!m_sql.SelectMySql(sqlBuf,1,resultList)){
		cout<<"SelectMysql error :"<<sqlBuf<<endl;
		return;
	}
	//4��������ѯ���
	int friendId = 0;
	while(resultList.size() > 0){
		//5�������ߺ��ѷ�������֪ͨ
		//5.1ȡ������id
		friendId = atoi(resultList.front().c_str());
		resultList.pop_front();
		//5.2�жϺ����Ƿ�����
		if(m_mapIdToSock.count(friendId) > 0){
			//5.3��������֪ͨ
			SOCKET sock = m_mapIdToSock[friendId];
			m_pServer->SendData(sock,buf,nLen);
		}
		
	}

	//6���������û���socket��map���Ƴ�
	if(m_mapIdToSock.count(userId) > 0){
		m_mapIdToSock.erase(userId);
	}
}