/*-----------------------------------------------------------------------------
	ステージセレクト画面クラス
	author	OkadaMoeto
	since	20150720
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CStageSelect.h"
#include "Define.h"
#include "CManager.h"
#include "CInput.h"
#include "CFade.h"
#include "CScene2D.h"
#include "CInputCommand.h"
#include "CStageConfig.h"
#include "CConfigRecorder.h"
#include "CSoundXAudio2.h"

/*-----------------------------------------------------------------------------
	背景の生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_STAGESELECTBG = "data/texture/stageselect_bg/stageselect_bg.png";
static const D3DXVECTOR3 POS_STAGESELECTBG = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
static const float WIDTH_STAGESELECTBG = 1280.0f;
static const float HEIGHT_STAGESELECTBG = 720.0f;

/*-----------------------------------------------------------------------------
	ステージ情報パネルの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_INFOPANEL = NULL;
static const D3DXVECTOR3 POS_INFOPANEL = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - 100.0f, 0.0f);
static const float WIDTH_INFOPANEL = 800.0f;
static const float HEIGHT_INFOPANEL = 450.0f;

/*-----------------------------------------------------------------------------
	ＳＳの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_SS_FRAME = NULL;
static const D3DXVECTOR3 POS_SS_FRAME = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - 50.0f, 0.0f);
static const float WIDTH_SS_FRAME = 490.0f;
static const float HEIGHT_SS_FRAME = 280.0f;

static const char* TEXTUREPATH_SS_STAGE1 = "data/texture/stageselect_ui/ss1.jpg";
static const D3DXVECTOR3 POS_SS_STAGE1 = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - 50.0f, 0.0f);
static const float WIDTH_SS_STAGE1 = 480.0f;
static const float HEIGHT_SS_STAGE1 = 270.0f;

static const char* TEXTUREPATH_SS_STAGE2 = "data/texture/stageselect_ui/ss2.jpg";
static const D3DXVECTOR3 POS_SS_STAGE2 = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - 50.0f, 0.0f);
static const float WIDTH_SS_STAGE2 = 480.0f;
static const float HEIGHT_SS_STAGE2 = 270.0f;

static const char* TEXTUREPATH_SS_STAGE3 = "data/texture/stageselect_ui/ss3.jpg";
static const D3DXVECTOR3 POS_SS_STAGE3 = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - 50.0f, 0.0f);
static const float WIDTH_SS_STAGE3 = 480.0f;
static const float HEIGHT_SS_STAGE3 = 270.0f;

static const char* TEXTUREPATH_SS_STAGE4 = "data/texture/stageselect_ui/ss4.jpg";
static const D3DXVECTOR3 POS_SS_STAGE4 = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - 50.0f, 0.0f);
static const float WIDTH_SS_STAGE4 = 480.0f;
static const float HEIGHT_SS_STAGE4 = 270.0f;

/*-----------------------------------------------------------------------------
	ステージテキストの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_STAGE_TEXT = "data/texture/font/stage.png";
static const D3DXVECTOR3 POS_STAGE_TEXT = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 120.0f, (SCREEN_HEIGHT * 0.5f) - 260.0f, 0.0f);
static const float WIDTH_STAGE_TEXT = 400.0f;
static const float HEIGHT_STAGE_TEXT = 100.0f;

/*-----------------------------------------------------------------------------
	ステージ番号の生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_STAGE_NUM = "data/texture/font/number.png";
static const D3DXVECTOR3 POS_STAGE_NUM = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + 200.0f, (SCREEN_HEIGHT * 0.5f) - 260.0f, 0.0f);
static const float WIDTH_STAGE_NUM = 100.0f;
static const float HEIGHT_STAGE_NUM = 100.0f;
static const D3DXVECTOR2 INIT_TEX0_STAGE_NUM = D3DXVECTOR2(0.1f, 0.0f);
static const D3DXVECTOR2 INIT_TEX1_STAGE_NUM = D3DXVECTOR2(0.2f, 0.0f);
static const D3DXVECTOR2 INIT_TEX2_STAGE_NUM = D3DXVECTOR2(0.1f, 1.0f);
static const D3DXVECTOR2 INIT_TEX3_STAGE_NUM = D3DXVECTOR2(0.2f, 1.0f);

/*-----------------------------------------------------------------------------
	足場オブジェクトの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_SCAFFOLD = "data/texture/stageselect_ui/scaffold.png";
static const D3DXVECTOR3 POS_SCAFFOLD = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + 200.0f, 0.0f);
static const float WIDTH_SCAFFOLD = 1000.0f;
static const float HEIGHT_SCAFFOLD = 200.0f;

/*-----------------------------------------------------------------------------
	ボタンの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_BUTTON = "data/texture/button/number_button.png";
static const D3DXVECTOR3 POS_BUTTON = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + 380.0f, (SCREEN_HEIGHT * 0.5f) + 330.0f, 0.0f);
static const float WIDTH_BUTTON = 40.0f;
static const float HEIGHT_BUTTON = 40.0f;
static const D3DXVECTOR2 INIT_TEX0_BUTTON = D3DXVECTOR2(0.0f, 0.0f);
static const D3DXVECTOR2 INIT_TEX1_BUTTON = D3DXVECTOR2(0.5f, 0.0f);
static const D3DXVECTOR2 INIT_TEX2_BUTTON = D3DXVECTOR2(0.0f, 1.0f);
static const D3DXVECTOR2 INIT_TEX3_BUTTON = D3DXVECTOR2(0.5f, 1.0f);

/*-----------------------------------------------------------------------------
	セレクトの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_SELECT_TEXT = "data/texture/font/select.png";
static const D3DXVECTOR3 POS_SELECT_TEXT = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + 525.0f, (SCREEN_HEIGHT * 0.5f) + 330.0f, 0.0f);
static const float WIDTH_SELECT_TEXT = 200.0f;
static const float HEIGHT_SELECT_TEXT = 40.0f;

/*-----------------------------------------------------------------------------
	ビーコンの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_BEECON = "data/texture/beecon/beecon.png";
static const D3DXVECTOR3 POS_BEECON = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 450.0f, (SCREEN_HEIGHT * 0.5f) + 150.0f, 0.0f);
static const float WIDTH_BEECON = 100.0f;
static const float HEIGHT_BEECON = 130.0f;
static const D3DXVECTOR2 INIT_TEX0_BEECON_WAIT = D3DXVECTOR2(0.0f, 0.0f);
static const D3DXVECTOR2 INIT_TEX1_BEECON_WAIT = D3DXVECTOR2(0.125f, 0.0f);
static const D3DXVECTOR2 INIT_TEX2_BEECON_WAIT = D3DXVECTOR2(0.0f, 0.325f);
static const D3DXVECTOR2 INIT_TEX3_BEECON_WAIT = D3DXVECTOR2(0.125f, 0.325f);

/*-----------------------------------------------------------------------------
	アントンの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_ANTON = "data/texture/anton/anton.png";
static const D3DXVECTOR3 POS_ANTON = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 400.0f, (SCREEN_HEIGHT * 0.5f) + 200.0f, 0.0f);
static const float WIDTH_ANTON = 100.0f;
static const float HEIGHT_ANTON = 130.0f;
static const D3DXVECTOR2 INIT_TEX0_ANTON_WAIT = D3DXVECTOR2(0.0f, 0.0f);
static const D3DXVECTOR2 INIT_TEX1_ANTON_WAIT = D3DXVECTOR2(0.125f, 0.0f);
static const D3DXVECTOR2 INIT_TEX2_ANTON_WAIT = D3DXVECTOR2(0.0f, 0.0833f);
static const D3DXVECTOR2 INIT_TEX3_ANTON_WAIT = D3DXVECTOR2(0.125f, 0.0833f);
static const D3DXVECTOR2 INIT_TEX0_ANTON_WALK = D3DXVECTOR2(0.0f, 0.0833f);
static const D3DXVECTOR2 INIT_TEX1_ANTON_WALK = D3DXVECTOR2(0.125f, 0.0833f);
static const D3DXVECTOR2 INIT_TEX2_ANTON_WALK = D3DXVECTOR2(0.0f, 0.086f);
static const D3DXVECTOR2 INIT_TEX3_ANTON_WALK = D3DXVECTOR2(0.125f, 0.086f);

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
void CStageSelect::Init(void)
{
	// ステージセレクト背景生成
	m_pStageSelectBg = CScene2D::Create(
								TEXTUREPATH_STAGESELECTBG,
								POS_STAGESELECTBG,
								VEC3_ZERO,
								WIDTH_STAGESELECTBG,
								HEIGHT_STAGESELECTBG);

	CrateStageInfoUI();

	m_pInfoPanel->SetDiffuse(COL_BLACK_ALPHA(0.3f));
	m_pStageNumText->SetTexcoord(
									INIT_TEX0_STAGE_NUM,
									INIT_TEX1_STAGE_NUM,
									INIT_TEX2_STAGE_NUM,
									INIT_TEX3_STAGE_NUM);

	CreateObjectSS();

	m_pFrameSS->SetDiffuse(COL_BLACK_ALPHA(0.8f));

	m_pStage2SS->SetDraw(false);
	m_pStage3SS->SetDraw(false);
	m_pStage4SS->SetDraw(false);

	// 足場オブジェクト生成
	m_pSelectScaffold = CScene2D::Create(
											TEXTUREPATH_SCAFFOLD,
											POS_SCAFFOLD,
											VEC3_ZERO,
											WIDTH_SCAFFOLD,
											HEIGHT_SCAFFOLD);

	CreateObjectCharacter();

	m_pAnton->SetTexcoord(
								INIT_TEX0_ANTON_WAIT,
								INIT_TEX1_ANTON_WAIT,
								INIT_TEX2_ANTON_WAIT,
								INIT_TEX3_ANTON_WAIT);

	m_pBeecon->SetTexcoord(
								INIT_TEX0_BEECON_WAIT,
								INIT_TEX1_BEECON_WAIT,
								INIT_TEX2_BEECON_WAIT,
								INIT_TEX3_BEECON_WAIT);

	// 頂点情報をいれるときにひしゃげるための対応措置（SetVertexが問題点）
	m_pAnton->SetScale(D3DXVECTOR2(130.0f, 100.0f));
	m_pAnton->SetVertex();
	m_pBeecon->SetScale(D3DXVECTOR2(130.0f, 100.0f));
	m_pBeecon->SetVertex();

	InitAnimationCharacter();

	CreateSelectUI();

	m_pSelectButton->SetTexcoord(
								INIT_TEX0_BUTTON,
								INIT_TEX1_BUTTON,
								INIT_TEX2_BUTTON,
								INIT_TEX3_BUTTON);

	m_pInputCommand = new CInputCommand(CManager::GetInputKeyboard(), CManager::GetInputJoypad());
	m_pInputCommand->Init();

	InitSelect();

	MoveCharacter();

	UpdateSS();

	UpdateStageNumUI();

	m_countBlink = 0.0f;

	m_bChange = false;
	m_countChange = 0.0f;

	PlayBgm();

	// フェードイン
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 30.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CStageSelect::Uninit(void)
{
	StopBgm();

	m_pInputCommand->Uninit();
	delete m_pInputCommand;

	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CStageSelect::Update(void)
{
	m_pInputCommand->Update();

	// フェードしていなければ更新
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		UpdateInputEvent();

		UpdateStageNumUI();

		UpdateSS();

		MoveCharacter();

		BlinkSelect(15.0f);

		if(IsSelected())
		{
			m_bChange = true;
		}
	}

	UpdateAnimationAnton();

	UpdateAnimationBeecon();

	// 画面遷移のフラグがONならば、SELECTの点滅を早めて認識させてからフェードアウトさせる
	if(m_bChange)
	{
		BlinkSelect(1.0f);

		m_countChange++;
		if(m_countChange > 15.0f)
		{
			// ゲーム画面に遷移するためのフェードアウト発生
			CManager::GetPhaseFade()->Start(
												CFade::FADETYPE_OUT,
												30.0f,
												D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ゲーム画面で読み込むために選択しているステージを記録
			CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_STAGESELECT, m_selectStage);

			// 無限防止
			m_bChange = false;
		}
	}

	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		// フェードアウト終了後にタイトル画面遷移
		CManager::SetPhase(CManager::PHASE_GAME);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CStageSelect.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:フェーズ遷移\n");
	CDebugProcDX9::Print("STAGE SELECT [A][D]：%d\n", m_selectStage + 1);
#endif
}

void CStageSelect::CrateStageInfoUI(void)
{
	// ステージ情報パネル生成
	m_pInfoPanel = CScene2D::Create(
										NULL,
										POS_INFOPANEL,
										VEC3_ZERO,
										WIDTH_INFOPANEL,
										HEIGHT_INFOPANEL);

	// ステージテキスト生成
	m_pStageText = CScene2D::Create(
										TEXTUREPATH_STAGE_TEXT,
										POS_STAGE_TEXT,
										VEC3_ZERO,
										WIDTH_STAGE_TEXT,
										HEIGHT_STAGE_TEXT);
	
	// ステージ番号生成
	m_pStageNumText = CScene2D::Create(
										TEXTUREPATH_STAGE_NUM,
										POS_STAGE_NUM,
										VEC3_ZERO,
										WIDTH_STAGE_NUM,
										HEIGHT_STAGE_NUM);
}

void CStageSelect::CreateObjectSS(void)
{
	m_pFrameSS = CScene2D::Create(
										NULL,
										POS_SS_FRAME,
										VEC3_ZERO,
										WIDTH_SS_FRAME,
										HEIGHT_SS_FRAME);

	m_pStage1SS = CScene2D::Create(
										TEXTUREPATH_SS_STAGE1,
										POS_SS_STAGE1,
										VEC3_ZERO,
										WIDTH_SS_STAGE1,
										HEIGHT_SS_STAGE1);

	m_pStage2SS = CScene2D::Create(
										TEXTUREPATH_SS_STAGE2,
										POS_SS_STAGE2,
										VEC3_ZERO,
										WIDTH_SS_STAGE2,
										HEIGHT_SS_STAGE2);
	
	m_pStage3SS = CScene2D::Create(
										TEXTUREPATH_SS_STAGE3,
										POS_SS_STAGE3,
										VEC3_ZERO,
										WIDTH_SS_STAGE3,
										HEIGHT_SS_STAGE3);
	
	m_pStage4SS = CScene2D::Create(
										TEXTUREPATH_SS_STAGE4,
										POS_SS_STAGE4,
										VEC3_ZERO,
										WIDTH_SS_STAGE4,
										HEIGHT_SS_STAGE4);
}

void CStageSelect::CreateSelectUI(void)
{
	// ボタン生成
	m_pSelectButton = CScene2D::Create(
										TEXTUREPATH_BUTTON,
										POS_BUTTON,
										VEC3_ZERO,
										WIDTH_BUTTON,
										HEIGHT_BUTTON);

	// SELECT生成
	m_pSelectText = CScene2D::Create(
										TEXTUREPATH_SELECT_TEXT,
										POS_SELECT_TEXT,
										VEC3_ZERO,
										WIDTH_SELECT_TEXT,
										HEIGHT_SELECT_TEXT);
}

void CStageSelect::CreateObjectCharacter(void)
{
	// アントン生成
	m_pAnton = CScene2D::Create(
									TEXTUREPATH_ANTON,
									POS_ANTON,
									VEC3_ZERO,
									WIDTH_ANTON,
									HEIGHT_ANTON);

	// ビーコン生成
	m_pBeecon = CScene2D::Create(
									TEXTUREPATH_BEECON,
									POS_BEECON,
									VEC3_ZERO,
									WIDTH_BEECON,
									HEIGHT_BEECON);
}

void CStageSelect::InitSelect(void)
{
	m_selectStage = 0;

	m_selectStage = CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_STAGESELECT);

	m_bSelected = false;
}

void CStageSelect::InitAnimationCharacter(void)
{
	m_selectAnimBeecon = AnimListBeecon::ALB_WAIT;

	for(int i = 0; i < 8; i++)
	{
		m_beeconAnimWait[i].wait = 3;

		float offset = i * 0.125f;
		m_beeconAnimWait[i].uv[0] = D3DXVECTOR2(INIT_TEX0_BEECON_WAIT.x + offset, INIT_TEX0_BEECON_WAIT.y);
		m_beeconAnimWait[i].uv[1] = D3DXVECTOR2(INIT_TEX1_BEECON_WAIT.x + offset, INIT_TEX1_BEECON_WAIT.y);
		m_beeconAnimWait[i].uv[2] = D3DXVECTOR2(INIT_TEX2_BEECON_WAIT.x + offset, INIT_TEX2_BEECON_WAIT.y);
		m_beeconAnimWait[i].uv[3] = D3DXVECTOR2(INIT_TEX3_BEECON_WAIT.x + offset, INIT_TEX3_BEECON_WAIT.y);
	}

	m_bRoopStopBeecon = false;
	m_countAnimBeecon = 0.0f;
	m_idxAnimBeecon = 0;

	m_selectAnimAnton = AnimListAnton::ALA_WAIT;

	for(int i = 0; i < 4; i++)
	{
		m_antonAnimWait[i].wait = 3;

		float offset = i * 0.125f;
		m_antonAnimWait[i].uv[0] = D3DXVECTOR2(INIT_TEX0_ANTON_WAIT.x + offset, INIT_TEX0_ANTON_WAIT.y);
		m_antonAnimWait[i].uv[1] = D3DXVECTOR2(INIT_TEX1_ANTON_WAIT.x + offset, INIT_TEX1_ANTON_WAIT.y);
		m_antonAnimWait[i].uv[2] = D3DXVECTOR2(INIT_TEX2_ANTON_WAIT.x + offset, INIT_TEX2_ANTON_WAIT.y);
		m_antonAnimWait[i].uv[3] = D3DXVECTOR2(INIT_TEX3_ANTON_WAIT.x + offset, INIT_TEX3_ANTON_WAIT.y);
	}

	m_bRoopStopAnton = false;
	m_countAnimAnton = 0.0f;
	m_idxAnimAnton = 0;
}

bool CStageSelect::IsSelected(void)
{
	return m_bSelected;
}

void CStageSelect::UpdateInputEvent(void)
{
	const bool bSelected = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_ENTER)
						|| m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CONNECT);

	if(bSelected)
	{
		CommandSelect();

		PlaySeSelect();
	}

	const bool bRight = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_RIGHT);
	if(bRight)
	{
		CommandRight();

		PlaySeLeftAndRight();
	}

	const bool bLeft = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_LEFT);
	if(bLeft)
	{
		CommandLeft();

		PlaySeLeftAndRight();
	}
}

void CStageSelect::CommandSelect(void)
{
	m_bSelected = true;
}

void CStageSelect::CommandLeft(void)
{
	m_selectStage--;

	if(m_selectStage < 0)
	{
		m_selectStage = 0;
	}
}

void CStageSelect::CommandRight(void)
{
	m_selectStage++;

	if(m_selectStage >= STAGEID_MAX)
	{
		m_selectStage = STAGEID_MAX - 1;
	}
}

void CStageSelect::UpdateStageNumUI(void)
{
	switch(m_selectStage)
	{
		case 0:
		{
			m_pStageNumText->SetTexcoord(
											INIT_TEX0_STAGE_NUM,
											INIT_TEX1_STAGE_NUM,
											INIT_TEX2_STAGE_NUM,
											INIT_TEX3_STAGE_NUM);
			break;
		}

		case 1:
		{
			m_pStageNumText->SetTexcoord(
											D3DXVECTOR2(0.2f, 0.0f),
											D3DXVECTOR2(0.3f, 0.0f),
											D3DXVECTOR2(0.2f, 1.0f),
											D3DXVECTOR2(0.3f, 1.0f));
			break;
		}

		case 2:
		{
			m_pStageNumText->SetTexcoord(
											D3DXVECTOR2(0.3f, 0.0f),
											D3DXVECTOR2(0.4f, 0.0f),
											D3DXVECTOR2(0.3f, 1.0f),
											D3DXVECTOR2(0.4f, 1.0f));
			break;
		}

		case 3:
		{
			m_pStageNumText->SetTexcoord(
											D3DXVECTOR2(0.4f, 0.0f),
											D3DXVECTOR2(0.5f, 0.0f),
											D3DXVECTOR2(0.4f, 1.0f),
											D3DXVECTOR2(0.5f, 1.0f));
			break;
		}
	}
}

void CStageSelect::UpdateSS(void)
{
	switch(m_selectStage)
	{
		case 0:
		{
			m_pStage1SS->SetDraw(true);
			m_pStage2SS->SetDraw(false);
			m_pStage3SS->SetDraw(false);
			m_pStage4SS->SetDraw(false);
			break;
		}

		case 1:
		{
			m_pStage1SS->SetDraw(false);
			m_pStage2SS->SetDraw(true);
			m_pStage3SS->SetDraw(false);
			m_pStage4SS->SetDraw(false);
			break;
		}

		case 2:
		{
			m_pStage1SS->SetDraw(false);
			m_pStage2SS->SetDraw(false);
			m_pStage3SS->SetDraw(true);
			m_pStage4SS->SetDraw(false);
			break;
		}

		case 3:
		{
			m_pStage1SS->SetDraw(false);
			m_pStage2SS->SetDraw(false);
			m_pStage3SS->SetDraw(false);
			m_pStage4SS->SetDraw(true);
			break;
		}
	}
}

void CStageSelect::UpdateAnimationBeecon(void)
{
	switch(m_selectAnimBeecon)
	{
		case AnimListBeecon::ALB_WAIT:
		{
			m_idxAnimBeecon = (int)(m_countAnimBeecon / m_beeconAnimWait[m_idxAnimBeecon].wait) % 8;

			m_pBeecon->SetTexcoord(
											m_beeconAnimWait[m_idxAnimBeecon].uv[0],
											m_beeconAnimWait[m_idxAnimBeecon].uv[1],
											m_beeconAnimWait[m_idxAnimBeecon].uv[2],
											m_beeconAnimWait[m_idxAnimBeecon].uv[3]);

			if(m_idxAnimBeecon >= 7)
			{
				m_idxAnimBeecon = 0;
				m_countAnimBeecon = 0.0f;
			}

			break;
		}

		default:
			break;
	}

	m_countAnimBeecon++;
}

void CStageSelect::UpdateAnimationAnton(void)
{
	switch(m_selectAnimAnton)
	{
		case AnimListAnton::ALA_WAIT:
		{
			m_idxAnimAnton = (int)(m_countAnimAnton / m_antonAnimWait[m_idxAnimAnton].wait) % 4;

			m_pAnton->SetTexcoord(
											m_antonAnimWait[m_idxAnimAnton].uv[0],
											m_antonAnimWait[m_idxAnimAnton].uv[1],
											m_antonAnimWait[m_idxAnimAnton].uv[2],
											m_antonAnimWait[m_idxAnimAnton].uv[3]);

			if(m_idxAnimAnton >= 7)
			{
				m_idxAnimAnton = 0;
				m_countAnimAnton = 0.0f;
			}

			break;
		}

		default:
			break;
	}

	m_countAnimAnton++;
}

void CStageSelect::MoveCharacter(void)
{
	switch(m_selectStage)
	{
		case 0:
		{
			m_pAnton->SetPosition(POS_ANTON);
			m_pAnton->SetVertex();
			m_pBeecon->SetPosition(POS_BEECON);
			m_pBeecon->SetVertex();
			break;
		}

		case 1:
		{
			m_pAnton->SetPosition(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 130.0f, (SCREEN_HEIGHT * 0.5f) + 200.0f, 0.0f));
			m_pAnton->SetVertex();
			m_pBeecon->SetPosition(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 180.0f, (SCREEN_HEIGHT * 0.5f) + 150.0f, 0.0f));
			m_pBeecon->SetVertex();
			break;
		}

		case 2:
		{
			m_pAnton->SetPosition(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + 150.0f, (SCREEN_HEIGHT * 0.5f) + 200.0f, 0.0f));
			m_pAnton->SetVertex();
			m_pBeecon->SetPosition(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + 200.0f, (SCREEN_HEIGHT * 0.5f) + 150.0f, 0.0f));
			m_pBeecon->SetVertex();
			break;
		}

		case 3:
		{
			m_pAnton->SetPosition(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + 440.0f, (SCREEN_HEIGHT * 0.5f) + 200.0f, 0.0f));
			m_pAnton->SetVertex();
			m_pBeecon->SetPosition(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) + 390.0f, (SCREEN_HEIGHT * 0.5f) + 150.0f, 0.0f));
			m_pBeecon->SetVertex();
			break;
		}
	}
}

void CStageSelect::BlinkSelect(float flashTime)
{
	m_countBlink++;
	
	// 明るい文字色に切り替え
	if(m_countBlink > flashTime * 2)
	{
		m_pSelectButton->SetDiffuse(COL_WHITE);
		m_pSelectText->SetDiffuse(COL_WHITE);
		m_countBlink = 0.0f;
		return; // 処理後はとばす
	}

	// 暗い文字色に切り替え
	if(m_countBlink > flashTime)
	{
		// 初期のアルファ値が１．０ならばうまく作用する
		m_pSelectButton->SetDiffuse(COL_WHITE_ALPHA(0.3f));
		m_pSelectText->SetDiffuse(COL_WHITE_ALPHA(0.3f));
	}
}

void CStageSelect::PlayBgm(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_BGM_STAGESELECT);
}

void CStageSelect::StopBgm(void)
{
	CManager::GetSoundXAudio2()->Stop(CSoundXAudio2::SL_BGM_STAGESELECT);
}

void CStageSelect::PlaySeLeftAndRight(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_SE_PI);
}

void CStageSelect::PlaySeSelect(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_SE_ENTER);
}
