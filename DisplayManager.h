#pragma once

#include "GLDeps.h"

#include <string>
#include <vector>

#include "Camera.h"
#include "GLSLShader.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "transformations.h"
#include "vectors.h"

class DisplayManager {

public:
	DisplayManager(int w, int h, const char* title);
	~DisplayManager();

	void Run();

	Mat4 projection;
	Mat4 view;
	Mat4 PMVmatrix;

	Camera MainCam;

private:

	void init();
	void displayLoop();
	void renderAllRaws();
	void renderRaw(int id);
	void renderObj(std::string objPath, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);

	int _windowHeight;
	int _windowWidth;

	GLFWwindow* _window;
	ResourceManager _resManager;
	static InputManager _inputManager;

	GLSLShader _defaultShader;

	std::vector<int> _rawIds;
};