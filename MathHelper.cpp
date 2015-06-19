/*-----------------------------------------------------------------------------
	数学関係処理
	author	OkadaMoeto
	since	20150327
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "MathHelper.h"

/*-----------------------------------------------------------------------------
	値を指定した範囲内に制限
-----------------------------------------------------------------------------*/
float Max(float val, float min)
{
	return val < min ? val : min;
}

float Min(float val, float max)
{
	return val > max ? val : max;
}

float Clamp(float val, float min, float max)
{
	//return Min(Max(min, val), max);
	if(val < min)
	{
		return min;
	}

	if(val > max)
	{
		return max;
	}

	return val;
}

/*-----------------------------------------------------------------------------
	距離の２乗
-----------------------------------------------------------------------------*/
float DistanceSquared(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	D3DXVECTOR3 v = v1 - v2;
	return (v.x + v.x) * (v.x + v.x) + (v.y + v.y) * (v.y + v.y);
}

/*-----------------------------------------------------------------------------
	長さ
-----------------------------------------------------------------------------*/
float Length(D3DXVECTOR3 *vec)
{
	float len = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	return len;
}

/*-----------------------------------------------------------------------------
	正規化
-----------------------------------------------------------------------------*/
void Normalize(D3DXVECTOR3 *vec, D3DXVECTOR3 *vec1)
{
	*vec = *vec1 / Length(vec1);
}
