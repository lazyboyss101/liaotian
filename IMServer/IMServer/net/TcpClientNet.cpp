#include "TcpClientNet.h"
#include"TcpClientMediator.h"
#include<process.h>
#include"PacDef.h"

TcpClientNet::TcpClientNet(INetMediator* pMediator):m_sock(INVALID_SOCKET),m_handle(0),m_isStop(false)
{
	m_pMediator = pMediator;
}
TcpClientNet::~TcpClientNet()
{
	UninitNet();
}

//��ʼ������
bool TcpClientNet::InitNet()
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


	//3����������
	sockaddr_in service;
    service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(_DEF_SERVER_IP);
    service.sin_port = htons(_DEF_TCP_PORT);
	err = connect(m_sock,(SOCKADDR *)&service, sizeof (service));  
	if(err == SOCKET_ERROR){
	    cout<<"connect failed with error %u\n"<<WSAGetLastError()<<endl;
		UninitNet();
        return false;
    }

	//4���������� -- ���������������߳�
	//createThread ����������C++����ʱ�� strcpy �������ٿռ䣬���˳��߳�ʱ������տռ�
	m_handle =(HANDLE) _beginthreadex(NULL,0, &RecvThread,this , 0,NULL); 

	return true;
}

unsigned int _stdcall TcpClientNet::RecvThread(void* IpVoid)

{
	TcpClientNet* pThis = (TcpClientNet*)IpVoid;
	pThis->RecvData();
	return 0;
}

//��������
bool TcpClientNet::SendData(long ISendIp,char*buf, int nLen)
{
	//1.�жϴ��봫������Ƿ�Ϸ�
	if(!buf|| nLen <= 0){
		return false;
	}

	//2.Ϊ�˷�ֹճ�����ȷ�����С  �ٷ�������
	if(send(m_sock,(char*)&nLen,sizeof(int),0) <= 0){
		cout<<"send error"<<WSAGetLastError()<<endl;
		return false;
	}
	if(send(m_sock, buf ,nLen, 0)<=0) {
		cout<<"send error"<<WSAGetLastError()<<endl;
		return false;
	}
	return true;
}

//�ر�����
void TcpClientNet::UninitNet()
{
	////�˳��߳�
	m_isStop = true;
	if (m_handle) {
		if(WAIT_TIMEOUT == WaitForSingleObject(m_handle,100)){
			TerminateThread(m_handle,-1);
		}
		CloseHandle(m_handle);
		m_handle = NULL;
	}

	////�ر��׽���
	if (m_sock && m_sock != INVALID_SOCKET){
		closesocket(m_sock);
	}

	//WSACleanup();
}

//��������
bool TcpClientNet::RecvData()
{
	//char recvBuf[_DEF_BUFFER_SIZE] = "";
	int nPackSize = 0;
	int nRecvNum = 0;
	int offset = 0;

	while(!m_isStop){
		//�Ƚ��հ���С  �ڽ��հ�����
		nRecvNum = recv(m_sock,(char*)&nPackSize,sizeof(int),0);
		if(nRecvNum <= 0){
			break;
		}
		char* recvBuf = new char[nPackSize];
		while(nPackSize){
			nRecvNum = recv(m_sock,recvBuf + offset,nPackSize, 0 );
			if(nRecvNum>0){
				offset += nRecvNum;
				nPackSize -= nRecvNum;
			}
		}
		this->m_pMediator->DealData(m_sock,recvBuf,offset);
		//TODO�� recvBuf�ǵû���
		offset = 0;
	}
	return true;
}

