#pragma once

#include "GLDeps.h"

#include "GLObjects.h"
#include "picoPNG.h"
#include "obj_parser.h"

#include <map>
#include <string>
#include <stdlib.h>

// Forward declaration
class GLSLShader;

class ResourceManager {

public:

	ResourceManager();
	~ResourceManager();

	GLTexture GetTexture(std::string texturePath);
	GLObj GetObject(std::string objPath, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);
	GLRaw GetRaw(int id);

	bool ScaleObject(std::string objectPath, Vec3 scale);
	bool TranslateObject(std::string objectPath, Vec3 trans);

	int LoadRaw(float* data, int len, int loc = 0);

	GLuint CreateVAO();
	void UnbindVAO();

	GLuint MakeVAO(GLSLShader* shader, float* vp, float* vn, float* vt, int size, bool tex);

    static unsigned char* ReadFileSource(const char* shaderFile, long &size);

private:
	
	void loadVAO(int loc, int size, float* data);

    std::map<std::string, GLTexture> _textureCache;
    std::map<std::string, GLObj> _objectCache;
    std::map<int, GLRaw> _rawCache;

    GLTexture loadPNG(std::string filename);
    GLObj loadObj(std::string filename, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);
};