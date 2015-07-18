/*-----------------------------------------------------------------------------
	�^�C�g����ʃt�F�[�Y�N���X
	author	OkadaMoeto
	since	20140713
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CTitle.h"
#include "Define.h"
#include "CManager.h"
#include "CInput.h"
#include "CInputCommand.h"
#include "CFade.h"
#include "CScene2D.h"
#include "CCamera.h"
#include "collisionDetection.h"

/*-----------------------------------------------------------------------------
	�^�C�g���w�i�̐����ݒ�
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_TITLEBG = "data/texture/game_bg/game_bg.jpg";
static const D3DXVECTOR3 POS_TITLEBG = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
static const float WIDTH_TITLEBG = 1280.0f;
static const float HEIGHT_TITLEBG = 720.0f;

/*-----------------------------------------------------------------------------
	�^�C�g�����S�̐����ݒ�
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_TITLE_LOGO = "data/texture/logo_title/title_logo.png";
static const D3DXVECTOR3 POS_TITLE_LOGO = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) * 0.5f, 0.0f);
static const float WIDTH_TITLE_LOGO = 800.0f;
static const float HEIGHT_TITLE_LOGO = 300.0f;

/*-----------------------------------------------------------------------------
	GAMESTART�e�L�X�g�\���̐����ݒ�
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_PRESSGAMESTART = "data/texture/font/gamestart.png";
static const D3DXVECTOR3 POS_PRESSGAMESTART = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + 150.0f, 0.0f);
static const float WIDTH_PRESSGAMESTART = 512.0f;
static const float HEIGHT_PRESSGAMESTART = 64.0f;

/*-----------------------------------------------------------------------------
	GAMESTART�e�L�X�g�\���i�����オ��j�̐����ݒ�
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_PRESSGAMESTART_F = "data/texture/font/gamestart.png";
static const D3DXVECTOR3 POS_PRESSGAMESTART_F = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 10.0f, ((SCREEN_HEIGHT * 0.5f) + 150.0f) - 10.0f, 0.0f);
static const float WIDTH_PRESSGAMESTART_F = 512.0f;
static const float HEIGHT_PRESSGAMESTART_F = 64.0f;

/*-----------------------------------------------------------------------------
	�J�[�\���i�r�[�R���j�̐����ݒ�
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_BEECON = "data/texture/beecon/beecon.png";
static const D3DXVECTOR3 POS_BEECON = D3DXVECTOR3(350.0f, 470.0f, 0.0f);
static const float WIDTH_BEECON = 100.0f;
static const float HEIGHT_BEECON = 130.0f;

/*-----------------------------------------------------------------------------
	�_�ŏ��
-----------------------------------------------------------------------------*/
static const float BLINK_TIME = 15.0f;
static const float BLINK_TIME_DEICIDE = 3.0f;

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CTitle::CTitle()
{
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
	m_pTitleBg = CScene2D::Create(
									TEXTUREPATH_TITLEBG,
									POS_TITLEBG,
									VEC3_ZERO,
									WIDTH_TITLEBG,
									HEIGHT_TITLEBG);

	// �^�C�g�����S�̐���
	m_pTitleLogo = CScene2D::Create(
									TEXTUREPATH_TITLE_LOGO,
									POS_TITLE_LOGO,
									VEC3_ZERO,
									WIDTH_TITLE_LOGO,
									HEIGHT_TITLE_LOGO);
	// �Q�[���X�^�[�g�̐���
	m_pPressGameStartText = CScene2D::Create(
											TEXTUREPATH_PRESSGAMESTART,
											POS_PRESSGAMESTART,
											VEC3_ZERO,
											WIDTH_PRESSGAMESTART,
											HEIGHT_PRESSGAMESTART);

	// �Q�[���X�^�[�g�̐����i�����オ��j
	m_pPressGameStartTextF = CScene2D::Create(
												TEXTUREPATH_PRESSGAMESTART_F,
												POS_PRESSGAMESTART_F,
												VEC3_ZERO,
												WIDTH_PRESSGAMESTART_F,
												HEIGHT_PRESSGAMESTART_F);
	
	// �r�[�R���J�[�\���̐���
	m_pBeeconCursor = CScene2D::Create(
										TEXTUREPATH_BEECON,
										POS_BEECON,
										VEC3_ZERO,
										WIDTH_BEECON,
										HEIGHT_BEECON);

	m_pPressGameStartText->SetDiffuse(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	// ���_���������Ƃ��ɂЂ��Ⴐ�邽�߂̑Ή��[�u�iSetVertex�����_�j
	m_pBeeconCursor->SetScale(D3DXVECTOR2(130.0f, 100.0f));
	m_pBeeconCursor->SetVertex();
	m_pBeeconCursor->SetTexcoord(
								D3DXVECTOR2(0.0f, 0.0f),
								D3DXVECTOR2(0.125f, 0.0f),
								D3DXVECTOR2(0.0f, 0.375f),
								D3DXVECTOR2(0.125f, 0.375f));

	m_pInputCommand = new CInputCommand(CManager::GetInputKeyboard(), CManager::GetInputJoypad());
	m_pInputCommand->Init();

	m_bDecide = false;

	m_speed = 1.0f;
	m_velocity = VEC2_ZERO;

	// �t�F�[�h�C��
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 30.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CTitle::Uninit(void)
{
	m_pInputCommand->Uninit();
	delete m_pInputCommand;

	// �`��ΏۃI�u�W�F�N�g�̊J��
	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CTitle::Update(void)
{
	m_pInputCommand->Update();

	// �t�F�[�h���Ă��Ȃ���΍X�V
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		AnimationTitleLogo();

		UpdateInputEvent();

		MoveBeeconCursor();
	}

	if(m_bDecide)
	{
		m_pPressGameStartText->SetDiffuse(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
		m_pPressGameStartTextF->SetDiffuse(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

		CManager::GetPhaseFade()->Start(
											CFade::FADETYPE_OUT,
											30.0f,
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// �����t�F�[�h�h�~
		m_bDecide = false;
	}

	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		// �t�F�[�h�A�E�g�I����ɃQ�[����ʑJ��
		CManager::SetPhase(CManager::PHASE_GAME);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CTitle.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:�t�F�[�Y�J��\n");

	CDebugProcDX9::Print("%f %f\n", m_pBeeconCursor->GetPosition().x, m_pBeeconCursor->GetPosition().y);
#endif
}

void CTitle::AnimationTitleLogo(void)
{

}

void CTitle::UpdateInputEvent(void)
{
	const bool bDecide = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_ENTER)
						|| m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CONNECT);
	
	// �^�C�g���̃e�L�X�g�\���ƃJ�[�\���i�r�[�R���j�������Ȃ��Ă��邩
	if(CheckCollisionCircleVsRect(
									m_pBeeconCursor->GetPosition(),
									m_pBeeconCursor->GetSizeHalf().y,
									m_pPressGameStartText->GetPosition(),
									m_pPressGameStartText->GetSizeHalf()))
	{
		m_pPressGameStartTextF->SetDraw(true);

		if(bDecide)
		{
			m_bDecide = true;
		}
	}
	else
	{
		m_pPressGameStartTextF->SetDraw(false);
	}

	const bool bRight = m_pInputCommand->IsPress(CInputCommand::COMMAND_RIGHT);
	if(bRight)
	{
		CommandLeft();
	}

	const bool bLeft = m_pInputCommand->IsPress(CInputCommand::COMMAND_LEFT);
	if(bLeft)
	{
		CommandRight();
	}

	const bool bUp = m_pInputCommand->IsPress(CInputCommand::COMMAND_UP);
	if(bUp)
	{
		CommandUp();
	}

	const bool bDown = m_pInputCommand->IsPress(CInputCommand::COMMAND_DOWN);
	if(bDown)
	{
		CommandDown();
	}
}

void CTitle::MoveBeeconCursor(void)
{
	// ���݈ʒu�擾
	D3DXVECTOR3 pos;
	pos = m_pBeeconCursor->GetPosition();

	// �ʒu�X�V
	pos += D3DXVECTOR3(m_velocity.x, m_velocity.y, 0.0f);

	// �ړ��ʂɊ�����������
	m_velocity.x += (0.0f - m_velocity.x) * 0.1f;
	m_velocity.y += (0.0f - m_velocity.y) * 0.1f;

	// �ʒu���f
	m_pBeeconCursor->SetPosition(pos);
	m_pBeeconCursor->SetVertex();
}

void CTitle::CommandLeft(void)
{
	m_velocity.x += m_speed;
}

void CTitle::CommandRight(void)
{
	m_velocity.x -= m_speed;
}

void CTitle::CommandUp(void)
{	
	m_velocity.y -= m_speed;
}

void CTitle::CommandDown(void)
{
	m_velocity.y += m_speed;
}
