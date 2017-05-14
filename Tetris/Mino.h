#pragma once

#include "Sprite.h"

// �e�g���~�m1���̃I�u�W�F�N�g
class Mino : public Sprite<4>
{
public:
	static constexpr int VERTS_COUNT = 4;

public:
	Mino() = delete;
	Mino(Vec2 aSize, Vec2 aPos);
	~Mino();
};

