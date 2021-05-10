#pragma once

enum 
{
	_EVENT_BOX_JUMP_,
	_EVENT_BOX_START_,
	_EVENT_BOX_END_
};

struct _EVENT_BOX_INFO_
{
	_EVENT_BOX_INFO_():nType(0),nID(0),nJump(0) {}
	int nType;
	int nID;
	int nJump;
};

