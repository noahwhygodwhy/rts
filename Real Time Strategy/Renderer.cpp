#include "Renderer.hpp"
#include "Camera.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Entity.hpp"



float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


static vec3 camVector = vec3();
static vec3 camPos = vec3();
//TODO: you were moving the camera from the renderer to the Player

Renderer::Renderer(int x, int y)
{
	EBO = 0;
	VBO = 0;
	VAO = 0;
	screenX = x;
	screenY = y;
	window = glfwCreateWindow(x, y, "Title Goes here", NULL, NULL);
	this->cam = Camera(vec2(0, 0), 500.0f, 1.0f);
}

Renderer::~Renderer()
{
}


void Renderer::processInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	bool w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	bool a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	bool s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	bool d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

	if (w || a || s || d)
	{
		vec2 newO = vec2((int)d - (int)a, (int)w - (int)s);
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		this->cam.move(newO, deltaTime);

	}
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

vec2 calculateMousePos(GLFWwindow* window)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	Renderer* r = (Renderer*)glfwGetWindowUserPointer(window);
	Camera cam = r->cam;
	vec4 mousePosInClip = vec4(
		((xpos * 2.0f) / r->screenX) - 1.0f,
		(1.0f - (2.0f * ypos) / r->screenY),
		0.0f,
		1.0f);

	vec4 mousePosInWorld = glm::inverse(r->projMat*cam.getView()) * mousePosInClip;

	printf("%f, %f\n", vec2(mousePosInWorld).x, vec2(mousePosInWorld).y);

	return vec2(mousePosInWorld);

	/*
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	Renderer* r = (Renderer*)glfwGetWindowUserPointer(window);
	Camera cam = r->cam;

	vec2 windowDims = vec2(r->screenX, r->screenY);
	vec2 scaledWindowDims = windowDims / cam.zoom;
	//vec2 viewCenter = cam.position + (scaledWindowDims/2.0f);
	vec2 posRatioToWindow = vec2(xpos / r->screenX, ypos / r->screenY);
	vec2 mousePosition = cam.position + (scaledWindowDims * posRatioToWindow);

	return mousePosition;*/
}

static void mouseButtCallback(GLFWwindow* window, int button, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		vec2 mousePos = calculateMousePos(window);

		Renderer* r = (Renderer*)glfwGetWindowUserPointer(window);
		r->things[0]->location = mousePos;// * vec2(r->screenY / r->screenX, r->screenX / r->screenY);*/

	}

	//divide position by true window dimension, and multiply it by the range visible, then add the min of the range visible
}
static void scrollCallback(GLFWwindow* window, double xoff, double yoff)
{
	Renderer* r = (Renderer*)glfwGetWindowUserPointer(window);
	r->cam.zoomCall(yoff);
}




void Renderer::addEntity(Entity* th)
{
	things.push_back(th);
}
void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	printf("######%#@%@#%@#new window size\n");
	glViewport(0, 0, width, height);
	Renderer* r = (Renderer*) glfwGetWindowUserPointer(window);
	r->screenX = width;
	r->screenY = height;
}

bool Renderer::initialize()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, 1);
	//glfwWindowHint(GLFW_SAMPLES, 16);
	this->window = glfwCreateWindow(screenX, screenY, "Title Goes here", NULL, NULL);


	if (this->window == NULL)
	{
		cout << "Window creation failed" << endl;
		exit(-1);
	}

	glfwMakeContextCurrent(this->window);
	glfwSetFramebufferSizeCallback(this->window, frameBufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "GLAD init failed" << endl;
		exit(-1);
	}

	shader = Shader("vertShader.glsl", "fragShader.glsl");

	glEnable(GL_CULL_FACE);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);

	//glDepthFunc(GL_LESS);

	//glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(this->window, this);
	glfwSetScrollCallback(this->window, scrollCallback);
	glfwSetMouseButtonCallback(this->window, mouseButtCallback);

	return true;
}
void Renderer::run()
{
	this->projMat = ortho(0.0f, screenX, screenY, 0.0f, -1.0f, 1.0f);
	while (!glfwWindowShouldClose(this->window))
	{	
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(this->window, deltaTime);
		shader.use();


		for (Entity* t : things) //Everything else
		{
			t->draw(shader);
			t->tick(deltaTime);
		}

		mat4 view = cam.getView();
		shader.setMatFour("view", view);

		shader.setMatFour("projection", this->projMat);

		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}
	glfwTerminate();
}


