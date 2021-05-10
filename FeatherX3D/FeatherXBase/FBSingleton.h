#pragma once

#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )

#include <assert.h>

template <class T>
class FBSingleton
{
	static T* ms_Singleton;

public:

	virtual ~FBSingleton()
	{
		ms_Singleton = 0;
	}

	static void	CreateSingleton()
	{
		ms_Singleton = new T;
	}

	static void	 DestroySingleton()
	{
		delete ms_Singleton;
	}

	static T* GetSingleton()
	{
		return ms_Singleton;
	}

protected:
	FBSingleton()
	{
	}
};

template <class T> T* FBSingleton<T>::ms_Singleton = 0;
