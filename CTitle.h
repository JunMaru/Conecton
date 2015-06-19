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
		// 2D�I�u�W�F�N�g�̓_��
		void Blink(CScene2D* pTarget, float flashTime);

		// �v���X�L�[�\���̃C���X�^���X�ۑ�
		CScene2D* m_pressKey;

		// �v���X�L�[�_�ŃJ�E���^�[
		float m_countBlink;

		// �v���X�L�[����
		bool m_bDecide;

		// �v���X�L�[������̃J�E���^�[
		float m_countDecided;
};
