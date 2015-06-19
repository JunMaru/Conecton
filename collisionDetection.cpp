/*-----------------------------------------------------------------------------
	�Փ˔��胆�[�e�B���e�B
	author	OkadaMoeto
	since	20150204
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "collisionDetection.h"
#include "Define.h"
#include "MathHelper.h"

/*-----------------------------------------------------------------------------
	���Ƌ��̏Փ˔���
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
	�~�Ɖ~�̏Փ˔���
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
	��`�Ƌ�`�̏Փ˔���
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
	�~�Ƌ�`�̏Փ˔���
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

	// �������~�̔��a��蒷���ꍇ�͓������Ă��Ȃ�
	return (distanceSquared <= (radius1 * radius1));

	return false ;
}

/*-----------------------------------------------------------------------------
	�~�Ɠ_�̏Փ˔���
	return	bool	�Փˌ���
	retval	true	�������Ă���
	retval	false	�������Ă��Ȃ�
	param	v1		�Ώۂ̉~�`�I�u�W�F�N�g�i�P�j�̌��݂̒��S���W
	param	radius1	�Ώۂ̉~�`�I�u�W�F�N�g�i�P�j�̔��a
	param	v2		�Ώۂ̓_�I�u�W�F�N�g�i�Q�j�̌��݂̒��S���W
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
	�_�Ƌ�`�̏Փ˔���
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
