#include "Renderer.hpp"
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Entity.hpp"
#include "UtilityFunctions.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static unordered_map<string, unsigned int> loadedTextures;



float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float mouseTime;

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
	return vec2(mousePosInWorld);
}

static void mouseButtCallback(GLFWwindow* window, int button, int action, int mods)
{
	Renderer* r = (Renderer*)glfwGetWindowUserPointer(window);
	vec2 mousePos = calculateMousePos(window);

	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_RELEASE)
		{
			Entity* target = 0;
			for (Entity* e : r->things)
			{
				if (r->sb.intersecting(e->location, e->location + e->dims, mousePos, mousePos))
				{
					target = e;
					break;
				}
			}
			if (target == 0)
			{
				for (Entity* e : r->sb.selected)
				{
					e->setTarget(mousePos);
				}
			}
			else
			{
				for (Entity* e : r->sb.selected)
				{
					e->setTarget(target);
				}
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mouseTime = glfwGetTime();
			r->sb.start(mousePos);
		}
		if (action == GLFW_RELEASE)
		{
			bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
			if (glfwGetTime() - mouseTime > 0.1f)
			{
				r->sb.stop(r->things, shift);
			}
			else
			{
				r->sb.stopPrematurely();
				r->sb.detectClickSelection(r->things, mousePos, shift);
			}
		}
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

	shader = Shader("entity.vertShader.glsl", "entity.fragShader.glsl");

	glEnable(GL_CULL_FACE);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);

	glEnable(GL_LINE_SMOOTH);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		vec2 mousePos = calculateMousePos(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(this->window, deltaTime);
		shader.use();

		
		if (sb.active)
		{
			this->sb.draw(shader);
			this->sb.tick(this->things, mousePos);
		}

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



Texture makeTexture(string filepath)
{
	Texture t;
	t.type = "diffuse";
	t.path = filepath;
	if (loadedTextures.count(filepath))
	{
		t.id = loadedTextures[filepath];
	}
	else
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 4);

		unsigned int texture;
		//printf("%p\n", &texture);
		glGenTextures(1, &texture);
		//printf("%i\n", texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		t.id = texture;
		t.dims = vec2(width, height);
	}

	return t;
}