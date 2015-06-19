/*-----------------------------------------------------------------------------
	�^�C�g����ʃt�F�[�Y�N���X
	author	OkadaMoeto
	since	20140713
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CTitle.h"
#include <string>
#include "Define.h"
#include "CManager.h"
#include "CInput.h"
#include "CFade.h"
#include "CScene2D.h"
#include "CCamera.h"

/*-----------------------------------------------------------------------------
	�}�N����`
-----------------------------------------------------------------------------*/
using std::string;

// �^�C�g���w�i���
static const string FILENAME_BG = "data/texture/gui_title/title_bg.jpg";
static const D3DXVECTOR3 POS_TITLE_BG	= D3DXVECTOR3(400.0f, 300.0f, 0.0f);
static const float WIDTH_TITLE_BG		= 800;
static const float HEIGHT_TITLE_BG		= 600;

// �^�C�g�����S���
static const string FILENAME_TITLE_LOGO = "data/texture/gui_title/title_logo.png";
static const D3DXVECTOR3 POS_TITLE_LOGO = D3DXVECTOR3(400.0f, 150.0f, 0.0f);
static const float WIDTH_TITLE_LOGO		= 500.0f;
static const float HEIGHT_TITLE_LOGO	= 100.0f;

// �v���X�L�[���
static const string FILENAME_PRESSKEY = "data/texture/gui_title/press_enterkey.png";
static const D3DXVECTOR3 POS_PRESSKEY	= D3DXVECTOR3(400.0f, 450.0f, 0.0f);
static const float WIDTH_PRESSKEY		= 500.0f;
static const float HEIGHT_PRESSKEY		= 100.0f;

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CTitle::CTitle()
{
	m_pressKey = nullptr;
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CTitle::~CTitle()
{
}

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
void CTitle::Init(void)
{
	// �^�C�g���w�i�̐���
	CScene2D::Create(
						FILENAME_BG.c_str(),
						POS_TITLE_BG,
						VEC3_ZERO,
						WIDTH_TITLE_BG,
						HEIGHT_TITLE_BG);

	// �^�C�g�����S�̐���
	CScene2D::Create(
						FILENAME_TITLE_LOGO.c_str(),
						POS_TITLE_LOGO,
						VEC3_ZERO,
						WIDTH_TITLE_LOGO,
						HEIGHT_TITLE_LOGO);

	// �v���X�L�[�̐���
	m_pressKey = CScene2D::Create(
									FILENAME_PRESSKEY.c_str(),
									POS_PRESSKEY,
									VEC3_ZERO,
									WIDTH_PRESSKEY,
									HEIGHT_PRESSKEY);

	m_countBlink = 0.0f;
	m_countDecided = 0.0f;

	m_bDecide = false;

	// �t�F�[�h�C��
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 60.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CTitle::Uninit(void)
{
	// �`��ΏۃI�u�W�F�N�g�̊J��
	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CTitle::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �t�F�[�h���Ă��Ȃ���΍X�V
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		if(pKeyboard->GetKeyTrigger(DIK_RETURN))
		{
			m_bDecide = true;
		}

		// �v���X�L�[�̌���O�_��
		Blink(m_pressKey, 30.0f);
	}

	// �v���X�L�[�̌����_��
	if(m_bDecide)
	{
		Blink(m_pressKey, 3.0f);
		m_countDecided++;

		// �����̉�ʑJ�ڃ^�C�~���O�Ńt�F�[�h�ڍs
		if(m_countDecided > 60.0f)
		{
			CManager::GetPhaseFade()->Start(
												CFade::FADETYPE_OUT,
												60.0f,
												D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_bDecide = false; // �����t�F�[�h�h�~
		}
	}

	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		// �t�F�[�h�A�E�g�I����ɃQ�[����ʑJ��
		CManager::SetPhase(CManager::PHASE_GAME);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CTitle.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:�t�F�[�Y�J��\n");
#endif
}

/*-----------------------------------------------------------------------------
	2D�I�u�W�F�N�g�̓_��
-----------------------------------------------------------------------------*/
void CTitle::Blink(CScene2D* pTarget, float flashTime)
{
	m_countBlink++;
	
	// ���邢�����F�ɐ؂�ւ�
	if(m_countBlink > flashTime * 2)
	{
		pTarget->SetDiffuse(COL_WHITE);
		m_countBlink = 0.0f;
		return; // ������͂Ƃ΂�
	}

	// �Â������F�ɐ؂�ւ�
	if(m_countBlink > flashTime)
	{
		// �����̃A���t�@�l���P�D�O�Ȃ�΂��܂���p����
		pTarget->SetDiffuse(COL_WHITE_ALPHA(0.3f));
	}
}
