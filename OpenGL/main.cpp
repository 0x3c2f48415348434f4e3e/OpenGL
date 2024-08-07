#include<glad.h>
#include<glfw3.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MESSAGE 1028
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
	glfwSetFramebufferSizeCallback(window, windowRestCallback);

	/*//create our vertext data
	float vertextPosition[9] = {
		0.0f,0.5f,0.0f,
		-0.5f,0.0f,0.0f,
		0.5f,0.0f,0.0f

	};*/

	//Draw a rectangle using triangle
	float vertextPosition[] = {
		//order for a single triangle does not matter, but for miltiple, it does
		//lets add a color attribe
		-0.45f,0.5f,0.0f,1.0f,0.0f,0.0f,
		-0.2f,-0.5f,0.0f,0.0f,1.0f,0.0f,
		-0.9f,-0.5f,0.0f,0.0f,0.0f,1.0f

	};



	//create vertex buffer object andd vertext Array object
	unsigned int VBO, VAO;
	//generate buffers
	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);

	//First stuff here
	glBindVertexArray(VAO);

	//bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//populate buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertextPosition), vertextPosition, GL_STATIC_DRAW);

	//we will nout unbine the EBO as VAO ia active

	//configure vertex pointer to be used for Vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*) (3*sizeof(float)));
	glEnableVertexAttribArray(1);
	//after configuring vertex buffer, unbind Vertex Array Object
	//unubind buffer object too
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//set up shader, start with vertex shader
	const char* vertexShader = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 colorPos;\n"
		"out vec3 aColor;"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"aColor = colorPos;\n"
		"}\n\0";
	
	//fragment shader
	const char* fragmentShader = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 aColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(aColor,1.0f);\n"
		"}\n\0";
	//compile shader and link them
	unsigned int vertexShaderCompile = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderCompile = glCreateShader(GL_FRAGMENT_SHADER);

	//add shader source code into object
	glShaderSource(vertexShaderCompile, 1, &vertexShader, NULL);
	//compile vertex shader
	glCompileShader(vertexShaderCompile);
	int success;
	char INFO[MESSAGE];
	glGetShaderiv(vertexShaderCompile, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderCompile, MESSAGE, NULL, INFO);
		printf("Unable to compile Vertex shader... Error %s", INFO);
		exit(-1);
	}

	//so same for fragment shader
	glShaderSource(fragmentShaderCompile, 1, &fragmentShader, NULL);
	//compile shader
	glCompileShader(fragmentShaderCompile);
	//check compilation error
	glGetShaderiv(fragmentShaderCompile, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderCompile, MESSAGE, NULL, INFO);
		printf("Unable to compile fragment shader... Error %s", INFO);
		exit(-1);
	}


	//if vertex and fragment shader succesfull, link shader
	//create program first
	unsigned int program = glCreateProgram();
	//attach compiled programs
	glAttachShader(program, vertexShaderCompile);
	glAttachShader(program, fragmentShaderCompile);
	//link shader to create final program
	glLinkProgram(program);

	//check for link errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, MESSAGE, NULL, INFO);
		printf("Unable to Link shaders... Error %s", INFO);
		exit(-1);
	}

	//rendering loop

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	while (!glfwWindowShouldClose(window)) {
		//clear color buffer
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//user first program
		glUseProgram(program);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//unbindd after
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//unbind VAO after rendering
	glBindVertexArray(0);

	//free resources like buffers and the programs generated
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//if program successfully linked, delete shaders
	glDeleteShader(vertexShaderCompile);
	glDeleteShader(fragmentShaderCompile);
	glDeleteProgram(program);
	glfwTerminate();

}

void windowRestCallback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width,height);
}