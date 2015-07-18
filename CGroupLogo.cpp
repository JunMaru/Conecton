/*-----------------------------------------------------------------------------
	�O���[�v���S��ʃt�F�[�Y�N���X
	author	OkadaMoeto
	since	20140717
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CGroupLogo.h"
#include "Define.h"
#include "CManager.h"
#include "CInput.h"
#include "CFade.h"

/*-----------------------------------------------------------------------------
	�O���[�v���S�̐����ݒ�
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_GROUPLOGO = "data/texture/logo_group/group_logo.png";
static const D3DXVECTOR3 POS_GROUPLOGO	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
static const float WIDTH_GROUPLOGO		= 400.0f;
static const float HEIGHT_GROUPLOGO		= 335.0f;

/*-----------------------------------------------------------------------------
	���S�\���^�C��
-----------------------------------------------------------------------------*/
static const float TIME_LOGODISP = 30.0f * 2.0f;

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
void CGroupLogo::Init(void)
{
	m_pLogoBg = CScene2D::Create(
									NULL,
									D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
									VEC3_ZERO,
									1280.0f,
									720.0f);

	// �O���[�v���S����
	m_pGroupLogo = CScene2D::Create(
									TEXTUREPATH_GROUPLOGO,
									POS_GROUPLOGO,
									VEC3_ZERO,
									WIDTH_GROUPLOGO,
									HEIGHT_GROUPLOGO);

	// ���S�w�i�͔��ɂ��Ȃ��ƌ����Ȃ�
	m_pLogoBg->SetDiffuse(COL_WHITE);

	m_bSkip = false;
	m_countDisp = 0.0f;

	// �t�F�[�h�C��
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 30.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CGroupLogo::Uninit(void)
{
	// �`��ΏۃI�u�W�F�N�g�̊J��
	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CGroupLogo::Update(void)
{
	// �t�F�[�h���Ă��Ȃ���΍X�V
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		m_countDisp++;

		UpdateInputEvent();

#ifdef _DEBUG
		UpdateInputEventDebug();
#endif

		Skip();
	}

	if(m_countDisp > TIME_LOGODISP)
	{
		CManager::GetPhaseFade()->Start(CFade::FADETYPE_OUT, 30.0f, COL_WHITE);

		// �����t�F�[�h�h�~
		m_bSkip = false;
		m_countDisp = 0.0f;
	}

	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		// �t�F�[�h�A�E�g�I����Ƀ^�C�g����ʑJ��
		CManager::SetPhase(CManager::PHASE_TITLE);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CGroupLogo.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:�t�F�[�Y�J��\n");
	CDebugProcDX9::Print("���S�\��(%f)\n", m_countDisp);
#endif
}

/*-----------------------------------------------------------------------------
	���̓C�x���g�X�V
-----------------------------------------------------------------------------*/
void CGroupLogo::UpdateInputEvent(void)
{
	CInputJoypad *pJoyPad = CManager::GetInputJoypad();

	if(pJoyPad == nullptr) return;

	if(pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_1)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_2)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_3)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_4)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_5)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_6)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_7)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_8)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_9)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_10)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_11)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_12))
	{
		m_bSkip = true;
	}
}

/*-----------------------------------------------------------------------------
	���̓C�x���g�X�V�i�f�o�b�O�j
-----------------------------------------------------------------------------*/
void CGroupLogo::UpdateInputEventDebug(void)
{
#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if(pKeyboard == nullptr) return;

	if(pKeyboard->GetKeyTrigger(DIK_RETURN))
	{
		m_bSkip = true;
	}
#endif
}

/*-----------------------------------------------------------------------------
	���S�\�����Ԃ̃X�L�b�v
-----------------------------------------------------------------------------*/
void CGroupLogo::Skip(void)
{
	if(m_bSkip)
	{
		m_countDisp = TIME_LOGODISP + 1;
	}
}
