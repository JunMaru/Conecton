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
#include < stdio.h >

//=============================================================================
// �}�N��
//=============================================================================
#define MAX_BLOCK_X ( 30 )
#define MAX_BLOCK_Y ( 50 )

//=============================================================================
// �O����`
//=============================================================================
class CBlock;

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

	static CBlockManager* Create( void );

	// �X�e�[�W��������
	bool CreateBlockMap(char *p_stagemap_filename);

	// �u���b�N�ւ̃A�h���X�Q�b�^�[
	static CBlock* GetBlock(int n_block_idx_x, int n_block_idx_y);		// �z��ԍ��w��
	static void SetBlock(int n_block_idx_x, int n_block_idx_y, CBlock* p_block){ m_pBlockArray[n_block_idx_y * MAX_BLOCK_X + n_block_idx_x] = p_block; }

private:
	// �X�e�[�W��̃u���b�N�ւ̃A�h���X�Ǘ��ꏊ
	static CBlock *m_pBlockArray[MAX_BLOCK_Y * MAX_BLOCK_X];

	// CSV�̏����󂯎��ꏊ�H
	int m_nMaxArrayNumX;	// �u���b�N�z��ő吔-X
	int m_nMaxArrayNumY;	// �u���b�N�z��ő吔-Y
};

// End of file