#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

class LinearRenderer 
{

public:
	LinearRenderer(const GLchar* vsPath, const GLchar* fsPath)
	{
		// Read shader code from files
		std::string vSource, fSource;
		std::ifstream vsFile, fsFile;

		vsFile.exceptions(std::ifstream::badbit);
		fsFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vsFile.open(vsPath);
			fsFile.open(fsPath);
			std::stringstream vsStream, fsStream;
			// Read into streams
			vsStream << vsFile.rdbuf();
			fsStream << fsFile.rdbuf();
			// Close files
			vsFile.close();
			fsFile.close();
			// Write into strings
			vSource = vsStream.str();
			fSource = fsStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR READING SHADERS FROM FILE" << std::endl;
		}
		const GLchar* vsSource = vSource.c_str();
		const GLchar* fsSource = fSource.c_str();

		// Compile shaders and link program
		GLuint vs, fs;
		GLint success;
		GLchar log[512];

		// Vertex shader
		vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vsSource, NULL);
		glCompileShader(vs);
		// Check is compilation was succesful
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vs, 512, NULL, log);
			std::cout << "ERROR: FAILED TO COMPILE VERTEX SHADER\n" << log << std::endl;
		}

		// Fragment shader
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fsSource, NULL);
		glCompileShader(fs);
		// Check if compilation was succesfull
		glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fs, 512, NULL, log);
			std::cout << "ERROR: FAILED TO COMPILE FRAGMENT SHADER\n" << log << std::endl;
		}

		// Link program
		this->program = glCreateProgram();
		glAttachShader(this->program, vs);
		glAttachShader(this->program, fs);
		glLinkProgram(this->program);
		// Check that linking was succesful
		glGetProgramiv(this->program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->program, 512, NULL, log);
			std::cout << "ERROR: FAILED TO LINK PROGRAM\n" << log << std::endl;
		}
		// Delete shaders
		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	float pi = glm::radians(180.0f);
	void render(GLuint* vao)
	{
		//Clear screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Determine color and draw stuff
		glUseProgram(this->program);
		glBindVertexArray(*vao);
		float t = (float)glfwGetTime();
		glm::vec3 colors[] = {
			glm::vec3(sin(t) / 2 + 0.5f, sin(t + pi) / 2 + 0.5f, 0),
			glm::vec3(0, sin(t) / 2 + 0.5f, sin(t + pi) / 2 + 0.5f),
			glm::vec3(sin(t + pi) / 2 + 0.5f, 0, sin(t) / 2 + 0.5f),
			glm::vec3(sin(t / 2) / 2 + 0.5f, sin(t / 3) / 2 + 0.5f, sin(t) / 2 + 0.5f),
		};
		glUniform3fv(0, 4, glm::value_ptr(colors[0]));
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	GLuint program;

};