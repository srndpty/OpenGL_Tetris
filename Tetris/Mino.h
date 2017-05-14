#pragma once

#include "Sprite.h"

// テトリミノ1個分のオブジェクト
class Mino : public Sprite<4>
{
public:
	static constexpr int VERTS_COUNT = 4;

public:
	Mino() = delete;
	Mino(Vec2 aSize, Vec2 aPos);
	~Mino();
};

