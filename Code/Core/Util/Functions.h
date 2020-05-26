#pragma once

namespace Util
{
	template<typename T>
	void SafeDelete(T*& ptr)
	{
		if (nullptr != ptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	template<typename T>
	void SafeRelease(T*& ptr)
	{
		if (nullptr != ptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}

	template<typename T>
	void SafeDeleteRelease(T*& ptr)
	{
		if (nullptr != ptr)
		{
			ptr->Release();
			delete ptr;
			ptr = nullptr;
		}
	}
}