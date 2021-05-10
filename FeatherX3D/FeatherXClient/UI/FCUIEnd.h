#pragma once

class FCUIEnd : public F3dZWnd
{
public:
	FCUIEnd(F3dZWndManager* pWndManager);
	virtual ~FCUIEnd();

	void Init();
	void Finish();

	virtual void OnShow();
	virtual void RePosition();
	virtual void Process( double dTime, double dMeanDT );

private:

	F3dZControl* m_pEnd;
	double m_dTime;
};

typedef boost::shared_ptr< FCUIEnd > FCUIEndPtr;