------------------~Engine2D~-----------------------
This custom engine is built upon the foundations of 
the given Minigin engine.
The engine uses SDL to render textures, which is its 
main capacity.
note: the name is very original

public git: https://github.com/kdedeurw/MiniginQBert

---------------------Main--------------------------
I tried recreating the generic style of Unity's game engine.
Where objects are  represented by empty game objects carrying components.
There are a few base components such as an FPSComponent, 
Texture2DComponent, SpriteComponent.

---------------------Core--------------------------
There exists a core class, which is the base application.
It handles the main game loop and call the SceneManager.
It also has 2 basic functions like "AddFPS" and "AddDemoScene"

---------------------Math2D------------------------
Math2D contains Vector classes; Vector2, Vector3 
and Vector4.
It also contains functions such as 
Clamp and overloaded operators for Vectors and RGBA.
Added ontop of Math2D, there's also Collision2D, which contains
basic overlapping functionality for Rectangles2D, Circles2D and Ellipses2D.

---------------------Transform----------------------
A TransformComponent is owned by ALL GameObjects.
A TransformComponent holds a Local and a World Transform. (POD)
A Transform struct holds the position, rotation and scaling.
A TransformComponent works with parenting and childing.

---------------------GameObject---------------------
A GameObject is an 'empty' object that carries a transform component.
A GameObject can be parented/childed by another 
GameObject because of its Transformcomponent.
It carries a vector of components and updates and
renders them accordingly.
A GameObject is created by a scene.

---------------------Scenes--------------------------
The engine uses a scene manager to hold scenes.
A scene consists out of a vector of GameObjects.
A scene is responsible for both updating and rendering its objects.
Every scene manages their Objects' memory.

---------------------Rendering-----------------------
The Renderer is a very expanded class and has a lot of functionability and compatibility with SDL.
It also has a bit of Debug Rendering options, which came in real useful during development.

---------------------Camera-------------------------
There exists a camera class, which can be used in rendering.
The GameState holds the current camera transform, default initialized.
Camera supports translation, rotation and scaling.

---------------------Resources-----------------------
There are a couple of base resources classes available,
such as Texture, Texture2D, 
A resource manager also exists as a singleton, it is 
used to either load in textures and/or fonts or 
retrieve already existing pointers to textures.
This makes it fairly easy and quick to swap textures.

---------------------Input--------------------------
It also has full support for gamepads and keyboard and mouse.
There exist 2 singletons named 'KeyboardAndMouse' and 
'ControllerListener'.
The controllerlistener uses the XInput API and the 
KeyboardAndMouse uses the SDL wrapper for keyboard and 
mouse input.
There's also support for custom Commands.

---------------------Memory--------------------------
The entirety of the engine's memory is being handled
by custom memory allocators which are easy to use.
There exists a singleton wrapper named; 
GlobalMemoryAllocators.
All GameObjects, Components, Scenes and projects 
should use these. The sizes of the allocated memory 
can be changed at compile-time.
Runtime memory increase is currently not supported.

=>DEPRECATED at last minute, since there was a chronic crashing issue
this might have been related to a mistake made in a wrapper class for Textures
All memory is now being managed by the Game

---------------------Globals------------------------
List of all Globals:
- GlobalMemoryPools (discarded)
- GameState (see below)
- SceneManager
- Renderer
- ResourceManager

---------------------GameState------------------------
GameState, a class which holds
DeltaTime, Current game scene, UI scene, WindowInfo, a Timer, KeyboardMouseListener, ControllerListener and Camera (for rendering)

---------------------Renderer------------------------
The Renderer is a very expanded class and has a lot of functionability and compatibility with SDL.
It also has a bit of Debug Rendering options, which came in real useful during development.

------------------Multithreading---------------------
The engine supports a multithreaded Audio event queue, which comes from a service locator.
That makes for easy use and easy access to audio processing.

-------------~3rdParty libraries~-------------------
-Catch2: standard for unit testing
-RapidJSON: for easy JSON parsing
-SDL2: base SDL2
-SDL2_image: for easy 2D rendering
-SDL2_mixer: for easy audio processing
-SDL2_ttf: for easy text rendering and processing

QBert Game:
The game itself was more of a challenge at first, trying to figure out how Grid based movement works.
After the initial basis was laid, development went more smoothly.
However I came to a near halt when I discovered a flaw in my implementation and left it on that.
Most features are implemented, though.

public git: https://github.com/kdedeurw/MiniginQBert
