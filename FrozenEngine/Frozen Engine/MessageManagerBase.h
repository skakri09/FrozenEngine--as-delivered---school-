/********************************************************************
	created:	2012/04/04
	created:	4:4:2012   14:00
	filename:   FrozenEngine\Frozen Engine\MessageManagerBase.h
	file path:	FrozenEngine\Frozen Engine
	file base:	MessageManagerBase
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	The class is a base class for the outgoing and incoming
				messageManagers. It holds the two main functions of either
				class as virtual functions, and a PackageType which both
				classes holds.
*********************************************************************/

#ifndef MessageManagerBase_h__
#define MessageManagerBase_h__

#include "Messages.h"

// The base for outgoing and incoming messages. CIncomingMessageManager
// and COutgoingMessageManager inherits from it class.
class CMessageManagerBase
{
public:
	CMessageManagerBase();
	virtual ~CMessageManagerBase();
	
	// Loads a message into the messageManager class.
	virtual void LoadMessage();

	// Unloads a message from the messageManager, to the target
	virtual void UnloadMessage();

protected:
	// Holds the last handled PackageType by the class.
	PackageType eCurrentHeldPackage; 
	
	// Holds the size of a message being handled by the class.
	
	
	
};

#endif // MessageManagerBase_h__