#pragma once
#define STREAM_SIZE 1024 * 1024 * 10 // 10M

class CFeatherXStream : public FBSingleton< CFeatherXStream >
{
public:
	CFeatherXStream();
	~CFeatherXStream();

	void	Init();
	void	Process();
	void	Render();
	void	Destroy();

	void Put(void *ptr,int offset);
	void* Get(int start_pos,int offset);
	//void Put()
	char *GetBuffer() {return m_pBuf;}
	int GetSize() {return m_nCurrent;}
private:
	char *m_pBuf;
	int m_nCurrent;
};