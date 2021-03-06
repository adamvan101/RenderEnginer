#include "ResourceManager.h"

// To resolve forward declaration
#include "GLSLShader.h"

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

GLObj ResourceManager::GetObject(std::string objectPath, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs) {

    auto object = _objectCache.find(objectPath);

    // Load and insert if not in cache
    if (object == _objectCache.end()) {
        GLObj newObject = loadObj(objectPath, shader, tex, textureFiles, textureLocs);
        _objectCache.insert(std::make_pair(objectPath, newObject));

        return newObject;
    }

    return object->second;
}

bool ResourceManager::ScaleObject(std::string objectPath, Vec3 scale) {
    auto object = _objectCache.find(objectPath);

    if (object != _objectCache.end()) {
        GLObj o = object->second;
        o.Scale = scale;
        object->second = o;

        return true;
    }

    return false;
}

bool ResourceManager::TranslateObject(std::string objectPath, Vec3 trans) {
    auto object = _objectCache.find(objectPath);

    if (object != _objectCache.end()) {
        GLObj o = object->second;
        o.Position = trans;
        object->second = o;

        return true;
    }

    return false;
}

GLRaw ResourceManager::GetRaw(int id) {

    auto raw = _rawCache.find(id);

    if (raw == _rawCache.end()) {
        return GLRaw();
    }

    return raw->second;
}

int ResourceManager::LoadRaw(float* data, int len, int loc) {

    GLuint id = CreateVAO();
    loadVAO(loc, len, data);
    UnbindVAO();

    GLRaw newRaw = {};
    newRaw.vaoId = id;
    newRaw.size = len;

    _rawCache.insert(std::make_pair(id, newRaw));

    return id;
}

GLuint ResourceManager::CreateVAO() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

void ResourceManager::UnbindVAO() {
    glBindVertexArray(0);
}

GLuint ResourceManager::MakeVAO(GLSLShader* shader, float* vp, float* vn, float* vt, int size, bool tex) {
    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * size, vp, GL_STATIC_DRAW);

    GLuint vbo_norm;
    glGenBuffers (1, &vbo_norm);
    glBindBuffer (GL_ARRAY_BUFFER, vbo_norm);
    glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * size, vn, GL_STATIC_DRAW);

    GLuint vbo_tex;
    if (tex)
    {
        glGenBuffers (1, &vbo_tex);
        glBindBuffer (GL_ARRAY_BUFFER, vbo_tex);
        glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 2 * size, vt, GL_STATIC_DRAW);

        // for (int i = 0; i < textureFiles.size(); i++) {
        //     GLTexture texture = loadPNG(textureFiles[i]);
        //     obj.textures.push_back(texture.GLid);
        //     obj.textureLocs.push_back(glGetUniformLocation(shader->GetId(), textureLocs[i].c_str()));
        // }
    }

    GLuint vao = CreateVAO();
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    GLuint pos = glGetAttribLocation(shader->GetId(), shader->GetPosition());
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    
    glBindBuffer (GL_ARRAY_BUFFER, vbo_norm);
    pos = glGetAttribLocation(shader->GetId(), shader->GetNorm());
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    if (tex)
    {
        glBindBuffer (GL_ARRAY_BUFFER, vbo_tex);
        pos = glGetAttribLocation(shader->GetId(), shader->GetTexture());
        glEnableVertexAttribArray(pos);
        glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);   
    }

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
GLObj ResourceManager::loadObj(std::string filename, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs) {
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

    // GLuint vbo;
    // glGenBuffers (1, &vbo);
    // glBindBuffer (GL_ARRAY_BUFFER, vbo);
    // glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * g_point_count, vp, GL_STATIC_DRAW);

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

    //     for (int i = 0; i < textureFiles.size(); i++) {
    //         GLTexture texture = loadPNG(textureFiles[i]);
    //         obj.textures.push_back(texture.GLid);
    //         obj.textureLocs.push_back(glGetUniformLocation(shader->GetId(), textureLocs[i].c_str()));
    //     }
    // }

    // GLuint vao = CreateVAO();
    // glBindBuffer (GL_ARRAY_BUFFER, vbo);
    // GLuint pos = glGetAttribLocation(shader->GetId(), shader->GetPosition());
    // glEnableVertexAttribArray(pos);
    // glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    
    // glBindBuffer (GL_ARRAY_BUFFER, vbo_norm);
    // pos = glGetAttribLocation(shader->GetId(), shader->GetNorm());
    // glEnableVertexAttribArray(pos);
    // glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    // if (tex)
    // {
    //     glBindBuffer (GL_ARRAY_BUFFER, vbo_tex);
    //     pos = glGetAttribLocation(shader->GetId(), shader->GetTexture());
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
    obj.vaoId = MakeVAO(shader, vp, vn, vt, g_point_count, tex);//, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);
    obj.filename = filename;
    obj.shader = shader->GetId();

    for (int i = 0; i < textureFiles.size(); i++) {
        GLTexture texture = loadPNG(textureFiles[i]);
        obj.textures.push_back(texture.GLid);
        obj.textureLocs.push_back(glGetUniformLocation(shader->GetId(), textureLocs[i].c_str()));
    }

    UnbindVAO();

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