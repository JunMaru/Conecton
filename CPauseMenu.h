/*-----------------------------------------------------------------------------
	ポーズメニュー
	author	OkadaMoeto
	since	20141211
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "main.h"

/*-----------------------------------------------------------------------------
	ポーズメニュークラス
-----------------------------------------------------------------------------*/
class CScene2D;
class CPauseMenu
{
	public:
		// ポーズメニュー種類
		enum PAUSEMENU
		{
			PAUSEMENU_RETRY = 0,		// ゲームをリトライ
			PAUSEMENU_EXIT,				// ゲームを終了してタイトルに戻る
			PAUSEMENU_MAX
		};

		CPauseMenu();
		virtual ~CPauseMenu();

		void Init(void);
		void Uninit(void);
		void Update(void);

		// ポーズメニュー表示
		void Disp(CScene2D* menu);
		// 全ポーズメニュー表示
		void DispAll(void);

		// ポーズメニュー非表示
		void Hide(CScene2D* menu);
		// 全ポーズメニュー非表示
		void HideAll(void);

		// メニューの色変更
		void SetColorRetry(D3DXCOLOR col);
		void SetColorExit(D3DXCOLOR col);

		// カーソルの位置設定
		void SetPositionCursor(D3DXVECTOR3 pos);
		// カーソルの位置取得
		D3DXVECTOR3 GetPositionCursor(void);

	private:
		void InitAnimationCursor(void);
		void UpdateAnimationCursor(void);

		// ポーズメニューオブジェクト
		CScene2D* m_background;
		CScene2D* m_cursor;
		CScene2D* m_retry;
		CScene2D* m_exit;

		enum AnimListBeecon
		{
			ALB_WAIT = 0,
			ALB_MAX
		};
		AnimListBeecon m_selectAnimBeecon;

		struct AnimInfoSimple
		{
			int wait;
			D3DXVECTOR2 uv[4];
		};

		AnimInfoSimple m_beeconAnimWait[8];
		bool m_bRoopStopBeecon;
		float m_countAnimBeecon;
		int m_idxAnimBeecon;
};
