/*-----------------------------------------------------------------------------
	�Փ˔��胆�[�e�B���e�B
	�Փˌ��o�̊֐��Q
	author	OkadaMoeto
	since	20150204
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
	���Ƌ��̏Փ˔���
	return	bool		�Փˌ���
	retval	true		�������Ă���
	retval	false		�������Ă��Ȃ�
	param	v1			�Ώۂ̋��`�I�u�W�F�N�g�i�P�j�̌��݂̒��S���W
	param	radius1		�Ώۂ̋��`�I�u�W�F�N�g�i�P�j�̔��a
	param	v2			�Ώۂ̋��`�I�u�W�F�N�g�i�Q�j�̌��݂̒��S���W
	param	radius2		�Ώۂ̋��`�I�u�W�F�N�g�i�Q�j�̔��a
-----------------------------------------------------------------------------*/
bool CheckCollisionSphereVsSphere(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2,
									float radius2);

/*-----------------------------------------------------------------------------
	�~�Ɖ~�̏Փ˔���
	return	bool		�Փˌ���
	retval	true		�������Ă���
	retval	false		�������Ă��Ȃ�
	param	v1			�Ώۂ̉~�`�I�u�W�F�N�g�i�P�j�̌��݂̒��S���W
	param	radius1		�Ώۂ̉~�`�I�u�W�F�N�g�i�P�j�̔��a
	param	v2			�Ώۂ̉~�`�I�u�W�F�N�g�i�Q�j�̌��݂̒��S���W
	param	radius2		�Ώۂ̉~�`�I�u�W�F�N�g�i�Q�j�̔��a
-----------------------------------------------------------------------------*/
bool CheckCollisionCircleVsCircle(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2,
									float radius2);

/*-----------------------------------------------------------------------------
	��`�Ƌ�`�̏Փ˔���
	return	bool		�Փˌ���
	retval	true		�������Ă���
	retval	false		�������Ă��Ȃ�
	param	v1			�Ώۂ̋�`�I�u�W�F�N�g�i�P�j�̌��݂̒��_������W
	param	size1		�Ώۂ̋�`�I�u�W�F�N�g�i�P�j�̉����Əc��
	param	v2			�Ώۂ̋�`�I�u�W�F�N�g�i�Q�j�̌��݂̒��_������W
	param	size2		�Ώۂ̋�`�I�u�W�F�N�g�i�Q�j�̉����Əc��
-----------------------------------------------------------------------------*/
bool CheckCollisionRectVsRect(
								D3DXVECTOR3 &v1,
								D3DXVECTOR3 &size1,
								D3DXVECTOR3 &v2,
								D3DXVECTOR3 &size2);

/*-----------------------------------------------------------------------------
	�~�Ƌ�`�̏Փ˔���
	return	bool		�Փˌ���
	retval	true		�������Ă���
	retval	false		�������Ă��Ȃ�
	param	v1			�Ώۂ̉~�`�I�u�W�F�N�g�i�P�j�̌��݂̒��S���W
	param	radius1		�Ώۂ̉~�`�I�u�W�F�N�g�i�P�j�̔��a
	param	v2			�Ώۂ̋�`�I�u�W�F�N�g�i�Q�j�̌��݂̒��S���W
	param	size2		�Ώۂ̋�`�I�u�W�F�N�g�i�Q�j�̉����Əc��
-----------------------------------------------------------------------------*/
bool CheckCollisionCircleVsRect(
								D3DXVECTOR3 &v1,
								float radius1,
								D3DXVECTOR3 &v2,
								D3DXVECTOR3 &size2);

/*-----------------------------------------------------------------------------
	�~�Ɠ_�̏Փ˔���
	return	bool		�Փˌ���
	retval	true		�������Ă���
	retval	false		�������Ă��Ȃ�
	param	v1			�Ώۂ̉~�`�I�u�W�F�N�g�i�P�j�̌��݂̒��S���W
	param	radius1		�Ώۂ̉~�`�I�u�W�F�N�g�i�P�j�̔��a
	param	v2			�Ώۂ̓_�I�u�W�F�N�g�i�Q�j�̌��݂̒��S���W
-----------------------------------------------------------------------------*/
bool CheckCollisionCircleVsPoint(
									D3DXVECTOR3 &v1,
									float radius1,
									D3DXVECTOR3 &v2);

/*-----------------------------------------------------------------------------
	�_�Ƌ�`�̏Փ˔���
	return	bool		�Փˌ���
	retval	true		�������Ă���
	retval	false		�������Ă��Ȃ�
	param	v1			�Ώۂ̋�`�I�u�W�F�N�g�i�P�j�̌��݂̒��_������W
	param	size1		�Ώۂ̋�`�I�u�W�F�N�g�i�P�j�̉����Əc��
	param	v2			�Ώۂ̓_�I�u�W�F�N�g�i�Q�j�̌��݂̒��S���W
	
	author Kikushima since 2015/06/12 
-----------------------------------------------------------------------------*/
bool CheckCollisionPointVsRect(
									D3DXVECTOR3 &v1,
									D3DXVECTOR3 &size1,
									D3DXVECTOR3 &v2);
