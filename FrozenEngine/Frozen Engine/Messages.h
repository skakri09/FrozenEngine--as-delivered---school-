/********************************************************************
	created:	2012/04/04
	created:	4:4:2012   14:06
	filename: 	Frozen Engine\Messages.h
	file path:	FrozenEngine\Frozen Engine
	file base:	Messages
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	The file holds the different message types we are using in
				our program, as well as constants describing their sizes 
				so we dont need to call sizeof() all over our code. It also
				has a public enum, holding the different message types used
				in the program. 

				Whenever referring to a size of a struct or buffer connected 
				to the net classes, one should use constants from this class,
				so the code is automatically updated if we change a struct.

				If adding a message type & struct that we want to use, the following
				locations must be changed to be able to handle the new struct & type:
				1) CNetManager::RecieveMessage() switch 
				2) CIncomingMessageManager::_FindMsgAndAddToVector() switch
*********************************************************************/
#include "AnimationObject.h"

#ifndef MESSAGES_H
#define MESSAGES_H


// Enum with the different package types we can send. All MessageManagment objects
// holds a PackageType to indicate the packet currently being stored in it. Types
// DISCONNECT and EMPTY are not package types, but gives the NetManager the info it 
// needs if a client/host has disconnected, or we somehow received an empty package.

enum PackageType{	DISCONNECT = 0,			/* Package indicates a disconnect or error, we close the socket			  */
					EMPTY = 1,				/* Package is empty. We do nothing										  */
					
					NEW_PLAYER = 2,			/* Package holds info on a new player in: PlayerBaseInfoMessage			  */
					NEW_NPC = 3,			/* Package holds info on a new NPC in: NewGameNPC                         */
					MOVEMENT_UPDATE = 4,	/* Package holds position update information in: CharacterVelocityUpdate  */
					POSITION_CORRECTION = 5,/* Package holds x/y pos correction in: CharacterPositionCorrection		  */
					LAUNCHING_ATTACH = 6,	/* Package holds attack information in: LaunchingAttack					  */
					HEALTH_CORRECTION = 7,  /* Package holds damage done on a target in: DamageDone					  */

					/* Control value to keep track of the nr of elements in the enum. Must always be last!*/
					PACKAGE_TYPE_NUMBER_OF_ELEMENTS 
				 };

// The base for all message types. All other messages should
// inherit from this struct. It holds a PackageType enum that we 
// set before sending the package, and check against to confirm if a received 
// package has the correct size compared to what PackageType it has. 
// Since the PackageType enum always will be the first few bytes of a message,
// we can rely on it to 'always' arrive, and be used as a package control check.
struct BaseMessage
{	
	PackageType ePackageType;
	int characterID; // a unique character ID for every character (player & npc) in the game.
};

// The information required to create a new character on the other side of a socket.
// Once a connection has been established, we send PlayerBaseInformation to all clients.
struct NewPlayerBaseInfo : public BaseMessage
{
	float startXPos;
	float startYPos;
	char playerName[30];
	char modelName[30]; //Must be min the size of max character model image name
};

// Initial information about a new NPC (none player character). On connection between two 
// clients, the hosting client must send a NewGameNPC struct for all the character's it's 
// controlling. If a new NPC is added on runtime, we also use this message.
struct NewGameNPC : public BaseMessage
{
	char modelName[15]; //Must be min the size of max character model image name
	float startXPos;
	float startYPos;
};

// When a player moves left/right, we send the x/y velocity together with x/y pos
// this way we don't need to send updates that often. Possibly only when a new key
// is pressed. 
struct MovementUpdate : public BaseMessage
{
	float xPos;
	float yPos;
	float xVel;
	float yVel;
};

// Should not constantly send x and y pos like this. It's better as a more rare control
// for the CharacterVelcoityUpdate.
struct PositionCorrection : public BaseMessage
{	
	float xPos;
	float yPos;
};

// To be sent if any character launches an at0tack so the other clients can see the animation.
// Animation type determined on receive end by the attackID.
struct LaunchingAttack : public BaseMessage
{
	int targetID;
	int attackID;
};

// Simply requests to change the health of target character. Can be both positive and negative.
struct HPCorrection : public BaseMessage
{
	int targetID;
	int healthCorrection;
};

namespace frozen
{
	// Size of buffers that are used for sending and receiving messages.
	// The size of this value has to be minimum the same size as the biggest
	// message struct, but can be much bigger without any issues as we never 
	// send the whole buffer size. Should however not have a size out of proportions,
	// as we are resetting it with '0's after using it.
	const int maxNetBufSize = 1024;

	// Sizes of the different message structs. Used throughout the different 
	// SDL_net classes so we dont have to call sizeof() all the time.
	const int newPlayerBaseInfoSize =		sizeof(NewPlayerBaseInfo);
	const int newGameNpcSize =					sizeof(NewGameNPC);
	const int movementUpdateSize =				sizeof(MovementUpdate);
	const int characterPositionCorrectionSize = sizeof(PositionCorrection);
	const int launchingAttackSize =				sizeof(LaunchingAttack);
	const int hpCorrectionSize =				sizeof(HPCorrection);
	
	const int PackageTypeSize =					sizeof(PackageType);
	
	// The number of elements in the PackageType enum that we can use. 
	// PACKAGE_TYPE_NUMBER_OF_ELEMENTS is a control value.
	const int elemsInPackageType = PACKAGE_TYPE_NUMBER_OF_ELEMENTS-1;
}


#endif // MESSAGES_H