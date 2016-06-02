/*
 * define file about portable socket class. 
 * description:this sock is suit both windows and linux
 * design:odison
 * e-mail:odison@126.com>
 * 
 */

#ifndef _ODSOCKET_H_
#define _ODSOCKET_H_

#ifdef WIN32
	#include <winsock.h>
	typedef int				socklen_t;
#endif

	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <fcntl.h>

	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
    #include <string.h>
    #include <stdio.h>
	#include "errno.h"
    #include <net/if.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   // #include <ifaddrs.h>
#endif

typedef int				SOCKET;

	//#pragma region define win32 const variable in linux
	#define INVALID_SOCKET	-1
	#define SOCKET_ERROR	-1
	//#pragma endregion

#include "cocos2d.h"

class CBSDSocket
{

public:
	CBSDSocket(SOCKET sock = INVALID_SOCKET);
	~CBSDSocket();

	// Create socket object for snd/recv data
	bool Create(int af, int type, int protocol = 0);

	// Connect socket
	bool Connect(const char* domain, unsigned short port);
    bool Connect(struct sockaddr_in svraddr);

    //#region server
	// Bind socket
	bool Bind(unsigned short port);

	// Listen socket
	bool Listen(int backlog = 5); 

	// Accept socket
	bool Accept(CBSDSocket& s, char* fromip = NULL);
	//#endregion
	bool select();
	// Send socket
	int Send(const char* buf, int len, int flags = 0);

	// Recv socket
	int Recv(char* buf, int len, int flags = 0);
	
	// Close socket
	int Close();

	// Get errno
	int GetError();
	
	//#pragma region just for win32
	// Init winsock DLL 
	static int Init();
    
	// Clean winsock DLL
	static int Clean();
    
	//#pragma endregion
    bool isLocalWIFI();
    
	// Domain parse
	static bool DnsParse(const char* domain, char* ip);
    
	CBSDSocket& operator = (SOCKET s);

	operator SOCKET ();

protected:
	SOCKET m_sock;
};

int check_conn_is_ok(SOCKET sock);
#endif
