#pragma once

#include <iostream>
#include <vector>
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

//SDL2 libs
//#pragma comment(lib, "SDL2.lib")
//#pragma comment(lib, "SDL2main.lib")
//#pragma comment(lib, "SDL2test.lib")
//#pragma comment(lib, "SDL2_image.lib")
//#pragma comment(lib, "SDL2_ttf.lib")
//#pragma comment(lib, "SDL2_mixer.lib")

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