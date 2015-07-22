/*-----------------------------------------------------------------------------
	タイトル画面フェーズクラス
	author	OkadaMoeto
	since	20140713
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
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
#include "CSoundXAudio2.h"
#include "CConfigRecorder.h"
#include "CLifeConfig.h"

/*-----------------------------------------------------------------------------
	タイトル背景の生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_TITLEBG = "data/texture/title_bg/title_bg.jpg";
static const D3DXVECTOR3 POS_TITLEBG = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
static const float WIDTH_TITLEBG = 1280.0f;
static const float HEIGHT_TITLEBG = 720.0f;

/*-----------------------------------------------------------------------------
	タイトルロゴの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_TITLE_LOGO = "data/texture/logo_title/title_logo.png";
static const D3DXVECTOR3 POS_TITLE_LOGO = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) * 0.5f, 0.0f);
static const float WIDTH_TITLE_LOGO = 800.0f;
static const float HEIGHT_TITLE_LOGO = 250.0f;

/*-----------------------------------------------------------------------------
	GAMESTARTテキスト表示の生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_PRESSGAMESTART = "data/texture/font/gamestart.png";
static const D3DXVECTOR3 POS_PRESSGAMESTART = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + 150.0f, 0.0f);
static const float WIDTH_PRESSGAMESTART = 512.0f;
static const float HEIGHT_PRESSGAMESTART = 64.0f;

/*-----------------------------------------------------------------------------
	GAMESTARTテキスト表示（浮き上がり）の生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_PRESSGAMESTART_F = "data/texture/font/gamestart.png";
static const D3DXVECTOR3 POS_PRESSGAMESTART_F = D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 10.0f, ((SCREEN_HEIGHT * 0.5f) + 150.0f) - 10.0f, 0.0f);
static const float WIDTH_PRESSGAMESTART_F = 512.0f;
static const float HEIGHT_PRESSGAMESTART_F = 64.0f;

/*-----------------------------------------------------------------------------
	カーソル（ビーコン）の生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_BEECON = "data/texture/beecon/beecon.png";
static const D3DXVECTOR3 POS_BEECON = D3DXVECTOR3(370.0f, 460.0f, 0.0f);
static const float WIDTH_BEECON = 100.0f;
static const float HEIGHT_BEECON = 130.0f;

/*-----------------------------------------------------------------------------
	決定後の待ち時間
-----------------------------------------------------------------------------*/
static const float DECIDE_TIME = 15.0f;

