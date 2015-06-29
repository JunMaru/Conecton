/*-----------------------------------------------------------------------------
	DirectXデバイスの生成と描画クラス
	author	OkadaMoeto
	since	20140415
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CRenderer.h"
#include "Define.h"
#include "CManager.h"
#include "CCamera.h"
#include "CScene.h"

/*-----------------------------------------------------------------------------
	静的メンバ変数の初期化
-----------------------------------------------------------------------------*/
bool CRenderer::m_bFillModeWireFrame = false;

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CRenderer::~CRenderer()
{
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == nullptr)
	{
		// DirectX User ランタイム取得しよう。
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}


	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= (UINT)SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= (UINT)SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成

	// D16以下でもダメならばエラーメッセージ
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;				// デプスバッファとして16bitを使う

	if(bWindow)
	{
		// ウィンドウモード
		d3dpp.BackBufferFormat				= D3DFMT_UNKNOWN;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz	= 0;								// リフレッシュレート
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{
		// フルスクリーンモード
		d3dpp.BackBufferFormat				= D3DFMT_R5G6B5;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz	= 0;			// リフレッシュレート
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(m_pD3D->CreateDevice(
									D3DADAPTER_DEFAULT,						// ディスプレイアダプタ
									D3DDEVTYPE_HAL,							// ディスプレイタイプ
									hWnd,									// フォーカスするウインドウへのハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,	// デバイス作成制御の組み合わせ
									&d3dpp,									// デバイスのプレゼンテーションパラメータ
									&m_pD3DDevice)))						// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(
					m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
					D3DDEVTYPE_HAL, 
					hWnd, 
					D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
					&d3dpp,
					&m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(
						m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
						D3DDEVTYPE_REF,
						hWnd, 
						D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
						&d3dpp,
						&m_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}

			// ハードウェアで生成出来なかったこの場合は、処理落ちなど正常に出力されないメッセージ出力
			MessageBox(hWnd, "処理落ちなどで正常に画面が出力されないおそれがあります", "WARNING", MB_ICONWARNING);
		}
	}

	// デバイスのプレゼンテーションパラメータの保存
	m_pParameters = d3dpp;

	// 裏面ポリゴンを消す
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Zバッファ有効
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// アルファブレンド有効
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// 通常ブレンドを設定
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// UV座標の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャの拡縮時のリニア補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// テクスチャの混ぜ合わせの透過
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// 背景部分透過
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CRenderer::Uninit(void)
{
	// Dirext3Dオブジェクトの解放
	if(m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	// デバイスオブジェクトの解放
	if(m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CRenderer::Update(void)
{
	CScene::UpdateAll();
}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/
void CRenderer::Draw(void)
{
	CCamera* pCamera = CManager::GetCamera();

	if(m_bFillModeWireFrame)
	{
		// 描画方法をワイヤーフレームに変更する
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		// 描画方法を元に戻す
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	// 画面のクリア
	m_pD3DDevice->Clear(
						0,
						nullptr,
						(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
						D3DCOLOR_RGBA(0, 0, 0, 0),
						1.0f,
						0);

	// 描画開始
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		pCamera->SetCamera();

		CScene::DrawAll();

#ifdef _DEBUG
		// デバッグ処理
		CDebugProcDX9::Draw();
#endif

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
