#pragma once

class MagiFile_Singleton
{
public:
	static void Create();
	static void Destroy();

	static void Init();
	static void InitDx();
	static void Finish();
};