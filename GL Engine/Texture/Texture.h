#pragma once
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <string>

class Texture
{
private:
	unsigned char* m_Texturedata;
	unsigned int m_TextureID;
	int m_Width, m_Height, m_NRChannels;

public:
	Texture(const char* path);
	inline void Bind() { glBindTexture(GL_TEXTURE_2D, this->m_TextureID); }
	inline unsigned int GetID() { return m_TextureID; }

};