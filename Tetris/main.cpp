#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <memory>
#include "linmath.h"

#include "Shader.h"
#include "Vec2.h"
#include "Def.h"
#include "Input.h"
#include "Sprite.h"
#include "Mino.h"
#include "Collision.h"
#include "Loader.h"

#pragma comment(lib, "opengl32.lib")

static Vec2 BAR_SIZE = { 0.1f, 0.5f };
static Vec2 NUM_SIZE = { 0.15f, 0.15f };
static constexpr int BALL_VERTS_COUNT = 32;
static constexpr int BAR_VERTS_COUNT = 4;

Input input;
Shader shader;

template<int VertsCount = 4>
class NumTex : public Sprite<VertsCount>
{
public:
	NumTex(Vec2 aSize, Vec2 aPos)
	{
		static_assert(VertsCount == 4, "VertsCount == 4");
		vertex[0] = geom[0] = { -aSize.x / 2, +aSize.y / 2 };
		vertex[1] = geom[1] = { +aSize.x / 2, +aSize.y / 2 };
		vertex[2] = geom[2] = { +aSize.x / 2, -aSize.y / 2 };
		vertex[3] = geom[3] = { -aSize.x / 2, -aSize.y / 2 };
		RefreshUv(0);
		pos = aPos;
		size = aSize * 0.5f;
		// ���f
		for (size_t i = 0; i < VertsCount; i++)
		{
			geom[i].x = pos.x + vertex[i].x;
			geom[i].y = pos.y + vertex[i].y;
		}
	}

	~NumTex()
	{
	}

	void Update(int aNum)
	{
		mNum = aNum;
		RefreshUv(aNum);
	}

	void RefreshUv(int index)
	{
		uv[0] = { index / 10.f, 1 };
		uv[1] = { (index + 1) / 10.f, 1 };
		uv[2] = { (index + 1) / 10.f, 0 };
		uv[3] = { index / 10.f, 0 };
	}

private:
	int mNum;
};


template<int VertsCount>
class Ball : public Sprite<VertsCount>
{
public:
	Ball(float aSize, float aDeg, float aSpeed)
		: mDeg(aDeg)
		, mSize(aSize)
		, mSpeed(aSpeed)
		, mMoveVec{ sin(aDeg / 180.0f * PI), cos(aDeg / 180.0f * PI) }
	{
		SetVertex();
		size = { aSize , aSize };
	}

	~Ball()
	{
	}

	void SetVertex()
	{
		for (size_t i = 0; i < VertsCount; i++)
		{
			vertex[i].x = cos((float)i / VertsCount * PI * 2) * mSize;
			vertex[i].y = sin((float)i / VertsCount * PI * 2) * mSize;
			uv[i].x = cos((float)i / VertsCount  * PI * 2) * 0.5f + 0.5f;
			uv[i].y = sin((float)i / VertsCount  * PI * 2) * 0.5f + 0.5f;
		}
	}

	void Move()
	{
		// �ړ�
		pos.x += mMoveVec.x * mSpeed;
		pos.y += mMoveVec.y * mSpeed;

		const float XLimit = 0.9f;
		const float YLimit = 0.55f;

		// ����
		// X�̓S�[������Ɏg���̂Ŕ��]���Ȃ�
		if (pos.x > XLimit - mSize)
		{
			//mMoveVec.x *= -1;
		}
		else if (pos.x < -XLimit + mSize)
		{
			//mMoveVec.x *= -1;
		}
		if (pos.y > YLimit - mSize)
		{
			mMoveVec.y *= -1;
		}
		else if (pos.y < -YLimit + mSize)
		{
			mMoveVec.y *= -1;
		}

		// ���f
		for (size_t i = 0; i < VertsCount; i++)
		{
			geom[i].x = pos.x + vertex[i].x;
			geom[i].y = pos.y + vertex[i].y;
		}
	}

	void SwitchX()
	{
		mMoveVec.x *= -1;
	}

private:
	Vec2 mMoveVec;
	float mDeg;
	float mSize;
	float mSpeed;
};

template<int VertsCount>
class Bar : public Sprite<VertsCount>
{
public:
	static constexpr float MOVE_SPEED = 0.015f;
	static constexpr float Y_LIMIT = 0.625f;

public:
	Bar(Vec2 aSize, Vec2 aPos)
	{
		vertex[0] = geom[0] = { -aSize.x / 2, +aSize.y / 2 };
		vertex[1] = geom[1] = { +aSize.x / 2, +aSize.y / 2 };
		vertex[2] = geom[2] = { +aSize.x / 2, -aSize.y / 2 };
		vertex[3] = geom[3] = { -aSize.x / 2, -aSize.y / 2 };
		uv[0] = { 0, 1 };
		uv[1] = { 1, 1 };
		uv[2] = { 1, 0 };
		uv[3] = { 0, 0 };
		pos = aPos;
		size = aSize * 0.5f;
		// ���f
		for (size_t i = 0; i < VertsCount; i++)
		{
			geom[i].x = pos.x + vertex[i].x;
			geom[i].y = pos.y + vertex[i].y;
		}
	}

