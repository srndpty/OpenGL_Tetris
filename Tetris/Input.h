#pragma once

struct KeyState
{
	bool pressed;
	bool lastPressed;
};

class Input
{
public:
	static constexpr int KEY_MAX = 512;

	void Update()
	{
		for (size_t i = 0; i < KEY_MAX; i++)
		{
			mKeyStates[i].lastPressed = mKeyStates[i].pressed;
		}
	}

	void ResetNow()
	{
		for (size_t i = 0; i < KEY_MAX; i++)
		{
			mKeyStates[i].pressed = false;
		}
	}

	bool GetButtomDown(int key)
	{
		return mKeyStates[key].lastPressed == false && mKeyStates[key].pressed == true;
	}

public:
	KeyState mKeyStates[KEY_MAX];
};
