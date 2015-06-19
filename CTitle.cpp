/*-----------------------------------------------------------------------------
	タイトル画面フェーズクラス
	author	OkadaMoeto
	since	20140713
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
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
	マクロ定義
-----------------------------------------------------------------------------*/
using std::string;

// タイトル背景情報
static const string FILENAME_BG = "data/texture/gui_title/title_bg.jpg";
static const D3DXVECTOR3 POS_TITLE_BG	= D3DXVECTOR3(400.0f, 300.0f, 0.0f);
static const float WIDTH_TITLE_BG		= 800;
static const float HEIGHT_TITLE_BG		= 600;

// タイトルロゴ情報
static const string FILENAME_TITLE_LOGO = "data/texture/gui_title/title_logo.png";
static const D3DXVECTOR3 POS_TITLE_LOGO = D3DXVECTOR3(400.0f, 150.0f, 0.0f);
static const float WIDTH_TITLE_LOGO		= 500.0f;
static const float HEIGHT_TITLE_LOGO	= 100.0f;

// プレスキー情報
static const string FILENAME_PRESSKEY = "data/texture/gui_title/press_enterkey.png";
static const D3DXVECTOR3 POS_PRESSKEY	= D3DXVECTOR3(400.0f, 450.0f, 0.0f);
static const float WIDTH_PRESSKEY		= 500.0f;
static const float HEIGHT_PRESSKEY		= 100.0f;

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CTitle::CTitle()
{
	m_pressKey = nullptr;
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
	CScene2D::Create(
						FILENAME_BG.c_str(),
						POS_TITLE_BG,
						VEC3_ZERO,
						WIDTH_TITLE_BG,
						HEIGHT_TITLE_BG);

	// タイトルロゴの生成
	CScene2D::Create(
						FILENAME_TITLE_LOGO.c_str(),
						POS_TITLE_LOGO,
						VEC3_ZERO,
						WIDTH_TITLE_LOGO,
						HEIGHT_TITLE_LOGO);

	// プレスキーの生成
	m_pressKey = CScene2D::Create(
									FILENAME_PRESSKEY.c_str(),
									POS_PRESSKEY,
									VEC3_ZERO,
									WIDTH_PRESSKEY,
									HEIGHT_PRESSKEY);

	m_countBlink = 0.0f;
	m_countDecided = 0.0f;

	m_bDecide = false;

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

		// プレスキーの決定前点滅
		Blink(m_pressKey, 30.0f);
	}

	// プレスキーの決定後点滅
	if(m_bDecide)
	{
		Blink(m_pressKey, 3.0f);
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

/*-----------------------------------------------------------------------------
	2Dオブジェクトの点滅
-----------------------------------------------------------------------------*/
void CTitle::Blink(CScene2D* pTarget, float flashTime)
{
	m_countBlink++;
	
	// 明るい文字色に切り替え
	if(m_countBlink > flashTime * 2)
	{
		pTarget->SetDiffuse(COL_WHITE);
		m_countBlink = 0.0f;
		return; // 処理後はとばす
	}

	// 暗い文字色に切り替え
	if(m_countBlink > flashTime)
	{
		// 初期のアルファ値が１．０ならばうまく作用する
		pTarget->SetDiffuse(COL_WHITE_ALPHA(0.3f));
	}
}
