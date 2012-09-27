/********************************************************************
	created:	2012/04/04
	created:	4:4:2012   14:10
	filename: 	NetManager.h
	file path:	FrozenEngine\Frozen Engine
	file base:	NetManager
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	A wrapper for all the net classes. It's a singleton class,
				but the correct overloaded Inst() function should be called
				the first time the class is referenced in order to properly
				control whether or not it's a host/client instance we create.  
				The class can take a reference to a vector of CCharacter objects,
				and will then automatically send updates regarding the characters
				in that vector. It will also automatically receive incoming messages
				from connected client(s) and handle them (including rendering remote
				character objects etc).
				
				It currently only supports 
				one socket (one client+host to a client), but the 
				different net-classes are written with the intent of 
				easily being extended to multiple connections. 
*********************************************************************/

#ifndef NetManager_h__
#define NetManager_h__

#include "TCPsocket.h"
#include "Task.h"
#include "Logger.h"
#include "Messages.h"
#include "MessageManagerOutgoing.h"
#include "MessageManagerIncoming.h"
#include "xmlSettingsHandling.h"
#include "CharacterObject.h"
#include "EnemyObject.h"
#include "RenderEngine.h"
#include "Timer.h"
#include "Kernel.h"

#include <string>
#include <map>
#include <vector>

using std::endl;

class CNetManager : public Task
{
public:
	/* Ctor used if hosting (only need the port we listen to for connecting client(s) */
	CNetManager(Uint16 port);

	/* Ctor used if we're the client (need the complete		*/
	/* IP address of the host we're trying to connect to)	*/
	CNetManager(std::string ipAddr, Uint16 port);
	
	~CNetManager();
	
	void ChangeToClientOrHost(bool toHost){_isHost = toHost;}
	/* Derived Task functions */
	bool Start();
	void Stop();
	void Update();

	/* Flags netmanager for kill in the kernel loop */
	void DisconnectNetManager(){FlagForKill();}
	
	

	/* Gives this class a reference to a vector of local player objects								 */
	void SetLocalPlayersVectorRef(std::vector<CCharacterObject*>* playersVectorReference);

	/* Gives this class a reference to a vector of local enemy objects								*/
	void SetLocalEnemiesVectorRef(std::vector<CEnemyObject*>* enemiesVectorReference);

	/* Returns true if netmanager is hosting */
	bool IsHost(){return _isHost;}
private:/* General private objects, variables and functions*/
	Logger log;			/* Logger object															*/

	//Task* thisTask;

	std::string _ipAddr;/* Given value in ctor, the ip address we are connecting to (if client)		*/
	Uint16 _port;		/* Given value in ctor, the port we are connecting/listening to				*/
	bool _isHost;		/* Given value in ctor, used to determine if host-only code should be used	*/
	
	CHostSocket* _host;			/* CHostSocket object, initialized if hosting ctor is used.							*/	
	CClientSocket* _client;		/* CClientSocket object, always initialized in ctor									*/
	bool Connected();			/* Returns true if we have a client object connected with another client or host	*/	
	void FreeAllocatedMemory(); /* Deletes and reset allocated memory in the class									*/
	void _CleanupClient();		/* Deletes our client and frees allocated memory associated with it if it exists.	*/

private: // Message handling

	// Unloads the message(s) from the incomingMessageManager to the correct place
	void RecieveMessage();

	/* Helper functions for RecieveMessage. One for each message type, will unload the	*/
	/* correct message from the incomingMessageManager to the correct struct object		*/
	void _UnloadNewPlayerInfo(int* msgNr);
	void _UnloadNewGameNpcInfo(int* msgNr);
	void _UnloadMovementUpdate(int* msgNr);
	void _UnloadPosCorrection(int* msgNr);
	void _UnloadLaunchedAttack(int* msgNr);
	void _UnloadHpCorrection(int* msgNr);
	
	COutgoingMessageManager* _outMsg;	/* msg object used to send messages		*/
	CIncomingMessageManager* _inMsg;	/* msg object used to receive messages	*/

	/* Message struct objects */
	NewPlayerBaseInfo* _newPlayer;
	NewGameNPC _newGameNPC;
	MovementUpdate _movementUpdate;
	PositionCorrection _PosCorrection;
	LaunchingAttack _launchedAttack;
	HPCorrection _hpCorrection;

	/* Used by Update() to determine if we should call send() on our outgoingMessageManager.	*/
	/* Is true when we want to send a message													*/
	bool _newMsgToSend; 

private: /* Render handling of remote character objects	*/
	
	/* Map holding all CCharacterObjects the other end of our socket is handling.	*/
	std::map<std::string, CCharacterObject*> remoteCharactersMap; 
	
	/* Map for keeping keeping an int ID for every string name*/
	std::map<int, std::string> remoteCharacterIDMap;

	/* Adds a new character from the other end of our socket to the remoteCharacters map */
	void AddNewCharacter(NewPlayerBaseInfo* newPlayerInfo);

	/* Applies the movement update currently in _movementUpdate */ 
	/* to the correct character by it's ID.						*/
	void UpdateMovement(int characterID);

	/* Updates the position of all the remote characters we have, by adding  */
	/* movement velocity multiplied with deltatics, to the current position. */
	void UpdateAllRemoteCharacters();

private: /* Sending of local information to remote connected client(s)/host */
	
	/* A reference to the gameManager characters vector */
	std::vector<CCharacterObject*>* localPlayerCharactersVectorRef;
	
	/* A reference to the gameManager enemies vector	*/
	std::vector<CEnemyObject*>* localEnemiesVectorRef;
	
	/* Send new character information to other end of our socket(s) from the param CCharacterObject. */
	bool SendNewCharacterInfo(CCharacterObject& charToSendInfoFrom);
	
	/* Send new character information to other end of our socket(s) from the param CCharacterObject. */
	bool SendNewEnemyInfo(CEnemyObject& charToSendInfoFrom);

	/* Send movement information to other end of our socket(s) from the param CCharacterObject. */
	void SendCharacterMovement(CCharacterObject& charToSendInfoFrom);

	/* Send movement information to other end of our socket(s) from the param CCharacterObject. */
	void SendCharacterMovement(CEnemyObject& charToSendInfoFrom);
	
	/* Sends movement updates for all our local characters, if there are any. */
	void SendLocalCharacterUpdates();

	/* Sends information about our local characters after connecting to a new client/host. */
	void SendInitialLocalCharactersInfo();
};


#endif // NetManager_h__
