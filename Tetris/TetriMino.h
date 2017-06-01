// TetriMino.h
// �ʏ�4���킳�����u���b�N�i�e�g���~�m�j���Ǘ�����

#pragma once

#include <cassert>
#include <memory>

#include "Mino.h"

struct MinoType;

// �e�g���~�m��\������N���X
class TetriMino
{
public:
	static const int MINO_TYPE_MAX = 7;
	static const int MINO_MAX = 4;
	static MinoType minoTypes[MINO_TYPE_MAX]; // �e�g���~�m�̃p�^�[���ꗗ

public:
	Vec2i mPosition; // ��̈ʒu
	std::unique_ptr<Mino> mMinos[MINO_MAX]; // ��������4�̃u���b�N
	int mType; // �e�g���~�m�̃^�C�v

public:
	/// special
	TetriMino();
	TetriMino(Vec2i aPos);
	~TetriMino();

	/// normal
	void SetType(int type); // �e�g���~�m�̃^�C�v��ݒ�
	void SetPos(const Vec2i& pos); // ��ʒu�̐ݒ�
	void Move(const Vec2i& amount); // �ړ�
	void Draw(int texId); // �`��
	void Rotate(); // ��]
};

// �e�g���~�m�̃^�C�v��\������N���X
struct MinoType
{
	Vec2i offset[TetriMino::MINO_MAX];
	Vec4f color; // �F�iRGBA�j
};


