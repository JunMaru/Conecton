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
#include "CFade.h"
#include "CScene2D.h"
#include "CCamera.h"

/*-----------------------------------------------------------------------------
	テクスチャの読み込み先のパス設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_GROUPLOGO = "data/texture/logo_group/group_logo.png";
static const D3DXVECTOR3 POS_GROUPLOGO	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
static const float WIDTH_GROUPLOGO		= 400.0f;
static const float HEIGHT_GROUPLOGO		= 335.0f;

static const char* TEXTUREPATH_TITLEBG = "data/texture/game_bg/game_bg.jpg";
static const D3DXVECTOR3 POS_TITLEBG	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
static const float WIDTH_TITLEBG		= 1280.0f;
static const float HEIGHT_TITLEBG		= 720.0f;

static const char* TEXTUREPATH_TITLE_LOGO = "data/texture/logo_title/title_logo.png";
static const D3DXVECTOR3 POS_TITLE_LOGO = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) * 0.5f, 0.0f);
static const float WIDTH_TITLE_LOGO		= 600.0f;
static const float HEIGHT_TITLE_LOGO	= 200.0f;

static const char* TEXTUREPATH_PRESSGAMESTART= "data/texture/font/gamestart.png";
static const D3DXVECTOR3 POS_PRESSGAMESTART	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + 150.0f, 0.0f);
static const float WIDTH_PRESSGAMESTART		= 512.0f;
static const float HEIGHT_PRESSGAMESTART	= 64.0f;

/*-----------------------------------------------------------------------------
	点滅情報
-----------------------------------------------------------------------------*/
static const float BLINK_TIME = 15.0f;
static const float BLINK_TIME_DEICIDE = 3.0f;

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
	m_logoPhaseBg = CScene2D::Create(
										NULL,
										D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
										VEC3_ZERO,
										1280.0f,
										720.0f);

	// グループロゴ生成
	m_groupLogo = CScene2D::Create(
									TEXTUREPATH_GROUPLOGO,
									POS_GROUPLOGO,
									VEC3_ZERO,
									WIDTH_GROUPLOGO,
									HEIGHT_GROUPLOGO);

	// ロゴ表示後のフェード作成
	m_logoPhaseFade = CFade::Create(
									D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
									VEC3_ZERO);

	// タイトル背景の生成
	m_titleBg = CScene2D::Create(
									TEXTUREPATH_TITLEBG,
									POS_TITLEBG,
									VEC3_ZERO,
									WIDTH_TITLEBG,
									HEIGHT_TITLEBG);

	// タイトルロゴの生成
	m_titleLogo = CScene2D::Create(
									TEXTUREPATH_TITLE_LOGO,
									POS_TITLE_LOGO,
									VEC3_ZERO,
									WIDTH_TITLE_LOGO,
									HEIGHT_TITLE_LOGO);
	// プレスゲームスタートの生成
	m_pressGameStartText = CScene2D::Create(
											TEXTUREPATH_PRESSGAMESTART,
											POS_PRESSGAMESTART,
											VEC3_ZERO,
											WIDTH_PRESSGAMESTART,
											HEIGHT_PRESSGAMESTART);


	// ロゴ背景は白にしないと見えない
	m_logoPhaseBg->SetDiffuse(COL_WHITE);

	// ロゴ表示中は非表示にしたいため描画OFF
	m_titleBg->SetDraw(false);
	m_titleLogo->SetDraw(false);
	m_pressGameStartText->SetDraw(false);

	m_bDecide = false;

	m_countDecided = 0.0f;
	m_countLogoDisp = 0.0f;

	// フェードイン
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 60.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CTitle::Uninit(void)
{
	// 描画対象オブジェクトの開放
	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CTitle::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// フェードしていなければ更新
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		if(pKeyboard->GetKeyTrigger(DIK_RETURN))
		{
			m_bDecide = true;
		}
	}

	// プレスキーの決定後点滅
	if(m_bDecide)
	{
		m_countDecided++;

		// 決定後の画面遷移タイミングでフェード移行
		if(m_countDecided > 60.0f)
		{
			CManager::GetPhaseFade()->Start(
												CFade::FADETYPE_OUT,
												60.0f,
												D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_bDecide = false; // 無限フェード防止
		}
	}

	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		// フェードアウト終了後にゲーム画面遷移
		CManager::SetPhase(CManager::PHASE_GAME);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CTitle.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:フェーズ遷移\n");
#endif
}
