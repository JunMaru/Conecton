/*-----------------------------------------------------------------------------
入力インターフェースクラス
author	OkadaMoeto
since	20140521
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
入力インターフェースクラス
-----------------------------------------------------------------------------*/
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual HRESULT Update(void) = 0;

protected:
	// IDirectInputDevice8インターフェースへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDIDevice;
	// IDirectInput8インターフェースへのポインタ
	static LPDIRECTINPUT8 m_pDInput;
};

/*-----------------------------------------------------------------------------
キーボード入力クラス
-----------------------------------------------------------------------------*/
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	/*-----------------------------------------------------------------------------
	キーボードデータ取得(押しているキーの取得)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	param	nKey		キー種類(DIK_xx)
	-----------------------------------------------------------------------------*/
	BOOL GetKeyPress(int nKey);

	/*-----------------------------------------------------------------------------
	キーボードデータ取得(押した瞬間のキー取得)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	param	nKey		キー種類(DIK_xx)
	-----------------------------------------------------------------------------*/
	BOOL GetKeyTrigger(int nKey);

	/*-----------------------------------------------------------------------------
	キーボードデータ取得(離した瞬間のキー取得)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	param	nKey		キー種類(DIK_xx)
	-----------------------------------------------------------------------------*/
	BOOL GetKeyRelease(int nKey);

	/*-----------------------------------------------------------------------------
	キーボードデータ取得(押した瞬間と押し続けているキーの取得)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	param	nKey		キー種類(DIK_xx)
	-----------------------------------------------------------------------------*/
	BOOL GetKeyRepeat(int nKey);

	/*-----------------------------------------------------------------------------
	キーボードデータ削除(押した瞬間のキーの内容削除)
	param	nKey		キー種類(DIK_xx)
	-----------------------------------------------------------------------------*/
	void FlushKeyTrigger(int nKey);

private:
	// キーボードの状態を受け取るワーク
	BYTE m_aKeyState[256];
	// トリガーワーク
	BYTE m_aKeyStateTrigger[256];
	// リリースワーク
	BYTE m_aKeyStateRelease[256];
	// リピートワーク
	BYTE m_aKeyStateRepeat[256];
	// リピートカウンタ
	int m_aKeyStateRepeatCnt[256];
};

/*-----------------------------------------------------------------------------
マウス入力クラス
-----------------------------------------------------------------------------*/
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	/*-----------------------------------------------------------------------------
	マウスデータ取得(左クリックしている)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	-----------------------------------------------------------------------------*/
	BOOL GetLeftPress(void);

	/*-----------------------------------------------------------------------------
	マウスデータ取得(左クリックした)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	-----------------------------------------------------------------------------*/
	BOOL GetLeftTrigger(void);

	/*-----------------------------------------------------------------------------
	マウスデータ取得(右クリックしている)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	-----------------------------------------------------------------------------*/
	BOOL GetRightPress(void);

	/*-----------------------------------------------------------------------------
	マウスデータ取得(右クリックした)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	-----------------------------------------------------------------------------*/
	BOOL GetRightTrigger(void);

	/*-----------------------------------------------------------------------------
	マウスデータ取得(中クリックしている)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	-----------------------------------------------------------------------------*/
	BOOL GetCenterPress(void);

	/*-----------------------------------------------------------------------------
	マウスデータ取得(中クリックした)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	-----------------------------------------------------------------------------*/
	BOOL GetCenterTrigger(void);

	/*-----------------------------------------------------------------------------
	マウスデータ取得(Ｘ軸移動)
	return	LONG		X軸移動量
	-----------------------------------------------------------------------------*/
	LONG GetAxisX(void);

	/*-----------------------------------------------------------------------------
	マウスデータ取得(Y軸移動)
	return	LONG		Y軸移動量
	-----------------------------------------------------------------------------*/
	LONG GetAxisY(void);

	/*-----------------------------------------------------------------------------
	マウスデータ取得(Z軸移動)
	return	LONG		Z軸移動量
	-----------------------------------------------------------------------------*/
	LONG GetAxisZ(void);

	/*-----------------------------------------------------------------------------
	マウスのスクリーン座標取得
	return POINT*		マウスのスクリーン座標の値
	-----------------------------------------------------------------------------*/
	POINT* GetPosWorld(void){ return &m_posMouseWorld; }

private:
	// マウスの状態を受け取るワーク
	DIMOUSESTATE2 m_mouseState;
	// トリガーワーク
	DIMOUSESTATE2 m_mouseStateTrigger;
	// マウスのスクリーン座標
	POINT m_posMouseWorld;
};

/*-----------------------------------------------------------------------------
ジョイパッド入力クラス
-----------------------------------------------------------------------------*/
class CInputJoypad : public CInput
{
public:
	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	// ゲームパッド版
	typedef enum
	{
		// ゲームパッド
		GAMEPAD_LSTICK_LEFT = 0,
		GAMEPAD_LSTICK_RIGHT,
		GAMEPAD_LSTICK_UP,
		GAMEPAD_LSTICK_DOWN,
		GAMEPAD_RSTICK_LEFT,
		GAMEPAD_RSTICK_RIGHT,
		GAMEPAD_RSTICK_UP,
		GAMEPAD_RSTICK_DOWN,
		GAMEPAD_1,
		GAMEPAD_2,
		GAMEPAD_3,
		GAMEPAD_4,
		GAMEPAD_5,
		GAMEPAD_6,
		GAMEPAD_7,
		GAMEPAD_8,
		GAMEPAD_9,
		GAMEPAD_10,
		GAMEPAD_11,
		GAMEPAD_12,
		GAMEPAD_MAX
	}GAMEPAD;

	/*-----------------------------------------------------------------------------
	ジョイパッド情報取得(押している)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	param	nKey		キー種類(GAMEPAD_xx)
	-----------------------------------------------------------------------------*/
	bool GetKeyPress(GAMEPAD key);

	/*-----------------------------------------------------------------------------
	ジョイパッド情報取得(押した)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	param	nKey		キー種類(GAMEPAD_xx)

	-----------------------------------------------------------------------------*/
	bool GetKeyTrigger(GAMEPAD key);

	/*-----------------------------------------------------------------------------
	ジョイパッド情報取得(離した)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	param	nKey		キー種類(GAMEPAD_xx)
	-----------------------------------------------------------------------------*/
	bool GetKeyRelease(GAMEPAD key);

	/*-----------------------------------------------------------------------------
	ジョイパッド情報取得(押し続けている)
	return	BOOL		入力結果
	retval	TRUE		押されている
	retval	FALSE		押されていない
	param	nKey		キー種類(GAMEPAD_xx)
	-----------------------------------------------------------------------------*/
	bool GetKeyRepeat(GAMEPAD key);

private:
	// コールバック関数
	static BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);
	static BOOL CALLBACK EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	// ステート処理
	void SetKeyState(void);

	// ジョイパッド状態を受け取るワーク
	DIJOYSTATE2 m_joypadState;
	// プレスワーク
	bool m_aKeyState[GAMEPAD_MAX];
	// トリガーワーク
	bool m_aKeyStateTrigger[GAMEPAD_MAX];
	// リリースワーク
	bool m_aKeyStateRelease[GAMEPAD_MAX];
	// リピートワーク
	bool m_aKeyStateRepeat[GAMEPAD_MAX];
	// リピートカウンタ
	int m_aKeyStateRepeatCnt[GAMEPAD_MAX];
};
