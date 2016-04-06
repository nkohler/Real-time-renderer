#include <iostream>

// OpenGL stuff
#include <glew.h>
#include <glfw3.h>

// Window/viewport dimensions
GLuint WIDTH = 800, HEIGHT = 600;

// TEMPORARY shader source

const GLchar* vs_source = "#version 450 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 0) uniform vec3 colors[3];\n"
"out vec4 vsColor;\n"
"void main()\n"
"{\n"
"vsColor = vec4(colors[gl_VertexID], 1.0f);\n"
"gl_Position = vec4(position, 1.0f);\n"
"}\n\0";

const GLchar* fs_source = "#version 450 core\n"
"in vec4 vsColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vsColor;\n"
"}\n\0";

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(w, GL_TRUE);
	}
}

int main(int argc, char** argv) 
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Realtime Renderer", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// Intialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set viewport
	glViewport(0, 0, WIDTH, HEIGHT);

	// Load the shaders
	// Vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);
	// Check for compile time errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link Program
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	// Check for linking errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);

	// TEMPORARY: Create data and all else needed to draw a triangle
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Rendering loop
	while (!glfwWindowShouldClose(window))
	{
		// First, check for any events
		glfwPollEvents();

		//Render stuff here
		//Clear screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Determine color and draw stuff
		glUseProgram(program);
		float t = (float)glfwGetTime();
		GLfloat colors[] = { 
			sin(t) / 2 + 0.5f,		sin(t / 2) / 2 + 0.5f,	sin(t / 3) / 2 + 0.5f,
			sin(t / 3) / 2 + 0.5f,	sin(t) / 2 + 0.5f,		sin(t / 2) / 2 + 0.5f,
			sin(t / 2) / 2 + 0.5f,	sin(t / 3) / 2 + 0.5f,	sin(t) / 2 + 0.5f
		};
		glUniform3fv(0, 3, colors);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Swap screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);

	glfwTerminate();
	return 0;
}
