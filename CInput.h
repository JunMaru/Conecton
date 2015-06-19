/*-----------------------------------------------------------------------------
���̓C���^�[�t�F�[�X�N���X
author	OkadaMoeto
since	20140521
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
���̓C���^�[�t�F�[�X�N���X
-----------------------------------------------------------------------------*/
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual HRESULT Update(void) = 0;

protected:
	// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDIDevice;
	// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
	static LPDIRECTINPUT8 m_pDInput;
};

/*-----------------------------------------------------------------------------
�L�[�{�[�h���̓N���X
-----------------------------------------------------------------------------*/
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	/*-----------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�擾(�����Ă���L�[�̎擾)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	param	nKey		�L�[���(DIK_xx)
	-----------------------------------------------------------------------------*/
	BOOL GetKeyPress(int nKey);

	/*-----------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�擾(�������u�Ԃ̃L�[�擾)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	param	nKey		�L�[���(DIK_xx)
	-----------------------------------------------------------------------------*/
	BOOL GetKeyTrigger(int nKey);

	/*-----------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�擾(�������u�Ԃ̃L�[�擾)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	param	nKey		�L�[���(DIK_xx)
	-----------------------------------------------------------------------------*/
	BOOL GetKeyRelease(int nKey);

	/*-----------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�擾(�������u�ԂƉ��������Ă���L�[�̎擾)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	param	nKey		�L�[���(DIK_xx)
	-----------------------------------------------------------------------------*/
	BOOL GetKeyRepeat(int nKey);

	/*-----------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�폜(�������u�Ԃ̃L�[�̓��e�폜)
	param	nKey		�L�[���(DIK_xx)
	-----------------------------------------------------------------------------*/
	void FlushKeyTrigger(int nKey);

private:
	// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE m_aKeyState[256];
	// �g���K�[���[�N
	BYTE m_aKeyStateTrigger[256];
	// �����[�X���[�N
	BYTE m_aKeyStateRelease[256];
	// ���s�[�g���[�N
	BYTE m_aKeyStateRepeat[256];
	// ���s�[�g�J�E���^
	int m_aKeyStateRepeatCnt[256];
};

/*-----------------------------------------------------------------------------
�}�E�X���̓N���X
-----------------------------------------------------------------------------*/
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�f�[�^�擾(���N���b�N���Ă���)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	-----------------------------------------------------------------------------*/
	BOOL GetLeftPress(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�f�[�^�擾(���N���b�N����)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	-----------------------------------------------------------------------------*/
	BOOL GetLeftTrigger(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�f�[�^�擾(�E�N���b�N���Ă���)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	-----------------------------------------------------------------------------*/
	BOOL GetRightPress(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�f�[�^�擾(�E�N���b�N����)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	-----------------------------------------------------------------------------*/
	BOOL GetRightTrigger(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�f�[�^�擾(���N���b�N���Ă���)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	-----------------------------------------------------------------------------*/
	BOOL GetCenterPress(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�f�[�^�擾(���N���b�N����)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	-----------------------------------------------------------------------------*/
	BOOL GetCenterTrigger(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�f�[�^�擾(�w���ړ�)
	return	LONG		X���ړ���
	-----------------------------------------------------------------------------*/
	LONG GetAxisX(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�f�[�^�擾(Y���ړ�)
	return	LONG		Y���ړ���
	-----------------------------------------------------------------------------*/
	LONG GetAxisY(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�f�[�^�擾(Z���ړ�)
	return	LONG		Z���ړ���
	-----------------------------------------------------------------------------*/
	LONG GetAxisZ(void);

	/*-----------------------------------------------------------------------------
	�}�E�X�̃X�N���[�����W�擾
	return POINT*		�}�E�X�̃X�N���[�����W�̒l
	-----------------------------------------------------------------------------*/
	POINT* GetPosWorld(void){ return &m_posMouseWorld; }

private:
	// �}�E�X�̏�Ԃ��󂯎�郏�[�N
	DIMOUSESTATE2 m_mouseState;
	// �g���K�[���[�N
	DIMOUSESTATE2 m_mouseStateTrigger;
	// �}�E�X�̃X�N���[�����W
	POINT m_posMouseWorld;
};

/*-----------------------------------------------------------------------------
�W���C�p�b�h���̓N���X
-----------------------------------------------------------------------------*/
class CInputJoypad : public CInput
{
public:
	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	// �Q�[���p�b�h��
	typedef enum
	{
		// �Q�[���p�b�h
		GAMEPAD_LSTICK_LEFT = 0,
		GAMEPAD_LSTICK_RIGHT,
		GAMEPAD_LSTICK_UP,
		GAMEPAD_LSTICK_DOWN,
		GAMEPAD_RSTICK_LEFT,
		GAMEPAD_RSTICK_RIGHT,
		GAMEPAD_RSTICK_UP,
		GAMEPAD_RSTICK_DOWN,
		GAMEPAD_1,
		GAMEPAD_2,
		GAMEPAD_3,
		GAMEPAD_4,
		GAMEPAD_5,
		GAMEPAD_6,
		GAMEPAD_7,
		GAMEPAD_8,
		GAMEPAD_9,
		GAMEPAD_10,
		GAMEPAD_11,
		GAMEPAD_12,
		GAMEPAD_MAX
	}GAMEPAD;

	/*-----------------------------------------------------------------------------
	�W���C�p�b�h���擾(�����Ă���)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	param	nKey		�L�[���(GAMEPAD_xx)
	-----------------------------------------------------------------------------*/
	bool GetKeyPress(GAMEPAD key);

	/*-----------------------------------------------------------------------------
	�W���C�p�b�h���擾(������)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	param	nKey		�L�[���(GAMEPAD_xx)

	-----------------------------------------------------------------------------*/
	bool GetKeyTrigger(GAMEPAD key);

	/*-----------------------------------------------------------------------------
	�W���C�p�b�h���擾(������)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	param	nKey		�L�[���(GAMEPAD_xx)
	-----------------------------------------------------------------------------*/
	bool GetKeyRelease(GAMEPAD key);

	/*-----------------------------------------------------------------------------
	�W���C�p�b�h���擾(���������Ă���)
	return	BOOL		���͌���
	retval	TRUE		������Ă���
	retval	FALSE		������Ă��Ȃ�
	param	nKey		�L�[���(GAMEPAD_xx)
	-----------------------------------------------------------------------------*/
	bool GetKeyRepeat(GAMEPAD key);

private:
	// �R�[���o�b�N�֐�
	static BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);
	static BOOL CALLBACK EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	// �X�e�[�g����
	void SetKeyState(void);

	// �W���C�p�b�h��Ԃ��󂯎�郏�[�N
	DIJOYSTATE2 m_joypadState;
	// �v���X���[�N
	bool m_aKeyState[GAMEPAD_MAX];
	// �g���K�[���[�N
	bool m_aKeyStateTrigger[GAMEPAD_MAX];
	// �����[�X���[�N
	bool m_aKeyStateRelease[GAMEPAD_MAX];
	// ���s�[�g���[�N
	bool m_aKeyStateRepeat[GAMEPAD_MAX];
	// ���s�[�g�J�E���^
	int m_aKeyStateRepeatCnt[GAMEPAD_MAX];
};
