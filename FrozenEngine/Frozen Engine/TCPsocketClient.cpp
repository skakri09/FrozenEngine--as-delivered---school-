#include "TCPsocket.h"
#include "xmlSettingsHandling.h"

CClientSocket::CClientSocket() : log("TCPsocketClient", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{

}

CClientSocket::CClientSocket( std::string hostIp, Uint32 port ) : log("TCPsocketClient", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
	CIPAddress remoteIp(hostIp.c_str(), port);
	if(!remoteIp.ReadyForUse())
	{
		_socket = NULL;
	}
	else
	{
		Connect(remoteIp);
	}
}

CClientSocket::~CClientSocket()
{
}

bool CClientSocket::Connect( CIPAddress& hostIp )
{
	TCPsocket clientSock;
	IPaddress ip = hostIp.GetIPAddress();
	if((clientSock = SDLNet_TCP_Open(&ip)))
	{
		SetSocket(clientSock);
		return true;
	}
	else
	{
		log << ERRORX << "SDLNet_TCP_Open: " << SDLNet_GetError() << endl;
		return false;
	}
}


void CClientSocket::SetSocket( TCPsocket sdlSocket )
{
	CTcpSocketBase::SetSocket(sdlSocket);

	IPaddress* remoteIP;
	if((remoteIP = SDLNet_TCP_GetPeerAddress(_socket)))
	{
		_remoteIP.SetIpAddress(*remoteIP);
		log << INFO << "Connected with " << SDLNet_Read32(&remoteIP->host) << endl;
	}
	
	else{log << WARN << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << endl;}
}

CIPAddress CClientSocket::getIpAddress()
{
	return _remoteIP;
}

void CClientSocket::Recieve( CIncomingMessageManager& recvData )
{
	if(_socket == NULL)
	{
		log << WARN << " Recieve() - Socket not connected" << endl;
	}
	char buffer[frozen::maxNetBufSize];
	int bytes = SDLNet_TCP_Recv(_socket, buffer, sizeof(buffer));
	
	recvData.LoadMessage(buffer, bytes);
}

bool CClientSocket::Send( COutgoingMessageManager& sendData )
{
	bool returnVal = false;
	
	if(_socket == NULL)
	{
		return returnVal;
	}
	
	int messageSize = 0;
	messageSize = sendData.UnloadMessage(buffer, frozen::maxNetBufSize);
	
	if(messageSize > 0)
	{
		if (SDLNet_TCP_Send(_socket, buffer, messageSize) < messageSize)
		{
			log << ERRORX << "SDLNet_TCP_Send error: " << SDLNet_GetError() << endl;
			_ResetBuffer();
		}
		else
		{
			_ResetBuffer();
			log << INFO << "Message successfully sent" << endl;
			returnVal = true;
		}
	}
	else{log << WARN << "messageSize <= 0 - no message sent" << endl;}
	return returnVal;
}
void CClientSocket::_ResetBuffer()
{
	for(int i = 0; i < frozen::maxNetBufSize; i++)
	{
		buffer[i] = 0;
	}
}