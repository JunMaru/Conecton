/*-----------------------------------------------------------------------------
	�A�v���P�[�V�����Ǘ��N���X
	author	OkadaMoeto
	since	20140507
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CManager.h"
#include "Define.h"
#include "CInput.h"
#include "CCamera.h"
#include "CLight.h"
#include "CSoundXAudio2.h"
#include "CGroupLogo.h"
#include "CTitle.h"
#include "CGame.h"
#include "CResult.h"
#include "ScreenCaptureDX9.h"
#include "CFade.h"

/*-----------------------------------------------------------------------------
	�ÓI�����o�ϐ��̏�����
-----------------------------------------------------------------------------*/
CRenderer* CManager::m_pRenderer				= nullptr;
CInputKeyboard* CManager::m_pInputKeyboard		= nullptr;
CInputMouse* CManager::m_pInputMouse			= nullptr;
CInputJoypad* CManager::m_pInputJoypad			= nullptr;
CCamera* CManager::m_pCamera					= nullptr;
CLight* CManager::m_pLight						= nullptr;
CSoundXAudio2* CManager::m_pSoundXAudio2		= nullptr;
CFade* CManager::m_pPhaseFade					= nullptr;
#ifdef _DEBUG
CDebugProcDX9* CManager::m_pDebugProcDX9		= nullptr;
#endif
// �t�F�[�Y�̏����ݒ� ����1
#ifdef _DEBUG
CManager::PHASE CManager::m_Phase = PHASE_TITLE;
CManager::PHASE CManager::m_OldPhase = PHASE_TITLE;
#else
CManager::PHASE CManager::m_Phase = PHASE_TITLE;
CManager::PHASE CManager::m_OldPhase = PHASE_TITLE;
#endif

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CManager::CManager()
{
	m_pPhase = nullptr;
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CManager::~CManager()
{
}

/*-----------------------------------------------------------------------------
	�}�l�[�W���̏�����(�e���\�[�X�Ȃǂ̐����Ə�����)
-----------------------------------------------------------------------------*/
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �����_���[����
	m_pRenderer = new CRenderer();
	m_pRenderer->Init(hWnd, bWindow);

	// �L�[�{�[�h����
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// �}�E�X����
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);

	// �W���C�p�b�h����
	m_pInputJoypad = new CInputJoypad;
	if(FAILED(m_pInputJoypad->Init(hInstance, hWnd)))
	{
		// �W���C�p�b�h�������Ƃ��͉��������
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}

	// �J��������
	m_pCamera = new CCamera();
	m_pCamera->Init();

	// ���C�g����
	m_pLight = new CLight();
	m_pLight->Init();

	// �T�E���h����
	m_pSoundXAudio2 = new CSoundXAudio2();
	m_pSoundXAudio2->Init(hWnd);

	// �t�F�[�Y�t�F�[�h����
	m_pPhaseFade = CFade::Create(
									D3DXVECTOR3((float)(SCREEN_WIDTH / 2), (float)(SCREEN_HEIGHT/ 2), 0.0f),
									VEC3_ZERO
	);

#ifdef _DEBUG
	// �f�o�b�O�\������
	m_pDebugProcDX9 = new CDebugProcDX9;
	m_pDebugProcDX9->Init();
#endif

	// �t�F�[�Y�̏����ݒ� ����2
#ifdef _DEBUG
	m_pPhase = new CTitle();
	m_pPhase->Init();
#else
	m_pPhase = new CTitle();
	m_pPhase->Init();
#endif

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�}�l�[�W���̏I��(�e���\�[�X�Ȃǂ̉��)
-----------------------------------------------------------------------------*/
void CManager::Uninit(void)
{
	// �t�F�[�Y�̉��
	if(m_pPhase)
	{
		m_pPhase->Uninit();
		delete m_pPhase;
		m_pPhase = nullptr;
	}

	// �����_�����O���
	if(m_pRenderer)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// �L�[�{�[�h���
	if(m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	// �}�E�X���
	if(m_pInputMouse)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	// �W���C�p�b�h���
	if(m_pInputJoypad)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}
	
	// �J�������
	if(m_pCamera)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// ���C�g���
	if(m_pLight)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	// �T�E���h���
	if(m_pSoundXAudio2)
	{
		m_pSoundXAudio2->Uninit();
		delete m_pSoundXAudio2;
		m_pSoundXAudio2 = nullptr;
	}

	// �t�F�[�Y�t�F�[�h���
	if(m_pPhaseFade)
	{
		m_pPhaseFade->Uninit();
		delete m_pPhaseFade;
		m_pPhaseFade = nullptr;
	}

#ifdef _DEBUG
	// �f�o�b�O�\�����
	if(m_pDebugProcDX9)
	{
		m_pDebugProcDX9->Uninit();
		delete m_pDebugProcDX9;
		m_pDebugProcDX9 = nullptr;
	}
#endif
}

/*-----------------------------------------------------------------------------
	�}�l�[�W���̍X�V
-----------------------------------------------------------------------------*/
void CManager::Update(void)
{
#ifdef _DEBUG
	// �f�o�b�O�\��
	if(m_pInputKeyboard->GetKeyTrigger(DIK_F1))
	{
		if(CDebugProcDX9::GetDisp())
		{
			CDebugProcDX9::SetDisp(false);
		}
		else
		{
			CDebugProcDX9::SetDisp(true);
		}
	}

	// ���C���[�t���[���\������
	if(m_pInputKeyboard->GetKeyTrigger(DIK_F2))
	{
		if(CRenderer::GetFillModeWireFrame())
		{
			CRenderer::SetFillModeWireFrame(false);
		}
		else
		{
			CRenderer::SetFillModeWireFrame(true);
		}
	}

	// �X�N���[���V���b�g����
	if(m_pInputKeyboard->GetKeyTrigger(DIK_F3))
	{
		CaptureImageFromBackBuffer(D3DXIFF_BMP);
	}
	if(m_pInputKeyboard->GetKeyTrigger(DIK_F4))
	{
		CaptureImageFromBackBuffer(D3DXIFF_JPG);
	}
#endif

	// ���ʃI�u�W�F�N�g�̍X�V
	if(m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}
	if(m_pInputMouse)
	{
		m_pInputMouse->Update();
	}
	if(m_pInputJoypad)
	{
		m_pInputJoypad->Update();
	}
	if(m_pCamera)
	{
		m_pCamera->Update();
	}
	if(m_pLight)
	{
		m_pLight->Update();
	}

	// �t�F�[�Y�̍X�V
	if(m_pPhase)
	{
		m_pPhase->Update();
	}

	// �t�F�[�Y�̈ڍs
	ChangePhase();

	// �I�u�W�F�N�g�̑S�X�V
	if(m_pRenderer)
	{
		m_pRenderer->Update();
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CManager.cpp]\n");
	CDebugProcDX9::Print("[F1]:�f�o�b�O�\��\n");
	CDebugProcDX9::Print("[F2]:���C���[�t���[���`��\n");
	CDebugProcDX9::Print("[F3]:�X�N���[���V���b�g(BMP)<�ۑ��ꏊ>data/screenshot\n");
	CDebugProcDX9::Print("[F4]:�X�N���[���V���b�g(JPG)<�ۑ��ꏊ>data/screenshot\n");
#endif
}

/*-----------------------------------------------------------------------------
	�}�l�[�W���̕`��
-----------------------------------------------------------------------------*/
void CManager::Draw(void)
{
	// �I�u�W�F�N�g�̑S�`��
	if(m_pRenderer){ m_pRenderer->Draw(); }
}

/*-----------------------------------------------------------------------------
	�t�F�[�Y�ڍs
-----------------------------------------------------------------------------*/
void CManager::ChangePhase(void)
{
	// �t�F�[�Y���قȂ��Ă�����A���̃t�F�[�Y�ֈڍs
	if( m_Phase != m_OldPhase )
	{
		// ���݂̃t�F�[�Y���I��
		m_pPhase->Uninit();
		delete m_pPhase;
		m_pPhase = nullptr;
		
		// ���̃t�F�[�Y�̐���
		switch(m_Phase)
		{
			case PHASE_GROUPLOGO:
			{
				m_pPhase = new CGroupLogo();
			}

			case PHASE_TITLE:
			{
				m_pPhase = new CTitle();
				break;
			}

			case PHASE_GAME:
			{
				m_pPhase = new CGame();
				break;
			}

			case PHASE_RESULT:
			{
				m_pPhase = new CResult();
				break;
			}

			default:
			{
				break;
			}
		}

		// ���ʃI�u�W�F�N�g��������
		m_pCamera->ResetCamera();

		// ���̃t�F�[�Y��������
		m_pPhase->Init();
	}
	
	// �t�F�[�Y��Ԃ𓯊�
	m_OldPhase = m_Phase;
}
