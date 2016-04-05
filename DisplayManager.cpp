#include "DisplayManager.h"
#include "time.h"

DisplayManager::DisplayManager(int w, int h, const char* title) :
	MainCam(20, Vec3(0, 0, 0)) {

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ()) {
    	fprintf (stderr, "ERROR: could not start GLFW3\n");
    	// exit(1);
    } 

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _windowHeight = h;
    _windowWidth = w;
    _window = glfwCreateWindow(w, h, title, NULL, NULL);
    if (!_window)
    {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        // exit(1);
    }

    glfwMakeContextCurrent(_window);

    // will segfault otherwise in init()
    glewExperimental = GL_TRUE;
    glewInit();

    _defaultShader = GLSLShader("Shaders/texture_vs.glsl", "Shaders/texture_fs.glsl");
	_terrainShader = GLSLShader("Shaders/multitexture_vs.glsl", "Shaders/multitexture_fs.glsl");

    // printf("%s\n", glGetString(GL_VERSION));

    // Set the clear color to light blue
    glClearColor(0, 191.0f/255.0f, 1, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glfwSetKeyCallback(_window, _inputManager.KeyboardCallback);
    glfwSetScrollCallback(_window, _inputManager.MouseWheelCallback);
}

DisplayManager::~DisplayManager() {
	glfwTerminate();
}

void DisplayManager::Run() {
	init();

	displayLoop();
}

void DisplayManager::init() {

	projection = Mat4::Identity();
	view = Mat4::Identity();
	PMVmatrix = Mat4::Identity();

	float vertices[] = { -2, 2, 0, -2, -2, 0, 2, -2, 0, 2, -2, 0, 2, 2, 0, -2, 2, 0 };
	float vertices2[] = { 0, 2, -2, 0, -2, -2, 0, -2, 2, 0, -2, 2, 0, 2, 2, 0, 2, -2 };

    _defaultShader.Use();
    GLuint loc = _defaultShader.GetAttribLocation("vPos");

    GLuint mtex_AP, mtex_DP, mtex_SP, mtex_Shine, mtex_LP;

    mtex_AP = glGetUniformLocation(_defaultShader.GetId(), "AmbientProduct");
    mtex_DP = glGetUniformLocation(_defaultShader.GetId(), "DiffuseProduct");
    mtex_SP = glGetUniformLocation(_defaultShader.GetId(), "SpecularProduct");
    mtex_Shine = glGetUniformLocation(_defaultShader.GetId(), "Shininess");
    mtex_LP = glGetUniformLocation(_defaultShader.GetId(), "LightPosition");

	glUniform4f(mtex_AP, Ambient.v[0], Ambient.v[1], Ambient.v[2], Ambient.v[3]);
    glUniform4f(mtex_DP, Diffuse.v[0], Diffuse.v[1], Diffuse.v[2], Diffuse.v[3]);
    glUniform4f(mtex_SP, Specular.v[0], Specular.v[1], Specular.v[2], Specular.v[3]);
    glUniform1f(mtex_Shine, shininess);
    glUniform4f(mtex_LP, LightPosition.v[0], LightPosition.v[1], LightPosition.v[2], LightPosition.v[3]);


    float fov = 45.0f;
    projection = Perspective(fov, _windowWidth/(float)_windowHeight, 0.1f, 10000.0f);
    view = MainCam.View();

    PMVmatrix = projection * view;
	// _rawIds.push_back(_resManager.LoadRaw(vertices, 18, loc));
	// _rawIds.push_back(_resManager.LoadRaw(vertices2, 18, loc));
}

