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
class CGauge;
class CBackGround;
class CScrollManager;
class CLaserManager;
class CInputCommand;
class CPseudoLight;
class C2DLogo;
class CPause;
class CScene2D;

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
	static CLaserManager* GetLaserManager(void){ return m_pLaserManager; }

	enum TRANSITIONID
	{
		TRANSITIONID_NONE = -1,
		TRANSITIONID_GAME_RETRY = 0,
		TRANSITIONID_STAGESELECT,
		TRANSITIONID_TITLE,
		TRANSITIONID_MAX
	};

	// ポーズメニューＩＤ
	enum PAUSEID
	{
		PAUSEID_RETRY = 1,
		PAUSEID_EXIT, // ステージ選択に戻る
		PAUSEID_MAX
	};

	void SetTransitionID(TRANSITIONID id){ m_transitionID = id; }

	// ポーズの取得
	static CPause* GetPause(void){ return m_pPause; }

	// ポーズの入力処理をチェックし切り替える
	void PauseTo(void);

	private:
		// プレイヤーへのポインタ
		static CPlayer* m_pPlayer;
		static CBlockManager* m_pBlockManager;
		static CLaserManager* m_pLaserManager;

		// ポーズへのポインタ
		static CPause *m_pPause;

		CScrollManager *m_pScrollManager;
		CBackGround *m_pBackGround;
		CAntonLifeUI *m_pLifeUI;
		CGauge *m_pGauge;
		CInputCommand *m_pInputCommand;
		float m_fScore;
		CPseudoLight *m_pPseudoLight;
		C2DLogo *m_pEndLogo;

		CScene2D *m_pGameOverBG;
		bool m_bGameOver;
		float m_autoChange;

		TRANSITIONID m_transitionID;
		bool m_bTransition;

		void InitGauge(void);
		void HitCheckMinimumAnton(void);
		void HitCheckMetalAnton(void);
		void HitCheckAnton(void);
		void CheckConnectAction(void);
		bool ConnectGimmickBlock(void);
		bool ConnectChangeAntonBlock(void);
		bool ConnectNormalBlock(void);
		void HitCheckItem(void);
		void CheckGameEnd(void);
		void InitStage(void);
		void CheckPauseSelect(void);
		void CheckTransition(void);
		void InitGameOverBG(void);
		void Retry(void);
		void ReturnToStageSelect(void);
		void CheckGameOver(void);
		void PlayBgm(void);
		void StopBgm(void);
		void PlayJingleClear(void);
		void PlayJingleOver(void);
		void PlaySeFood(void);
		void PlaySeMetamorPowerUp(int metamorState);
};
