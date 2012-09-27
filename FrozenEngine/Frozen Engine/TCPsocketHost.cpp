#include "TCPsocket.h"
#include "xmlSettingsHandling.h"

CHostSocket::CHostSocket(CIPAddress& sdlIpAddress ) 
	: log("TcpSocketHost", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	IPaddress hostIp = sdlIpAddress.GetIPAddress();
	if(!(_socket = SDLNet_TCP_Open(&hostIp)))
	{
		SDLNet_FreeSocketSet(_socketSet);
	}
}

CHostSocket::CHostSocket( Uint32 port ) 
	: log("TcpSocketHost", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	CIPAddress iplistener(port);
	if (!iplistener.ReadyForUse()) 
	{
		_socket = NULL;
		log << WARN << "IPaddress not ready for use" << endl;
	}
	else 
	{
		IPaddress ipHost = iplistener.GetIPAddress();
		if (!(_socket = SDLNet_TCP_Open(&ipHost))) 
		{
			SDLNet_FreeSocketSet(_socketSet);
			log << ERRORX << "SDLNet_TCP_Open: " << SDLNet_GetError() << endl;
		}
		else 
		{
			log << INFO << "Listening for clients on port: " << port << endl;
		}
	}

}

CHostSocket::~CHostSocket()
{

}

bool CHostSocket::Accept(CClientSocket& clientSocket)
{
	TCPsocket clientSock;
	
	if((clientSock = SDLNet_TCP_Accept(_socket)))
	{
		clientSocket.SetSocket(clientSock);
		return true;
	}
	else{return false;}
}
