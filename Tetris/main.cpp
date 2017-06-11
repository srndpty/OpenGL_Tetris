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
#include "Random.h"
#include "NumDisp.h"

#pragma comment(lib, "opengl32.lib")

Input input;
Shader shader;
Random random;


//auto mino = std::make_unique<Mino>(Vec2f{ 0.1f, 0.1f }, Vec2f{ 0, 0 });

std::unique_ptr<Mino> minoList[Game::FIELD_HEIGHT][Game::FIELD_WIDTH];
std::unique_ptr<TetriMino> current;
std::unique_ptr<TetriMino> next;
std::unique_ptr<Game> game;
auto score = std::make_unique<NumDisp<4>>(Vec2f{ +0.5f, 0.4f });

//--------------------------------------------------------------------------------
// �G���[�R�[���o�b�N
void ErrorCallback2(int error, const char* description)
{
	std::cerr << "Error Occured code: " << error << " desc: " << description << "\n";
}

//--------------------------------------------------------------------------------
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

#ifdef _MSC_VER
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

//--------------------------------------------------------------------------------
std::string GetCurrentWorkingDir(void)
{
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

//--------------------------------------------------------------------------------
// ENTRY POINT
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

	shader.SetUp();

	GLuint minoId = LoadBmp("images/mino.bmp");
	GLuint numId =  LoadBmp("images/num.bmp");

	int point = 0;
	int frameCount = 0;

	// �~�m����
	for (int i = 0; i < Game::FIELD_SIZE.y; i++)
	{
		for (int j = 0; j < Game::FIELD_SIZE.x; j++)
		{
			minoList[i][j] = std::make_unique<Mino>(j, i);
		}
	}

	game = std::make_unique<Game>();
	current = std::make_unique<TetriMino>(Vec2i{ Game::FIELD_WIDTH / 2, Game::FIELD_HEIGHT });
	next = std::make_unique<TetriMino>(Vec2i{ Game::FIELD_WIDTH / 2, Game::FIELD_HEIGHT });
	next->SetType(current->GetNextType());
	next->SetForcePosition({ 0.5f, 0.3f });

	bool firstGameOver = true;

	// �Q�[�����[�v
	while (!glfwWindowShouldClose(window))
	{
		// -- �v�Z --
		++frameCount;
		score->Update(frameCount);
		if (game->IsGameOver())
		{
			if (firstGameOver)
			{
				// �Ԃ�����
				for (size_t i = 0; i < Game::FIELD_SIZE.y; i++)
				{
					for (size_t j = 0; j < Game::FIELD_SIZE.x; j++)
					{
						if (game->mExists[i][j])
						{
							minoList[i][j]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
						}
					}
				}

				std::cout << "Game Over!\n";
				firstGameOver = false;
			}
		}
		else
		{
			// �S�̏���
			game->Process();

			// ���E�ړ�
			if (input.GetButtomDown(GLFW_KEY_A))
			{
				// ��
				if (game->IsMovable(*current, -1, 0))
				{
					current->Move({ -1, 0 });
				}
			}
			else if (input.GetButtomDown(GLFW_KEY_D))
			{
				// �E
				if (game->IsMovable(*current, +1, 0))
				{
					current->Move({ +1, 0 });
				}
			}
			else if (input.GetButtomDown(GLFW_KEY_S))
			{
				// ��
				if (game->IsMovable(*current, 0, -1))
				{
					current->Move({ 0, -1 });
				}
			}
			else if (input.GetButtomDown(GLFW_KEY_W))
			{
				// ��]
				if (game->IsRotatable(*current))
				{
					current->Rotate();
				}
			}
			else if (input.GetButtomDown(GLFW_KEY_ENTER))
			{
				// �����؂�ɗ���
				while (game->IsMovable(*current, 0, -1))
				{
					current->Move({ 0, -1 });
				}
			}



			// ����
			if (game->mToBeDropped)
			{
				if (game->IsMovable(*current, 0, -1))
				{
					current->Move({ 0, -1 });
				}
				else
				{
					game->PlaceCurrent(*current);
					for (size_t i = 0; i < TetriMino::MINO_MAX; i++)
					{
						auto pos = current->mMinos[i]->mPosition;
						minoList[pos.y][pos.x]->SetColor(current->minoTypes[current->mType].color);
					}
					game->DropLines();
					current->SetType(current->GetNextType());
					next->SetType(current->GetNextType());
					next->SetForcePosition({ 0.5f, 0.3f });
					current->SetPos({ Game::FIELD_WIDTH / 2, Game::FIELD_HEIGHT });
				}
				game->mToBeDropped = false;
			}
		}


		input.Update();
		input.ResetNow();

		// -- �`�� -- 
		// ��ʂ̏�����
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
		next->Draw(minoId);
		score->Draw(numId);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}