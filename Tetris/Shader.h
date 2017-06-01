// Shader.h
// シェーダーを管理する

#pragma once

#include "GLFW/glfw3.h"

// シェーダーを管理するクラス
class Shader
{
public:
	// シェーダーのプロパティ名
	static const char* ID_position;
	static const char* ID_uv;
	static const char* ID_texture;
	static const char* ID_MVP;

public:
	int mPositionLocation;
	int mUvLocation;
	int mTextureLocation;
	int mMvpLocation;

private:
	GLuint mProgramId;

public:
	/// special
	Shader();
	~Shader();

	/// normal
	void SetUp();
};

