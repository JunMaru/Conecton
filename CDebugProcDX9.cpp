/*-----------------------------------------------------------------------------
	デバッグ表示クラス
	author	OkadaMoeto
	since	20140722
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	警告抑制
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS // セキュア関数に関するワーニングを消す

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CDebugProcDX9.h"
#include <stdio.h>
#include <stdarg.h>
#include "Define.h"
#include "CManager.h"
#include "CRenderer.h"

/*-----------------------------------------------------------------------------
	静的メンバ変数の初期化
-----------------------------------------------------------------------------*/
LPD3DXFONT CDebugProcDX9::m_pD3DXFont = nullptr;
char CDebugProcDX9::m_aStr[LENGTH_STRING_BUFF] = {"\0"};
bool CDebugProcDX9::m_bDisp = true;
int CDebugProcDX9::m_countFrame = 0;

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CDebugProcDX9::CDebugProcDX9()
{
	// バッファクリア
	memset(m_aStr, 0, sizeof m_aStr);
}


/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CDebugProcDX9::~CDebugProcDX9()
{
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
void CDebugProcDX9::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 情報表示用フォントを設定
	D3DXCreateFont(
					pDevice,				// デバイス
					FONT_HEIGHT,			// 文字の高さ(0:自動調整)
					FONT_WIDTH,				// 文字の幅
					FW_NORMAL,				// フォントの太さ
					0,						// ミニマップレベル数
					FALSE,					// イタリック体
					SHIFTJIS_CHARSET,		// 文字セット(シフトJIS)
					OUT_DEFAULT_PRECIS,		// 出力精度
					DEFAULT_QUALITY,		// 出力品質
					DEFAULT_PITCH,			// フォントピッチとファミリ
					"Terminal",				// フォント名
					&m_pD3DXFont);			// フォントポインタへのアドレス

	m_bDisp = false;
	m_countFrame = 0;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CDebugProcDX9::Uninit(void)
{
	// 情報表示用フォント解放
	if(m_pD3DXFont)
	{
		m_pD3DXFont->Release();
		m_pD3DXFont = nullptr;
	}
}

/*-----------------------------------------------------------------------------
	書き込み
	param	:*fmt	書式文字列(printfと同様)
	param	:...	可変個引数
-----------------------------------------------------------------------------*/
void CDebugProcDX9::Print(char *fmt,...)
{
#ifdef _DEBUG
	va_list list;
	char *pCur;
	char aBuf[256]={"\0"};
	char aWk[32];
	
	char aConvertion[8] = {"\0"};
	char aPercent[8] = {"%."};
	char aFloating[8] = {"f"};
	char aDigit[8] = {"\0"};
	
	va_start(list, fmt);

	pCur = fmt;
	for( ; *pCur; ++pCur)
	{
		// '%'の記号(書式指定子)までは1文字出力
		if(*pCur != '%')
		{
			sprintf(aWk, "%c", *pCur);
		}
		else
		{
			// 書式指定子の開始'%'のあとの変換指定子の位置にずらす
			pCur++;

			switch(*pCur)
			{
				case 'd':
				{
					// 整数型に固定指定(va_arg)
					sprintf(aWk, "%d", va_arg(list, int));
					break;
				}

				case 'f':
				{
					// 浮動小数(double型で指定)
					sprintf(aWk, "%f", va_arg(list, double));
					break;
				}

				case 's':
				{	
					// 文字列
					sprintf(aWk, "%s", va_arg(list, char*));
					break;
				}

				case 'c':
				{
					// 文字
					sprintf(aWk, "%c", va_arg(list, char));
					break;
				}

				default:
				{	
					sprintf(aWk, "%c", *pCur);
					break;
				}
			}
		}
		strcat(aBuf, aWk);
	}

	va_end(list);

	if((strlen(m_aStr) + strlen(aBuf)) < sizeof m_aStr - 1)
	{
		strcat(m_aStr, aBuf);
	}
#endif
}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/
void CDebugProcDX9::Draw(void)
{
#ifdef _DEBUG
	m_countFrame++;
	if(m_countFrame > GetFPS())
	{
		m_countFrame = 0;
	}

	if(m_bDisp)
	{
		RECT rect = {0, 0, (LONG)(SCREEN_WIDTH), (LONG)(SCREEN_HEIGHT)};

		m_pD3DXFont->DrawText(
								nullptr,
								m_aStr,
								-1,
								&rect,
								DT_LEFT,
								COL_BLACK);

		// デバッグ情報のバッファクリア
		memset(m_aStr, 0, sizeof m_aStr);
	}
#endif
}
