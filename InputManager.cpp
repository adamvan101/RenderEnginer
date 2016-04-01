#include "InputManager.h"

InputManager::InputManager() {

}

InputManager::~InputManager() {

}

void InputManager::KeyboardCallback(GLFWwindow* window, int key, int code, int action, int mods) {

	switch (key) {
		// escape, Q, q
        case GLFW_KEY_ESCAPE:
        case 81:
        case 113:
          glfwSetWindowShouldClose(window, GL_TRUE);
          break;
	}
}