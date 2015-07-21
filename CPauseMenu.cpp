/*-----------------------------------------------------------------------------
	ポーズメニュー
	author	OkadaMoeto
	since	20141211
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CPauseMenu.h"
#include "define.h"
#include "CScene2D.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
// 読み込み2Dテクスチャのルート
#define FILENAME_PAUSEMENU_CURSOR	("data/texture/beecon/beecon.png")
#define FILENAME_PAUSEMENU_RETRY	("data/texture/font/retry.png")
#define FILENAME_PAUSEMENU_EXIT		("data/texture/font/exit.png")

static const float WIDTH_BEECON = 100.0f;
static const float HEIGHT_BEECON = 130.0f;
static const D3DXVECTOR2 INIT_TEX0_BEECON_WAIT = D3DXVECTOR2(0.0f, 0.0f);
static const D3DXVECTOR2 INIT_TEX1_BEECON_WAIT = D3DXVECTOR2(0.125f, 0.0f);
static const D3DXVECTOR2 INIT_TEX2_BEECON_WAIT = D3DXVECTOR2(0.0f, 0.325f);
static const D3DXVECTOR2 INIT_TEX3_BEECON_WAIT = D3DXVECTOR2(0.125f, 0.325f);

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CPauseMenu::CPauseMenu()
{
	m_background = nullptr;
	m_cursor = nullptr;
	m_retry = nullptr;
	m_exit = nullptr;
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CPauseMenu::~CPauseMenu()
{
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
void CPauseMenu::Init(void)
{
	D3DXVECTOR3 cursor	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f), 0.0f);
	D3DXVECTOR3 retry	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) - 150.0f, 0.0f);
	D3DXVECTOR3 exit	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + 150.0f, 0.0f);

	// 背景の生成
	m_background = CScene2D::Create(
										NULL,
										D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
										VEC3_ZERO,
										1280.0f,
										720.0f,
										COL_BLACK_ALPHA(0.5f));

	// ポーズメニューカーソルの生成
	m_cursor = CScene2D::Create(
									FILENAME_PAUSEMENU_CURSOR,
									cursor,
									VEC3_ZERO,
									WIDTH_BEECON,
									HEIGHT_BEECON);

	m_cursor->SetTexcoord(
							INIT_TEX0_BEECON_WAIT,
							INIT_TEX1_BEECON_WAIT,
							INIT_TEX2_BEECON_WAIT,
							INIT_TEX3_BEECON_WAIT);

	m_cursor->SetScale(D3DXVECTOR2(130.0f, 100.0f));
	m_cursor->SetVertex();

	InitAnimationCursor();

	// リトライの生成
	m_retry = CScene2D::Create(
									FILENAME_PAUSEMENU_RETRY,
									retry,
									VEC3_ZERO,
									400.0f,
									100.0f);

	// ゲームを終了の生成（タイトルに戻る）
	m_exit = CScene2D::Create(
								FILENAME_PAUSEMENU_EXIT,
								exit,
								VEC3_ZERO,
								400.0f,
								100.0f);
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CPauseMenu::Uninit(void)
{
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CPauseMenu::Update(void)
{
	UpdateAnimationCursor();
}

/*-----------------------------------------------------------------------------
	ポーズメニュー表示
-----------------------------------------------------------------------------*/
void CPauseMenu::Disp(CScene2D* menu)
{
	menu->SetDraw(true);
}

/*-----------------------------------------------------------------------------
	ポーズメニュー非表示
-----------------------------------------------------------------------------*/
void CPauseMenu::Hide(CScene2D* menu)
{
	menu->SetDraw(false);
}

/*-----------------------------------------------------------------------------
	全ポーズメニュー表示
-----------------------------------------------------------------------------*/
void CPauseMenu::DispAll(void)
{
	m_background->SetDraw(true);
	m_cursor->SetDraw(true);
	m_retry->SetDraw(true);
	m_exit->SetDraw(true);
}

/*-----------------------------------------------------------------------------
	全ポーズメニュー非表示
-----------------------------------------------------------------------------*/
void CPauseMenu::HideAll(void)
{
	m_background->SetDraw(false);
	m_cursor->SetDraw(false);
	m_retry->SetDraw(false);
	m_exit->SetDraw(false);
}

/*-----------------------------------------------------------------------------
	メニューの色変更
-----------------------------------------------------------------------------*/
void CPauseMenu::SetColorRetry(D3DXCOLOR col)
{
	m_retry->SetDiffuse(col);
}

void CPauseMenu::SetColorExit(D3DXCOLOR col)
{
	m_exit->SetDiffuse(col);
}

/*-----------------------------------------------------------------------------
	カーソルの位置設定
-----------------------------------------------------------------------------*/
void CPauseMenu::SetPositionCursor(D3DXVECTOR3 pos)
{
	m_cursor->SetPosition(pos);
	m_cursor->SetVertex();
}

/*-----------------------------------------------------------------------------
	カーソルの位置取得
-----------------------------------------------------------------------------*/
D3DXVECTOR3 CPauseMenu::GetPositionCursor(void)
{
	return m_cursor->GetPosition();
}

void CPauseMenu::InitAnimationCursor(void)
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
}

void CPauseMenu::UpdateAnimationCursor(void)
{
	switch(m_selectAnimBeecon)
	{
		case AnimListBeecon::ALB_WAIT:
		{
			m_idxAnimBeecon = (int)(m_countAnimBeecon / m_beeconAnimWait[m_idxAnimBeecon].wait) % 8;

			m_cursor->SetTexcoord(
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
