#pragma once

#include "GLDeps.h"

#include <string>
#include <vector>

#include "Camera.h"
#include "GLSLShader.h"
#include "InputManager.h"
#include "ResourceManager.h"
// #include "Terrain.h"
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
	double MouseLastX = 0;
	double MouseLastY = 0;

private:

	void init();
	void displayLoop();
	void renderAllRaws();
	void renderRaw(int id);
	void renderObj(GLSLShader shader, std::string objPath, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);
	void updateCam();

	int _windowHeight;
	int _windowWidth;

	// Terrain _terrain;

	GLFWwindow* _window;
	ResourceManager _resManager;
	static InputManager _inputManager;

	GLSLShader _defaultShader;
	GLSLShader _terrainShader;

	std::vector<int> _rawIds;

	Vec4 LightPosition = Vec4( 400, 400, 400, 1 );
	Vec4 SkyAmbience = Vec4( -0.7f, -0.7f, -0.6f, 1.0f );
	Vec4 Ambient = Vec4( 0.2f, 0.2f, 0.2f, 1.0f);
	Vec4 Diffuse = Vec4( 0.8f, 0.8f, 0.8f, 1.0f );
	Vec4 Specular = Vec4( 0.01f, 0.01f, 0.01f, 1.0f );
	GLfloat shininess = 0.2f;
};