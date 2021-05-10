#pragma once

class FCUIStart : public F3dZWnd
{
public:
	FCUIStart(F3dZWndManager* pWndManager);
	virtual ~FCUIStart();

	void Init();
	void Finish();

	virtual void OnShow();
	virtual void RePosition();
	virtual void Process( double dTime, double dMeanDT );

private:

	F3dZControl* m_pStart;
	double m_dTime;
};

typedef boost::shared_ptr< FCUIStart > FCUIStartPtr;