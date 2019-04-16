#pragma once

template<typename T>
void Safe_Delete(T*& ptr)
{
	if (nullptr != ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

//template<typename T>
//void Safe_Release(T*& ptr)
//{
//	if (nullptr != ptr)
//	{
//		ptr->Release();
//		ptr = nullptr;
//	}
//}