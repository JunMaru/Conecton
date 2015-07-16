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
		// �C���X�^���X�ۑ��p
		CScene2D *m_logoPhaseBg;
		CScene2D *m_groupLogo;
		CScene2D *m_titleLogo;
		CScene2D *m_titleBg;
		CScene2D *m_pressGameStartText;
		CFade *m_logoPhaseFade;

		// �v���X�L�[����
		bool m_bDecide;

		// �v���X�L�[������̃J�E���^�[
		float m_countDecided;

		// ���S�\���J�E���^�[
		float m_countLogoDisp;
};
