#pragma once

#include "GLDeps.h"
#include "vectors.h"

#include <string>
#include <vector>

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
        std::vector<GLuint> textures;
        std::vector<GLuint> textureLocs;
        Vec3 Position = Vec3(0, 0, 0);
        Vec4 Rotation = Vec4(0, 1, 0, 0);
        Vec3 Scale = Vec3(1, 1, 1);
};

struct GLRaw {
	unsigned int vaoId = 0;
	int size = 0;

};