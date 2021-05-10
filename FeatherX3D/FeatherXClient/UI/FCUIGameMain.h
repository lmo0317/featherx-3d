#pragma once

class FCUIGameMain : public F3dZWnd
{
public:

	FCUIGameMain(F3dZWndManager* pWndManager);
	virtual ~FCUIGameMain();

	void Init();
	void Finish();

	virtual void OnShow();
	virtual void RePosition();
	virtual void Process( double dTime, double dMeanDT );

	void SetTime(double dTime);
	void OnControlNotify( _CONTROL_NOTIFY_MSG_ notify_msg, const WCHAR* ID, WPARAM wParam=0, LPARAM lParam=0 );



private:

	F3dZControlNumber* m_pTime;
};

typedef boost::shared_ptr< FCUIGameMain > FCUIGameMainPtr;