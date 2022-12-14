#include "UdpNet.h"
#include<process.h>
#include"UdpMediator.h"
#include"PacDef.h"

UdpNet::UdpNet(INetMediator* pMediator):m_sock(INVALID_SOCKET),m_handle(0),m_isStop(false)
{
	m_pMediator = pMediator;
}
UdpNet::~UdpNet()
{
	UninitNet();
}

//初始化网络
bool UdpNet::InitNet()
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

	m_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (m_sock == INVALID_SOCKET){
        cout<<"socket function failed with error = %d\n"<<WSAGetLastError()<<endl;
       UninitNet();
        return false;
	}
	//有限广播，广播权限
	BOOL bval = TRUE;
	setsockopt(m_sock,SOL_SOCKET,SO_BROADCAST,(char*)&bval,sizeof(bval));


	//3、找地方摆摊--绑定IP地址
	sockaddr_in service;
    service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;//绑定任意网卡
    service.sin_port = htons(_DEF_UDP_PORT);
	err = bind(m_sock,(SOCKADDR *)&service, sizeof (service));  
	if(err == SOCKET_ERROR){
	    cout<<"bind failed with error %u\n"<<WSAGetLastError()<<endl;
		UninitNet();
        return false;
    }

	//4、TODO:接收数据 -- 阻塞函数，创建线程
	//createThread 当函数中有C++运行时库 strcpy 函数开辟空间，在退出线程时不会回收空间
	m_handle =(HANDLE) _beginthreadex(NULL,0, &RecvThread,this , 0,NULL); 

	return true;
}

unsigned int _stdcall UdpNet::RecvThread(void* IpVoid)
{
	UdpNet* pThis = (UdpNet*)IpVoid;
	pThis->RecvData();
	return 0;
}

//发送数据
bool UdpNet::SendData(long ISendIp,char*buf, int nLen)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_DEF_UDP_PORT);
	addr.sin_addr.S_un.S_addr = ISendIp;
	if(sendto(m_sock,buf,nLen,0,(sockaddr*)&addr,sizeof(addr))<=0){
		cout<<"sendto error:"<<WSAGetLastError()<<endl;
		return false;
	}
	return true;
}

//关闭网络
void UdpNet::UninitNet()
{
	//退出线程
	m_isStop = true;
	if (m_handle) {
		if(WAIT_TIMEOUT == WaitForSingleObject(m_handle,100)){
			TerminateThread(m_handle,-1);
		}
		CloseHandle(m_handle);
		m_handle = NULL;
	}

	//关闭套接字
	if (m_sock && m_sock != INVALID_SOCKET){
		closesocket(m_sock);
	}

	WSACleanup();
}

//接收数据
bool UdpNet::RecvData()
{
	char recvBuf[_DEF_BUFFER_SIZE] = "";
	SOCKADDR_IN addrClient;
	int nSize = sizeof(addrClient);
	int nRecvNum = 0;
	m_isStop = false;
	while(!m_isStop) {
		nRecvNum = recvfrom(m_sock,recvBuf,_DEF_BUFFER_SIZE,0,(sockaddr*)&addrClient,&nSize);
		if(nRecvNum > 0) {
			char*pack = new char[nRecvNum];
			if(pack) {
				memcpy(pack,recvBuf,nRecvNum);  //数据拷贝
				m_pMediator->DealData(m_sock,pack, nRecvNum);
			} 
		}else{
			cout<<"recvfrom error:" << WSAGetLastError()<<endl;
		}
	}
	return true;
}

