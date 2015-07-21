/*-----------------------------------------------------------------------------
	フェードクラス
	author	OkadaMoeto
	since	20140709
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CFade.h"
#include "Define.h"

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CFade::CFade(
				int nPriority,
				CScene::OBJTYPE objtype) : CScene2D(nPriority, objtype)
{
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f ,0.0f);
	m_timeFrame = 0.0f;
	m_bUse = false;
	m_alpha = 0.0f;
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CFade::~CFade()
{
}

/*-----------------------------------------------------------------------------
	2Dポリゴンのフェード生成
-----------------------------------------------------------------------------*/
CFade* CFade::Create(
						D3DXVECTOR3 pos,
						D3DXVECTOR3 rot,
						D3DXCOLOR col)
{
	CFade *pFade;

	// フェードの生成と初期化
	pFade = new CFade();
	pFade->Init(pos, rot, col);

	pFade->SetPosition(pos);
	pFade->SetRotation(rot);
	pFade->m_color = col;

	return pFade;
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
HRESULT CFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	// 2Dポリゴンの初期化処理
	CScene2D::Init(
					NULL,
					pos,
					rot,
					col,
					(float)SCREEN_WIDTH,
					(float)SCREEN_HEIGHT);

	// フェードの初期化
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f , 0.0f);
	m_timeFrame = 0.0f;
	m_bUse = false;
	m_fadetype = FADETYPE_NONE;
	m_alpha = 0.0f;

	SetDiffuse(m_color);

	// シーン間を継続
	SetGlobal(true);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CFade::Uninit(void)
{
	// シーン間を継続を無効
	SetGlobal(false);

	// 2Dポリゴンの終了処理
	CScene2D::Uninit();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CFade::Update(void)
{
	// フェードを使用していなければ、更新しない
	if(m_bUse == false){ return; }

	// 2Dポリゴンの更新
	CScene2D::Update();

	if(m_bUse == true && m_fadetype != FADETYPE_NONE)
	{
		// フェード処理中
		switch(m_fadetype)
		{
			case FADETYPE_OUT:
			{
				// 透明値を加算して画面を消していく
				m_alpha += 1.0f / m_timeFrame;
				if(m_alpha >= 1.0f)
				{
					// フェードアウト終了に切り替える
					m_fadetype = FADETYPE_UNOUT;
				}
			}
			break;

			case FADETYPE_UNOUT:
			{
				// Nowloadingなどに合わせる為にフェードの停止状態に切り替える
				m_alpha = 1.0f;
				m_fadetype = FADETYPE_INFINITY;
			}
			break;

			case FADETYPE_IN:
			{
				// α値を減算して画面を浮き上がらせる
				m_alpha -= 1.0f / m_timeFrame;
				if(m_alpha <= 0.0f)
				{
					// フェードイン終了に切り替える
					m_fadetype = FADETYPE_UNIN;
				}
			}
			break;

			case FADETYPE_UNIN:
			{
				m_alpha = 0.0f;
				m_bUse = false;
				m_fadetype = FADETYPE_NONE;
			}
			break;

			case FADETYPE_INFINITY:
			{
				m_alpha = 1.0f;

				if(m_fadetype != FADETYPE_INFINITY)
				{
					m_bUse = false;
				}
			}
			break;
		}

		// 透明値を設定
		m_color.a = m_alpha;
		// ポリゴン色を設定
		SetDiffuse(m_color);
	}
}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/
void CFade::Draw(void)
{
	// フェードを使用していなければ、描画しない
	if(m_bUse == false){ return; }

	// 2Dポリゴンの描画
	CScene2D::Draw();
}

/*-----------------------------------------------------------------------------
	フェードの開始
-----------------------------------------------------------------------------*/
void CFade::Start(FADETYPE type, float time_frame, D3DXCOLOR color)
{
	// フェードON
	m_bUse = true;

	// 状態を設定
	m_fadetype = type;

	// 時間を設定
	m_timeFrame = time_frame;

	// 色を設定
	m_color = color;

	// アルファ値の設定
	if(m_fadetype == FADETYPE_IN)
	{
		m_alpha = 1.0f;
	}
	else if(m_fadetype == FADETYPE_OUT)
	{
		m_alpha = 0.0f;
	}

	m_color.a = m_alpha;

	// 色を設定
	SetDiffuse(m_color);
}

// フェードを初期状態に戻す（フェードなし, FADETYPE_NONE）
void CFade::Reset(void)
{
	m_alpha = 0.0f;
	m_bUse = false;
	m_fadetype = FADETYPE_NONE;
	m_color.a = m_alpha;
	SetDiffuse(m_color);
}

