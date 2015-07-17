/*-----------------------------------------------------------------------------
	�A�v���P�[�V�����Ǘ��N���X
	author	OkadaMoeto
	since	20140507
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "Main.h"
#include "CPhase.h"

/*-----------------------------------------------------------------------------
	���������[�N�s�o�͗p
-----------------------------------------------------------------------------*/
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif
#endif

/*-----------------------------------------------------------------------------
	�}�l�[�W���N���X
-----------------------------------------------------------------------------*/
class CRenderer;
class CScene;
class CInputKeyboard;
class CInputMouse;
class CInputJoypad;
class CCamera;
class CLight;
class CSoundXAudio2;
class CFade;
#ifdef _DEBUG
class CDebugProcDX9;
#endif
class CManager
{
	public:
		// �t�F�[�Y���
		enum PHASE
		{
			PHASE_GROUPLOGO = 0,
			PHASE_TITLE,
			PHASE_GAME,
			PHASE_RESULT,
			PHASE_MAX
		};

		CManager();
		virtual ~CManager();

		HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
		void Uninit(void);
		void Update(void);
		void Draw(void);

		/*-----------------------------------------------------------------------------
			�����_���[�擾
			return	CRenderer*		�����_���[�ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		static CRenderer* GetRenderer(void){ return m_pRenderer; }

		/*-----------------------------------------------------------------------------
			�L�[�{�[�h�擾
			return	CInputKeyboard*		�L�[�{�[�h�ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		static CInputKeyboard* GetInputKeyboard(void){ return m_pInputKeyboard; }

		/*-----------------------------------------------------------------------------
			�}�E�X�擾
			return	CInputMouse*		�}�E�X�ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		static CInputMouse* GetInputMouse(void){ return m_pInputMouse; }

		/*-----------------------------------------------------------------------------
			�W���C�p�b�h�擾
			return	CInputJoypad*		�W���C�p�b�h�ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		static CInputJoypad* GetInputJoypad(void){ return m_pInputJoypad; }

		/*-----------------------------------------------------------------------------
			�J�����擾
			return	CCamera*		�J�����ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		static CCamera* GetCamera(void){ return m_pCamera; }

		/*-----------------------------------------------------------------------------
			���C�g�擾
			return	CLight*		���C�g�ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		static CLight* GetLight(void){ return m_pLight; }

		/*-----------------------------------------------------------------------------
			�T�E���h�擾
			return	CSoundXAudio2*		�T�E���h�ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		static CSoundXAudio2* GetSoundXAudio2(void){ return m_pSoundXAudio2; }

		/*-----------------------------------------------------------------------------
			��ʑJ�ڗp�̃t�F�[�h�擾
			return	CFade*		��ʑJ�ڗp�̃t�F�[�h�ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		static CFade* GetPhaseFade(void){ return m_pPhaseFade; }

		/*-----------------------------------------------------------------------------
			���݂̃t�F�[�Y�擾
			return	PHASE		���݂̃t�F�[�Y�̒l
		-----------------------------------------------------------------------------*/
		static PHASE GetPhase(void){ return m_Phase; }
		/*-----------------------------------------------------------------------------
			���݂̃t�F�[�Y�ݒ�
			param	phase		���݂̃t�F�[�Y�̐ݒ�l
		-----------------------------------------------------------------------------*/
		static void SetPhase(PHASE phase){ m_Phase = phase; }

		// �t�F�[�Y�ڍs
		void ChangePhase(void);

	private:
		// �O��t�F�[�Y
		static PHASE m_OldPhase;
		// ���t�F�[�Y
		static PHASE m_Phase;
		// ���݃t�F�[�Y
		CPhase* m_pPhase;

		// �����_���[�ւ̃|�C���^
		static CRenderer* m_pRenderer;
		// �L�[�{�[�h���͂ւ̃|�C���^
		static CInputKeyboard* m_pInputKeyboard;
		// �}�E�X���͂ւ̃|�C���^
		static CInputMouse* m_pInputMouse;
		// �W���C�p�b�h���͂ւ̃|�C���^
		static CInputJoypad* m_pInputJoypad;
		// �J�����ւ̃|�C���^
		static CCamera* m_pCamera;
		// ���C�g�ւ̃|�C���^
		static CLight* m_pLight;
		// �T�E���h�ւ̃|�C���^
		static CSoundXAudio2* m_pSoundXAudio2;
		// �t�F�[�Y�J�ڎ��ɂ͂��ރt�F�[�h�ւ̃|�C���^
		static CFade* m_pPhaseFade;

#ifdef _DEBUG
		// �f�o�b�O�\���ւ̃|�C���^
		static CDebugProcDX9* m_pDebugProcDX9;
#endif
};
