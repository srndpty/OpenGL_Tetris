// Mino.h
// �u���b�N1����\��

#pragma once

#include "Sprite.h"

// �e�g���~�m1���̃I�u�W�F�N�g
class Mino : public Sprite<4>
{
public:
	static const int VERTS_COUNT = 4;

	Vec2i mPosition{}; // �ʒu
	Vec2i mOffset{}; // �e�g���~�m�ɂ�����A���S�̃u���b�N�̈ʒu���猩��offset

private:
	int mDropCount = 0;


public:
	/// special
	Mino();
	Mino(Vec2f aPos);
	Mino(int aX, int aY);
	~Mino();

	/// normal
	void Move(const Vec2i& amount);
	void SetPos(const Vec2i& pos);

private:
	void InitInfo(Vec2f aSize, Vec2f aPos);
	void ApplyPos();
};


