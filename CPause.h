/*-----------------------------------------------------------------------------
	�|�[�Y�N���X
	author	OkadaMoeto
	since	20140716
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "main.h"

/*-----------------------------------------------------------------------------
	�|�[�Y�N���X
-----------------------------------------------------------------------------*/
class CScene2D;
class CPauseMenu;
class CInputCommand;
class CPause
{
	public:
		CPause();
		virtual ~CPause();

		HRESULT Init(void);
		void Uninit(void);
		void Update(void);

		// �|�[�Y�̗L��
		void Enable(void);
		// �|�[�Y�̖���
		void Disable(void);

		// �|�[�Y�t���O�擾
		bool GetPause(void){ return m_bPause; }

		void UpdateInputEvent(CInputCommand* pInputCmd);

	private:
		void CommandSelect(void);
		void CommandUp(void);
		void CommandDown(void);

		// ���͂ɍ��킹���J�[�\���̈ړ�
		void MoveCursor(void);
		// �J�[�\���ʒu���㏸
		void UpCoursorPos(void);
		// �J�[�\���ʒu�����~
		void DownCoursorPos(void);
		// �I�����Ă���|�[�Y���j���[�̌��菈���ƑΉ������ԑJ��
		void SelectMenu(void);

		void SelectRetry(void);
		void SelectExit(void);

		void InitMenuDiffuse(void);
		void UpdateMenuDiffuse(void);

		// �|�[�Y���t���O
		bool m_bPause;

		// �I�𒆃��j���[�ʒu
		int m_menuCursorPos;

		// �|�[�Y���j���[�I�u�W�F�N�g
		CPauseMenu* m_pauseMenu;

		// �_�ŗp
		float m_curve;

		bool m_bSelsect;
		bool m_bUp;
		bool m_bDown;
};