/*-----------------------------------------------------------------------------
	タイトルロゴの拡縮値
-----------------------------------------------------------------------------*/
static const float SCALING_X_SPEED = 5.0f;
static const float SCALING_Y_SPEED = 2.0f;

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CTitle::CTitle()
{
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CTitle::~CTitle()
{
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
void CTitle::Init(void)
{
	// タイトル背景の生成
	m_pTitleBg = CScene2D::Create(
									TEXTUREPATH_TITLEBG,
									POS_TITLEBG,
									VEC3_ZERO,
									WIDTH_TITLEBG,
									HEIGHT_TITLEBG);

	// タイトルロゴの生成
	m_pTitleLogo = CScene2D::Create(
									TEXTUREPATH_TITLE_LOGO,
									POS_TITLE_LOGO,
									VEC3_ZERO,
									WIDTH_TITLE_LOGO,
									HEIGHT_TITLE_LOGO);
	// ゲームスタートの生成
	m_pPressGameStartText = CScene2D::Create(
											TEXTUREPATH_PRESSGAMESTART,
											POS_PRESSGAMESTART,
											VEC3_ZERO,
											WIDTH_PRESSGAMESTART,
											HEIGHT_PRESSGAMESTART);

	// ゲームスタートの生成（浮き上がり）
	m_pPressGameStartTextF = CScene2D::Create(
												TEXTUREPATH_PRESSGAMESTART_F,
												POS_PRESSGAMESTART_F,
												VEC3_ZERO,
												WIDTH_PRESSGAMESTART_F,
												HEIGHT_PRESSGAMESTART_F);
	
	// ビーコンカーソルの生成
	m_pBeeconCursor = CScene2D::Create(
										TEXTUREPATH_BEECON,
										POS_BEECON,
										VEC3_ZERO,
										WIDTH_BEECON,
										HEIGHT_BEECON);

	// 表示物の設定
	m_pTitleBg->SetTexcoord(
								D3DXVECTOR2(0.0f, 0.0f),
								D3DXVECTOR2(1.0f, 0.0f),
								D3DXVECTOR2(0.0f, 0.5f),
								D3DXVECTOR2(1.0f, 0.5f));

	m_pPressGameStartText->SetDiffuse(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	// 頂点情報をいれるときにひしゃげるための対応措置（SetVertexが問題点）
	m_pBeeconCursor->SetScale(D3DXVECTOR2(130.0f, 100.0f));
	m_pBeeconCursor->SetVertex();
	m_pBeeconCursor->SetTexcoord(
								D3DXVECTOR2(0.0f, 0.0f),
								D3DXVECTOR2(0.125f, 0.0f),
								D3DXVECTOR2(0.0f, 0.325f),
								D3DXVECTOR2(0.125f, 0.325f));

	InitAnimationBeeconCursor();

	m_bDecide = false;
	m_countDecide = 0.0f;

	m_speed = 1.0f;
	m_velocity = VEC2_ZERO;

	scrollBg = 0.0f;

	InitAnimationTitleLogo();

	m_pInputCommand = new CInputCommand(CManager::GetInputKeyboard(), CManager::GetInputJoypad());
	m_pInputCommand->Init();

	m_pBeeconCursor->SetDraw(false);
	m_pPressGameStartText->SetDraw(false);
	m_pPressGameStartTextF->SetDraw(false);

	// ライフ初期化（ループ対応）
	CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_RETRYLIFE, INIT_RETRYLIFE);

	// 選択ステージ初期化（ループ対応）
	CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_STAGESELECT, 0);

	PlayBgm();

	// フェードイン
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 30.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CTitle::Uninit(void)
{
	StopBgm();

	m_pInputCommand->Uninit();
	delete m_pInputCommand;

	// 描画対象オブジェクトの開放
	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CTitle::Update(void)
{
	m_pInputCommand->Update();

	// フェードしていなければ更新
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		if(m_bScaling == false)
		{
			UpdateInputEvent();

			MoveBeeconCursor();
		}
	}

	if(m_bScaling)
	{
		UpdateAnimationTitleLogo();
	}
	else
	{
		UpdateAnimationTitleBg();

		UpdateAnimationBeeconCursor();
	}

	if(m_bDecide)
	{
		m_pPressGameStartText->SetDiffuse(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
		m_pPressGameStartTextF->SetDiffuse(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

		m_countDecide++;

		if(m_countDecide >= DECIDE_TIME)
		{
			CManager::GetPhaseFade()->Start(
												CFade::FADETYPE_OUT,
												30.0f,
												D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			// 無限フェード防止
			m_bDecide = false;
		}
	}

	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		// フェードアウト終了後にゲーム画面遷移
		CManager::SetPhase(CManager::PHASE_STAGESELECT);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CTitle.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:フェーズ遷移\n");
#endif
}

void CTitle::InitAnimationTitleLogo(void)
{
	m_pTitleLogo->SetScale(0.0f, 0.0f);

	D3DXVECTOR2 size;

	const float sizeUp = 1.0f;
	const float sizeDown = -1.0f;

	m_scaleAttribute[0].countLimit = 30.0f;
	m_scaleAttribute[0].scaleLimit = D3DXVECTOR2(900.0f, 350.0f);
	size.x = m_pTitleLogo->GetSize().x - m_scaleAttribute[0].scaleLimit.x;
	size.y = m_pTitleLogo->GetSize().y - m_scaleAttribute[0].scaleLimit.y;
	size *= -1.0f;
	m_scaleAttribute[0].scale = sizeUp * size;
 	m_scaleAttribute[0].next = &m_scaleAttribute[1];

	m_scaleAttribute[1].countLimit = 8.0f;
	m_scaleAttribute[1].scaleLimit = D3DXVECTOR2(700.0f, 250.0f);
	size.x = m_scaleAttribute[0].scaleLimit.x - m_scaleAttribute[1].scaleLimit.x;
	size.y = m_scaleAttribute[0].scaleLimit.y - m_scaleAttribute[1].scaleLimit.y;
	size *= -1.0f;
	m_scaleAttribute[1].scale = -sizeDown * size;
	m_scaleAttribute[1].next = &m_scaleAttribute[2];
	
	m_scaleAttribute[2].countLimit = 5.0f;
	m_scaleAttribute[2].scaleLimit = D3DXVECTOR2(720.0f, 260.0f);
	size.x = m_scaleAttribute[1].scaleLimit.x - m_scaleAttribute[2].scaleLimit.x;
	size.y = m_scaleAttribute[1].scaleLimit.y - m_scaleAttribute[2].scaleLimit.y;
	size *= -1.0f;
	m_scaleAttribute[2].scale = sizeUp * size;
	m_scaleAttribute[2].next = &m_scaleAttribute[3];
	
	m_scaleAttribute[3].countLimit = 3.0f;
	m_scaleAttribute[3].scaleLimit = D3DXVECTOR2(680.0f, 240.0f);
	size.x = m_scaleAttribute[2].scaleLimit.x - m_scaleAttribute[3].scaleLimit.x;
	size.y = m_scaleAttribute[2].scaleLimit.y - m_scaleAttribute[3].scaleLimit.y;
	size *= -1.0f;
	m_scaleAttribute[3].scale = -sizeDown * size;
	m_scaleAttribute[3].next = &m_scaleAttribute[4];
	
	m_scaleAttribute[4].countLimit = 5.0f;
	m_scaleAttribute[4].scaleLimit = D3DXVECTOR2(700.0f, 250.0f);
	size.x = m_scaleAttribute[3].scaleLimit.x - m_scaleAttribute[4].scaleLimit.x;
	size.y = m_scaleAttribute[3].scaleLimit.y - m_scaleAttribute[4].scaleLimit.y;
	size *= -1.0f;
	m_scaleAttribute[4].scale = sizeUp * size;
	m_scaleAttribute[4].next = nullptr;

	m_currentScaleAnim = &m_scaleAttribute[0];

	m_basicScale.x = m_pTitleLogo->GetSize().x;
	m_basicScale.y = m_pTitleLogo->GetSize().y;

	m_countScale = 0.0f;
	m_bScaling = true;
}

void CTitle::UpdateAnimationTitleLogo(void)
{
	D3DXVECTOR2 scaleLogo;

	m_countScale++;

	scaleLogo.x = m_currentScaleAnim->scale.x * (m_countScale/ m_currentScaleAnim->countLimit);
	scaleLogo.y = m_currentScaleAnim->scale.y * (m_countScale / m_currentScaleAnim->countLimit);

	m_pTitleLogo->SetScale(m_basicScale.x + scaleLogo.x, m_basicScale.y + scaleLogo.y);

	if(m_countScale >= m_currentScaleAnim->countLimit)
	{
		// スケーリングするアニメをひとつ進める
		if(m_currentScaleAnim->next != nullptr)
		{
			m_basicScale.x = m_basicScale.x + scaleLogo.x;
			m_basicScale.y = m_basicScale.y + scaleLogo.y;

			m_currentScaleAnim = m_currentScaleAnim->next;
		}
		else
		{
			m_bScaling = false;

			m_pBeeconCursor->SetDraw(true);
			m_pPressGameStartText->SetDraw(true);
			m_pPressGameStartTextF->SetDraw(true);
		}

		m_countScale = 0.0f;
	}
}

void CTitle::UpdateAnimationTitleBg(void)
{
	scrollBg += 0.001f;

	m_pTitleBg->SetTexcoord(
								D3DXVECTOR2(0.0f, 0.0f + scrollBg),
								D3DXVECTOR2(1.0f, 0.0f + scrollBg),
								D3DXVECTOR2(0.0f, 0.5f + scrollBg),
								D3DXVECTOR2(1.0f, 0.5f + scrollBg));
}

void CTitle::UpdateInputEvent(void)
{
	const bool bDecide = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_ENTER)
						|| m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CONNECT);
	
	// タイトルのテキスト表示とカーソル（ビーコン）がかさなっているか
	if(CheckCollisionCircleVsRect(
									m_pBeeconCursor->GetPosition(),
									m_pBeeconCursor->GetSizeHalf().y,
									m_pPressGameStartText->GetPosition(),
									m_pPressGameStartText->GetSizeHalf()))
	{
		m_pPressGameStartTextF->SetDraw(true);

		// 重なっている時に選択すれば遷移条件ＯＫ
		if(bDecide)
		{
			PlaySeDecied();

			m_selectAnim = AnimList::AL_CONNECT;

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
		CommandRight();
	}

	const bool bLeft = m_pInputCommand->IsPress(CInputCommand::COMMAND_LEFT);
	if(bLeft)
	{
		CommandLeft();
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
	// 現在位置取得
	D3DXVECTOR3 pos;
	pos = m_pBeeconCursor->GetPosition();

	// 位置更新
	pos += D3DXVECTOR3(m_velocity.x, m_velocity.y, 0.0f);

	// 移動量に慣性をかける
	m_velocity.x += (0.0f - m_velocity.x) * 0.1f;
	m_velocity.y += (0.0f - m_velocity.y) * 0.1f;

	// 位置反映
	m_pBeeconCursor->SetPosition(pos);
	m_pBeeconCursor->SetVertex();
}

void CTitle::CommandLeft(void)
{
	m_velocity.x -= m_speed;
}

void CTitle::CommandRight(void)
{
	m_velocity.x += m_speed;
}

void CTitle::CommandUp(void)
{	
	m_velocity.y -= m_speed;
}

void CTitle::CommandDown(void)
{
	m_velocity.y += m_speed;
}

void CTitle::InitAnimationBeeconCursor(void)
{
	m_selectAnim = AnimList::AL_WAIT;

	for(int i = 0; i < 8; i++)
	{
		m_beeconAnimWait[i].wait = 3;

		float offset = i * 0.125f;
		m_beeconAnimWait[i].uv[0] = D3DXVECTOR2(0.0f + offset, 0.0f);
		m_beeconAnimWait[i].uv[1] = D3DXVECTOR2(0.125f + offset, 0.0f);
		m_beeconAnimWait[i].uv[2] = D3DXVECTOR2(0.0f + offset, 0.325f);
		m_beeconAnimWait[i].uv[3] = D3DXVECTOR2(0.125f + offset, 0.325f);
	}

	for(int i = 0; i < 8; i++)
	{
		m_beeconAnimConnect[i].wait = 3;

		float offset = i * 0.125f;
		m_beeconAnimConnect[i].uv[0] = D3DXVECTOR2(0.0f + offset, 0.325f);
		m_beeconAnimConnect[i].uv[1] = D3DXVECTOR2(0.125f + offset, 0.325f);
		m_beeconAnimConnect[i].uv[2] = D3DXVECTOR2(0.0f + offset, 0.625f);
		m_beeconAnimConnect[i].uv[3] = D3DXVECTOR2(0.125f + offset, 0.625f);
	}

	m_bRoopStop = false;
	m_countAnim = 0.0f;
	m_idxAnim = 0;
}

void CTitle::UpdateAnimationBeeconCursor(void)
{
	switch(m_selectAnim)
	{
		case AnimList::AL_WAIT:
		{
			m_idxAnim = (int)(m_countAnim / m_beeconAnimWait[m_idxAnim].wait) % 8;

			m_pBeeconCursor->SetTexcoord(
											m_beeconAnimWait[m_idxAnim].uv[0],
											m_beeconAnimWait[m_idxAnim].uv[1],
											m_beeconAnimWait[m_idxAnim].uv[2],
											m_beeconAnimWait[m_idxAnim].uv[3]);

			if(m_idxAnim >= 7)
			{
				m_idxAnim = 0;
				m_countAnim = 0.0f;
			}

			break;
		}

		case AnimList::AL_CONNECT:
		{
			if(m_bRoopStop == true)
			{
				return;
			}

			m_idxAnim = (int)(m_countAnim / m_beeconAnimConnect[m_idxAnim].wait) % 8;

			m_pBeeconCursor->SetTexcoord(
											m_beeconAnimConnect[m_idxAnim].uv[0],
											m_beeconAnimConnect[m_idxAnim].uv[1],
											m_beeconAnimConnect[m_idxAnim].uv[2],
											m_beeconAnimConnect[m_idxAnim].uv[3]);

			if(m_idxAnim >= 7)
			{
				m_idxAnim = 0;
				m_countAnim = 0.0f;
				m_bRoopStop = true;
			}

			break;
		}

		default:
			break;
	}

	m_countAnim++;
}

void CTitle::PlayBgm(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_BGM_TITLE);
}

void CTitle::StopBgm(void)
{
	CManager::GetSoundXAudio2()->Stop(CSoundXAudio2::SL_BGM_TITLE);
}

void CTitle::PlaySeDecied(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_SE_ENTER);
}
