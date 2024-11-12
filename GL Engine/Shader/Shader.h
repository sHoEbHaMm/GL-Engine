#pragma once

#include<iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>

class Shader 
{
public:
	Shader() = delete;
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

private:
	unsigned int ID;

public:

	inline void useProgram() { glUseProgram(ID); }
	inline unsigned int GetID() { return ID;  }

private:
	void CheckCompileErrors(unsigned int shader, std::string type);
};

