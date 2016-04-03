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
    GLObj loadObj(std::string filename, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);
};