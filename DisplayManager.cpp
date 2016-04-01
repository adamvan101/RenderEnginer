#include "DisplayManager.h"

DisplayManager::DisplayManager(int w, int h, const char* title) {

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

    _defaultShader = GLSLShader("Shaders/main_vs.glsl", "Shaders/main_fs.glsl");

    // printf("%s\n", glGetString(GL_VERSION));

    // Set the clear color to light blue
    glClearColor(0, 191.0f/255.0f, 1, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glfwSetKeyCallback(_window, _inputManager.KeyboardCallback);
}

DisplayManager::~DisplayManager() {
	glfwTerminate();
}

void DisplayManager::Run() {
	init();

	displayLoop();
}

void DisplayManager::init() {

	float vertices[] = { -0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0, -0.5, 0.5, 0 };

    _defaultShader.Use();
    GLuint loc = _defaultShader.GetAttribLocation("vPos");

	// _rawIds.push_back(_resManager.LoadRaw(vertices, 18, loc));
}

void DisplayManager::displayLoop() { 

	while (!glfwWindowShouldClose (_window)) {
		//Set the base depth to 1.0
	    glClearDepth(1.0);
	    //Clear the color and depth buffer
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderAllRaws();

		renderObj("Objects/DeathStar.obj");

		glfwSwapBuffers (_window);

    	glfwPollEvents();
	}

	glfwTerminate();
}

void DisplayManager::renderAllRaws() {

	for (int i = 0; i < _rawIds.size(); i++) {
		renderRaw(_rawIds[i]);
	}
}

void DisplayManager::renderRaw(int id) {
	GLRaw raw = _resManager.GetRaw(id);

	if (raw.vaoId != 0) {
		glBindVertexArray(raw.vaoId);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, raw.size);
	
		// Unbind
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

	} else {
		printf("NULL RAW OBJ!\n");
	}
}

void DisplayManager::renderObj(std::string objPath) {

	GLObj model = _resManager.GetObject(objPath, _defaultShader.GetId());
	glUseProgram(model.shader);

	glBindVertexArray(model.vaoId);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, model.size);
	
	// Unbind
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}