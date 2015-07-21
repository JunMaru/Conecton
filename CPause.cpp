/*-----------------------------------------------------------------------------
	ポーズクラス
	author	OkadaMoeto
	since	20140716
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CPause.h"
#include "define.h"
#include "CScene2D.h"
#include "CInput.h"
#include "CGame.h"
#include "CPauseMenu.h"
#include "CManager.h"
#include "CFade.h"
#include "CConfigRecorder.h"
#include "CInputCommand.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
static const D3DXVECTOR3 STARTPOS_TOP_CURSOR	= D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 250.0f, 150.0f, 0.0f);
static const D3DXVECTOR3 STARTPOS_UNDER_CURSOR	= D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 250.0f, 450.0f, 0.0f);

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CPause::CPause()
{
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CPause::~CPause()
{
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
HRESULT CPause::Init(void)
{
	// ポーズメニューの生成
	m_pauseMenu = new CPauseMenu();
	m_pauseMenu->Init();

	// ポーズの更新をさせないためにOFF
	m_bPause = false;

	// 初期状態で表示をしないようにする
	m_pauseMenu->HideAll();

	m_curve = 0.0f;

	m_bSelsect = false;
	m_bUp = false;
	m_bDown = false;

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CPause::Uninit(void)
{
	// ポーズメニューの解放
	if(m_pauseMenu)
	{
		delete m_pauseMenu;
		m_pauseMenu = nullptr;
	}
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CPause::Update(void)
{
	// 使用していなかったら更新しない
	if(!m_bPause){ return; }

	m_pauseMenu->Update();

	MoveCursor();

	SelectMenu();
}

/*-----------------------------------------------------------------------------
	ポーズメニュー表示
-----------------------------------------------------------------------------*/
void CPause::Enable(void)
{
	m_bPause = true;

	if(CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE) == 0)
	{
		// リトライ回数０のときは、初期位置をＥＸＩＴ設定
		m_pauseMenu->SetPositionCursor(STARTPOS_UNDER_CURSOR);
		m_menuCursorPos = CPauseMenu::PAUSEMENU_EXIT;

		// メニュー選択できないため、非表示にする
		m_pauseMenu->SetColorRetry(COL_BLACK_ALPHA(0.0f));
	}
	else
	{
		// ポーズメニュー初期位置をＲＥＴＲＹ設定
		m_pauseMenu->SetPositionCursor(STARTPOS_TOP_CURSOR);
		m_menuCursorPos = CPauseMenu::PAUSEMENU_RETRY;
	}

	m_pauseMenu->DispAll();
}

/*-----------------------------------------------------------------------------
	ポーズメニュー非表示
-----------------------------------------------------------------------------*/
void CPause::Disable(void)
{
	m_bPause = false;

	m_pauseMenu->HideAll();
}

/*-----------------------------------------------------------------------------
	カーソル位置を上昇
-----------------------------------------------------------------------------*/
void CPause::UpCoursorPos(void)
{
	// 選択しているメニューから上のメニューを選択しているようにずらす
	m_menuCursorPos--;

	bool bMenuUpLimit = m_menuCursorPos < CPauseMenu::PAUSEMENU_RETRY;
	if(bMenuUpLimit)
	{
		// メニュー位置が一番上の状態でさらに同様の入力をしたときに下にループさせる
		m_pauseMenu->SetPositionCursor(STARTPOS_UNDER_CURSOR);
		m_menuCursorPos = CPauseMenu::PAUSEMENU_EXIT;
	}
	else
	{
		// メニュー位置の上げる
		D3DXVECTOR3 cursorPos = m_pauseMenu->GetPositionCursor();
		float offsetY = 300.0f;
		cursorPos.y -= offsetY;
		m_pauseMenu->SetPositionCursor(cursorPos);
	}
}

/*-----------------------------------------------------------------------------
	入カーソル位置を下降
-----------------------------------------------------------------------------*/
void CPause::DownCoursorPos(void)
{
	// 選択しているメニューから下のメニューを選択しているようにずらす
	m_menuCursorPos++;

	bool bMenuDownLimit = m_menuCursorPos >= CPauseMenu::PAUSEMENU_MAX;
	if(bMenuDownLimit)
	{
		if(CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE) != 0)
		{
			// メニュー位置が一番下の状態でさらに同様の入力をしたときに上にループさせる
			m_pauseMenu->SetPositionCursor(STARTPOS_TOP_CURSOR);
			m_menuCursorPos = CPauseMenu::PAUSEMENU_RETRY;
		}
	}
	else
	{
		// メニュー位置を下げる
		D3DXVECTOR3 cursorPos = m_pauseMenu->GetPositionCursor();
		float offsetY = 300.0f;
		cursorPos.y += offsetY;
		m_pauseMenu->SetPositionCursor(cursorPos);
	}
}

/*-----------------------------------------------------------------------------
	入力に合わせたカーソルの移動
-----------------------------------------------------------------------------*/
void CPause::MoveCursor(void)
{
	if(m_bUp)
	{
		if(CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE) != 0)
		{
			UpCoursorPos();
		}
	}

	if(m_bDown)
	{
		DownCoursorPos();
	}
}

/*-----------------------------------------------------------------------------
	選択しているポーズメニューの決定処理と対応する状態遷移
-----------------------------------------------------------------------------*/
void CPause::SelectMenu(void)
{
	// ポーズメニューの頂点カラー
	D3DXCOLOR selectCol = COL_WHITE;
	D3DXCOLOR unSelectCol = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.75f);

	switch(m_menuCursorPos)
	{
		case CPauseMenu::PAUSEMENU_RETRY:
		{
			if(CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE) != 0)
			{
				m_pauseMenu->SetColorRetry(selectCol);
			}

			m_pauseMenu->SetColorExit(unSelectCol);

			if(m_bSelsect)
			{
				SelectRetry();
			}
			break;
		}

		case CPauseMenu::PAUSEMENU_EXIT:
		{
			if(CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE) != 0)
			{
				m_pauseMenu->SetColorRetry(unSelectCol);
			}

			m_pauseMenu->SetColorExit(selectCol);

			if(m_bSelsect)
			{
				SelectExit();
			}
			break;
		}
	}
}

void CPause::UpdateInputEvent(CInputCommand* pInputCmd)
{
	if(pInputCmd == nullptr)
	{
		return;
	}

	bool bSelect = pInputCmd->IsTrigger(CInputCommand::COMMAND_CONNECT);
	if(bSelect)
	{
		CommandSelect();
	}
	else
	{
		m_bSelsect = false;
	}

	bool bUp = pInputCmd->IsTrigger(CInputCommand::COMMAND_UP);
	if(bUp)
	{
		CommandUp();
	}
	else
	{
		m_bUp = false;
	}

	bool bDown = pInputCmd->IsTrigger(CInputCommand::COMMAND_DOWN);
	if(bDown)
	{
		CommandDown();
	}
	else
	{
		m_bDown = false;
	}
}

void CPause::CommandSelect(void)
{
	m_bSelsect = true;
}

void CPause::CommandUp(void)
{
	m_bUp = true;
}

void CPause::CommandDown(void)
{
	m_bDown = true;
}

void CPause::SelectRetry(void)
{
	m_pauseMenu->HideAll();
	Disable();
	CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_PAUSESLECT, CGame::PAUSEID_RETRY);
}

void CPause::SelectExit(void)
{

	m_pauseMenu->HideAll();
	Disable();
	CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_PAUSESLECT, CGame::PAUSEID_EXIT);
}

void CPause::InitMenuDiffuse(void)
{

}

void CPause::UpdateMenuDiffuse(void)
{

}
