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
		newO = newO * vec2(((float)height) / ((float)width), ((float)width) / ((float)height));
		this->cam.move(newO, deltaTime);

		/*if (newO == vec2(0))
		{
			newO = vec2(1, 0);
		}
		this->things[0]->setOrientation(newO);*/
	}
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}
static void mouseButtCallback(GLFWwindow* window, int button, int action, int mods)
{
	//int width, height;
	//glfwGetWindowSize(window, &width, &height);
	//float hafh = height / 2;
	//float hafw = width / 2;


	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	Renderer* r = (Renderer*)glfwGetWindowUserPointer(window);

	printf("fullscreen: %f, %f\n", r->screenX, r->screenY);



	float hafw = r->screenX / 2;
	float hafh = r->screenY / 2;
	printf("half screen: %f, %f\n", hafw, hafh);

	Camera cam = r->cam;
	vec2 viewCenter = cam.position + vec2(hafw, hafh);

	printf("windowCenter: %f, %f\n", viewCenter.x, viewCenter.y);

	float minx = viewCenter.x - (hafw * cam.zoom);
	float miny = viewCenter.y - (hafh * cam.zoom);
	float maxx = viewCenter.x + (hafw * cam.zoom);
	float maxy = viewCenter.y + (hafh * cam.zoom);

	printf("minx %f\n", minx);
	printf("miny %f\n", miny);
	printf("maxx %f\n", maxx);
	printf("maxy %f\n", maxy);

	double trueXpos = ((xpos / r->screenX) * (maxx - minx)) + minx;
	double trueYpos = ((ypos /  r->screenY) * (maxy - miny)) + miny;

	printf("%f, %f\n", trueXpos, trueYpos);

	r->things[0]->location = vec2(trueXpos, trueYpos)* vec2(r->screenY/r->screenX, r->screenX/r->screenY);

	//divide position by true window dimension, and multiply it by the range visible, then add the min of the range visible
}
static void scrollCallback(GLFWwindow* window, double xoff, double yoff)
{
	Renderer* r = (Renderer*)glfwGetWindowUserPointer(window);
	Camera cam = r->cam;
	cam.zoomCall(yoff);
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

	//glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(this->window, this);
	glfwSetScrollCallback(this->window, scrollCallback);
	glfwSetMouseButtonCallback(this->window, mouseButtCallback);

	return true;
}
void Renderer::run()
{
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

		mat4 projection = ortho(0.0f, screenY, screenX, 0.0f, -1.0f, 1.0f);
		shader.setMatFour("projection", projection);

		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}
	glfwTerminate();
}