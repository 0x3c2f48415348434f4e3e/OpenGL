#include<glad.h>
#include<glfw3.h>
#include<stdio.h>
#include<stdlib.h>

void windowRestCallback(GLFWwindow* window, int x, int y);
int main(void) {
	//initialise glfw
	if (glfwInit() == GLFW_FALSE) {
		printf("Unable to intlise GLFW");
		exit(-1);
	}
	//set up window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//set up profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//make window
	GLFWwindow* window = glfwCreateWindow(800, 800, "MY OpenGL", NULL, NULL);
	//check if succcessfully
	if (window == NULL) {
		printf("Unable to set up OpenGL window");
		exit(-1);
	}

	//make window current context
	glfwMakeContextCurrent(window);

	//initiate glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Unable to load OpeenGL function pounters");
		glfwTerminate();
		exit(-1);
	}

	//set up viewport window
	windowRestCallback(window, 800, 800);

	//call back
	//glfwSetFramebufferSizeCallback(window, windowRestCallback);

	//rendering loop
	while (!glfwWindowShouldClose(window)) {
		//clear color buffer
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

}

void windowRestCallback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width,height);
}