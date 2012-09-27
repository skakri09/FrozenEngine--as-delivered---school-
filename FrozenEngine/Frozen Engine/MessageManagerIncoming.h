/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   23:15
	filename: 	Frozen Engine\MessageManagerIncoming.h
	file path:	FrozenEngine\Frozen Engine
	file base:	MessageManagerIncoming
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Message manager for incoming messages from a TCP socket.
				The class will handle any incoming messages that are
				defined in Messages.h class. If incomplete messages arrive,
				they are stored and later put together when the rest of the
				full message arrives.

				The class can unload the stored message(s) to a void* source.
*********************************************************************/
#ifndef IncomingMessageManager_h__
#define IncomingMessageManager_h__

#include <cstring>
#include <vector>
#include <boost/tuple/tuple.hpp>

#include "Logger.h"
#include "Messages.h"
#include "MessageManagerBase.h"
#include "xmlSettingsHandling.h"

using std::endl; //log messages

typedef boost::tuple<PackageType, char*, int> MsgTuple;
typedef std::vector<MsgTuple> VectorPack;

// Manager for incoming messages. Inherits from CMessageManagerBase.
// The class can load a messages from a char source buffer. If the message
// contains several PackageTypes they will be split, and placed in a vector
// paired with their PacketType and size. The class can unload a message from
// the vector to a target.
class CIncomingMessageManager : public CMessageManagerBase
{
public:
	CIncomingMessageManager();
	~CIncomingMessageManager();

	// Loads a message from a char array source into this class. If there are
	// multiple messages in the sourceBuffer, it will be split into its correct 
	// sizes, and put into the _messages vector, paired with a matching PackageType and size
	void LoadMessage(char* sourceBuffer, int messageSize);

	// Unloads the specified msgNumber from the _messages vector into the target.
	// Returns false if the specified msg dont exist in the vector, or the 
	// targetCapacity is not equal to the size of the message.
	bool UnloadMessage(int msgNumber, void* target, int targetCapacity);

	// Returns the PackageType this class currently holds. Will be EMPTY unless
	// other end has disconnected, in which case it is DISCONNECT. If we hold an
	// incomplete message from last Load, it will hold the PacketType of that previous
	// incomplete message.
	PackageType GetTheMsgType(){return eCurrentHeldPackage;}

	// Returns pointer to the vector with messages in
	VectorPack* GetMessages(){return &_messages;}

	// Returns whether or not we store an incomplete message.
	bool IsIncompleteMessage(){return incompleteMessage;}

private: // Objects
	Logger log;

	// Vector with sorted packets <PacketType, char* message>
	VectorPack _messages; 

	// Will be allocated with size matching the message it's going to hold in the
	// _FindMsgAndAddToVector() function, and placed in the _messages vector when 
	// its a complete message. Memory is freed with the _ClearMessagesVector() function.
	char* _messageBuffer; 

private: // Functions
	// Used by LoadMessage if the message size is < 0 or > maxNetBufSize
	// Logs proper error, and puts eCurrentHeldMessage
	void _LoadMessageErrorHandling(int* msgSize);

	// Helper function for LoadMessage(). It places the exact message in a new char* buffer
	// and places that in the _messages vector paired with a PacketType, after confirming it's size. 
	int _FindMsgAndAddToVector(char* sourceBuffer, PackageType type);
	
	// Helper function for _FindMsgAndAddToVector(). Allocates _messageBuffer of correct size,
	// returns the returnVal and calls _CtrlAndReplaceMsgInCharBuf().
	int _HandleMsgType(int msgSize, char* sourceBuf);
	
	// Helper function for _ControlAndPlaceMsgInVector(). It controlls the size of the msg
	// and places it in the char* buffer provided by _ControlAndPlaceMsgInVector.
	void _CtrlAndPlaceMsgInCharBuf(char* sourceBuffer, int tarSize);

	// Compares the param PackageType with all the values in PackageType enum, returns
	// true if it matches any of them.
	bool _VertifyPackageTypeExists(PackageType& packType);

	// Frees dynamic allocated memory in the messages vector and .clear() it.
	void _ClearMessagesVector();

	// Resets buffers and other data used by the class so it's ready to be used
	// for new incoming messages. If completeReset is set to true, all variables
	// and buffers are reset. If false, only vector of messages is reset while we 
	// keep any other buffers or variables (probably used for incomplete messages).
	void FinishedReading(bool completeReset);

private: // class variables
	// True if this class holds an incomplete package.
	bool incompleteMessage; 

	// Holds the number of bytes missing of an incomplete message.
	int _sizeLeftOfMissingMsg;
	
	// Holds the amount of bytes we have not yet handled in the original message
	int unhandledDataSize;
	
};

#endif // IncomingMessageManager_h__