void DisplayManager::displayLoop() { 

	std::string dsFile = "Objects/DeathStar.obj";
	std::vector<std::string> dsTexts;
	dsTexts.push_back("Textures/DeathStar.png");
	std::vector<std::string> dsLocs;
	dsLocs.push_back("tex");

	std::string terrainFile = "Objects/craterlake.obj";
	std::vector<std::string> terTexts;
	terTexts.push_back("Textures/sand.png");
	terTexts.push_back("Textures/grass.png");
	terTexts.push_back("Textures/rock.png");
	terTexts.push_back("Textures/snow.png");
	std::vector<std::string> terLocs;
	terLocs.push_back("sand");
	terLocs.push_back("grass");
	terLocs.push_back("rock");
	terLocs.push_back("snow");

	_terrainShader.Use();
	_resManager.GetObject(terrainFile, &_terrainShader, terTexts.size() > 0, terTexts, terLocs);
	if (!_resManager.ScaleObject(terrainFile, Vec3(100, 100, 100))) {
		printf("Object not found\n");
	}

	if (!_resManager.TranslateObject(terrainFile, Vec3(0, -10, 0))) {
		printf("Object not found\n");
	}

	GLuint mtex_AP, mtex_DP, mtex_SP, mtex_Shine, mtex_LP;

    mtex_AP = glGetUniformLocation(_terrainShader.GetId(), "AmbientProduct");
    mtex_DP = glGetUniformLocation(_terrainShader.GetId(), "DiffuseProduct");
    mtex_SP = glGetUniformLocation(_terrainShader.GetId(), "SpecularProduct");
    mtex_Shine = glGetUniformLocation(_terrainShader.GetId(), "Shininess");
    mtex_LP = glGetUniformLocation(_terrainShader.GetId(), "LightPosition");

	glUniform4f(mtex_AP, Ambient.v[0], Ambient.v[1], Ambient.v[2], Ambient.v[3]);
    glUniform4f(mtex_DP, Diffuse.v[0], Diffuse.v[1], Diffuse.v[2], Diffuse.v[3]);
    glUniform4f(mtex_SP, Specular.v[0], Specular.v[1], Specular.v[2], Specular.v[3]);
    glUniform1f(mtex_Shine, shininess);
    glUniform4f(mtex_LP, LightPosition.v[0], LightPosition.v[1], LightPosition.v[2], LightPosition.v[3]);

	float lastUpdate = clock();

	while (!glfwWindowShouldClose (_window)) {
		//Set the base depth to 1.0
	    glClearDepth(1.0);
	    //Clear the color and depth buffer
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    // FPS limiter
	    while (clock() - 16 < lastUpdate);
	    lastUpdate = clock();

	    updateCam();

		renderObj(_terrainShader, terrainFile, terTexts, terLocs);

		if (_rawIds.size() > 0) {
			renderAllRaws();
		}

		renderObj(_defaultShader, dsFile, dsTexts, dsLocs);

		glfwSwapBuffers(_window);

    	glfwPollEvents();
	}

	glfwTerminate();
}

void DisplayManager::updateCam() {

	double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);

    if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
    	MainCam.Update(xpos - MouseLastX, ypos - MouseLastY);
    }

    MainCam.DeltaDistance(InputManager::MouseWheel);
    InputManager::MouseWheel = 0;

    view = MainCam.View();

    MouseLastX = xpos;
   	MouseLastY = ypos;
}

void DisplayManager::renderAllRaws() {

	_defaultShader.Use();

	for (int i = 0; i < _rawIds.size(); i++) {
		renderRaw(_rawIds[i]);
	}
}

void DisplayManager::renderRaw(int id) {
	GLRaw raw = _resManager.GetRaw(id);

	if (raw.vaoId != 0) {
		glBindVertexArray(raw.vaoId);
		glEnableVertexAttribArray(0);

		glUniformMatrix4fv(_defaultShader.GetMV(), 1, GL_FALSE,  view.matrix);
    	glUniformMatrix4fv(_defaultShader.GetProj(), 1, GL_FALSE,  projection.matrix);

		glDrawArrays(GL_TRIANGLES, 0, raw.size);
	
		// Unbind
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

	} else {
		printf("NULL RAW OBJ!\n");
	}
}

void DisplayManager::renderObj(GLSLShader shader, std::string objPath, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs) {

	// shader.Use();
	GLObj model = _resManager.GetObject(objPath, &shader, textureFiles.size() > 0, textureFiles, textureLocs);
	glUseProgram(model.shader);

	for (int i = 0; i < textureFiles.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, model.textures[i]);
		glUniform1i(model.textureLocs[i], i);
	}

	glBindVertexArray(model.vaoId);
	glEnableVertexAttribArray(0);

	glUniformMatrix4fv(shader.GetMV(), 1, GL_FALSE, (view * Translate(model.Position) * Rotate(model.Rotation) * Scale(model.Scale)).matrix);
    glUniformMatrix4fv(shader.GetProj(), 1, GL_FALSE,  projection.matrix);

	glDrawArrays(GL_TRIANGLES, 0, model.size);
	
	// Unbind
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}