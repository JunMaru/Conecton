/*-----------------------------------------------------------------------------
	���U���g��ʃt�F�[�Y�N���X
	author	OkadaMoeto
	since	20140713
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CResult.h"
#include <string>
#include "Define.h"
#include "CManager.h"
#include "CInput.h"
#include "CFade.h"
#include "CScene2D.h"

/*-----------------------------------------------------------------------------
	�}�N����`
-----------------------------------------------------------------------------*/
using std::string;

// �����J�ڃJ�E���g��
static const float COUNT_AUTO_CHANGE = 180.0f;

// �^�C�g���w�i���
static const string FILENAME_THANKMSG = "data/texture/gui_result/thankmsg.png";
static const D3DXVECTOR3 POS_RESULT_BG	= D3DXVECTOR3(400.0f, 300.0f, 0.0f);
static const float WIDTH_RESULT_BG		= 800.0f;
static const float HEIGHT_RESULT_BG		= 600.0f;

// �V��ł����...���
static const string FILENAME_BG = "data/texture/gui_result/result_bg.jpg";
static const D3DXVECTOR3 POS_THANKMSG	= D3DXVECTOR3(400.0f, 300.0f, 0.0f);
static const float WIDTH_THANKMSG		= 800.0f;
static const float HEIGHT_THANKMSG		= 100.0f;

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CResult::CResult()
{
	m_thankmsg = nullptr;
	m_countAutoChange = 0.0f;
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CResult::~CResult()
{
}

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
void CResult::Init(void)
{
	// ���U���g�w�i�̐���
	CScene2D::Create(
						FILENAME_BG.c_str(),
						POS_RESULT_BG,
						VEC3_ZERO,
						WIDTH_RESULT_BG,
						HEIGHT_RESULT_BG);

	// �V��ł���Ă��肪�Ƃ��̃��b�Z�[�W����
	m_thankmsg = CScene2D::Create(
									FILENAME_THANKMSG.c_str(),
									POS_THANKMSG,
									VEC3_ZERO,
									WIDTH_THANKMSG,
									HEIGHT_THANKMSG);

	// �t�F�[�h�C��
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 60.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CResult::Uninit(void)
{
	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CResult::Update(void)
{
	// �L�[�{�[�h���͂��擾
	CInputKeyboard *pKeyboard = nullptr;
	pKeyboard = CManager::GetInputKeyboard();

	// �t�F�[�h���Ă��Ȃ���΍X�V
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		if(pKeyboard->GetKeyTrigger(DIK_RETURN))
		{
			CManager::GetPhaseFade()->Start(
											CFade::FADETYPE_OUT,
											60.0f,
											COL_WHITE);
		}

		// �����J�ڂ̏���
		m_countAutoChange++;
		if(m_countAutoChange > COUNT_AUTO_CHANGE)
		{
			CManager::GetPhaseFade()->Start(
											CFade::FADETYPE_OUT,
											60.0f,
											COL_WHITE);
		}
	}

	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		// �t�F�[�h�A�E�g�I����Ƀ^�C�g����ʑJ��
		CManager::SetPhase(CManager::PHASE_TITLE);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CResult.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:�t�F�[�Y�J��\n");
#endif
}
