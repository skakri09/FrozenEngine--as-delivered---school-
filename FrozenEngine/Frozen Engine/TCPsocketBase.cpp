#include "TCPsocket.h"
#include "xmlSettingsHandling.h"
CTcpSocketBase::CTcpSocketBase() : log("TCPsocketBase", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	_socket = NULL;
	_socketSet = SDLNet_AllocSocketSet(1);
	
}

CTcpSocketBase::~CTcpSocketBase()
{
	if(!(_socket == NULL))
	{
		SDLNet_TCP_DelSocket(_socketSet, _socket);
		SDLNet_FreeSocketSet(_socketSet);
		SDLNet_TCP_Close(_socket);
	}
}

void CTcpSocketBase::SetSocket( TCPsocket socket )
{
	if(!(_socket == NULL))
	{
		SDLNet_TCP_DelSocket(_socketSet, _socket);
		SDLNet_TCP_Close(_socket);
	}
	_socket = socket;
	if(SDLNet_TCP_AddSocket(_socketSet, _socket) == -1)
	{
		log << ERRORX << "SDLNet_TCP_AddSocket: " << SDLNet_GetError() << endl;
		_socket = NULL;
	}
}



bool CTcpSocketBase::SockRdyToBeRead() 
{
	if(SocketEstablished())
	{
		int socksRdy = SDLNet_CheckSockets(_socketSet, 0);
		if(socksRdy > 0)
		{
			return true;
		}
		if(socksRdy == -1)
		{
			log << WARN << "SDLNet_CheckSockets " << SDLNet_GetError() << endl;
			return false;
		}
	}
	return false;
}


bool CTcpSocketBase::SocketEstablished()
{
	return !(_socket == NULL);
}