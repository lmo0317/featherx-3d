#pragma once

enum 
{
	TYPE_BOX, TYPE_WALL_X, TYPE_WALL_Z, TYPE_EFFECT , TYPE_EVENT_BOX,
};

struct MAP_INFO_ELEMENT
{
	int type;
	int nEventBoxID;
	std::wstring strFileName;
	F3dVector vecPostion;
	F3dVector vecRotation;
};

struct MAP_INFO
{
	std::vector<MAP_INFO_ELEMENT> vecElement;
};

