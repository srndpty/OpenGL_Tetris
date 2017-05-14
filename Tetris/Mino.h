#pragma once

#include "Sprite.h"

struct XY
{
	float x, y;
};

// テトリミノ1個分のオブジェクト
class Mino : public Sprite<4>
{
public:
	static const int VERTS_COUNT = 4;
	static float BLOCK_SIDE_LENGTH;
	static Vec2f BLOCK_SIZE;
	static Vec2f FIELD_BOT_LEFT;

public:
	Mino() = delete;
	Mino(Vec2f aSize, Vec2f aPos);
	~Mino();
};


