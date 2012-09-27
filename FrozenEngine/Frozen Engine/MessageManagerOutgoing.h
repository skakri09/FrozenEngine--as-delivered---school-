/********************************************************************
	created:	2012/04/04
	created:	4:4:2012   14:04
	filename: 	MessageManagerOutgoing.h
	file path:	FrozenEngine\Frozen Engine
	file base:	MessageManagerOutgoing
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	The class handles messages that are to be sent. It can load
				a given message, and unload it to a target. If several
				messages are loaded into the class, they will all be unloaded
				in order at the unload target.
*********************************************************************/

#ifndef OutgoingMessageManager_h__
#define OutgoingMessageManager_h__

#include <cstring> //memcpy()

#include "Logger.h"
#include "Messages.h"
#include "MessageManagerBase.h"
#include "xmlSettingsHandling.h"

using std::endl; //log messages

// Manager for outgoing messages. Inherits from CMessageManagerBase.
// The class can load a message from a specified memory location, and
// unload it to a specified char* targetbuffer.
class COutgoingMessageManager : public CMessageManagerBase
{
public:
	COutgoingMessageManager();
	~COutgoingMessageManager();

	// Loads a message into this class. PackageType is used to specify what package
	// this class now holds, the void* must point to where in the memory the msg
	// we are loading is, and the messageSize is the number of bytes we are loading.
	// messageSize should be equal to size of the PackageType we have loaded. The 
	// function will handle several LoadMessage calls to same object itself, by placing
	// the messages after each other in the buffer.
	void LoadMessage(PackageType ePackageType, void* messageStart, int messageSize);

	// Unloads the message this class currently holds (if any) into 
	// the specified buffer. Returns the length of the message unloaded.
	int UnloadMessage(char* targetBuffer, int targetCapacity);

private:
	Logger log;

	//The complete buffer that will be sent
	char messageBuffer[frozen::maxNetBufSize];
	
	// Resets the messageBuffer and other variables in the class, making it ready to load new 
	// messages after sending off the old buffer.
	void _ResetBuffAndVars();

	// The total size(bytes) of messages we currently have in the messageBuffer
	int sizeOfAllMessages;
};

#endif // OutgoingMessageManager_h__