#pragma once

#include <iostream>

//Engine lib
//#pragma comment(lib, "Engine.lib")
//>already defined in additional dependencies

//postbuild event
//xcopy / y / d  "$(SolutionDir)3rdParty\SDL2\lib\x86\SDL2.dll" "$(TargetDir)"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>