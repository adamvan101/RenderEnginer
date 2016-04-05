#include "Terrain.h"

// Forward declaration
class GLSLShader;

Terrain::Terrain() {
	
}

Terrain::Terrain(std::string heightmap, GLSLShader shader, ResourceManager resManager) {

	_shader = shader;
	_resManager = resManager;

    long in_size;
    unsigned char* in = ResourceManager::ReadFileSource(heightmap.c_str(), in_size);
    std::vector<unsigned char> out;

    if (in == NULL) {
        fprintf(stderr, "Failed to load PNG file to buffer %s!\n", heightmap.c_str());
		exit(EXIT_FAILURE);
    }

    int err = decodePNG(out, _width, _length, in, in_size);
    if (err != 0) {
        fprintf(stderr, "decodePNG failed with error:  %d!\n", err);
		exit(EXIT_FAILURE);
    }

    _heights = new float[_width * _length];
    _normals = new float[_width * _length * 3];
    _vertices = new float[_width * _length * 3];
    _textures = new float[_width * _length * 2];

    float pointHeight;

    printf("Length: %lu\n", out.size());
    printf("Height Width: %lu x %lu\n", _length, _width);

    for (int i = 0; i < _length; i++) {
		for (int j = 0; j < _width; j++) {
		
			pointHeight = out[(i*_width*4 + j)] / 256.0;
		}
	}

	 for ( unsigned int j = 0; j < _length; ++j )
    {
        for ( unsigned i = 0; i < _width; ++i )
        {
            unsigned int index = ( j * _width ) + i;
            float heightValue = out[index * 4] / 256.0;//GetHeightValue( &heightMap[index * bytesPerPixel], bytesPerPixel );
 
            float X = ( i / (float)(_width - 1) );
            float Z = ( j / (float)(_length - 1) );
 
            // float X = ( S * terrainWidth ) - halfTerrainWidth;
            float Y = heightValue;// * m_fHeightScale;
            // float Z = ( T * terrainHeight ) - halfTerrainHeight;
 
            // Blend 3 textures depending on the height of the terrain
            // float tex0Contribution = 1.0f - GetPercentage( heightValue, 0.0f, 0.75f );
            // float tex2Contribution = 1.0f - GetPercentage( heightValue, 0.75f, 1.0f );
 
            _normals[index * 3] = 0;
            _normals[index * 3 + 1] = 1;
            _normals[index * 3 + 2] = 0;

            _vertices[index * 3] = X;
            _vertices[index * 3 + 1] = Y;
            _vertices[index * 3 + 2] = Z;

            _textures[index * 2] = X;
            _textures[index * 2 + 1] = Z;
        }
    }

    _numIndexes = generateIndexBuffer();

    // _rawId = _resManager.MakeVAO(&shader, _vertices, _normals, _textures, _width * _length, true);

    generateVertexBuffers();

    // Unbind
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

    _resManager.UnbindVAO();
}

Terrain::~Terrain() {
	delete[] _vertices;
	delete[] _normals;
	delete[] _heights;
	delete[] _textures;
}

void Terrain::Render(Mat4 view, Mat4 projection) {
	_shader.Use();

	// if (_rawId != 0) { 

		// glBindVertexArray(_rawId);
		// glEnableVertexAttribArray(0);

		// // glUniformMatrix4fv(_shader.GetMV(), 1, GL_FALSE,  (view * Scale(Vec3(0.04, 0, 0.04))).matrix);
		// glUniformMatrix4fv(_shader.GetMV(), 1, GL_FALSE,  view.matrix);
		// glUniformMatrix4fv(_shader.GetProj(), 1, GL_FALSE,  projection.matrix);

		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, _resManager.GetTexture("Textures/height.png").GLid);
		// glUniform1i(_shader.GetUniformLocation("heights"), 0);

		// glDrawArrays(GL_TRIANGLES, 0, _vCnt);

		// // Unbind
		// glDisableVertexAttribArray(0);
		// glBindVertexArray(0);

		glActiveTextureARB( GL_TEXTURE0_ARB );
	    glMatrixMode( GL_TEXTURE );
	    glPushMatrix();
	    glScalef( 32.0f, 32.0f , 1.0f );

	    glEnableClientState( GL_VERTEX_ARRAY );
	    glEnableClientState( GL_COLOR_ARRAY );
	    glEnableClientState( GL_NORMAL_ARRAY );

	    glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBuffer );
	    glVertexPointer( 3, GL_FLOAT, 0, (void*)NULL );
	    glBindBufferARB( GL_ARRAY_BUFFER_ARB, _normalBuffer );
	    glNormalPointer( GL_FLOAT, 0, (void*)NULL );

	    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _indexBuffer );
	    glDrawElements( GL_TRIANGLES, _numIndexes, GL_UNSIGNED_INT, (void*)NULL );

	    glDisableClientState( GL_NORMAL_ARRAY );
	    glDisableClientState( GL_COLOR_ARRAY );
	    glDisableClientState( GL_VERTEX_ARRAY );

	    glPopMatrix();

	    glMatrixMode( GL_MODELVIEW );
	    glPopMatrix();

	// } else {
	// 	printf("CANNOT FIND TERRAIN\n");
	// }

}

int Terrain::generateIndexBuffer()
{
    // 2 triangles for every quad of the terrain mesh
    const unsigned int numTriangles = ( _width - 1 ) * ( _length - 1 ) * 2;
    _indeces = new GLuint[numTriangles * 3];
 
    unsigned int index = 0; // Index in the index buffer
    for (unsigned int j = 0; j < (_length - 1); ++j )
    {
        for (unsigned int i = 0; i < (_width - 1); ++i )
        {
            int vertexIndex = (j * _width) + i;
            // Top
            _indeces[index++] = vertexIndex;
            _indeces[index++] = vertexIndex + _width + 1;
            _indeces[index++] = vertexIndex + 1;
            // Bottom
            _indeces[index++] = vertexIndex;
            _indeces[index++] = vertexIndex + _width;
            _indeces[index++] = vertexIndex + _width + 1;
        }
    }

    return numTriangles * 3;
}

void Terrain::generateVertexBuffers()
{
    // // First generate the buffer object ID's
    _vertexBuffer = _resManager.CreateVAO();
    _normalBuffer = _resManager.CreateVAO();
    _indexBuffer = _resManager.CreateVAO();
 
    // Copy the host data into the vertex buffer objects
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBuffer );
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(_vertices), _vertices, GL_STATIC_DRAW_ARB ); 
 
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, _normalBuffer );
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(_normals), _normals, GL_STATIC_DRAW_ARB ); 
 
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _indexBuffer );
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(_indeces), _indeces, GL_STATIC_DRAW_ARB );
 
    glBindBuffer( GL_ARRAY_BUFFER_ARB, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
 
}