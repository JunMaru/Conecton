/*-----------------------------------------------------------------------------
	衝突判定ユーティリティ
	author	OkadaMoeto
	since	20150204
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "collisionDetection.h"
#include "Define.h"
#include "MathHelper.h"

/*-----------------------------------------------------------------------------
	球と球の衝突判定
-----------------------------------------------------------------------------*/
bool CheckCollisionSphereVsSphere(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2,
									float radius2)
{
	D3DXVECTOR3 v = v1 - v2;
	float dot = v.x * v.x + v.y * v.y + v.z * v.z;

	return (dot < (radius1 + radius2) * (radius1 + radius2));
}

/*-----------------------------------------------------------------------------
	円と円の衝突判定
-----------------------------------------------------------------------------*/
bool CheckCollisionCircleVsCircle(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2,
									float radius2)
{
	D3DXVECTOR3 v = v1 - v2;
	float dot = v.x * v.x + v.y * v.y;

	return (dot < (radius1 + radius2) * (radius1 + radius2));
}

/*-----------------------------------------------------------------------------
	矩形と矩形の衝突判定
-----------------------------------------------------------------------------*/
bool CheckCollisionRectVsRect(
								D3DXVECTOR3 &v1,
								D3DXVECTOR3 &size1,
								D3DXVECTOR3 &v2,
								D3DXVECTOR3 &size2)
{
	if((v1.x < v2.x + size2.x)
	&& (v2.x < v1.x + size1.x)
	&& (v1.y < v2.y + size2.y)
	&& (v2.y < v1.y + size1.y))
	{
		return true;
	}

	return false;
}

/*-----------------------------------------------------------------------------
	円と矩形の衝突判定
-----------------------------------------------------------------------------*/
bool CheckCollisionCircleVsRect(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2,
									D3DXVECTOR3 &size2)
{
	D3DXVECTOR3 v = VEC3_ZERO;
	v.x = abs(v1.x - v2.x);
	v.y = abs(v1.y - v2.y);

	if(v.x > size2.x + radius1) return false;
	if(v.y > size2.y + radius1) return false;

	if(v.x <= size2.x) return true;
	if(v.y <= size2.y) return true;

	float distanceSquared = (v.x - size2.x) * (v.x - size2.x)
							+ (v.y - size2.y) * (v.y - size2.y);

	// 距離が円の半径より長い場合は当たっていない
	return (distanceSquared <= (radius1 * radius1));

	return false ;
}

/*-----------------------------------------------------------------------------
	円と点の衝突判定
	return	bool	衝突結果
	retval	true	当たっている
	retval	false	当たっていない
	param	v1		対象の円形オブジェクト（１）の現在の中心座標
	param	radius1	対象の円形オブジェクト（１）の半径
	param	v2		対象の点オブジェクト（２）の現在の中心座標
-----------------------------------------------------------------------------*/
bool CheckCollisionCircleVsPoint(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2)
{
	D3DXVECTOR3 v = v1 - v2;
	float dot = v.x * v.x + v.y * v.y;

	return (dot < (radius1 * radius1));

}

/*-----------------------------------------------------------------------------
	点と矩形の衝突判定
	author kikushima  since 0612
-----------------------------------------------------------------------------*/
bool CheckCollisionPointVsRect(
								D3DXVECTOR3 &v1,
								D3DXVECTOR3 &size1,
								D3DXVECTOR3 &v2)
{
	if ((v1.x < v2.x)
		&& (v2.x < v1.x + size1.x)
		&& (v1.y < v2.y)
		&& (v2.y < v1.y + size1.y))
	{
		return true;
	}

	return false;
}
