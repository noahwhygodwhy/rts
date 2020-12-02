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
	layerCount = 0;
	EBO = 0;
	VBO = 0;
	VAO = 0;
	screenX = x;
	screenY = y;
	window = glfwCreateWindow(x, y, "Title Goes here", NULL, NULL);
	this->cam = Camera(vec3(0, 1, 0), vec3(0, 1, 0), 0, 0, 10, 1, 1);
}

Renderer::~Renderer()
{
}


void Renderer::processInput(GLFWwindow* window)
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
		if (newO == vec2(0))
		{
			newO = vec2(1, 0);
		}
		this->things[0]->setOrientation(newO);
	}
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}
static void mouseButtCallback(GLFWwindow* window, int button, int action, int mods)
{
}
static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{}




void Renderer::addEntity(Entity* th)
{
	things.push_back(th);
}
void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	printf("######%#@%@#%@#new window size\n");
	glViewport(0, 0, width, height);
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

	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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



		processInput(this->window);
		shader.use();


		for (Entity* t : things) //Everything else
		{
			t->draw(shader);
			t->tick(deltaTime);
		}





		const float radius =1;
		float camX = sin(glfwGetTime()) * radius;
		float camY = cos(glfwGetTime()) * radius;
		//camX = 0;
		//camZ = 10;
		//camX = 10;
		//float camY = -2;
		//vec3 obloc = vec3(((Thing*)things.at(0))->transform[3]);
		mat4 view = glm::lookAt(glm::vec3(camX, camY, 0), vec3(camX,camY,-1), glm::vec3(0.0, 1.0, 0.0));
		view = mat4(1);
		//mat4 view(1);
		//camPos = vec3(camX, camY, camZ);
		//camVector = glm::normalize(obloc - camPos);
		//mat4 view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));


		//mat4 view = lookAt(vec3(32 + sin(glfwGetTime()/10)*20, 85, 32+cos(glfwGetTime()/10)*20), vec3(32, 60, 32), vec3(0.0f, 1.0f, 0.0f));
		shader.setMatFour("view", view);
		mat4 projection = ortho(0.0f, (float)screenY, (float)screenX, 0.0f, -1.0f, 1.0f);
		shader.setMatFour("projection", projection);


		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}
	glfwTerminate();
}