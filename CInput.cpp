/*-----------------------------------------------------------------------------
入力インターフェースクラス
author	OkadaMoeto
since	20140521
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
インクルードファイル
-----------------------------------------------------------------------------*/
#include "CInput.h"
#include "Define.h"

/*-----------------------------------------------------------------------------
マクロ定義
-----------------------------------------------------------------------------*/
// スティックの操作を受け付けない範囲
static const float LSTICK_DEAD_ZONE = 500.0f;
static const float RSTICK_DEAD_ZONE = 500.0f;

// リピートキー間隔
static const int REPEAT_FRAME = 20;

/*-----------------------------------------------------------------------------
静的メンバ変数の初期化
-----------------------------------------------------------------------------*/
// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUT8 CInput::m_pDInput = nullptr;

/*-----------------------------------------------------------------------------
コンストラクタ
-----------------------------------------------------------------------------*/
CInput::CInput()
{
	m_pDIDevice = nullptr;
}

/*-----------------------------------------------------------------------------
デストラクタ
-----------------------------------------------------------------------------*/
CInput::~CInput()
{
}

/*-----------------------------------------------------------------------------
入力インターフェースの初期化
-----------------------------------------------------------------------------*/
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	// DirectInputオブジェクトの作成
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
入力インターフェースの終了
-----------------------------------------------------------------------------*/
void CInput::Uninit(void)
{
	// デバイスオブジェクト解放
	if (m_pDIDevice)
	{
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = nullptr;
	}

	// DirectInputオブジェクト解放
	if (m_pDInput)
	{
		m_pDInput->Release();
		m_pDInput = nullptr;
	}
}

/*-----------------------------------------------------------------------------
コンストラクタ
-----------------------------------------------------------------------------*/
CInputKeyboard::CInputKeyboard()
{
	// ワークのクリア
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

/*-----------------------------------------------------------------------------
デストラクタ
-----------------------------------------------------------------------------*/
CInputKeyboard::~CInputKeyboard()
{
}

/*-----------------------------------------------------------------------------
キーボード入力の初期化
-----------------------------------------------------------------------------*/
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init(hInstance, hWnd);

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, nullptr);
	if (FAILED(hr) || m_pDIDevice == nullptr)
	{
		MessageBox(hWnd, "キーボードがありません", "WARNING", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "WARNING", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "WARNING", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDeviceアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}


/*-----------------------------------------------------------------------------
キーボード入力の終了
-----------------------------------------------------------------------------*/
void CInputKeyboard::Uninit(void)
{
	// 入力処理の開放
	CInput::Uninit();
}

/*-----------------------------------------------------------------------------
キーボード入力の更新
-----------------------------------------------------------------------------*/
HRESULT CInputKeyboard::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[256];

	if (!m_pDIDevice)
	{
		return S_FALSE;
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);
	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < 256; nCntKey++)
		{
			// 押されたキーを取得
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			// 離されたキーを取得
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if (aKeyState[nCntKey])
			{
				// 押し続けているキーを監視
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

			// 押しているキーを取得
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
キーボードデータ取得(押しているキーの取得)
-----------------------------------------------------------------------------*/
BOOL CInputKeyboard::GetKeyPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
キーボードデータ取得(押した瞬間のキー取得)
-----------------------------------------------------------------------------*/
BOOL CInputKeyboard::GetKeyTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
キーボードデータ取得(離した瞬間のキー取得)
-----------------------------------------------------------------------------*/
BOOL CInputKeyboard::GetKeyRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
キーボードデータ取得(押した瞬間と押し続けているキーの取得)
-----------------------------------------------------------------------------*/
BOOL CInputKeyboard::GetKeyRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
キーボードデータ削除(押した瞬間のキーの内容削除)
-----------------------------------------------------------------------------*/
void CInputKeyboard::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[nKey] = 0;
}

/*-----------------------------------------------------------------------------
コンストラクタ
-----------------------------------------------------------------------------*/
CInputMouse::CInputMouse()
{
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));
	ZeroMemory(&m_mouseStateTrigger, sizeof(m_mouseStateTrigger));
}

/*-----------------------------------------------------------------------------
デストラクタ
-----------------------------------------------------------------------------*/
CInputMouse::~CInputMouse()
{
}

/*-----------------------------------------------------------------------------
マウス入力の初期化
-----------------------------------------------------------------------------*/
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init(hInstance, hWnd);

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, nullptr);
	if (FAILED(hr) || m_pDIDevice == nullptr)
	{
		MessageBox(hWnd, "マウスがありません", "警告", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告", MB_ICONWARNING);
		return hr;
	}

	// デバイスの設定
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = m_pDIDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}

/*-----------------------------------------------------------------------------
マウス入力の終了
-----------------------------------------------------------------------------*/
void CInputMouse::Uninit(void)
{
	// 入力処理の開放
	CInput::Uninit();
}

