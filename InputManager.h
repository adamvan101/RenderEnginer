#pragma once

#include "GLDeps.h"

class InputManager {

public:
	InputManager();
	~InputManager();

	static void KeyboardCallback(GLFWwindow* window, int key, int code, int action, int mods);
	static void MouseWheelCallback(GLFWwindow *w, double xoffset, double yoffest);
	static double MouseWheel;
private:

};