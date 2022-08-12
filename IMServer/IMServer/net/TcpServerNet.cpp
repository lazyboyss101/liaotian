#include "TcpServerNet.h"
#include"TcpServerMediator.h"
#include<process.h>
#include"PacDef.h"

TcpServerNet::TcpServerNet(INetMediator* pMediator):m_sock(INVALID_SOCKET),/*m_handle(0),*/m_isStop(false)
{
	m_pMediator = pMediator;
}
TcpServerNet::~TcpServerNet()
{
	UninitNet();
}

//��ʼ������
bool TcpServerNet::InitNet()
{
	//1�����ؿ�
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        cout<<"WSAStartup failed with error: %d\n"<< err <<endl;
        return false;
    }

/* Confirm that the WinSock DLL supports 2.2.*/
/* Note that if the DLL supports versions greater    */
/* than 2.2 in addition to 2.2, it will still return */
/* 2.2 in wVersion since that is the version we      */
/* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        cout<<"Could not find a usable version of Winsock.dll\n"<<endl;
        UninitNet();
        return false;
    }
//2�������׽���

	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET){
        cout<<"socket function failed with error = %d\n"<<WSAGetLastError()<<endl;
       UninitNet();
        return false;
	}


	//3���ҵط���̯--��IP��ַ
	sockaddr_in service;
    service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;//����������
    service.sin_port = htons(_DEF_TCP_PORT);
	err = bind(m_sock,(SOCKADDR *)&service, sizeof (service));  
	if(err == SOCKET_ERROR){
	    cout<<"bind failed with error %u\n"<<WSAGetLastError()<<endl;
		UninitNet();
        return false;
    }

	//4������
	err = listen(m_sock,10);
		if(err == SOCKET_ERROR){
	    cout<<"listen failed with error %u\n"<<WSAGetLastError()<<endl;
		UninitNet();
        return false;
    }
	//5����������-- ���������������߳�
	//createThread ����������C++����ʱ�� strcpy �������ٿռ䣬���˳��߳�ʱ������տռ�
	HANDLE handle =(HANDLE) _beginthreadex(NULL,0, &AcceptThread,this , 0,NULL); 
	if(handle){
		m_hThreadHandleList.push_back(handle);
	}
	return true;
}

unsigned int __stdcall TcpServerNet::AcceptThread(void* IpVoid)
{
	TcpServerNet* pThis = (TcpServerNet*)IpVoid;
	sockaddr_in addrClient;
	int nSize=sizeof(addrClient);
	while(!pThis->m_isStop){
		SOCKET sock = accept(pThis->m_sock,(SOCKADDR*)&addrClient,&nSize);
		cout<<"ip"<<inet_ntoa(addrClient.sin_addr)<<endl;
		//��ÿ���ͻ��˴���һ����Ӧ�Ľ������ݵ��߳�
	unsigned int nThreadId = 0;
	HANDLE handle =(HANDLE)_beginthreadex(NULL,0, &RecvThread,pThis , 0,&nThreadId); 
		if(handle){
			pThis->m_hThreadHandleList.push_back(handle);
			pThis->m_mapThreadIdToSocket[nThreadId] = sock;
		}
	}
	return 0;
}

unsigned int _stdcall TcpServerNet::RecvThread(void* IpVoid)

{
	TcpServerNet* pThis = (TcpServerNet*)IpVoid;
	pThis->RecvData();
	return 0;
}
//��������
bool TcpServerNet::SendData(long ISendIp,char*buf, int nLen)
{
	//ISendIp����װ����socket
	//1.�жϴ��봫������Ƿ�Ϸ�
	if(!buf|| nLen <= 0){
		return false;
	}

	//2.Ϊ�˷�ֹճ�����ȷ�����С  �ٷ�������
	if(send(ISendIp,(char*)&nLen,sizeof(int),0) <= 0){
		cout<<"send error"<<WSAGetLastError()<<endl;
		return false;
	}
	if(send(ISendIp, buf ,nLen, 0)<=0) {
		cout<<"send error"<<WSAGetLastError()<<endl;
		return false;
	}
	return true;
}

//�ر�����
void TcpServerNet::UninitNet()
{
	////�˳��߳�
	m_isStop = true;
	for(auto ite =m_hThreadHandleList.begin();ite!=m_hThreadHandleList.end(); ){
		if (*ite) {
			if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100)){
				TerminateThread(*ite,-1);
			}
			CloseHandle(*ite);
			*ite = NULL;
		}
		m_hThreadHandleList.erase(ite);
	}

	////�ر��׽���
	if (m_sock && m_sock != INVALID_SOCKET){
		closesocket(m_sock);
	}
	for(auto ite = m_mapThreadIdToSocket.begin();ite!=m_mapThreadIdToSocket.end();){
		if(ite->second && INVALID_SOCKET != ite ->second) {
			closesocket(ite->second);
		}
		m_mapThreadIdToSocket.erase(ite);
	}
	WSACleanup();
}

//��������
bool TcpServerNet::RecvData()
{
//1����ȡ��ͻ���ͨ�ŵ�socket
	//��Ϊ�̴߳����Ժ����̾�ִ�У������߳�id��socket����map��Ҳ��Ҫʱ��ִ�У�������Ҫ������100ms
	Sleep(100);
	SOCKET sock = m_mapThreadIdToSocket[GetCurrentThreadId()];

	//2���ж�socket�Ϸ���
	if(!sock||sock == INVALID_SOCKET){
		return false;
	}
	//3����������
	int nPackSize = 0; //�����С
	int nRecvNum = 0;  //ʵ�ʽ��մ�С
	int offset = 0;		//ƫ����

	while(!m_isStop){
		//�Ƚ��հ���С���ٽ��հ�����
		nRecvNum = recv(sock,(char*)&nPackSize,sizeof(int),0);
		if(nRecvNum <= 0){
			break;
		}
		char* recvBuf = new char[nPackSize];
		while(nPackSize){
			nRecvNum = recv(sock,recvBuf + offset,nPackSize,0);
			if(nRecvNum > 0){
				offset += nRecvNum;
				nPackSize -= nRecvNum;
			}
		}
		this->m_pMediator->DealData(sock,recvBuf,offset);
		//TODO��recvBuf�ǵû���
		offset = 0;
	}

	return true;
}

