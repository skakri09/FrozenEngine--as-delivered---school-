#include "IPaddress.h"

CIPAddress::CIPAddress()// : log("IPaddress", WARN)
{
	_ip.host = 0;
	_ip.port = 0;
}

CIPAddress::CIPAddress( Uint16 port )// : log("IPaddress", WARN)
{
	if(SDLNet_ResolveHost(&_ip, NULL, port) < 0)
	{
		//log << WARN << "SDLNet_ResolveHost(): " << SDLNet_GetError() << endl;
		cout << "SDLNet_ResolveHost(): " << SDLNet_GetError() << endl; /* !!!!!REMOVE IF POSSIBLE!!!! */
		_ip.host = 0;
		_ip.port = 0;
	}
}

CIPAddress::CIPAddress( std::string hostIP, Uint16 port ) //: log("IPaddress", WARN)
{
	if(SDLNet_ResolveHost(&_ip, hostIP.c_str(), port) < 0)
	{
		//log << WARN << "SDLNet_ResolveHost(): " << SDLNet_GetError() << endl;
		cout << "SDLNet_ResolveHost(): " << SDLNet_GetError() << endl;
		_ip.host = 0;
		_ip.port = 0;
	}

}

CIPAddress::~CIPAddress()
{

}

void CIPAddress::SetIpAddress( IPaddress sdlIpAddress )
{
	_ip = sdlIpAddress;
}

bool CIPAddress::ReadyForUse()
{
	return !(_ip.port == 0);
}

IPaddress CIPAddress::GetIPAddress()
{
	return _ip;
}

Uint32 CIPAddress::GetHostAddr()
{
	return _ip.host;
}

Uint16 CIPAddress::GetPort()
{
	return _ip.port;
}
