//=============================================================================
//
// CBlockManager ��`����[ CBlockManager.h ]
// Author : Yutaka Ichikawa
//
//=============================================================================

// ��d�C���N���[�h�h�~
#pragma once

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "Main.h"
#include "CBlock.h"
#include < stdio.h >

//=============================================================================
// �}�N��
//=============================================================================
#define MAX_BLOCK_X ( 30 )
#define MAX_BLOCK_Y ( 100 )
#define MAX_WOOD_BOX ( 10 )

//=============================================================================
// �O����`
//=============================================================================
class CBlock;
class CWoodBox;

//=============================================================================
// �N���X��`
//=============================================================================
class CBlockManager
{
public:
	CBlockManager(){}
	~CBlockManager(){}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CBlockManager* Create( char *pFileName );

	// �X�e�[�W��������
	bool CreateBlockMap(char *p_stagemap_filename);

	// �u���b�N�ւ̃A�h���X�Q�b�^�[
	// �S�ẴI�u�W�F�N�g�������Ȃ̂ŁA�Q�b�g����ۂ͈�x���S���W�ɂ���
	// �u���b�N�̑傫���Ŋ���Ɛ��m�Ȕz��ԍ�������B
	static CBlock* GetBlock(int n_block_idx_x, int n_block_idx_y);		// �z��ԍ��w��
	static void SetBlock(int n_block_idx_x, int n_block_idx_y, CBlock* p_block){ m_pBlockArray[n_block_idx_y * MAX_BLOCK_X + n_block_idx_x] = p_block; }
	static CBlock* GetLaserStart(){ return m_pLaserStart; }
	static CBlock* GetLaserGoal(){ return m_pLaserGoal; }

	static CWoodBox* GetWoodBoxAddress( int nWoodBoxArrayNum ){ return m_pWoodBoxArray[ nWoodBoxArrayNum ]; }

	// �u���b�N�z��ԍ��Z�o�֐�
	static void CalculateBlockArrayNum(D3DXVECTOR3 pos, int *p_array_x, int *p_array_y);

	// �u���b�NID�Q�b�^�[
	CBlock::BLOCKID GetBlockID( D3DXVECTOR3 pos );
	int GetBlockHitLaserNo(D3DXVECTOR3 pos);
	int GetFoodNum(void){ return m_nFoodNum; }
	int GetWoodBoxCnt(void){ return m_nWoodBoxCnt; }

	// �M�~�b�N�u���b�N�֏㏑������֐�
	void OverwriteGimmickBlock( CBlock::BLOCKID block_type, D3DXVECTOR3 pos );

	// ���[�v�u���b�N�]�ڐ�ݒ�֐�( CreateBlockMap�I�����ɌĂ� )
	void SetWarpPoint();

private:
	// �X�e�[�W��̃u���b�N�ւ̃A�h���X�Ǘ��ꏊ
	static CBlock *m_pBlockArray[MAX_BLOCK_Y * MAX_BLOCK_X];
	static CWoodBox *m_pWoodBoxArray[ MAX_WOOD_BOX ];
	static CBlock *m_pLaserStart;		// ���[�U�[�X�^�[�g�ʒu�ւ̃A�h���X
	static CBlock *m_pLaserGoal;		// ���[�U�[�S�[���ʒu�ւ̃A�h���X

	// CSV�̏����󂯎��ꏊ�H
	int m_nMaxArrayNumX;	// �u���b�N�z��ő吔-X
	int m_nMaxArrayNumY;	// �u���b�N�z��ő吔-Y
	int m_nFoodNum;
	int m_nWoodBoxCnt;

	char *m_pFileName;
};

// End of file