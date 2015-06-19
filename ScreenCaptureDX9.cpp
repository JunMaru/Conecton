/*-----------------------------------------------------------------------------
	DirectX9スクリーンショットバックバッファ出力処理
	author	OkadaMoeto
	since	20140617
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	警告抑制
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS // セキュア関数に関するワーニングを消す

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "ScreenCaptureDX9.h"
#include "Define.h"
#include <stdio.h>
#include <tchar.h>
#include <ctime>
#include "CDebugProcDX9.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
// 保存ファイルパス（年_月_日_時_分_フレーム）
#define FILENAME_OUTPUT_DIR_BMP ("data/screenshot/%d_%02d_%02d_%02d_%02d_%02d_%02d.bmp")
#define FILENAME_OUTPUT_DIR_JPG ("data/screenshot/%d_%02d_%02d_%02d_%02d_%02d_%02d.jpg")

/*-----------------------------------------------------------------------------
	バックバッファ出力(スクリーンキャプチャ)
-----------------------------------------------------------------------------*/
bool CaptureImageFromBackBuffer(D3DXIMAGE_FILEFORMAT file_format)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DPRESENT_PARAMETERS pParam = CManager::GetRenderer()->GetParameters();
	// サーフェイス
	LPDIRECT3DSURFACE9 pSurface = NULL;
	// バックバッファ
	LPDIRECT3DSURFACE9 pBackBuff = NULL;
	// ファイルパス名
	char fileName[256];
	// タイムスタンプ
	time_t timer;
	struct tm *local;
		
	// タイムスタンプ作成
	timer = time(NULL);
	// 地方時に変換
	local = localtime(&timer);

	// BMP, JPG形式のフォーマットをサポート
	if(file_format != D3DXIFF_BMP && file_format != D3DXIFF_JPG)
	{
		return false;
	}

	//出力するフォーマットによってファイルの拡張子を変更する。
	switch(file_format)
	{
		case D3DXIFF_BMP:
			// tm_year + 1900 (年 - 1900)されているので1900足す
			// tm_mon + 1 ０始まりなので＋１したほうが分かり易い
			_stprintf(
						fileName,
						FILENAME_OUTPUT_DIR_BMP,
						local->tm_year + 1900,
						local->tm_mon + 1,
						local->tm_mday,
						local->tm_hour,
						local->tm_min,
						local->tm_sec,
						CDebugProcDX9::GetCountFrame()
			);
			break;

		case D3DXIFF_JPG:
			_stprintf(
						fileName,
						FILENAME_OUTPUT_DIR_JPG,
						local->tm_year + 1900,
						local->tm_mon + 1,
						local->tm_mday,
						local->tm_hour, 
						local->tm_min,
						local->tm_sec,
						CDebugProcDX9::GetCountFrame()
			);
			break;
	}

	// キャプチャ用のサーフェイスを作成
	if(FAILED(pDevice->CreateOffscreenPlainSurface(
													(UINT)SCREEN_WIDTH,
													(UINT)SCREEN_HEIGHT, 
													pParam.BackBufferFormat,
													D3DPOOL_SYSTEMMEM,
													&pSurface,
													NULL
	)))
	{
		return false;
	}

	// バックバッファの取得
	pDevice->GetRenderTarget(0, &pBackBuff);

	// バックバッファデータの転送
	pDevice->GetRenderTargetData(pBackBuff, pSurface);

	// 用意したものは解放
	pBackBuff->Release();
	pBackBuff = nullptr;

	// ファイル出力
	D3DXSaveSurfaceToFile(fileName, file_format, pSurface, NULL, NULL);
	
	// 用意したものは解放
	pSurface->Release();
	pSurface = nullptr;
	
	return true;
}
