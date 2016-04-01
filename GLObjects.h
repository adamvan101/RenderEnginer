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

#include <string>

struct GLTexture {
        std::string filename;
        unsigned int GLid;
        int width;
        int height;
};

struct GLObj {
        std::string filename;
        unsigned int vaoId;
        int size;
        GLuint shader;
};

struct GLRaw {
	unsigned int vaoId = 0;
	int size = 0;

};