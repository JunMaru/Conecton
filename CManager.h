/*-----------------------------------------------------------------------------
	アプリケーション管理クラス
	author	OkadaMoeto
	since	20140507
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"
#include "CPhase.h"

/*-----------------------------------------------------------------------------
	メモリリーク行出力用
-----------------------------------------------------------------------------*/
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif
#endif

/*-----------------------------------------------------------------------------
	マネージャクラス
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
		// フェーズ種類
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
			レンダラー取得
			return	CRenderer*		レンダラーへのポインタ
		-----------------------------------------------------------------------------*/
		static CRenderer* GetRenderer(void){ return m_pRenderer; }

		/*-----------------------------------------------------------------------------
			キーボード取得
			return	CInputKeyboard*		キーボードへのポインタ
		-----------------------------------------------------------------------------*/
		static CInputKeyboard* GetInputKeyboard(void){ return m_pInputKeyboard; }

		/*-----------------------------------------------------------------------------
			マウス取得
			return	CInputMouse*		マウスへのポインタ
		-----------------------------------------------------------------------------*/
		static CInputMouse* GetInputMouse(void){ return m_pInputMouse; }

		/*-----------------------------------------------------------------------------
			ジョイパッド取得
			return	CInputJoypad*		ジョイパッドへのポインタ
		-----------------------------------------------------------------------------*/
		static CInputJoypad* GetInputJoypad(void){ return m_pInputJoypad; }

		/*-----------------------------------------------------------------------------
			カメラ取得
			return	CCamera*		カメラへのポインタ
		-----------------------------------------------------------------------------*/
		static CCamera* GetCamera(void){ return m_pCamera; }

		/*-----------------------------------------------------------------------------
			ライト取得
			return	CLight*		ライトへのポインタ
		-----------------------------------------------------------------------------*/
		static CLight* GetLight(void){ return m_pLight; }

		/*-----------------------------------------------------------------------------
			サウンド取得
			return	CSoundXAudio2*		サウンドへのポインタ
		-----------------------------------------------------------------------------*/
		static CSoundXAudio2* GetSoundXAudio2(void){ return m_pSoundXAudio2; }

		/*-----------------------------------------------------------------------------
			画面遷移用のフェード取得
			return	CFade*		画面遷移用のフェードへのポインタ
		-----------------------------------------------------------------------------*/
		static CFade* GetPhaseFade(void){ return m_pPhaseFade; }

		/*-----------------------------------------------------------------------------
			現在のフェーズ取得
			return	PHASE		現在のフェーズの値
		-----------------------------------------------------------------------------*/
		static PHASE GetPhase(void){ return m_Phase; }
		/*-----------------------------------------------------------------------------
			現在のフェーズ設定
			param	phase		現在のフェーズの設定値
		-----------------------------------------------------------------------------*/
		static void SetPhase(PHASE phase){ m_Phase = phase; }

		// フェーズ移行
		void ChangePhase(void);

	private:
		// 前回フェーズ
		static PHASE m_OldPhase;
		// 次フェーズ
		static PHASE m_Phase;
		// 現在フェーズ
		CPhase* m_pPhase;

		// レンダラーへのポインタ
		static CRenderer* m_pRenderer;
		// キーボード入力へのポインタ
		static CInputKeyboard* m_pInputKeyboard;
		// マウス入力へのポインタ
		static CInputMouse* m_pInputMouse;
		// ジョイパッド入力へのポインタ
		static CInputJoypad* m_pInputJoypad;
		// カメラへのポインタ
		static CCamera* m_pCamera;
		// ライトへのポインタ
		static CLight* m_pLight;
		// サウンドへのポインタ
		static CSoundXAudio2* m_pSoundXAudio2;
		// フェーズ遷移時にはさむフェードへのポインタ
		static CFade* m_pPhaseFade;

#ifdef _DEBUG
		// デバッグ表示へのポインタ
		static CDebugProcDX9* m_pDebugProcDX9;
#endif
};
