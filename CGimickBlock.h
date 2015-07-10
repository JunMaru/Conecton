//=============================================================================
// �M�~�b�N�u���b�N�N���X
// AUTHOR : Yutaka Ichikawa
// SINCE  : 20150707
//=============================================================================

// �Q�d�C���N���[�h�h�~
#pragma once

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================

#include "CScene.h"
#include "CBlock.h"

//=============================================================================
// �}�N��
//=============================================================================

//=============================================================================
// class
//=============================================================================
class CInstancingObject;
class CBlock;

//=============================================================================
// �M�~�b�N�u���b�N�N���X
//=============================================================================
class CGimmickBlock : public CBlock
{
public:
	typedef enum
	{
		SWITCH_ACTION_CREATE_GRASS = 0,		// ���u���b�N�����M�~�b�N
		SWITCH_ACTION_CREATE_
	}SWITCH_ACTION;

	CGimmickBlock(const int Layer = 0){}
	virtual ~CGimmickBlock(){}

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetWarpFlag(bool b_warp_flag){ m_bWarpFlag = b_warp_flag; }
	bool GetWarpFlag(){ return m_bWarpFlag; }

	CGimmickBlock *GetWarpPoint(){ return m_pWarpPoint; }	// �]�ڐ�̃A�h���X�Q�b�^�[

private:
	CGimmickBlock *m_pWarpPoint;	// ���̃��[�v�]�[���̓]�ڐ�ւ̃A�h���X
	bool m_bWarpFlag;				// ���[�v�t���O

};

// End of file