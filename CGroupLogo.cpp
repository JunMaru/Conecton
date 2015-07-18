/*-----------------------------------------------------------------------------
	グループロゴ画面フェーズクラス
	author	OkadaMoeto
	since	20140717
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CGroupLogo.h"
#include "Define.h"
#include "CManager.h"
#include "CInput.h"
#include "CFade.h"

/*-----------------------------------------------------------------------------
	グループロゴの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_GROUPLOGO = "data/texture/logo_group/group_logo.png";
static const D3DXVECTOR3 POS_GROUPLOGO	= D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
static const float WIDTH_GROUPLOGO		= 400.0f;
static const float HEIGHT_GROUPLOGO		= 335.0f;

/*-----------------------------------------------------------------------------
	ロゴ表示タイム
-----------------------------------------------------------------------------*/
static const float TIME_LOGODISP = 30.0f * 2.0f;

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
void CGroupLogo::Init(void)
{
	m_pLogoBg = CScene2D::Create(
									NULL,
									D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
									VEC3_ZERO,
									1280.0f,
									720.0f);

	// グループロゴ生成
	m_pGroupLogo = CScene2D::Create(
									TEXTUREPATH_GROUPLOGO,
									POS_GROUPLOGO,
									VEC3_ZERO,
									WIDTH_GROUPLOGO,
									HEIGHT_GROUPLOGO);

	// ロゴ背景は白にしないと見えない
	m_pLogoBg->SetDiffuse(COL_WHITE);

	m_bSkip = false;
	m_countDisp = 0.0f;

	// フェードイン
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 30.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CGroupLogo::Uninit(void)
{
	// 描画対象オブジェクトの開放
	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CGroupLogo::Update(void)
{
	// フェードしていなければ更新
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		m_countDisp++;

		UpdateInputEvent();

#ifdef _DEBUG
		UpdateInputEventDebug();
#endif

		Skip();
	}

	if(m_countDisp > TIME_LOGODISP)
	{
		CManager::GetPhaseFade()->Start(CFade::FADETYPE_OUT, 30.0f, COL_WHITE);

		// 無限フェード防止
		m_bSkip = false;
		m_countDisp = 0.0f;
	}

	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		// フェードアウト終了後にタイトル画面遷移
		CManager::SetPhase(CManager::PHASE_TITLE);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CGroupLogo.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:フェーズ遷移\n");
	CDebugProcDX9::Print("ロゴ表示(%f)\n", m_countDisp);
#endif
}

/*-----------------------------------------------------------------------------
	入力イベント更新
-----------------------------------------------------------------------------*/
void CGroupLogo::UpdateInputEvent(void)
{
	CInputJoypad *pJoyPad = CManager::GetInputJoypad();

	if(pJoyPad == nullptr) return;

	if(pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_1)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_2)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_3)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_4)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_5)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_6)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_7)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_8)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_9)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_10)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_11)
	|| pJoyPad->GetKeyTrigger(CInputJoypad::GAMEPAD_12))
	{
		m_bSkip = true;
	}
}

/*-----------------------------------------------------------------------------
	入力イベント更新（デバッグ）
-----------------------------------------------------------------------------*/
void CGroupLogo::UpdateInputEventDebug(void)
{
#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if(pKeyboard == nullptr) return;

	if(pKeyboard->GetKeyTrigger(DIK_RETURN))
	{
		m_bSkip = true;
	}
#endif
}

/*-----------------------------------------------------------------------------
	ロゴ表示時間のスキップ
-----------------------------------------------------------------------------*/
void CGroupLogo::Skip(void)
{
	if(m_bSkip)
	{
		m_countDisp = TIME_LOGODISP + 1;
	}
}
