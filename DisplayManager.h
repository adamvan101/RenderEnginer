#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
//uncomment these two and comment glfw3.h if you use glut on linux
//#  include <GL/freeglut.h>
//#  include <GL/freeglut_ext.h>
#  include <GLFW/glfw3.h>
#endif  // __APPLE__

#include <string>
#include <vector>
#include "GLSLShader.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include "vectors.h"
#include "transformations.h"

class DisplayManager {

public:
	DisplayManager(int w, int h, const char* title);
	~DisplayManager();

	void Run();

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