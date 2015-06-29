/*-----------------------------------------------------------------------------
	エントリポイント(ウィンドウ生成)処理
	author	OkadaMoeto
	since	20140414
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	メモリリークチェック監視用
-----------------------------------------------------------------------------*/
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"
#include <string>
#include "Define.h"
#include "Resource.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
using std::string;

// ウインドウのクラス名
static const string CLASS_NAME = "AppClass";
// ウインドウのキャプション名
static const string WINDOW_NAME = "こねくとん";
// フレームレート
static const float FRAME_RATE = 1000.0f / 30.0f;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
// FPSカウンタ
static int g_countFPS = 0;

/*-----------------------------------------------------------------------------
	FPS取得
-----------------------------------------------------------------------------*/
int GetFPS()
{
	return g_countFPS;
}

/*-----------------------------------------------------------------------------
	エントリポイント
-----------------------------------------------------------------------------*/
int APIENTRY WinMain(
						HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPSTR lpCmdLine,
						int nCmdShow
)
{
	// アプリケーションの終了時にメモリリークチェック(出力のデバッグを参照)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 無くても良いけど、警告が出る
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	// システムタイマーの分解能を１ｍｓに設定
	timeBeginPeriod(1);

	// ウィンドウ構造体の内容設定
	WNDCLASSEX	wcex = {
						sizeof(WNDCLASSEX),									// WNDCLASSEXのメモリサイズを指定
						CS_CLASSDC,											// 表示するウインドウのスタイルを設定
						WndProc,											// ウィンドウプロシージャのアドレスを指定
						0,													// 通常は使用しないので"0"を指定
						0,													// 通常は使用しないので"0"を指定
						hInstance,											// WinMainのパラメータのインスタンスハンドルを設定
						LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1)),			// 使用するアイコンを指定(Windowsがもっているアイコンを使うならNULL)
						LoadCursor(NULL, IDC_ARROW),						// マウスカーソルを指定
						(HBRUSH)(COLOR_WINDOW + 1),							// ウインドウのクライアント領域の背景色を設定
						NULL,												// Windowにつけるメニューを設定
						CLASS_NAME.c_str(),									// ウインドウクラスの名前
						LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),	// 小さいアイコンが設定された場合の情報を記述
	};
	
	// ウィンドウのハンドル(識別子)
	HWND hWnd;
	// メッセージ
	MSG msg;
	// クライアント領域
	RECT rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	// ウィンドウサイズ固定, ウィンドウの最大化使用不可
	DWORD style = WS_OVERLAPPEDWINDOW &~ WS_THICKFRAME &~ WS_MAXIMIZEBOX;
	// ウィンドウの幅
	int windowWidth;
	// ウィンドウの高さ
	int windowHeight;
	// ウィンドウのX座標
	int windowX;
	// ウィンドウのY座標
	int windowY;

	// 描画領域を指定する
	AdjustWindowRect(&rect, style, FALSE);

	// ウィンドウの座標を中心にする位置を算出する
	windowWidth = rect.right - rect.left;
	windowHeight = rect.bottom - rect.top;
	windowX = ((GetSystemMetrics(SM_CXFULLSCREEN) - (windowWidth)) / 2);
	windowY = ((GetSystemMetrics(SM_CYFULLSCREEN) - (windowHeight)) / 2);

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(
							0,						// 拡張ウィンドウスタイル
							CLASS_NAME.c_str(),		// ウィンドウクラスの名前
							WINDOW_NAME.c_str(),	// ウィンドウの名前
							style,					// ウィンドウスタイル
							windowX,				// ウィンドウの左上Ｘ座標
							windowY,				// ウィンドウの左上Ｙ座標
							windowWidth,			// ウィンドウの幅
							windowHeight,			// ウィンドウの高さ
							NULL,					// 親ウィンドウのハンドル
							NULL,					// メニューハンドルまたは子ウィンドウID
							hInstance,				// インスタンスハンドル
							NULL					// ウィンドウ作成データ
	);

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ゲーム管理者を生成
	CManager* pManager = NULL;
	pManager = new CManager();
	pManager->Init(hInstance, hWnd, TRUE);

	// システム時刻をミリ秒単位で取得
	dwExecLastTime	= 
	dwFPSLastTime	= timeGetTime();
	// フレームカウント初期化
	dwCurrentTime	= 
	dwFrameCount	= 0;
	
	// メッセージループ
	while(1)
	{
		// メッセージを取得しなかった場合"0"を返す
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windowsの処理
			if(msg.message == WM_QUIT)
			{
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{
			// 現在のシステム時刻を取得(タイマー値)
			dwCurrentTime = timeGetTime();
			// 0.5秒ごとに実行して計測
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// FPSを測定
				g_countFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
		
				// タイマー値を更新
				dwFPSLastTime = dwCurrentTime;
				// フレームカウンタをリセット
				dwFrameCount = 0;
			}
			// 1/60秒ごとに実行(60FPS)
			if((dwCurrentTime - dwExecLastTime) >= FRAME_RATE)
			{
#ifdef _DEBUG
				// FPS出力
				CDebugProcDX9::Print("デバッグ情報\n");
				CDebugProcDX9::Print("FPS:%d\n", g_countFPS);
#endif
				// タイマー値を更新
				dwExecLastTime = dwCurrentTime;

				pManager->Update();
				
				pManager->Draw();

				// フレームカウントをカウントアップ
				dwFrameCount++;
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME.c_str(), wcex.hInstance);

	// システムタイマーの分解能を元に戻す
 	timeEndPeriod(1);

	// ゲーム管理者の解放
	if(pManager)
	{
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	return (int)msg.wParam;
}

/*-----------------------------------------------------------------------------
	ウィンドウプロシージャ
-----------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		// ウィンドウが閉じられた時
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		case WM_KEYDOWN:
		{
			switch(wParam)
			{
				// ESCキーが押されたらウィンドウを削除
				case VK_ESCAPE:
				{
					DestroyWindow(hWnd);
					break;
				}
			}
			break;
		}

		default:
		{
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
