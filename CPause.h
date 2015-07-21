/*-----------------------------------------------------------------------------
	ポーズクラス
	author	OkadaMoeto
	since	20140716
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "main.h"

/*-----------------------------------------------------------------------------
	ポーズクラス
-----------------------------------------------------------------------------*/
class CScene2D;
class CPauseMenu;
class CInputCommand;
class CPause
{
	public:
		CPause();
		virtual ~CPause();

		HRESULT Init(void);
		void Uninit(void);
		void Update(void);

		// ポーズの有効
		void Enable(void);
		// ポーズの無効
		void Disable(void);

		// ポーズフラグ取得
		bool GetPause(void){ return m_bPause; }

		void UpdateInputEvent(CInputCommand* pInputCmd);

	private:
		void CommandSelect(void);
		void CommandUp(void);
		void CommandDown(void);

		// 入力に合わせたカーソルの移動
		void MoveCursor(void);
		// カーソル位置を上昇
		void UpCoursorPos(void);
		// カーソル位置を下降
		void DownCoursorPos(void);
		// 選択しているポーズメニューの決定処理と対応する状態遷移
		void SelectMenu(void);

		void SelectRetry(void);
		void SelectExit(void);

		void InitMenuDiffuse(void);
		void UpdateMenuDiffuse(void);

		// ポーズ中フラグ
		bool m_bPause;

		// 選択中メニュー位置
		int m_menuCursorPos;

		// ポーズメニューオブジェクト
		CPauseMenu* m_pauseMenu;

		// 点滅用
		float m_curve;

		bool m_bSelsect;
		bool m_bUp;
		bool m_bDown;
};
