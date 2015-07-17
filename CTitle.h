/*-----------------------------------------------------------------------------
	�^�C�g����ʃt�F�[�Y�N���X
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
	�^�C�g����ʃt�F�[�Y�N���X
-----------------------------------------------------------------------------*/
class CScene2D;
class CTitle : public CPhase
{
	public:
		CTitle();
		~CTitle();

		void Init(void);
		void Uninit(void);
		void Update(void);

	private:
		// �C���X�^���X�ۑ��p
		CScene2D *m_titleLogo;
		CScene2D *m_titleBg;
		CScene2D *m_pressGameStartText;
		CScene2D *m_beeconCursor;

		// �v���X�L�[����
		bool m_bDecide;
};
