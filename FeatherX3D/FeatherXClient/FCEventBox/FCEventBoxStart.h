#pragma once

class FCEventBoxStart : public FCEventBox
{
public:
	FCEventBoxStart();
	~FCEventBoxStart();

	virtual int getType() {return _EVENT_BOX_START_;}

private:

};