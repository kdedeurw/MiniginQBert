#pragma once
#include "InputManager.h"
#include "SingletonRef.h"
#include <map>

enum class Key
{
	W,
	A,
	S,
	D,
	Q,
	E,
	R,
	F,
	G,
	C,
	V,
	I,
	M,
	P,
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	ArrowLeft,
	ArrowRight,
	ArrowUp,
	ArrowDown,
	Space,
	LShift,
	Tab,
	Escape,
	Enter,
	Backspace,
};

enum class Mouse
{
	LMB,
	RMB,
	Mouse3,
};

enum class ScrollWheel
{
	Up,
	Down,
};

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseWheelEvent;
class KeyboardMouseListener final : public InputManager, public SingletonRef<KeyboardMouseListener>
{
public:
	//friend class GlobalInput;
	~KeyboardMouseListener();

	virtual bool ProcessInput() override;
	virtual bool IsAction(Action action) const override;
	bool IsPressed(Key key) const;
	bool IsReleased(Key key) const { return !IsPressed(key); };
	bool IsMouseButtonPressed(Mouse button);
	bool IsMouseMoving() const;
	const Vector2& GetMousePos() const;
	bool IsScrollWheelMove(ScrollWheel scrollWheel) const;
	//raw scrollwheel.y data: 1, 0 or -1
	int GetScrollWheel() const;

	virtual Command* HandleInput(PlayerId playerId = PlayerId::Player1) const override;

	void SetKeyBindings();
	void SetKeyBinding(Command* pCommand, Action action);
	void SetActionBindings();
	void SetActionBinding(Action action, Key button);
	void SetActionBinding(Action action, Mouse mouseButton);
	void SetActionBinding(Action action, ScrollWheel scrollWheel);

private:
	friend static KeyboardMouseListener& SingletonRef<KeyboardMouseListener>::GetInstance();
	KeyboardMouseListener();

	bool m_IsMouseMoved;
	int m_ScrollWheel;
	Vector2 m_MousePos;
	//TODO: rework?, use bitmasking and store SDL_events
	//OR add callback listeners (overkill?)
	std::map<Key, bool> m_Keys;
	std::map<Mouse, bool> m_MouseButtons;

	std::map<Key, Action> m_KeyToActionBindings;
	std::map<Mouse, Action> m_MouseButtonToActionBindings;
	std::map<ScrollWheel, Action> m_ScrollWheelToActionBindings;
	std::pair<Vector2, Action> m_MouseMoveToActionBinding;
	std::map<Action, Key> m_ActionToKeyBindings;
	std::map<Action, Mouse> m_ActionToMouseButtonBindings;
	std::map<Action, ScrollWheel> m_ActionToScrollWheelBindings;
	std::map<Action, Command*> m_KeyboardMouseBindings;

	void HandleKeyEvent(SDL_KeyboardEvent& e, bool isPressed);
	void HandleMouseMotionEvent(SDL_MouseMotionEvent& e);
	void HandleMouseEvent(SDL_MouseButtonEvent& e, bool isPressed);
	void HandleMouseWheelEvent(SDL_MouseWheelEvent& e);

	void InitializeKeys();
	void InitializeMouseButtons();
};