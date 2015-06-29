/*-----------------------------------------------------------------------------
	ゲーム画面フェーズクラス
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
	ゲーム画面フェーズクラス
-----------------------------------------------------------------------------*/
class CPlayer;
class CFade;
class CAntonLifeUI;
class CBlockManager;
class CGame : public CPhase
{
	public:
		CGame();
		~CGame();

		void Init(void);
		void Uninit(void);
		void Update(void);

		/*-----------------------------------------------------------------------------
			プレイヤーの取得
			return	CPlayer*		プレイヤーへのポインタ
		-----------------------------------------------------------------------------*/
		static CPlayer* GetPlayer(void){ return m_pPlayer; }
		static CBlockManager* GetBlockManager(void){ return m_pBlockManager; }

	private:
		// プレイヤーへのポインタ
		static CPlayer* m_pPlayer;
		static CBlockManager* m_pBlockManager;

		CAntonLifeUI *m_pLifeUI;

		void HitCheckMinimumAnton(void);
		void HitCheckMetalAnton(void);
		void HitCheckAnton(void);
};
