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
#include "TextureLoader.h"

/*-----------------------------------------------------------------------------
	�^�C�g����ʃt�F�[�Y�N���X
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
		void UpdateInputEvent(void);
		void MoveBeeconCursor(void);
		void CommandLeft(void);
		void CommandRight(void);
		void CommandUp(void);
		void CommandDown(void);
		void InitAnimationBeeconCursor(void);
		void UpdateAnimationBeeconCursor(void);
		void InitAnimationTitleLogo(void);
		void UpdateAnimationTitleLogo(void);
		void UpdateAnimationTitleBg(void);
		void PlayBgm(void);
		void StopBgm(void);
		void PlaySeDecied(void);

		CInputCommand *m_pInputCommand;

		// �^�C�g����ʕ\�����̃C���X�^���X�ۑ��p
		CScene2D *m_pTitleLogo;
		CScene2D *m_pTitleBg;
		CScene2D *m_pPressGameStartText;
		CScene2D *m_pPressGameStartTextF;
		CScene2D *m_pBeeconCursor;

		// �v���X�L�[����
		bool m_bDecide;
		float m_countDecide;

		// �J�[�\��������
		float m_speed;
		D3DXVECTOR2 m_velocity;

		// �A�j���[�V�������i�ȈՔŁj
		enum AnimList
		{
			AL_WAIT = 0,
			AL_CONNECT,
			AL_MAX
		};
		AnimList m_selectAnim;

		struct AnimInfoSimple
		{
			int wait;
			D3DXVECTOR2 uv[4];
		};
		AnimInfoSimple m_beeconAnimWait[8];
		AnimInfoSimple m_beeconAnimConnect[8];

		bool m_bRoopStop;
		float m_countAnim;
		int m_idxAnim;

		float scrollBg;

		struct SclaeAttribute
		{
			D3DXVECTOR2 scale;
			float countLimit;
			D3DXVECTOR2 scaleLimit;
			SclaeAttribute *next;
		};

		SclaeAttribute m_scaleAttribute[5];
		SclaeAttribute *m_currentScaleAnim;
		D3DXVECTOR2 m_basicScale;
		float m_countScale;
		bool m_bScaling;

		bool m_bDeciedSe;
};
