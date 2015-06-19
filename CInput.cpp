/*-----------------------------------------------------------------------------
���̓C���^�[�t�F�[�X�N���X
author	OkadaMoeto
since	20140521
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CInput.h"
#include "Define.h"

/*-----------------------------------------------------------------------------
�}�N����`
-----------------------------------------------------------------------------*/
// �X�e�B�b�N�̑�����󂯕t���Ȃ��͈�
static const float LSTICK_DEAD_ZONE = 500.0f;
static const float RSTICK_DEAD_ZONE = 500.0f;

// ���s�[�g�L�[�Ԋu
static const int REPEAT_FRAME = 20;

/*-----------------------------------------------------------------------------
�ÓI�����o�ϐ��̏�����
-----------------------------------------------------------------------------*/
// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUT8 CInput::m_pDInput = nullptr;

/*-----------------------------------------------------------------------------
�R���X�g���N�^
-----------------------------------------------------------------------------*/
CInput::CInput()
{
	m_pDIDevice = nullptr;
}

/*-----------------------------------------------------------------------------
�f�X�g���N�^
-----------------------------------------------------------------------------*/
CInput::~CInput()
{
}

/*-----------------------------------------------------------------------------
���̓C���^�[�t�F�[�X�̏�����
-----------------------------------------------------------------------------*/
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	// DirectInput�I�u�W�F�N�g�̍쐬
	if (!m_pDInput)
	{
		hr = DirectInput8Create(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&m_pDInput,
			nullptr);
	}

	return hr;
}

/*-----------------------------------------------------------------------------
���̓C���^�[�t�F�[�X�̏I��
-----------------------------------------------------------------------------*/
void CInput::Uninit(void)
{
	// �f�o�C�X�I�u�W�F�N�g���
	if (m_pDIDevice)
	{
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = nullptr;
	}

	// DirectInput�I�u�W�F�N�g���
	if (m_pDInput)
	{
		m_pDInput->Release();
		m_pDInput = nullptr;
	}
}

