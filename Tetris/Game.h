// Game.h
// �Q�[���S�̂̐���

#pragma once

#include "Vec2.h"
#include "Mino.h"
#include "TetriMino.h"

// �Q�[���̃��W�b�N�Ǘ�
class Game
{
public:
	static float BLOCK_SIDE_LENGTH;
	static Vec2f BLOCK_SIZE;
	static Vec2f FIELD_BOT_LEFT;

	static const int SENTINELS_COUNT = 2;
	static const int FIELD_WIDTH = 10 + SENTINELS_COUNT * 2;
	static const int FIELD_HEIGHT = 20 + SENTINELS_COUNT * 2; // �Q�[���I�[�o�[����p�̏�̕�����ǉ�
	static const int DROP_INTERVAL = 20;
	static Vec2i FIELD_SIZE;
	static const int DELETE_LINE_POINTS = 300;
	static const int DROP_MINO_POINTS = 2;

	bool mExists[FIELD_HEIGHT][FIELD_WIDTH]; // �e�}�X�ڂɃu���b�N�����݂��邩�ۂ�
	bool mToBeDropped = false; // �e�g���~�m�̗��������݂邩�ǂ���
	bool mToBeRotated = false; // �e�g���~�m�̉�]�����݂邩�ǂ���

private:
	int mDropCount = 0; // �����̂��߂̃t���[���J�E���^

public:
	/// special
	Game();
	~Game();

	/// normal
	void Process(); // �}�C�t���[���̏���
	bool IsMovable(const Mino& mino, int horizontal, int vertical); // �ړ��\����
	bool IsMovable(const TetriMino& mino, int horizontal, int vertical); // �ړ��\����
	bool IsRotatable(const TetriMino& mino); // ��]�\����
	void PlaceCurrent(const Mino& mino); // ���݂̃I�u�W�F�N�g�����̈ʒu�Őݒu�m�肳����
	void PlaceCurrent(const TetriMino& mino); // ���݂̃I�u�W�F�N�g�����̈ʒu�Őݒu�m�肳����
	int DropLines(); // ������Ă��郉�C�������o���A�u���b�N��S�̓I�ɉ��� ret: �������s��
	bool IsGameOver(); // �Q�[���I�[�o�[����

private:
	bool IsLineFilled(int y); // �w��̗񂪖��܂��Ă��邩�ǂ���
	void MoveLine(int from, int to); // ���C���̈ړ�

};

