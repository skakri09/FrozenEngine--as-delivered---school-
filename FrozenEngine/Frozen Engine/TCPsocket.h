/********************************************************************
	created:	2012/04/04
	created:	4:4:2012   14:25
	filename: 	TCPsocket.h
	file path:	FrozenEngine\Frozen Engine
	file base:	TCPsocket
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	The file holds headers for CTCPsocket base class, as well
				as derived classes CClientSocket and CHostSOcket. Specific
				description of the classes are found above each specific 
				class.
*********************************************************************/
#ifndef TCPsocket_h__
#define TCPsocket_h__

#include <string>
#include <SDL_net.h>
#include <vector>

#include "IPaddress.h"
#include "Logger.h"
#include "MessageManagerOutgoing.h"
#include "MessageManagerIncoming.h"
#include "Messages.h"
#include "xmlSettingsHandling.h"

using std::endl;

//typedef std::vector<TCPsocket> SocketsVec;

/* ---- C T c p S o c k e t B a s e ---- */
/*---------------------------------------*/
// A base class for both Host and Client sockets. Holds a TCPsocket and a SDLNet_SocketSet,
// as well as a function to check if there are any sockets ready to be read and to set a socket.
// CtcpSocketBase is an abstract class (pure virtual function OnReady), and only be inherited from
class CTcpSocketBase 
{
public:
	CTcpSocketBase();
	virtual ~CTcpSocketBase();

	// Set a socket from an existing socket
	virtual void SetSocket(TCPsocket socket); 
	
	// Return true if there are data ready to be read in socketset
	bool SockRdyToBeRead();

	// True if there is a TCPsocket associated with the object, and it's ready for use
	bool SocketEstablished();

protected:
	TCPsocket _socket; // Our TCP socket
	SDLNet_SocketSet _socketSet; // Holds a set of sockets. We use it to check for ready packets

private:
	Logger log;
	

}; // End CTcpSocketBase




/* ---- C H o s t S o c k e t ---- */
/*---------------------------------*/
// A host socket, inherits from CTcpSocketBase and Task. The Host socket listens to a specified port for 
// clients trying to connect, and accepts them if so.
class CClientSocket;// Forward declaration to CClientSocket 
class CHostSocket : public CTcpSocketBase//, Task
{
public:	
	// Create and open a new socket with a chosen port, 
	// will create a new CIpaddress for you.
	CHostSocket(Uint32 port); 

	// Create and open a socket to an existing CIpaddress object
	CHostSocket(CIPAddress& sdlIpAddress); 
	~CHostSocket();

	// Accepts an incoming connection, and establishes a connection
	// between the connecting client and a provided client object 
	// on our end. If successful, our client object can then communicate 
	// with the client at the other end.
	bool Accept(CClientSocket& clientSocket);

private:
	Logger log;
}; // End CHostSocket


/* ---- C C l i e n t S o c k e t ---- */
/*-------------------------------------*/
// A client socket. Can connect to a host socket trough a specified IP and port. 
// When connected, it can send data trough the socket.
class CClientSocket : public CTcpSocketBase//, Task
{
public:
	// Creates an empty client object. The object can be passed as Accept()
	// param to a CHostSocket object to act as host-side client.
	CClientSocket();
	
	// Creates a client object with IP provided trough param. Will
	// try to connect to the provided IP.
	CClientSocket(std::string hostIp, Uint32 port);
	~CClientSocket();

	// Connect to a remote host with a CIPAddress object. Can be used on an empty
	// CClientSocket object, otherwise used by the ctor taking host and port param.
	bool Connect(CIPAddress& hostIp); 

	// Used by CHostSocket to set the TCPsocket and ipaddr of an accepted
	// connection.
	void SetSocket(TCPsocket sdlSocket);

	// Returns the IPaddress this object is using.
	CIPAddress getIpAddress();
	
	// Receives data from the other end of the socket and places it in the
	// CIncomingMessageManager object provided in the parameter.
	void Recieve(CIncomingMessageManager& recvData);

	// Sends the message the COutgoingMessageManager object provided in parameter
	// is holding.
	bool Send(COutgoingMessageManager& sendData);


private:
	Logger log;

	CIPAddress _remoteIP;

	// Buffer we are sending from.
	char buffer[frozen::maxNetBufSize];

	// Resets the data in the buffer above.
	void _ResetBuffer();

}; // End CClientSocket



#endif // TCPsocket_h__