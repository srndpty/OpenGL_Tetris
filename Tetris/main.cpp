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
#include "Game.h"
#include "TetriMino.h"

#pragma comment(lib, "opengl32.lib")

static Vec2f NUM_SIZE = { 0.15f, 0.15f };

Input input;
Shader shader;

template<int VertsCount = 4>
class NumTex : public Sprite<VertsCount>
{
public:
	NumTex(Vec2f aSize, Vec2f aPos)
	{
		static_assert(VertsCount == 4, "VertsCount == 4");
		vertex[0] = geom[0] = { -aSize.x / 2, +aSize.y / 2 };
		vertex[1] = geom[1] = { +aSize.x / 2, +aSize.y / 2 };
		vertex[2] = geom[2] = { +aSize.x / 2, -aSize.y / 2 };
		vertex[3] = geom[3] = { -aSize.x / 2, -aSize.y / 2 };
		RefreshUv(0);
		pos = aPos;
		size = aSize * 0.5f;
		// 反映
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


auto leftScore = std::make_unique<NumTex<>>(NUM_SIZE, Vec2f{ -0.5f, 0.4f });
auto rightScore = std::make_unique<NumTex<>>(NUM_SIZE, Vec2f{ +0.5f, 0.4f });

//auto mino = std::make_unique<Mino>(Vec2f{ 0.1f, 0.1f }, Vec2f{ 0, 0 });

std::unique_ptr<Mino> minoList[Game::FIELD_HEIGHT][Game::FIELD_WIDTH];
std::unique_ptr<TetriMino> current;
std::unique_ptr<Game> game;

// エラーコールバック
void ErrorCallback2(int error, const char* description)
{
	std::cerr << "Error Occured code: " << error << " desc: " << description << "\n";
}

// 入力コールバック
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

	// ESCで終了
	if (input.mKeyStates[GLFW_KEY_ESCAPE].pressed)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

#ifdef _MSC_VER
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

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

	// モニタとの同期
	glfwMakeContextCurrent(window);
	auto addr = (GLADloadproc)glfwGetProcAddress;
	gladLoadGLLoader(addr);
	glfwSwapInterval(1);

	shader.SetUp();

	GLuint minoId = LoadBmp("mino.bmp");

	int leftPoint = 0, rightPoint = 0;

	// ミノ生成
	for (int i = 0; i < Game::FIELD_SIZE.y; i++)
	{
		for (int j = 0; j < Game::FIELD_SIZE.x; j++)
		{
			minoList[i][j] = std::make_unique<Mino>(j, i);
		}
	}

	game = std::make_unique<Game>();
	current = std::make_unique<TetriMino>(Vec2i{ Game::FIELD_WIDTH / 2, Game::FIELD_HEIGHT });

	// ゲームループ
	while (!glfwWindowShouldClose(window))
	{
		// -- 計算 --

		// 全体処理
		game->Process();

		// 左右移動
		if (input.GetButtomDown(GLFW_KEY_A))
		{
			if (game->IsMovable(*current, -1, 0))
			{
				current->Move({ -1, 0 });
			}
		}
		else if (input.GetButtomDown(GLFW_KEY_D))
		{
			if (game->IsMovable(*current, +1, 0))
			{
				current->Move({ +1, 0 });
			}
		}
		else if (input.GetButtomDown(GLFW_KEY_S))
		{
			if (game->IsMovable(*current, 0, -1))
			{
				current->Move({ 0, -1 });
			}
		}

		// 落下
		if (game->mToBeDropped)
		{
			if (game->IsMovable(*current, 0, -1))
			{
				current->Move({ 0, -1 });
			}
			else
			{
				game->PlaceCurrent(*current);
				game->DropLines();
				current->SetPos({ Game::FIELD_WIDTH / 2, Game::FIELD_HEIGHT });
			}
			game->mToBeDropped = false;
		}

		input.Update();
		input.ResetNow();

		// -- 描画 -- 
		// 画面の初期化
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearDepth(1.0);

		for (size_t i = 0; i < Game::FIELD_SIZE.y; i++)
		{
			for (size_t j = 0; j < Game::FIELD_SIZE.x; j++)
			{
				if (game->mExists[i][j])
				{
					minoList[i][j]->Draw(minoId);
				}
			}
		}

		current->Draw(minoId);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}