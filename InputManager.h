#pragma once

#include "GLDeps.h"

class InputManager {

public:
	InputManager();
	~InputManager();

	static void KeyboardCallback(GLFWwindow* window, int key, int code, int action, int mods);
private:

};