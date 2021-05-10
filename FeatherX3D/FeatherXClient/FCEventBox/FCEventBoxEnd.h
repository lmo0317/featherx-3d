#pragma once

class FCEventBoxEnd : public FCEventBox
{
public:
	FCEventBoxEnd();
	~FCEventBoxEnd();

	virtual int getType() {return _EVENT_BOX_END_;}
	virtual void CB_Contact(NxActor **actors);

private:
};