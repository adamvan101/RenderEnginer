#pragma once

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

#include "GLObjects.h"
#include "picoPNG.h"
#include "obj_parser.h"
#include "GLSLShader.h"

#include <map>
#include <string>
#include <stdlib.h>

class ResourceManager {

public:

	ResourceManager();
	~ResourceManager();

	GLTexture GetTexture(std::string texturePath);
	GLObj GetObject(std::string objPath, GLuint shader);
	GLRaw GetRaw(int id);

	int LoadRaw(float* data, int len, int loc = 0);

    static unsigned char* ReadFileSource(const char* shaderFile, long &size);

private:

	GLuint createVAO();
	void loadVAO(int loc, int size, float* data);
	void unbindVAO();

    std::map<std::string, GLTexture> _textureCache;
    std::map<std::string, GLObj> _objectCache;
    std::map<int, GLRaw> _rawCache;

    GLTexture loadPNG(std::string filename);
    GLObj loadObj(std::string filename, GLuint shader);//, GLuint shader, int &count, bool tex);
};