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

// #include "glm/glm.hpp"
// #include "glm/gtc/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"