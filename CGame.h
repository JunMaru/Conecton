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

	private:
		// �v���C���[�ւ̃|�C���^
		static CPlayer* m_pPlayer;
		static CBlockManager* m_pBlockManager;
		static CLaserManager* m_pLaserManager;

		CScrollManager *m_pScrollManager;
		CBackGround *m_pBackGround;
		CAntonLifeUI *m_pLifeUI;
		CGauge *m_pGauge;
		CInputCommand *m_pInputCommand;
		float m_fScore;
		CPseudoLight *m_pPseudoLight;
		C2DLogo *m_pEndLogo;

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
};
