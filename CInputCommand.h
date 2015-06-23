//=============================================================================
//
// �C���v�b�g�R�}���h���� [CInputCommand.h]
// Author : JUN MARUYAMA
// Since  : 2015/06/22
//
// �C���v�b�g�n���܂Ƃ߂��N���X
// new����Init�Ă��Update������Ɠ��͏���ǂݍ���ł��܂��B
// �������炿����ƈ��Ԃ�����ƃL�[�R���t�B�O����ꂽ�肷��B
// ���Ԃ�������΂�����c�͂�
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h�K�[�h
//*****************************************************************************
#pragma once

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Main.h"

//*****************************************************************************
// �}�N���錾
//*****************************************************************************
#define DEBUGMODE_INPUTCOMMAND (1)			// �f�o�b�O�X�C�b�`�B��������(1:ON/0:OFF)

//*****************************************************************************
// �O���N���X�錾
//*****************************************************************************
class CInputKeyborad;
class CInputJoypad;

//*****************************************************************************
// �C���v�b�g�R�}���h�N���X
//*****************************************************************************
class CInputCommand
{
public:

	// �R�}���h���
	enum COMMAND
	{
		COMMAND_ENTER = 1 << 0,
		COMMAND_CANCEL = 1 << 1,
		COMMAND_LEFT = 1 << 2,
		COMMAND_RIGHT = 1 << 3,
		COMMAND_UP = 1 << 4,
		COMMAND_DOWN = 1 << 5,
		COMMAND_CHANGEANTON = 1 << 6,
		COMMAND_CONNECT = 1 << 7,
		COMMAND_CALLANTON = 1 << 8,
		// �ȉ����i�K�̂��́B�{�����ł�COMMAND_CHANGEANTON�ɏW�񂳂��
#if DEBUGMODE_INPUTCOMMAND
		COMMAND_METTAL = 1 << 9,
		COMMAND_MINIMAMU = 1 << 10,
		COMMAND_POWERFULL = 1 << 11,
		COMMAND_NORMAL = 1 << 12,
		COMMAND_MAX = 13,
#else
		COMMAND_MAX = 9,
#endif
	};

	// �L�[���ƃR�}���h
	struct KEY2COM
	{
		unsigned short key;
		unsigned short command;
	};

	// �W���C�p�b�h���ƃR�}���h
	struct JOY2COM
	{
		unsigned short joy;
		unsigned short command;
	};

	//=============================================================================
	// �R���X�g���N�^
	// arg1 : CInputKeyboard *keyboard(�g�p���Ă���L�[�{�[�h���|�C���^)
	// arg2 : CInputJoypad *joypad(�g�p���Ă���W���C�p�b�h���|�C���^)
	//=============================================================================
	CInputCommand(CInputKeyboard *keyboard, CInputJoypad *joypad);

	//=============================================================================
	// �f�X�g���N�^
	//=============================================================================
	~CInputCommand();

	//=============================================================================
	// ������
	// return : HRESULT(����)
	//=============================================================================
	HRESULT Init(void);

	//=============================================================================
	// �I������
	//=============================================================================
	void Uninit(void);

	//=============================================================================
	// �X�V����
	//=============================================================================
	void Update(void);

	//=============================================================================
	// �v���X���擾
	// arg : COMMAND command(�v���X�����擾�������R�}���h)
	// return : bool(true:�����Ă���/false:������Ă��Ȃ�)
	//=============================================================================
	bool IsPress(COMMAND command);

	//=============================================================================
	// �g���K�[���擾
	// arg : COMMAND command(�g���K�[�����擾�������R�}���h)
	// return : bool(true:������/false:�����Ă��Ȃ�)
	//=============================================================================
	bool IsTrigger(COMMAND command);

	//=============================================================================
	// �����[�X���擾
	// arg : COMMAND command(�����[�X�����擾�������R�}���h)
	// return : bool(true:������/false:�����Ă��Ȃ�)
	//=============================================================================
	bool IsRelease(COMMAND command);

	//=============================================================================
	// ���s�[�g���擾
	// arg : COMMAND command(���s�[�g�����擾�������R�}���h)
	// return : bool(true:���������Ă���/false:���������Ă��Ȃ�)
	//=============================================================================
	bool IsRepeat(COMMAND command);

private:
	//=============================================================================
	// ���`�F�b�N(�L�[�{�[�h/�W���C�p�b�h)
	//=============================================================================
	void CheckKeyboard(void);
	void CheckJoypad(void);

	unsigned int m_controller;				// ���͏��
	unsigned int m_oldController;			// 1�t���[���O�̓��͏��
	unsigned int m_repeatController;		// ���s�[�g���
	KEY2COM m_key2com[COMMAND_MAX];			// �L�[���ƃR�}���h�̊֘A�t���ϐ�
	JOY2COM m_joy2com[COMMAND_MAX];			// �p�b�h���ƃR�}���h�̊֘A�t���ϐ�
	CInputJoypad *m_pInputJoypad;			// �W���C�p�b�h���
	CInputKeyboard *m_pInputKeyboard;		// �L�[�{�[�h���
};