	~Bar()
	{
	}

	void MoveUp()
	{
		Move(Vec2{ 0, +MOVE_SPEED });
	}

	void MoveDown()
	{
		Move(Vec2{ 0, -MOVE_SPEED });
	}

	void Move(Vec2 aVec)
	{
		pos.x += aVec.x;
		pos.y += aVec.y;

		if (pos.y > Y_LIMIT - vertex[0].y)
		{
			pos.y = Y_LIMIT - vertex[0].y;
		}
		if (pos.y < -Y_LIMIT + vertex[0].y)
		{
			pos.y = -Y_LIMIT + vertex[0].y;
		}

		// ���f
		for (size_t i = 0; i < VertsCount; i++)
		{
			geom[i].x = pos.x + vertex[i].x;
			geom[i].y = pos.y + vertex[i].y;
		}
	}

private:

};

auto ball = std::make_unique<Ball<BALL_VERTS_COUNT>>(0.15f, 50.0f, 0.01f);
auto bar0 = std::make_unique<Bar<BAR_VERTS_COUNT>>(BAR_SIZE, Vec2{ -0.5f, 0.f });
auto bar1 = std::make_unique<Bar<BAR_VERTS_COUNT>>(BAR_SIZE, Vec2{ +0.5f, 0.f });
auto leftScore = std::make_unique<NumTex<>>(NUM_SIZE, Vec2{ -0.5f, 0.4f });
auto rightScore = std::make_unique<NumTex<>>(NUM_SIZE, Vec2{ +0.5f, 0.4f });

auto mino = std::make_unique<Mino>(Vec2{ 0.2f, 0.2f }, Vec2{ 0, 0 });

// �G���[�R�[���o�b�N
void ErrorCallback2(int error, const char* description)
{
	std::cerr << "Error Occured code: " << error << " desc: " << description << "\n";
}

// ���̓R�[���o�b�N
void KeyCallback2(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		input.mKeyStates[key].pressed = true;
	}
	else if (action == GLFW_RELEASE)
	{
		input.mKeyStates[key].pressed = false;
	}

	// ESC�ŏI��
	if (input.mKeyStates[GLFW_KEY_ESCAPE].pressed)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
#include <direct.h>
#define GetCurrentDir _getcwd
std::string GetCurrentWorkingDir(void)
{
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

int main()
{
	std::cout << "current directory is " << GetCurrentWorkingDir().c_str() << "\n";

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Tetris Game", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetErrorCallback(ErrorCallback2);
	glfwSetKeyCallback(window, KeyCallback2);

	// ���j�^�Ƃ̓���
	glfwMakeContextCurrent(window);
	auto addr = (GLADloadproc)glfwGetProcAddress;
	gladLoadGLLoader(addr);
	glfwSwapInterval(1);

	//GLuint programId = CreateShader();
	shader.SetUp();

	GLuint barId = LoadBmp("wood.bmp");
	GLuint ballId = LoadBmp("ball.bmp");
	GLuint numId = LoadBmp("num.bmp");
	GLuint minoId = LoadBmp("mino.bmp");

	int leftPoint = 0, rightPoint = 0;

	// �Q�[�����[�v
	while (!glfwWindowShouldClose(window))
	{
		// -- �v�Z --
		// �o�[�̈ړ�
		if (input.mKeyStates[GLFW_KEY_W].pressed)
		{
			bar0->MoveUp();
		}
		else if (input.mKeyStates[GLFW_KEY_S].pressed)
		{
			bar0->MoveDown();
		}

		if (input.mKeyStates[GLFW_KEY_UP].pressed)
		{
			bar1->MoveUp();
		}
		else if (input.mKeyStates[GLFW_KEY_DOWN].pressed)
		{
			bar1->MoveDown();
		}

		// �{�[���̈ړ�
		// ���W��geom�ւ̓K�p��K���Ō��
		const float X_LIMIT = 0.8f;
		if (ball->pos.x > +X_LIMIT)
		{
			leftPoint++;
			leftScore->Update(leftPoint);
			ball->pos.x = 0;
			ball->SwitchX();
		}
		else if (ball->pos.x < -X_LIMIT)
		{
			rightPoint++;
			rightScore->Update(rightPoint);
			ball->pos.x = 0;
			ball->SwitchX();
		}

		ball->Move();
		// �����蔻��
		if (IsCollidingSqSq(*ball, *bar0))
		{
			ball->SwitchX();
			ball->pos.x = bar0->pos.x + bar0->size.x / 2 + ball->size.x / 2;
		}
		if (IsCollidingSqSq(*ball, *bar1))
		{
			ball->SwitchX();
			ball->pos.x = bar1->pos.x - bar1->size.x / 2 - ball->size.x / 2;
		}


		// -- �`�� -- 
		// ��ʂ̏�����
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearDepth(1.0);

		//bar0->Draw(barId);
		//bar1->Draw(barId);
		//ball->Draw(ballId);
		//leftScore->Draw(numId);
		//rightScore->Draw(numId);
		mino->Draw(minoId);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}