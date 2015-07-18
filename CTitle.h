/*-----------------------------------------------------------------------------
	タイトル画面フェーズクラス
	author	OkadaMoeto
	since	20140713
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"
#include "CPhase.h"

/*-----------------------------------------------------------------------------
	タイトル画面フェーズクラス
-----------------------------------------------------------------------------*/
class CScene2D;
class CInputCommand;
class CTitle : public CPhase
{
	public:
		CTitle();
		~CTitle();

		void Init(void);
		void Uninit(void);
		void Update(void);

	private:
		void AnimationTitleLogo(void);
		void UpdateInputEvent(void);
		void MoveBeeconCursor(void);
		void CommandLeft(void);
		void CommandRight(void);
		void CommandUp(void);
		void CommandDown(void);

		CInputCommand *m_pInputCommand;

		// タイトル画面表示物のインスタンス保存用
		CScene2D *m_pTitleLogo;
		CScene2D *m_pTitleBg;
		CScene2D *m_pPressGameStartText;
		CScene2D *m_pPressGameStartTextF;
		CScene2D *m_pBeeconCursor;

		// プレスキー押下
		bool m_bDecide;

		// カーソル動作情報
		float m_speed;
		D3DXVECTOR2 m_velocity;
};
