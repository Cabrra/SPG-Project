/***************************************************************
|	File:		MessageID.h
|	Author:		Jagoba Marcos
|	Course:		
|	Purpose:	MessageID enum declares the message enumerators
***************************************************************/

#ifndef MESSAGEID_H
#define MESSAGEID_H


/**************************************************************/
// MessageID enum
//	- enumerated list of message identifiers
enum class MessageID
{
	MSG_UNKNOWN,
	MSG_DESTROY_ENTITY,
	MSG_CREATE_ARROW,
	MSG_CREATE_SHURIKEN,
	MSG_CREATE_HOOK
};


#endif //MESSAGEID_H