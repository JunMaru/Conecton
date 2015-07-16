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
class CFade;
class CTitle : public CPhase
{
	public:
		CTitle();
		~CTitle();

		void Init(void);
		void Uninit(void);
		void Update(void);

	private:
		// インスタンス保存用
		CScene2D *m_logoPhaseBg;
		CScene2D *m_groupLogo;
		CScene2D *m_titleLogo;
		CScene2D *m_titleBg;
		CScene2D *m_pressGameStartText;
		CFade *m_logoPhaseFade;

		// プレスキー押下
		bool m_bDecide;

		// プレスキー押下後のカウンター
		float m_countDecided;

		// ロゴ表示カウンター
		float m_countLogoDisp;
};
