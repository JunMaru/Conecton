/*-----------------------------------------------------------------------------
	衝突判定ユーティリティ
	衝突検出の関数群
	author	OkadaMoeto
	since	20150204
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
	球と球の衝突判定
	return	bool		衝突結果
	retval	true		当たっている
	retval	false		当たっていない
	param	v1			対象の球形オブジェクト（１）の現在の中心座標
	param	radius1		対象の球形オブジェクト（１）の半径
	param	v2			対象の球形オブジェクト（２）の現在の中心座標
	param	radius2		対象の球形オブジェクト（２）の半径
-----------------------------------------------------------------------------*/
bool CheckCollisionSphereVsSphere(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2,
									float radius2);

/*-----------------------------------------------------------------------------
	円と円の衝突判定
	return	bool		衝突結果
	retval	true		当たっている
	retval	false		当たっていない
	param	v1			対象の円形オブジェクト（１）の現在の中心座標
	param	radius1		対象の円形オブジェクト（１）の半径
	param	v2			対象の円形オブジェクト（２）の現在の中心座標
	param	radius2		対象の円形オブジェクト（２）の半径
-----------------------------------------------------------------------------*/
bool CheckCollisionCircleVsCircle(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2,
									float radius2);

/*-----------------------------------------------------------------------------
	矩形と矩形の衝突判定
	return	bool		衝突結果
	retval	true		当たっている
	retval	false		当たっていない
	param	v1			対象の矩形オブジェクト（１）の現在の頂点左上座標
	param	size1		対象の矩形オブジェクト（１）の横幅と縦幅
	param	v2			対象の矩形オブジェクト（２）の現在の頂点左上座標
	param	size2		対象の矩形オブジェクト（２）の横幅と縦幅
-----------------------------------------------------------------------------*/
bool CheckCollisionRectVsRect(
								D3DXVECTOR3 &v1,
								D3DXVECTOR3 &size1,
								D3DXVECTOR3 &v2,
								D3DXVECTOR3 &size2);

/*-----------------------------------------------------------------------------
	円と矩形の衝突判定
	return	bool		衝突結果
	retval	true		当たっている
	retval	false		当たっていない
	param	v1			対象の円形オブジェクト（１）の現在の中心座標
	param	radius1		対象の円形オブジェクト（１）の半径
	param	v2			対象の矩形オブジェクト（２）の現在の中心座標
	param	size2		対象の矩形オブジェクト（２）の横幅と縦幅
-----------------------------------------------------------------------------*/
bool CheckCollisionCircleVsRect(
								D3DXVECTOR3 &v1,
								float radius1,
								D3DXVECTOR3 &v2,
								D3DXVECTOR3 &size2);

/*-----------------------------------------------------------------------------
	円と点の衝突判定
	return	bool		衝突結果
	retval	true		当たっている
	retval	false		当たっていない
	param	v1			対象の円形オブジェクト（１）の現在の中心座標
	param	radius1		対象の円形オブジェクト（１）の半径
	param	v2			対象の点オブジェクト（２）の現在の中心座標
-----------------------------------------------------------------------------*/
bool CheckCollisionCircleVsPoint(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2);

/*-----------------------------------------------------------------------------
	点と矩形の衝突判定
	return	bool		衝突結果
	retval	true		当たっている
	retval	false		当たっていない
	param	v1			対象の矩形オブジェクト（１）の現在の頂点左上座標
	param	size1		対象の矩形オブジェクト（１）の横幅と縦幅
	param	v2			対象の点オブジェクト（２）の現在の中心座標
	
	author Kikushima since 2015/06/12 
-----------------------------------------------------------------------------*/
bool CheckCollisionPointVsRect(
									D3DXVECTOR3 &v1,
									D3DXVECTOR3 &size1,
									D3DXVECTOR3 &v2);
