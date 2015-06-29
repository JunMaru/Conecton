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

	private:
		// �v���C���[�ւ̃|�C���^
		static CPlayer* m_pPlayer;
		static CBlockManager* m_pBlockManager;

		CScrollManager *m_pScrollManager;
		CBackGround *m_pBackGround;
		CAntonLifeUI *m_pLifeUI;
		CGauge *m_pGauge;

		void InitGauge(void);
		void HitCheckMinimumAnton(void);
		void HitCheckMetalAnton(void);
		void HitCheckAnton(void);
};
