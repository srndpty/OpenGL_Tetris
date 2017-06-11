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

namespace 
{
	GLFWwindow* window = nullptr;
	std::unique_ptr<Mino> minoList[Game::FIELD_HEIGHT][Game::FIELD_WIDTH];
	std::unique_ptr<TetriMino> current;
	std::unique_ptr<TetriMino> next;
	std::unique_ptr<TetriMino> pocket;
	std::unique_ptr<Game> game;
	auto scoreDisp = std::make_unique<NumDisp<4>>(Vec2f{ +0.5f, 0.4f });
	bool firstGameOver = true;
	int scorePoint = 0;
	GLuint minoId;
	GLuint numId;
}


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
// ��ƃf�B���N�g���̎擾
std::string GetCurrentWorkingDir(void)
{
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

//--------------------------------------------------------------------------------
// ���͂���̏���
void ProcessInputs()
{

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
			scorePoint += Game::DROP_MINO_POINTS;
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
	else if (input.GetButtomDown(GLFW_KEY_SPACE))
	{
		// �����؂�ɗ���
		while (game->IsMovable(*current, 0, -1))
		{
			current->Move({ 0, -1 });
			scorePoint += Game::DROP_MINO_POINTS;
		}
	}
	else if (input.GetButtomDown(GLFW_KEY_P))
	{
		// �|�P�b�g�ƌ���
		if (pocket->GetActive())
		{
			// ���݂��Ă���Ό���
			int pocketMinoType = pocket->GetCurrentType();
			pocket->ForcePositionAsType(current->GetCurrentType(), Game::POCKET_MINO_POS);
			current->ResetAsType(pocketMinoType);
		}
		else
		{
			// ���݂��Ȃ����
			pocket->ForcePositionAsType(current->GetCurrentType(), Game::POCKET_MINO_POS);
			pocket->SetActive(true);
			current->ResetAsType(current->GetNextType());
		}
	}
}

//--------------------------------------------------------------------------------
// �~�m�𗎉�������
void Drop()
{
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
			int deletedLines = game->DropLines();
			scorePoint += deletedLines * Game::DELETE_LINE_POINTS;
			current->ResetAsType(current->GetNextType());
			current->ProceedNextType();
			next->ForcePositionAsType(current->GetNextType(), Game::NEXT_MINO_POS);
		}
		game->mToBeDropped = false;
	}
}

//--------------------------------------------------------------------------------
// �Q�[���I�[�o�[�̏���
void ProcessGameover()
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

//--------------------------------------------------------------------------------
// �`��
void Draw()
{
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
	pocket->Draw(minoId);
	scoreDisp->Draw(numId);
}

//--------------------------------------------------------------------------------
// ���C�u�����̏�����
int LibInit()
{
	std::cout << "current directory is " << GetCurrentWorkingDir().c_str() << "\n";

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Tetris Game", NULL, NULL);
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

	return 0;
}

//--------------------------------------------------------------------------------
// ENTRY POINT
int main()
{
	if (LibInit())
	{
		return -1;
	}

	shader.SetUp();

	minoId = LoadBmp("images/mino.bmp");
	numId = LoadBmp("images/num.bmp");

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
	// ���̃~�m
	next = std::make_unique<TetriMino>(Vec2i{ Game::FIELD_WIDTH / 2, Game::FIELD_HEIGHT });
	next->ForcePositionAsType(current->GetNextType(), Game::NEXT_MINO_POS);
	// �|�P�b�g�~�m
	pocket = std::make_unique<TetriMino>(Vec2i{ Game::FIELD_WIDTH / 2, Game::FIELD_HEIGHT });
	pocket->ForcePositionAsType(current->GetNextType(), Game::POCKET_MINO_POS);
	pocket->SetActive(false);

	// �Q�[�����[�v
	while (!glfwWindowShouldClose(window))
	{
		// -- �v�Z --
		scoreDisp->Update(scorePoint);
		if (game->IsGameOver())
		{
			ProcessGameover();
		}
		else
		{
			// �S�̏���
			game->Process();
			ProcessInputs();

			// ����
			Drop();
		}


		input.Update();
		input.ResetNow();

		// -- �`�� -- 
		Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}