/*-----------------------------------------------------------------------------
�R���X�g���N�^
-----------------------------------------------------------------------------*/
CInputKeyboard::CInputKeyboard()
{
	// ���[�N�̃N���A
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

/*-----------------------------------------------------------------------------
�f�X�g���N�^
-----------------------------------------------------------------------------*/
CInputKeyboard::~CInputKeyboard()
{
}

/*-----------------------------------------------------------------------------
�L�[�{�[�h���͂̏�����
-----------------------------------------------------------------------------*/
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	CInput::Init(hInstance, hWnd);

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, nullptr);
	if (FAILED(hr) || m_pDIDevice == nullptr)
	{
		MessageBox(hWnd, "�L�[�{�[�h������܂���", "WARNING", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "WARNING", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "WARNING", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDevice�A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	return hr;
}


/*-----------------------------------------------------------------------------
�L�[�{�[�h���͂̏I��
-----------------------------------------------------------------------------*/
void CInputKeyboard::Uninit(void)
{
	// ���͏����̊J��
	CInput::Uninit();
}

/*-----------------------------------------------------------------------------
�L�[�{�[�h���͂̍X�V
-----------------------------------------------------------------------------*/
HRESULT CInputKeyboard::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[256];

	if (!m_pDIDevice)
	{
		return S_FALSE;
	}

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);
	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < 256; nCntKey++)
		{
			// �����ꂽ�L�[���擾
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			// �����ꂽ�L�[���擾
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if (aKeyState[nCntKey])
			{
				// ���������Ă���L�[���Ď�
				m_aKeyStateRepeatCnt[nCntKey]++;
				if (m_aKeyStateRepeatCnt[nCntKey] >= REPEAT_FRAME)
				{
					m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;
				m_aKeyStateRepeat[nCntKey] = 0;
			}

			// �����Ă���L�[���擾
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

/*-----------------------------------------------------------------------------
�L�[�{�[�h�f�[�^�擾(�����Ă���L�[�̎擾)
-----------------------------------------------------------------------------*/
BOOL CInputKeyboard::GetKeyPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�L�[�{�[�h�f�[�^�擾(�������u�Ԃ̃L�[�擾)
-----------------------------------------------------------------------------*/
BOOL CInputKeyboard::GetKeyTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�L�[�{�[�h�f�[�^�擾(�������u�Ԃ̃L�[�擾)
-----------------------------------------------------------------------------*/
BOOL CInputKeyboard::GetKeyRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�L�[�{�[�h�f�[�^�擾(�������u�ԂƉ��������Ă���L�[�̎擾)
-----------------------------------------------------------------------------*/
BOOL CInputKeyboard::GetKeyRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�L�[�{�[�h�f�[�^�폜(�������u�Ԃ̃L�[�̓��e�폜)
-----------------------------------------------------------------------------*/
void CInputKeyboard::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[nKey] = 0;
}

/*-----------------------------------------------------------------------------
�R���X�g���N�^
-----------------------------------------------------------------------------*/
CInputMouse::CInputMouse()
{
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));
	ZeroMemory(&m_mouseStateTrigger, sizeof(m_mouseStateTrigger));
}

/*-----------------------------------------------------------------------------
�f�X�g���N�^
-----------------------------------------------------------------------------*/
CInputMouse::~CInputMouse()
{
}

/*-----------------------------------------------------------------------------
�}�E�X���͂̏�����
-----------------------------------------------------------------------------*/
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	CInput::Init(hInstance, hWnd);

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, nullptr);
	if (FAILED(hr) || m_pDIDevice == nullptr)
	{
		MessageBox(hWnd, "�}�E�X������܂���", "�x��", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�̐ݒ�
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = m_pDIDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	return hr;
}

/*-----------------------------------------------------------------------------
�}�E�X���͂̏I��
-----------------------------------------------------------------------------*/
void CInputMouse::Uninit(void)
{
	// ���͏����̊J��
	CInput::Uninit();
}

/*-----------------------------------------------------------------------------
�}�E�X���͂̍X�V
-----------------------------------------------------------------------------*/
HRESULT CInputMouse::Update(void)
{
	HRESULT hr;
	DIMOUSESTATE2 mouseState;

	if (!m_pDIDevice)
	{
		return S_FALSE;
	}

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevice->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(hr))
	{
		m_mouseStateTrigger.lX = ((m_mouseState.lX ^ mouseState.lX) & mouseState.lX);
		m_mouseStateTrigger.lY = ((m_mouseState.lY ^ mouseState.lY) & mouseState.lY);
		m_mouseStateTrigger.lZ = ((m_mouseState.lZ ^ mouseState.lZ) & mouseState.lZ);
		for (int nCntKey = 0; nCntKey < 8; nCntKey++)
		{
			m_mouseStateTrigger.rgbButtons[nCntKey] = ((m_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey]) & mouseState.rgbButtons[nCntKey]);
		}

		m_mouseState = mouseState;

		// �X�N���[�����W���擾
		GetCursorPos(&m_posMouseWorld);
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

/*-----------------------------------------------------------------------------
�}�E�X�f�[�^�擾(���N���b�N���Ă���)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetLeftPress(void)
{
	return (m_mouseState.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�}�E�X�f�[�^�擾(���N���b�N����)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetLeftTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�}�E�X�f�[�^�擾(�E�N���b�N���Ă���)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetRightPress(void)
{
	return (m_mouseState.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�}�E�X�f�[�^�擾(�E�N���b�N����)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetRightTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�}�E�X�f�[�^�擾(���N���b�N���Ă���)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetCenterPress(void)
{
	return (m_mouseState.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�}�E�X�f�[�^�擾(���N���b�N����)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetCenterTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
�}�E�X�f�[�^�擾(�w���ړ�)
-----------------------------------------------------------------------------*/
LONG CInputMouse::GetAxisX(void)
{
	return m_mouseState.lX;
}

/*-----------------------------------------------------------------------------
�}�E�X�f�[�^�擾(Y���ړ�)
-----------------------------------------------------------------------------*/
LONG CInputMouse::GetAxisY(void)
{
	return m_mouseState.lY;
}

/*-----------------------------------------------------------------------------
�}�E�X�f�[�^�擾(Z���ړ�)
-----------------------------------------------------------------------------*/
LONG CInputMouse::GetAxisZ(void)
{
	return m_mouseState.lZ;
}

/*-----------------------------------------------------------------------------
�R���X�g���N�^
-----------------------------------------------------------------------------*/
CInputJoypad::CInputJoypad()
{
}

/*-----------------------------------------------------------------------------
�f�X�g���N�^
-----------------------------------------------------------------------------*/
CInputJoypad::~CInputJoypad()
{
}

/*-----------------------------------------------------------------------------
�W���C�p�b�h���͂̏�����
-----------------------------------------------------------------------------*/
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	CInput::Init(hInstance, hWnd);

	hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, CInputJoypad::EnumJoyCallback, this, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr))
	{
		return hr;
	}
	else if (m_pDIDevice == nullptr)
	{
		//MessageBox(hWnd, "�W���C�p�b�h������܂���", "�x��", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�W���C�p�b�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�W���C�p�b�h�̋������[�h��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return hr;
	}

	hr = m_pDIDevice->EnumObjects(CInputJoypad::EnumAxesCallback, this, DIDFT_AXIS);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�W���C�p�b�h�̎��ݒ��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	return hr;
}

/*-----------------------------------------------------------------------------
�W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
-----------------------------------------------------------------------------*/
BOOL CALLBACK CInputJoypad::EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	// �f�o�C�X���
	DIDEVCAPS	diDevCaps;
	HRESULT		hRes;
	CInputJoypad *pThis = reinterpret_cast<CInputJoypad*>(pvRef);

	// �W���C�X�e�B�b�N�p�f�o�C�X�I�u�W�F�N�g���쐬
	hRes = pThis->m_pDInput->CreateDevice(lpddi->guidInstance, &pThis->m_pDIDevice, nullptr);
	if (FAILED(hRes))
	{
		// �񋓂𑱂���
		return DIENUM_CONTINUE;
	}

	// �W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = pThis->m_pDIDevice->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		SAFE_RELEASE(pThis->m_pDIDevice);
		// �񋓂𑱂���
		return DIENUM_CONTINUE;
	}

	// ���̃f�o�C�X���g���̂ŗ񋓂��I������
	return DIENUM_STOP;
}

/*-----------------------------------------------------------------------------
�W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
-----------------------------------------------------------------------------*/
BOOL CALLBACK CInputJoypad::EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	HRESULT hr;
	CInputJoypad *pThis = reinterpret_cast<CInputJoypad*>(pvRef);

	// ���̒l�͈̔͂�ݒ�i-1000�`1000�j
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -1000;
	diprg.lMax = +1000;
	hr = pThis->m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

/*-----------------------------------------------------------------------------
�W���C�p�b�h�̏I��
-----------------------------------------------------------------------------*/
void CInputJoypad::Uninit(void)
{
	// ���͏����̊J��
	CInput::Uninit();
}

/*-----------------------------------------------------------------------------
�W���C�p�b�h�̍X�V
-----------------------------------------------------------------------------*/
HRESULT CInputJoypad::Update(void)
{
	HRESULT hr;
	bool aKeyStateOld[GAMEPAD_MAX];

	if (!m_pDIDevice)
	{
		return S_FALSE;
	}

	// �O��̃f�[�^��ۑ�
	for (int nCntKey = 0; nCntKey < GAMEPAD_MAX; nCntKey++)
	{
		aKeyStateOld[nCntKey] = m_aKeyState[nCntKey];
	}

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevice->GetDeviceState(sizeof(m_joypadState), &m_joypadState);
	if (SUCCEEDED(hr))
	{
		// �L�[���ݒ�
		SetKeyState();

		for (int nCntKey = 0; nCntKey < GAMEPAD_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyState[nCntKey]) & m_aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyState[nCntKey]) & !m_aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if (m_aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;
				if (m_aKeyStateRepeatCnt[nCntKey] >= REPEAT_FRAME)
				{
					m_aKeyStateRepeat[nCntKey] = m_aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;
				m_aKeyStateRepeat[nCntKey] = 0;
			}
		}
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

/*-----------------------------------------------------------------------------
�W���C�p�b�h�̃L�[���ݒ�
-----------------------------------------------------------------------------*/
void CInputJoypad::SetKeyState(void)
{
	// ���X�e�B�b�N
	if (m_joypadState.lX < -LSTICK_DEAD_ZONE)
	{
		m_aKeyState[GAMEPAD_LSTICK_LEFT] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_LSTICK_LEFT] = false;
	}

	if (m_joypadState.lX > LSTICK_DEAD_ZONE)
	{
		m_aKeyState[GAMEPAD_LSTICK_RIGHT] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_LSTICK_RIGHT] = false;
	}

	if (m_joypadState.lY < -LSTICK_DEAD_ZONE)
	{
		m_aKeyState[GAMEPAD_LSTICK_UP] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_LSTICK_UP] = false;
	}

	if (m_joypadState.lY > LSTICK_DEAD_ZONE)
	{
		m_aKeyState[GAMEPAD_LSTICK_DOWN] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_LSTICK_DOWN] = false;
	}

	// �E�X�e�B�b�N
	if (m_joypadState.lZ < -RSTICK_DEAD_ZONE)
	{
		m_aKeyState[GAMEPAD_RSTICK_LEFT] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_RSTICK_LEFT] = false;
	}

	if (m_joypadState.lZ > RSTICK_DEAD_ZONE)
	{
		m_aKeyState[GAMEPAD_RSTICK_RIGHT] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_RSTICK_RIGHT] = false;
	}

	if (m_joypadState.lRz < -RSTICK_DEAD_ZONE)
	{
		m_aKeyState[GAMEPAD_RSTICK_UP] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_RSTICK_UP] = false;
	}

	if (m_joypadState.lRz > RSTICK_DEAD_ZONE)
	{
		m_aKeyState[GAMEPAD_RSTICK_DOWN] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_RSTICK_DOWN] = false;
	}

	if (m_joypadState.rgbButtons[0])
	{
		m_aKeyState[GAMEPAD_1] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_1] = false;
	}

	if (m_joypadState.rgbButtons[1])
	{
		m_aKeyState[GAMEPAD_2] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_2] = false;
	}

	if (m_joypadState.rgbButtons[2])
	{
		m_aKeyState[GAMEPAD_3] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_3] = false;
	}

	if (m_joypadState.rgbButtons[3])
	{
		m_aKeyState[GAMEPAD_4] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_4] = false;
	}

	if (m_joypadState.rgbButtons[4])
	{
		m_aKeyState[GAMEPAD_5] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_5] = false;
	}

	if (m_joypadState.rgbButtons[5])
	{
		m_aKeyState[GAMEPAD_6] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_6] = false;
	}

	if (m_joypadState.rgbButtons[6])
	{
		m_aKeyState[GAMEPAD_7] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_7] = false;
	}

	if (m_joypadState.rgbButtons[7])
	{
		m_aKeyState[GAMEPAD_8] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_8] = false;
	}

	if (m_joypadState.rgbButtons[8])
	{
		m_aKeyState[GAMEPAD_9] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_9] = false;
	}

	if (m_joypadState.rgbButtons[9])
	{
		m_aKeyState[GAMEPAD_10] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_10] = false;
	}

	if (m_joypadState.rgbButtons[10])
	{
		m_aKeyState[GAMEPAD_11] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_11] = false;
	}

	if (m_joypadState.rgbButtons[11])
	{
		m_aKeyState[GAMEPAD_12] = true;
	}
	else
	{
		m_aKeyState[GAMEPAD_12] = false;
	}
}

/*-----------------------------------------------------------------------------
�W���C�p�b�h���擾(�����Ă���)
-----------------------------------------------------------------------------*/
bool CInputJoypad::GetKeyPress(GAMEPAD key)
{
	return m_aKeyState[key];
}

/*-----------------------------------------------------------------------------
�W���C�p�b�h���擾(������)
-----------------------------------------------------------------------------*/
bool CInputJoypad::GetKeyTrigger(GAMEPAD key)
{
	return m_aKeyStateTrigger[key];
}

/*-----------------------------------------------------------------------------
�W���C�p�b�h���擾(������)
-----------------------------------------------------------------------------*/
bool CInputJoypad::GetKeyRelease(GAMEPAD key)
{
	return m_aKeyStateRelease[key];
}

/*-----------------------------------------------------------------------------
�W���C�p�b�h���擾(���������Ă���)
return	:BOOL	���͌���
retval	:TRUE	������Ă���
retval	:FALSE	������Ă��Ȃ�
param	:nKey	�L�[���(GAMEPAD_xx)
-----------------------------------------------------------------------------*/
bool CInputJoypad::GetKeyRepeat(GAMEPAD key)
{
	return m_aKeyStateRepeat[key];
}
