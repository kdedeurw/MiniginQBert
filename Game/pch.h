#pragma once

#include <iostream>
#include <vector>
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

//Engine lib
//#pragma comment(lib, "Engine.lib")
//>already defined in additional dependencies

//postbuild event
//xcopy / y / d  "$(SolutionDir)3rdParty\SDL2\lib\x86\SDL2.dll" "$(TargetDir)"

#include "SceneManager.h"
#include "GlobalMemoryPools.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include "InputManager.h"

#include "Vectors.h"
#include "Math2D.h"
#include "RGBAColour.h"
#include "Collision2D.h"

#include "Components.h"
#include "GameObject.h"
#include "Scene.h"

#include "GameState.h"
#include "Timer.h"
#include "WindowInfo.h"