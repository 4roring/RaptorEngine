#pragma once

#include <windows.h>
#include <crtdbg.h>
#include "Typedef.h"

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif