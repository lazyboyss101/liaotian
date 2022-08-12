#include<iostream>
#include "Kernel.h"
using namespace std;

int main()
{
	CKernel kernel;
	if(kernel.startServer()){
	while(1){
		Sleep(5000);
		cout<<"server is running"<<endl;
		}
	}else {
		cout<<"start server fail"<<endl;
	}


	system ("pause");
	return 0;
}