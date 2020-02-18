#include "Shader.h"
#include<fstream>
#include<iostream>
#include<sstream>

#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW\glfw3.h>
using namespace std;
Shader::Shader(const char* vertexPath,const char* fragmentPath)
{
	ifstream vertexFile;
	ifstream fragmentFile;
	stringstream vertexSStream;
	stringstream fragmentSStream;
	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);

	vertexFile.exceptions(ifstream::failbit || ifstream::badbit);
	fragmentFile.exceptions(ifstream::failbit || ifstream::badbit);

	try {
		if (!vertexFile.is_open()||!fragmentFile.is_open())
		{
			throw exception("open file error");
		}

		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();

		vertexString = vertexSStream.str();
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();

		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompileErroe(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompileErroe(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErroe(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		
	}
	catch (const exception &p) {
		cout << p.what() << endl;
	}
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::checkCompileErroe(unsigned int ID, std::string type)
{
	int success;
	char infolog[512];

	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, NULL, infolog);
			cout << "shader compile error: " << infolog << endl;
				
		}
	}
	else
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infolog);
			cout << "program LINKING error: " << infolog << endl;
		}
	}
}


