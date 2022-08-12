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

//初始化网络
bool TcpServerNet::InitNet()
{
	//1、加载库
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
//2、创建套接字

	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET){
        cout<<"socket function failed with error = %d\n"<<WSAGetLastError()<<endl;
       UninitNet();
        return false;
	}


	//3、找地方摆摊--绑定IP地址
	sockaddr_in service;
    service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;//绑定任意网卡
    service.sin_port = htons(_DEF_TCP_PORT);
	err = bind(m_sock,(SOCKADDR *)&service, sizeof (service));  
	if(err == SOCKET_ERROR){
	    cout<<"bind failed with error %u\n"<<WSAGetLastError()<<endl;
		UninitNet();
        return false;
    }

	//4、监听
	err = listen(m_sock,10);
		if(err == SOCKET_ERROR){
	    cout<<"listen failed with error %u\n"<<WSAGetLastError()<<endl;
		UninitNet();
        return false;
    }
	//5、接受连接-- 阻塞函数，创建线程
	//createThread 当函数中有C++运行时库 strcpy 函数开辟空间，在退出线程时不会回收空间
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
		//给每个客户端创建一个对应的接收数据的线程
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
//发送数据
bool TcpServerNet::SendData(long ISendIp,char*buf, int nLen)
{
	//ISendIp里面装的是socket
	//1.判断传入传入参数是否合法
	if(!buf|| nLen <= 0){
		return false;
	}

	//2.为了防止粘包，先发包大小  再发包内容
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

//关闭网络
void TcpServerNet::UninitNet()
{
	////退出线程
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

	////关闭套接字
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

//接收数据
bool TcpServerNet::RecvData()
{
//1、获取与客户端通信的socket
	//因为线程创建以后立刻就执行，但是线程id和socket存入map中也需要时间执行，所以需要先休眠100ms
	Sleep(100);
	SOCKET sock = m_mapThreadIdToSocket[GetCurrentThreadId()];

	//2、判断socket合法性
	if(!sock||sock == INVALID_SOCKET){
		return false;
	}
	//3、接受数据
	int nPackSize = 0; //存包大小
	int nRecvNum = 0;  //实际接收大小
	int offset = 0;		//偏移量

	while(!m_isStop){
		//先接收包大小，再接收包内容
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
		//TODO：recvBuf记得回收
		offset = 0;
	}

	return true;
}

