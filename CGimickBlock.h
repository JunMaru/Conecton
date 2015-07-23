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
#define WARP_WAIT_TIME ( 500 )
#define WOODBOX_MOVE	(  5.0f )
#define STICK_LENGTH	( 200.0f )

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

	static void SetRetryWarpWaitTime(int n_retry_warp_wait_time){ m_nRetryWarpWaitTime = n_retry_warp_wait_time; }

	CGimmickBlock *GetWarpPoint(){ return m_pWarpPoint; }	// �]�ڐ�̃A�h���X�Q�b�^�[
	void SetWarpPoint(CGimmickBlock *p_warp_point){ m_pWarpPoint = p_warp_point; }

private:
	void PlaySeBoxSlip(void);

	CGimmickBlock *m_pWarpPoint;	// ���̃��[�v�]�[���̓]�ڐ�ւ̃A�h���X
	bool m_bWarpFlag;				// ���[�v�t���O
	static int m_nRetryWarpWaitTime;		// �Ăу��[�v��������܂ł̑҂�����

};

// End of file