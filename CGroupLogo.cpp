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
#include "CInputCommand.h"

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

	m_pInputCommand = new CInputCommand(CManager::GetInputKeyboard(), CManager::GetInputJoypad());
	m_pInputCommand->Init();

	// フェードイン
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 30.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CGroupLogo::Uninit(void)
{
	m_pInputCommand->Uninit();
	delete m_pInputCommand;

	// 描画対象オブジェクトの開放
	CScene::ReleaseAll();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CGroupLogo::Update(void)
{
	m_pInputCommand->Update();

	// フェードしていなければ更新
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		m_countDisp++;

		UpdateInputEvent();

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
	const bool bDecide = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_ENTER)
						|| m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CONNECT)
						|| m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CANCEL)
						|| m_pInputCommand->IsTrigger(CInputCommand::COMMAND_GIMMICKACTION);

	if(bDecide)
	{
		m_bSkip = true;
	}
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
