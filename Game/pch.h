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

#include "SceneManager.h"
//#include "GlobalMemoryPools.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include "InputManager.h"
#include "Observer.h"
#include "Subject.h"

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