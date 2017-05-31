#include "Game.h"


float Game::BLOCK_SIDE_LENGTH = 0.09f;
Vec2f Game::BLOCK_SIZE = { BLOCK_SIDE_LENGTH, BLOCK_SIDE_LENGTH };
Vec2f Game::FIELD_BOT_LEFT = { -0.55f, -0.45f };
Vec2i Game::FIELD_SIZE = { 10 + 2, 20 + 1 }; // ç∂âEÇ∆â∫Ç…î‘ï∫Çïtó^

Game::Game()
	//:mExists{}
{
	for (size_t i = 0; i < FIELD_HEIGHT; i++)
	{
		for (size_t j = 0; j < FIELD_WIDTH; j++)
		{
			if (j == FIELD_WIDTH - 1 || j == 0 || i == 0)
			{
				mExists[i][j] = true;
			}
			else
			{
				mExists[i][j] = false;
			}
		}
	}
}


Game::~Game()
{
}

void Game::Process()
{
	++mDropCount;

	if (mDropCount % DROP_INTERVAL == 0)
	{
		mToBeDropped = true;
	}
}

bool Game::IsDroppable(const Mino& mino)
{
	return !mExists[mino.mPosition.y - 1][mino.mPosition.x];
}

bool Game::IsMovable(const Mino & mino, int horizontal, int vertical)
{
	return !mExists[mino.mPosition.y + vertical][mino.mPosition.x + horizontal];
}

void Game::PlaceCurrent(const Mino& mino)
{
	mExists[mino.mPosition.y][mino.mPosition.x] = true;
}

void Game::DropLines()
{
	int targetLine = 1;

	for (size_t i = 1; i < FIELD_HEIGHT; i++)
	{
		MoveLine(i, targetLine);
		if (!IsLineFilled(i))
		{
			++targetLine;
		}
	}
}

bool Game::IsLineFilled(int y)
{
	for (size_t i = 1; i < FIELD_WIDTH - 1; i++)
	{
		if (!mExists[y][i])
		{
			return false;
		}
	}
	return true;
}

void Game::MoveLine(int from, int to)
{
	for (size_t i = 1; i < FIELD_WIDTH - 1; i++)
	{
		mExists[to][i] = mExists[from][i];
	}
}

