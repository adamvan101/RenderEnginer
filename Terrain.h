#pragma once

#include "GLDeps.h"
#include "GLObjects.h"
#include "GLSLShader.h"
#include "picoPNG.h"
#include "ResourceManager.h"
#include "transformations.h"
#include "vectors.h"

#include <string>
#include <vector>

// Forward declaration
class GLSLShader;

class Terrain {

public: 
	Terrain();
	Terrain(std::string heightmap, GLSLShader shader, ResourceManager resManager);
	~Terrain();

	void Render(Mat4 view, Mat4 proj);

private:

	float* _vertices;
	float* _textures;

	float* _normals;
	float* _heights;

	GLuint* _indeces;
	int _numIndexes;

	int _vCnt;
	unsigned long _length;
	unsigned long _width;

	GLuint _vertexBuffer;
    GLuint _normalBuffer;
    GLuint _indexBuffer;

	GLuint _rawId;
	GLSLShader _shader;
	ResourceManager _resManager;

	int generateIndexBuffer();
	void generateVertexBuffers();

};