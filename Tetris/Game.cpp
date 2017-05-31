#include "Game.h"


float Game::BLOCK_SIDE_LENGTH = 0.09f;
Vec2f Game::BLOCK_SIZE = { BLOCK_SIDE_LENGTH, BLOCK_SIDE_LENGTH };
Vec2f Game::FIELD_BOT_LEFT = { -0.55f, -0.50f };
Vec2i Game::FIELD_SIZE = { FIELD_WIDTH, FIELD_HEIGHT };

Game::Game()
	//:mExists{}
{
	for (size_t i = 0; i < FIELD_HEIGHT; i++)
	{
		for (size_t j = 0; j < FIELD_WIDTH; j++)
		{
			if (j >= FIELD_WIDTH - SENTINELS_COUNT || j < SENTINELS_COUNT || i < SENTINELS_COUNT)
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

bool Game::IsMovable(const Mino & mino, int horizontal, int vertical)
{
	return !mExists[mino.mPosition.y + vertical][mino.mPosition.x + horizontal];
}

bool Game::IsMovable(const TetriMino & mino, int horizontal, int vertical)
{
	for (size_t i = 0; i < TetriMino::MINO_MAX; i++)
	{
		if (mExists[mino.mMinos[i]->mPosition.y + vertical][mino.mMinos[i]->mPosition.x + horizontal])
		{
			return false;
		}
	}
	return true;
}

bool Game::IsRotatable(const TetriMino & mino)
{
	for (size_t i = 0; i < TetriMino::MINO_MAX; i++)
	{
		auto tmp = mino.mMinos[i]->mOffset;
		if (mExists[mino.mPosition.y + -tmp.x][mino.mPosition.x + tmp.y])
		{
			return false;
		}
	}

	return true;
}

void Game::PlaceCurrent(const Mino& mino)
{
	mExists[mino.mPosition.y][mino.mPosition.x] = true;
}

void Game::PlaceCurrent(const TetriMino & mino)
{
	for (size_t i = 0; i < TetriMino::MINO_MAX; i++)
	{
		mExists[mino.mMinos[i]->mPosition.y][mino.mMinos[i]->mPosition.x] = true;
	}
}

void Game::DropLines()
{
	int targetLine = SENTINELS_COUNT;

	for (size_t i = SENTINELS_COUNT; i < FIELD_HEIGHT; i++)
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
	for (size_t i = SENTINELS_COUNT; i < FIELD_WIDTH - SENTINELS_COUNT; i++)
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
	for (size_t i = SENTINELS_COUNT; i < FIELD_WIDTH - SENTINELS_COUNT; i++)
	{
		mExists[to][i] = mExists[from][i];
	}
}

