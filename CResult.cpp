/*-----------------------------------------------------------------------------
	リザルト画面フェーズクラス
	author	OkadaMoeto
	since	20140713
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CResult.h"
#include <string>
#include "Define.h"
#include "CManager.h"
#include "CInput.h"
#include "CFade.h"
#include "CScene2D.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
using std::string;

// 自動遷移カウント数
static const float COUNT_AUTO_CHANGE = 180.0f;

// タイトル背景情報
static const string FILENAME_THANKMSG = "data/texture/gui_result/thankmsg.png";
static const D3DXVECTOR3 POS_RESULT_BG	= D3DXVECTOR3(400.0f, 300.0f, 0.0f);
static const float WIDTH_RESULT_BG		= 800.0f;
static const float HEIGHT_RESULT_BG		= 600.0f;

// 遊んでくれて...情報
static const string FILENAME_BG = "data/texture/gui_result/result_bg.jpg";
static const D3DXVECTOR3 POS_THANKMSG	= D3DXVECTOR3(400.0f, 300.0f, 0.0f);
static const float WIDTH_THANKMSG		= 800.0f;
static const float HEIGHT_THANKMSG		= 100.0f;

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CResult::CResult()
{
	m_thankmsg = nullptr;
	m_countAutoChange = 0.0f;
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CResult::~CResult()
{
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
void CResult::Init(void)
{
	// リザルト背景の生成
	CScene2D::Create(
						FILENAME_BG.c_str(),
						POS_RESULT_BG,
						VEC3_ZERO,
						WIDTH_RESULT_BG,
						HEIGHT_RESULT_BG);

	// 遊んでくれてありがとうのメッセージ生成
	m_thankmsg = CScene2D::Create(
									FILENAME_THANKMSG.c_str(),
									POS_THANKMSG,
									VEC3_ZERO,
									WIDTH_THANKMSG,
									HEIGHT_THANKMSG);

	// フェードイン
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 60.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CResult::Uninit(void)
{
	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CResult::Update(void)
{
	// キーボード入力を取得
	CInputKeyboard *pKeyboard = nullptr;
	pKeyboard = CManager::GetInputKeyboard();

	// フェードしていなければ更新
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		if(pKeyboard->GetKeyTrigger(DIK_RETURN))
		{
			CManager::GetPhaseFade()->Start(
											CFade::FADETYPE_OUT,
											60.0f,
											COL_WHITE);
		}

		// 自動遷移の処理
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
		// フェードアウト終了後にタイトル画面遷移
		CManager::SetPhase(CManager::PHASE_TITLE);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CResult.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:フェーズ遷移\n");
#endif
}
