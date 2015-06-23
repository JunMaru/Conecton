/*-----------------------------------------------------------------------------
	テクスチャシートのロード系関数
	author	HaradaYuto
	since	20150616
-----------------------------------------------------------------------------*/
#pragma once
#include <d3dx9.h>

namespace TexLoader
{
	typedef struct
	{
		D3DXVECTOR2 uv;
		D3DXVECTOR2 size;
	}UvInfo;

	/*------------------------------------------------------------------------------
		テクスチャ情報ロード
		param		テクスチャファイルパス（テキストへの）
					テクスチャuvサイズポインタ（内部でnewされる)
					テクスチャポインタ
		return		生成された配列要素数
	-------------------------------------------------------------------------------*/
	int LoadTexSheetFromText( const char *file_path, UvInfo *& uv_info, LPDIRECT3DTEXTURE9 *tex = NULL );
	int LoadTexSheetFromBin ( const char *file_path, UvInfo *& uv_info, LPDIRECT3DTEXTURE9 *tex = NULL );

}
