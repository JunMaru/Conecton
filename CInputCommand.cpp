//=============================================================================
//
// �C���v�b�g�R�}���h���� [CInputCommand.cpp]
// Author : JUN MARUYAMA
// Since  : 2015/06/22
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CInputCommand.h"
#include "CInput.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputCommand::CInputCommand(CInputKeyboard *keyboard, CInputJoypad *joypad)
{
	m_controller = 0;
	m_oldController = 0;
	m_repeatController = 0;

	m_pInputJoypad = joypad;
	m_pInputKeyboard = keyboard;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputCommand::~CInputCommand()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputCommand::Init(void)
{
	// �R�}���h�\�̐ݒ�
	// TODO::��ł���t�@�C���ǂݍ���
	const KEY2COM keyWork[COMMAND_MAX] =
	{
		{ DIK_RETURN, COMMAND_ENTER },
		{ DIK_C, COMMAND_CANCEL },
		{ DIK_A, COMMAND_LEFT },
		{ DIK_D, COMMAND_RIGHT },
		{ DIK_W, COMMAND_UP },
		{ DIK_S, COMMAND_DOWN },
		{ DIK_M, COMMAND_CHANGEANTON },
		{ DIK_N, COMMAND_CONNECT },
		{ DIK_4, COMMAND_CALLANTON },
#if DEBUGMODE_INPUTCOMMAND
		// �f�o�b�O�p
		{ DIK_2, COMMAND_METTAL },
		{ DIK_3, COMMAND_MINIMAMU },
		{ DIK_5, COMMAND_POWERFULL },
		{ DIK_1, COMMAND_NORMAL },
#endif
	};
	memcpy(m_key2com, keyWork, sizeof(m_key2com));

	// TODO::��ł���t�@�C���ǂݍ���
	const JOY2COM joyWork[COMMAND_MAX] =
	{
		{ CInputJoypad::GAMEPAD_1, COMMAND_ENTER },
		{ CInputJoypad::GAMEPAD_2, COMMAND_CANCEL },
		{ CInputJoypad::GAMEPAD_LSTICK_LEFT, COMMAND_LEFT },
		{ CInputJoypad::GAMEPAD_LSTICK_RIGHT, COMMAND_RIGHT },
		{ CInputJoypad::GAMEPAD_LSTICK_UP, COMMAND_UP },
		{ CInputJoypad::GAMEPAD_LSTICK_DOWN, COMMAND_DOWN },
		{ CInputJoypad::GAMEPAD_1, COMMAND_CHANGEANTON },
		{ CInputJoypad::GAMEPAD_2, COMMAND_CONNECT },
		{ CInputJoypad::GAMEPAD_3, COMMAND_CALLANTON },
#if DEBUGMODE_INPUTCOMMAND
		// �f�o�b�O�p(�W���C�p�b�h�Ɋւ��Ă̓{�^����������Ȃ��̂œK��)
		{ CInputJoypad::GAMEPAD_2, COMMAND_METTAL },
		{ CInputJoypad::GAMEPAD_4, COMMAND_MINIMAMU },
		{ CInputJoypad::GAMEPAD_5, COMMAND_POWERFULL },
		{ CInputJoypad::GAMEPAD_1, COMMAND_NORMAL },
#endif
	};
	memcpy(m_joy2com, joyWork, sizeof(m_joy2com));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputCommand::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CInputCommand::Update(void)
{
	m_oldController = m_controller;
	m_controller = 0;

	CheckKeyboard();
	CheckJoypad();
}

//=============================================================================
// �L�[�{�[�h���擾
//=============================================================================
void CInputCommand::CheckKeyboard(void)
{
	if (m_pInputKeyboard == NULL)
	{
		return;
	}

	for (int nCnt = 0; nCnt < COMMAND_MAX; ++nCnt)
	{
		if (m_pInputKeyboard->GetKeyPress(m_key2com[nCnt].key))
		{
			m_controller |= m_key2com[nCnt].command;
		}

		if (m_pInputKeyboard->GetKeyRepeat(m_key2com[nCnt].key))
		{
			m_repeatController |= m_key2com[nCnt].command;
		}
	}
}

//=============================================================================
// �W���C�p�b�h���擾
//=============================================================================
void CInputCommand::CheckJoypad(void)
{
	if (m_pInputJoypad == NULL)
	{
		return;
	}

	for (int nCnt = 0; nCnt < COMMAND_MAX; ++nCnt)
	{
		if (m_pInputJoypad->GetKeyPress(static_cast<CInputJoypad::GAMEPAD>(m_joy2com[nCnt].joy)))
		{
			m_controller |= m_joy2com[nCnt].command;
		}

		if (m_pInputJoypad->GetKeyRepeat(static_cast<CInputJoypad::GAMEPAD>(m_joy2com[nCnt].joy)))
		{
			m_repeatController |= m_joy2com[nCnt].command;
		}
	}
}

//=============================================================================
// �v���X���擾
//=============================================================================
bool CInputCommand::IsPress(COMMAND command)
{
	if (m_controller & command)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �g���K�[���擾
//=============================================================================
bool CInputCommand::IsTrigger(COMMAND command)
{
	if ((m_controller ^ m_oldController) & m_controller & command)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �����[�X���擾
//=============================================================================
bool CInputCommand::IsRelease(COMMAND command)
{
	if ((m_controller ^ m_oldController) & ~m_controller & command)
	{
		return true;
	}

	return false;
}

//=============================================================================
// ���s�[�g���擾
//=============================================================================
bool CInputCommand::IsRepeat(COMMAND command)
{
	if (m_repeatController & command)
	{
		return true;
	}

	return false;
}

