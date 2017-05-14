#pragma once

#include "GLFW/glfw3.h"

class Shader
{
public:
	static const char* ID_position;
	static const char* ID_uv;
	static const char* ID_texture;
	static const char* ID_MVP;
public:
	Shader();
	~Shader();
	void SetUp();

	int mPositionLocation;
	int mUvLocation;
	int mTextureLocation;
	int mMvpLocation;

private:
	GLuint mProgramId;
};

