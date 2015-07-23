/*-----------------------------------------------------------------------------
	�Q�[����ʃt�F�[�Y�N���X
	author	OkadaMoeto
	since	20140713
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "Main.h"
#include "CPhase.h"

/*-----------------------------------------------------------------------------
	�Q�[����ʃt�F�[�Y�N���X
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
		�v���C���[�̎擾
		return	CPlayer*		�v���C���[�ւ̃|�C���^
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

	// �|�[�Y���j���[�h�c
	enum PAUSEID
	{
		PAUSEID_RETRY = 1,
		PAUSEID_EXIT, // �X�e�[�W�I���ɖ߂�
		PAUSEID_MAX
	};

	void SetTransitionID(TRANSITIONID id){ m_transitionID = id; }

	// �|�[�Y�̎擾
	static CPause* GetPause(void){ return m_pPause; }

	// �|�[�Y�̓��͏������`�F�b�N���؂�ւ���
	void PauseTo(void);

	private:
		// �v���C���[�ւ̃|�C���^
		static CPlayer* m_pPlayer;
		static CBlockManager* m_pBlockManager;
		static CLaserManager* m_pLaserManager;

		// �|�[�Y�ւ̃|�C���^
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
