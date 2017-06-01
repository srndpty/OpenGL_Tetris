
#include "TetriMino.h"

#include "Random.h"

extern Random random;

MinoType TetriMino::minoTypes[MINO_TYPE_MAX] =
{
	{{ Vec2i{ 0, 0 },Vec2i{ 0,-1 },Vec2i{-1, 0 },Vec2i{+1, 0 },}, {1.0f, 0.0f, 1.0f, 1.0f} }, // T
	{{ Vec2i{ 0, 0 },Vec2i{ 0,-1 },Vec2i{ 0,-2 },Vec2i{ 0,+1 },}, {0.0f, 1.0f, 1.0f, 1.0f} }, // I
	{{ Vec2i{ 0, 0 },Vec2i{+1, 0 },Vec2i{ 0,-1 },Vec2i{+1,-1 },}, {1.0f, 1.0f, 0.0f, 1.0f} }, // O
	{{ Vec2i{ 0, 0 },Vec2i{ 0,+1 },Vec2i{ 0,-1 },Vec2i{+1,-1 },}, {1.0f, 0.0f, 1.0f, 1.0f} }, // L
	{{ Vec2i{ 0, 0 },Vec2i{ 0,+1 },Vec2i{ 0,-1 },Vec2i{-1,-1 },}, {0.0f, 0.0f, 1.0f, 1.0f} }, // J
	{{ Vec2i{ 0, 0 },Vec2i{+1, 0 },Vec2i{ 0,-1 },Vec2i{-1,-1 },}, {0.0f, 1.0f, 0.0f, 1.0f} }, // S
	{{ Vec2i{ 0, 0 },Vec2i{-1, 0 },Vec2i{ 0,-1 },Vec2i{+1,-1 },}, {1.0f, 0.0f, 0.0f, 1.0f} }, // Z
};

//--------------------------------------------------------------------------------
TetriMino::TetriMino()
{
}


//--------------------------------------------------------------------------------
TetriMino::TetriMino(Vec2i aPos)
{
	for (size_t i = 0; i < MINO_MAX; i++)
	{
		mMinos[i] = std::make_unique<Mino>(0, 0);
	}
	mPosition = aPos;
	SetType(random(MINO_TYPE_MAX));
	SetPos(mPosition);
}

//--------------------------------------------------------------------------------
TetriMino::~TetriMino()
{
}

//--------------------------------------------------------------------------------
void TetriMino::SetType(int type)
{
	assert(type < MINO_TYPE_MAX);

	for (size_t i = 0; i < MINO_MAX; i++)
	{
		mMinos[i]->mOffset = minoTypes[type].offset[i];
		mMinos[i]->SetColor(minoTypes[type].color);
	}
}

//--------------------------------------------------------------------------------
void TetriMino::SetPos(const Vec2i& pos)
{
	mPosition = pos;
	for (size_t i = 0; i < MINO_MAX; i++)
	{
		mMinos[i]->SetPos(pos + mMinos[i]->mOffset);
	}
}

//--------------------------------------------------------------------------------
void TetriMino::Move(const Vec2i& amount)
{
	mPosition += amount;
	SetPos(mPosition);
}

//--------------------------------------------------------------------------------
void TetriMino::Draw(int texId)
{
	for (size_t i = 0; i < MINO_MAX; i++)
	{
		mMinos[i]->Draw(texId);
	}
}

//--------------------------------------------------------------------------------
void TetriMino::Rotate()
{
	for (size_t i = 0; i < MINO_MAX; i++)
	{
		auto tmp = mMinos[i]->mOffset;
		mMinos[i]->mOffset.x = tmp.y;
		mMinos[i]->mOffset.y = -tmp.x;
	}
	SetPos(mPosition);
}
