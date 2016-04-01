#include "ResourceManager.h"

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

GLTexture ResourceManager::GetTexture(std::string texturePath) {

    auto text = _textureCache.find(texturePath);

    // Load and insert if not in cache
    if (text == _textureCache.end()) {
        GLTexture newTexture = loadPNG(texturePath);
        _textureCache.insert(std::make_pair(texturePath, newTexture));

        return newTexture;
    }

    return text->second;
}

GLObj ResourceManager::GetObject(std::string objectPath, GLuint shader) {

    auto object = _objectCache.find(objectPath);

    // Load and insert if not in cache
    if (object == _objectCache.end()) {
        GLObj newObject = loadObj(objectPath, shader);
        _objectCache.insert(std::make_pair(objectPath, newObject));

        return newObject;
    }

    return object->second;
}

GLRaw ResourceManager::GetRaw(int id) {

    auto raw = _rawCache.find(id);

    if (raw == _rawCache.end()) {
        return GLRaw();
    }

    return raw->second;
}

GLuint ResourceManager::createVAO() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

void ResourceManager::loadVAO(int loc, int size, float* data) {
	GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * size, data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int ResourceManager::LoadRaw(float* data, int len, int loc) {

	GLuint id = createVAO();
	loadVAO(loc, len, data);
	unbindVAO();

	GLRaw newRaw = {};
	newRaw.vaoId = id;
	newRaw.size = len;

	_rawCache.insert(std::make_pair(id, newRaw));

	return id;
}

void ResourceManager::unbindVAO() {
	glBindVertexArray(0);
}

GLTexture ResourceManager::loadPNG(std::string filename) {
    GLTexture texture = {};

    unsigned long width, height;

    long in_size;
    unsigned char* in = ResourceManager::ReadFileSource(filename.c_str(), in_size);
    std::vector<unsigned char> out;

    if (in == NULL) {
        fprintf(stderr, "Failed to load PNG file to buffer %s!\n", filename.c_str());
		exit(EXIT_FAILURE);
    }

    int err = decodePNG(out, width, height, in, in_size);
    if (err != 0) {
        fprintf(stderr, "decodePNG failed with error:  %d!\n", err);
		exit(EXIT_FAILURE);
    }

    glGenTextures(1, &(texture.GLid));

    glBindTexture(GL_TEXTURE_2D, texture.GLid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);

    texture.width = width;
    texture.height = height;
    texture.filename = filename;

    return texture;
}

// Modified from:
// https://github.com/capnramses/antons_opengl_tutorials_book/blob/master/21_cube_mapping/main.cpp
GLObj ResourceManager::loadObj(std::string filename, GLuint shader) {//, GLuint shader, int &count, bool tex)
	GLObj obj = {};

    GLfloat* vp = NULL; // array of vertex points
    GLfloat* vn = NULL; // array of vertex normals
    GLfloat* vt = NULL; // array of texture coordinates
    int g_point_count = 0;
    if (!load_obj_file (filename.c_str(), vp, vt, vn, g_point_count))
    {
        fprintf(stderr, "Failed to load OBJ file to buffer %s!\n", filename.c_str());
		exit(EXIT_FAILURE);
    }

    if (vp == NULL || vn == NULL)
    {
        fprintf(stderr, "Failed to load OBJ file to buffer %s!\n", filename.c_str());
		exit(EXIT_FAILURE);
    }

    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * g_point_count, vp, GL_STATIC_DRAW);

    // GLuint vbo_norm;
    // glGenBuffers (1, &vbo_norm);
    // glBindBuffer (GL_ARRAY_BUFFER, vbo_norm);
    // glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * g_point_count, vn, GL_STATIC_DRAW);

    // GLuint vbo_tex;
    // if (tex)
    // {
    //     glGenBuffers (1, &vbo_tex);
    //     glBindBuffer (GL_ARRAY_BUFFER, vbo_tex);
    //     glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 2 * g_point_count, vt, GL_STATIC_DRAW);
    // }

    GLuint vao = createVAO();
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    GLuint pos = glGetAttribLocation(shader, "vPos");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    
    // glBindBuffer (GL_ARRAY_BUFFER, vbo_norm);
    // pos = glGetAttribLocation(shader, "vNorm");
    // glEnableVertexAttribArray(pos);
    // glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    // if (tex)
    // {
    //     glBindBuffer (GL_ARRAY_BUFFER, vbo_tex);
    //     pos = glGetAttribLocation(shader, "vTex");
    //     glEnableVertexAttribArray(pos);
    //     glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);   
    // }


    if (vp != NULL)
        free(vp);
    if (vn != NULL)
        free(vn);
    if (vt != NULL)
        free(vt);

    obj.size = g_point_count;
    obj.vaoId = vao;
    obj.filename = filename;
    obj.shader = shader;
    return obj;
}

unsigned char* ResourceManager::ReadFileSource(const char* shaderFile, long &size) {
	FILE* fp = fopen(shaderFile, "r");
	size = 0;

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	unsigned char* buf = (unsigned char *) malloc(size + 1);
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}