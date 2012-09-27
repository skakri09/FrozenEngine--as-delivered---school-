/********************************************************************
	created:	2012/04/04
	created:	4:4:2012   13:50
	filename	IPaddress.h
	file path:	FrozenEngine\Frozen Engine
	file base:	IPaddress
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	The class holds a SDL_net IPaddress, and provides a simple
				framework for handling it. The IPaddress can either be set
				trough the constructors, or with an existing IPaddres. You
				can get the port and host associated with the class trough 
				getters. 
*********************************************************************/
#ifndef IPaddress_h__
#define IPaddress_h__

#include <string>
#include <SDL_net.h>

//#include "Logger.h"
#include <iostream> // because logger don't work, replace with logger if get time

using std::endl;
using std::cout;

class CIPAddress
{
public:
	// Creates an empty CIpAddress 
	CIPAddress();
	// Creates a CIpAddress object with a port, used by hosts
	CIPAddress(Uint16 port);
	// Creates a CIpAddress object with a full IP address, used to connect to a host
	CIPAddress(std::string hostIP, Uint16 port);
	~CIPAddress(); //dtor
	
	 // Returns true if the CIPaddress object has a port associated with it
	bool ReadyForUse();
	
	// Manually set the IP address in an empty CIpAddress object.
	void SetIpAddress(IPaddress sdlIpAddress);
	
	// Returns the SDL_net IPaddress associated with this obj
	IPaddress GetIPAddress(); 
	
	// Returns the host associated with this object
	Uint32 GetHostAddr(); 
	
	// Returns the port associated with this object
	Uint16 GetPort(); 
	
private:
	IPaddress _ip; // The IP address 
	//Logger log;

};


#endif // IPaddress_h__
