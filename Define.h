/*-----------------------------------------------------------------------------
	共通定義ファイル
	author	OkadaMoeto
	since	20150302
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
// ifdefマクロ コメントアウトするとＯＦＦ
//#define ON_SOUND // サウンド再生ON/OFF

// 頂点数
#define	NUM_VERTEX		(4)
// ポリゴン数
#define	NUM_POLYGON		(2)

// 解放マクロ
#define SAFE_RELEASE(p)			{ if(p){ (p)->Release(); (p) = nullptr; } }
#define SAFE_DELETE(p)			{ if(p) { delete (p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p){ delete[] (p); (p) = nullptr; } 

// スクリーンの幅と高さ
#define SCREEN_WIDTH	(1280.0f)
#define SCREEN_HEIGHT	(720.0f)

// スクリーンの中心座標
#define SCREEN_CENTER_X		(SCREEN_WIDTH / 2)
#define SCREEN_CENTER_Y		(SCREEN_HEIGHT / 2)

// D3DXVECTOE3型
#define VEC3_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define VEC3_ONE	(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
#define VEC3_PX		(D3DXVECTOR3(1.0f, 0.0f, 0.0f))
#define VEC3_PY		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define VEC3_PZ		(D3DXVECTOR3(0.0f, 0.0f, 1.0f))
#define VEC3_NX		(D3DXVECTOR3(-1.0f, 0.0f, 0.0f))
#define VEC3_NY		(D3DXVECTOR3(0.0f, -1.0f, 0.0f))
#define VEC3_NZ		(D3DXVECTOR3(0.0f, 0.0f, -1.0f))

// D3DXVECTOE2型
#define VEC2_ZERO	(D3DXVECTOR2(0.0f, 0.0f))
#define VEC2_ONE	(D3DXVECTOR2(1.0f, 1.0f))
#define VEC2_PX		(D3DXVECTOR2(1.0f, 0.0f))
#define VEC2_PY		(D3DXVECTOR2(0.0f, 1.0f))
#define VEC2_NX		(D3DXVECTOR2(-1.0f, 0.0f))
#define VEC2_NY		(D3DXVECTOR2(0.0f, -1.0f))

// D3DXCOLOR型
#define COL_BLACK				(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))
#define COL_WHITE				(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
#define COL_RED					(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))
#define COL_GREEN				(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))
#define COL_BLUE				(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))
#define COL_CYAN				(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f))
#define COL_MAGENTA				(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f))
#define COL_YELLOW				(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))
#define COL_BLACK_ALPHA(a)		(D3DXCOLOR(0.0f, 0.0f, 0.0f, a))
#define COL_WHITE_ALPHA(a)		(D3DXCOLOR(1.0f, 1.0f, 1.0f, a))

// 抵抗値(地上)
#define REGIST_MOVE			(0.55f)
// 抵抗値(空中)
#define REGIST_MOVE_AIR		(0.95f)
// 抵抗値(空中落下)
#define REGIST_MOVE_AIR_Y	(0.985f)
// 抵抗値(ギミック床)
#define REGIST_MOVE_SLIDE	(0.965f)

// 重力値
#define GRAVITY		(9.80f)
