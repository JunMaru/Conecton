/*-----------------------------------------------------------------------------
	アプリケーション管理クラス
	author	OkadaMoeto
	since	20140507
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
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
	静的メンバ変数の初期化
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
// フェーズの初期設定 その1
#ifdef _DEBUG
CManager::PHASE CManager::m_Phase = PHASE_TITLE;
CManager::PHASE CManager::m_OldPhase = PHASE_TITLE;
#else
CManager::PHASE CManager::m_Phase = PHASE_TITLE;
CManager::PHASE CManager::m_OldPhase = PHASE_TITLE;
#endif

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CManager::CManager()
{
	m_pPhase = nullptr;
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CManager::~CManager()
{
}

/*-----------------------------------------------------------------------------
	マネージャの初期化(各リソースなどの生成と初期化)
-----------------------------------------------------------------------------*/
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダラー生成
	m_pRenderer = new CRenderer();
	m_pRenderer->Init(hWnd, bWindow);

	// キーボード生成
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// マウス生成
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);

	// ジョイパッド生成
	m_pInputJoypad = new CInputJoypad;
	if(FAILED(m_pInputJoypad->Init(hInstance, hWnd)))
	{
		// ジョイパッドが無いときは解放したい
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}

	// カメラ生成
	m_pCamera = new CCamera();
	m_pCamera->Init();

	// ライト生成
	m_pLight = new CLight();
	m_pLight->Init();

	// サウンド生成
	m_pSoundXAudio2 = new CSoundXAudio2();
	m_pSoundXAudio2->Init(hWnd);

	// フェーズフェード生成
	m_pPhaseFade = CFade::Create(
									D3DXVECTOR3((float)(SCREEN_WIDTH / 2), (float)(SCREEN_HEIGHT/ 2), 0.0f),
									VEC3_ZERO
	);

#ifdef _DEBUG
	// デバッグ表示生成
	m_pDebugProcDX9 = new CDebugProcDX9;
	m_pDebugProcDX9->Init();
#endif

	// フェーズの初期設定 その2
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
	マネージャの終了(各リソースなどの解放)
-----------------------------------------------------------------------------*/
void CManager::Uninit(void)
{
	// フェーズの解放
	if(m_pPhase)
	{
		m_pPhase->Uninit();
		delete m_pPhase;
		m_pPhase = nullptr;
	}

	// レンダリング解放
	if(m_pRenderer)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// キーボード解放
	if(m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	// マウス解放
	if(m_pInputMouse)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	// ジョイパッド解放
	if(m_pInputJoypad)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}
	
	// カメラ解放
	if(m_pCamera)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// ライト解放
	if(m_pLight)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	// サウンド解放
	if(m_pSoundXAudio2)
	{
		m_pSoundXAudio2->Uninit();
		delete m_pSoundXAudio2;
		m_pSoundXAudio2 = nullptr;
	}

	// フェーズフェード解放
	if(m_pPhaseFade)
	{
		m_pPhaseFade->Uninit();
		delete m_pPhaseFade;
		m_pPhaseFade = nullptr;
	}

#ifdef _DEBUG
	// デバッグ表示解放
	if(m_pDebugProcDX9)
	{
		m_pDebugProcDX9->Uninit();
		delete m_pDebugProcDX9;
		m_pDebugProcDX9 = nullptr;
	}
#endif
}

/*-----------------------------------------------------------------------------
	マネージャの更新
-----------------------------------------------------------------------------*/
void CManager::Update(void)
{
#ifdef _DEBUG
	// デバッグ表示
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

	// ワイヤーフレーム表示入力
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

	// スクリーンショット入力
	if(m_pInputKeyboard->GetKeyTrigger(DIK_F3))
	{
		CaptureImageFromBackBuffer(D3DXIFF_BMP);
	}
	if(m_pInputKeyboard->GetKeyTrigger(DIK_F4))
	{
		CaptureImageFromBackBuffer(D3DXIFF_JPG);
	}
#endif

	// 共通オブジェクトの更新
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

	// フェーズの更新
	if(m_pPhase)
	{
		m_pPhase->Update();
	}

	// フェーズの移行
	ChangePhase();

	// オブジェクトの全更新
	if(m_pRenderer)
	{
		m_pRenderer->Update();
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CManager.cpp]\n");
	CDebugProcDX9::Print("[F1]:デバッグ表示\n");
	CDebugProcDX9::Print("[F2]:ワイヤーフレーム描画\n");
	CDebugProcDX9::Print("[F3]:スクリーンショット(BMP)<保存場所>data/screenshot\n");
	CDebugProcDX9::Print("[F4]:スクリーンショット(JPG)<保存場所>data/screenshot\n");
#endif
}

/*-----------------------------------------------------------------------------
	マネージャの描画
-----------------------------------------------------------------------------*/
void CManager::Draw(void)
{
	// オブジェクトの全描画
	if(m_pRenderer){ m_pRenderer->Draw(); }
}

/*-----------------------------------------------------------------------------
	フェーズ移行
-----------------------------------------------------------------------------*/
void CManager::ChangePhase(void)
{
	// フェーズが異なっていたら、次のフェーズへ移行
	if( m_Phase != m_OldPhase )
	{
		// 現在のフェーズを終了
		m_pPhase->Uninit();
		delete m_pPhase;
		m_pPhase = nullptr;
		
		// 次のフェーズの生成
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

		// 共通オブジェクトを初期化
		m_pCamera->ResetCamera();

		// 次のフェーズを初期化
		m_pPhase->Init();
	}
	
	// フェーズ状態を同期
	m_OldPhase = m_Phase;
}
