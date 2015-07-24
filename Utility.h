/*-----------------------------------------------------------------------------
ユーティリティ
author	HaradaYuto
since	20150724
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

namespace Utility
{
	/*-----------------------------------------------------------------------------
	Csvの内容を配列にいれる(データは数値限定)
	param 二次元配列用ポインタ　要素数格納用ポインタ
	------------------------------------------------------------------------------*/
	bool LoadCsv(const char *file_path, int **&array_2d, int *x_elem_num, int *y_elem_num);

	/*-----------------------------------------------------------------------------
	二次元配列デリート（intのみ
	param 配列ポインタ ｙ要素数
	------------------------------------------------------------------------------*/
	void Delete2DArrayInt(int **&array_2d, int y_elem_num);
}