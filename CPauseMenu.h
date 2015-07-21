/*-----------------------------------------------------------------------------
	�|�[�Y���j���[
	author	OkadaMoeto
	since	20141211
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "main.h"

/*-----------------------------------------------------------------------------
	�|�[�Y���j���[�N���X
-----------------------------------------------------------------------------*/
class CScene2D;
class CPauseMenu
{
	public:
		// �|�[�Y���j���[���
		enum PAUSEMENU
		{
			PAUSEMENU_RETRY = 0,		// �Q�[�������g���C
			PAUSEMENU_EXIT,				// �Q�[�����I�����ă^�C�g���ɖ߂�
			PAUSEMENU_MAX
		};

		CPauseMenu();
		virtual ~CPauseMenu();

		void Init(void);
		void Uninit(void);
		void Update(void);

		// �|�[�Y���j���[�\��
		void Disp(CScene2D* menu);
		// �S�|�[�Y���j���[�\��
		void DispAll(void);

		// �|�[�Y���j���[��\��
		void Hide(CScene2D* menu);
		// �S�|�[�Y���j���[��\��
		void HideAll(void);

		// ���j���[�̐F�ύX
		void SetColorRetry(D3DXCOLOR col);
		void SetColorExit(D3DXCOLOR col);

		// �J�[�\���̈ʒu�ݒ�
		void SetPositionCursor(D3DXVECTOR3 pos);
		// �J�[�\���̈ʒu�擾
		D3DXVECTOR3 GetPositionCursor(void);

	private:
		void InitAnimationCursor(void);
		void UpdateAnimationCursor(void);

		// �|�[�Y���j���[�I�u�W�F�N�g
		CScene2D* m_background;
		CScene2D* m_cursor;
		CScene2D* m_retry;
		CScene2D* m_exit;

		enum AnimListBeecon
		{
			ALB_WAIT = 0,
			ALB_MAX
		};
		AnimListBeecon m_selectAnimBeecon;

		struct AnimInfoSimple
		{
			int wait;
			D3DXVECTOR2 uv[4];
		};

		AnimInfoSimple m_beeconAnimWait[8];
		bool m_bRoopStopBeecon;
		float m_countAnimBeecon;
		int m_idxAnimBeecon;
};
