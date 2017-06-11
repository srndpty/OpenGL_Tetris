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
	int mNextType; // ���ɍ~���Ă���~�m�̃^�C�v
	bool mIsActive = true;

public:
	/// special
	TetriMino();
	TetriMino(Vec2i aPos);
	~TetriMino();

	/// normal
	void SetType(int type); // �e�g���~�m�̃^�C�v��ݒ�
	void SetPos(const Vec2i& pos); // ��ʒu�̐ݒ�
	void SetForcePosition(const Vec2f& aPos); // �����I�ɍ��W�����㏑���i���̃~�m�p�j
	void Move(const Vec2i& amount); // �ړ�
	void Draw(int texId); // �`��
	void Rotate(); // ��]
	void ResetAsType(int type);
	void ForcePositionAsType(int type, const Vec2f& pos);
	void ProceedNextType(); // ���̃~�m���X�V����

	// get/set
	int GetNextType() const { return mNextType; }
	void SetActive(bool isActive) { mIsActive = isActive; }
	bool GetActive() const { return mIsActive; }
	int GetCurrentType() const { return mType; }
};

// �e�g���~�m�̃^�C�v��\������N���X
struct MinoType
{
	Vec2i offset[TetriMino::MINO_MAX];
	Vec4f color; // �F�iRGBA�j
};