/*-----------------------------------------------------------------------------
マウス入力の更新
-----------------------------------------------------------------------------*/
HRESULT CInputMouse::Update(void)
{
	HRESULT hr;
	DIMOUSESTATE2 mouseState;

	if (!m_pDIDevice)
	{
		return S_FALSE;
	}

	// デバイスからデータを取得
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

		// スクリーン座標を取得
		GetCursorPos(&m_posMouseWorld);
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

/*-----------------------------------------------------------------------------
マウスデータ取得(左クリックしている)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetLeftPress(void)
{
	return (m_mouseState.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
マウスデータ取得(左クリックした)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetLeftTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
マウスデータ取得(右クリックしている)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetRightPress(void)
{
	return (m_mouseState.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
マウスデータ取得(右クリックした)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetRightTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
マウスデータ取得(中クリックしている)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetCenterPress(void)
{
	return (m_mouseState.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
マウスデータ取得(中クリックした)
-----------------------------------------------------------------------------*/
BOOL CInputMouse::GetCenterTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

/*-----------------------------------------------------------------------------
マウスデータ取得(Ｘ軸移動)
-----------------------------------------------------------------------------*/
LONG CInputMouse::GetAxisX(void)
{
	return m_mouseState.lX;
}

/*-----------------------------------------------------------------------------
マウスデータ取得(Y軸移動)
-----------------------------------------------------------------------------*/
LONG CInputMouse::GetAxisY(void)
{
	return m_mouseState.lY;
}

/*-----------------------------------------------------------------------------
マウスデータ取得(Z軸移動)
-----------------------------------------------------------------------------*/
LONG CInputMouse::GetAxisZ(void)
{
	return m_mouseState.lZ;
}

/*-----------------------------------------------------------------------------
コンストラクタ
-----------------------------------------------------------------------------*/
CInputJoypad::CInputJoypad()
{
}

/*-----------------------------------------------------------------------------
デストラクタ
-----------------------------------------------------------------------------*/
CInputJoypad::~CInputJoypad()
{
}

/*-----------------------------------------------------------------------------
ジョイパッド入力の初期化
-----------------------------------------------------------------------------*/
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init(hInstance, hWnd);

	hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, CInputJoypad::EnumJoyCallback, this, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr))
	{
		return hr;
	}
	else if (m_pDIDevice == nullptr)
	{
		//MessageBox(hWnd, "ジョイパッドがありません", "警告", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ジョイパッドのデータフォーマットを設定できませんでした。", "警告", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ジョイパッドの協調モードを設定できませんでした。", "警告", MB_ICONWARNING);
		return hr;
	}

	hr = m_pDIDevice->EnumObjects(CInputJoypad::EnumAxesCallback, this, DIDFT_AXIS);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ジョイパッドの軸設定を設定できませんでした。", "警告", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}

/*-----------------------------------------------------------------------------
ジョイスティック問い合わせ用コールバック関数
-----------------------------------------------------------------------------*/
BOOL CALLBACK CInputJoypad::EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	// デバイス情報
	DIDEVCAPS	diDevCaps;
	HRESULT		hRes;
	CInputJoypad *pThis = reinterpret_cast<CInputJoypad*>(pvRef);

	// ジョイスティック用デバイスオブジェクトを作成
	hRes = pThis->m_pDInput->CreateDevice(lpddi->guidInstance, &pThis->m_pDIDevice, nullptr);
	if (FAILED(hRes))
	{
		// 列挙を続ける
		return DIENUM_CONTINUE;
	}

	// ジョイスティックの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = pThis->m_pDIDevice->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		SAFE_RELEASE(pThis->m_pDIDevice);
		// 列挙を続ける
		return DIENUM_CONTINUE;
	}

	// このデバイスを使うので列挙を終了する
	return DIENUM_STOP;
}

/*-----------------------------------------------------------------------------
ジョイスティック問い合わせ用コールバック関数
-----------------------------------------------------------------------------*/
BOOL CALLBACK CInputJoypad::EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	HRESULT hr;
	CInputJoypad *pThis = reinterpret_cast<CInputJoypad*>(pvRef);

	// 軸の値の範囲を設定（-1000～1000）
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
ジョイパッドの終了
-----------------------------------------------------------------------------*/
void CInputJoypad::Uninit(void)
{
	// 入力処理の開放
	CInput::Uninit();
}

/*-----------------------------------------------------------------------------
ジョイパッドの更新
-----------------------------------------------------------------------------*/
HRESULT CInputJoypad::Update(void)
{
	HRESULT hr;
	bool aKeyStateOld[GAMEPAD_MAX];

	if (!m_pDIDevice)
	{
		return S_FALSE;
	}

	// 前回のデータを保存
	for (int nCntKey = 0; nCntKey < GAMEPAD_MAX; nCntKey++)
	{
		aKeyStateOld[nCntKey] = m_aKeyState[nCntKey];
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(m_joypadState), &m_joypadState);
	if (SUCCEEDED(hr))
	{
		// キー情報設定
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
ジョイパッドのキー情報設定
-----------------------------------------------------------------------------*/
void CInputJoypad::SetKeyState(void)
{
	// 左スティック
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

	// 右スティック
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
ジョイパッド情報取得(押している)
-----------------------------------------------------------------------------*/
bool CInputJoypad::GetKeyPress(GAMEPAD key)
{
	return m_aKeyState[key];
}

/*-----------------------------------------------------------------------------
ジョイパッド情報取得(押した)
-----------------------------------------------------------------------------*/
bool CInputJoypad::GetKeyTrigger(GAMEPAD key)
{
	return m_aKeyStateTrigger[key];
}

/*-----------------------------------------------------------------------------
ジョイパッド情報取得(離した)
-----------------------------------------------------------------------------*/
bool CInputJoypad::GetKeyRelease(GAMEPAD key)
{
	return m_aKeyStateRelease[key];
}

/*-----------------------------------------------------------------------------
ジョイパッド情報取得(押し続けている)
return	:BOOL	入力結果
retval	:TRUE	押されている
retval	:FALSE	押されていない
param	:nKey	キー種類(GAMEPAD_xx)
-----------------------------------------------------------------------------*/
bool CInputJoypad::GetKeyRepeat(GAMEPAD key)
{
	return m_aKeyStateRepeat[key];
}
