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

//=============================================================================
// �}�N��
//=============================================================================
#define MAX_BLOCK_X ( 10 )
#define MAX_BLOCK_Y ( 10 )

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
	static CBlock* GetBlock(D3DXVECTOR3 character_pos, D3DXVECTOR2 character_size);		// ���W����
	static CBlock* GetBlock(int n_block_idx_x, int n_block_idx_y);						// �z��ԍ��w��

private:
	// �X�e�[�W��̃u���b�N�ւ̃A�h���X�Ǘ��ꏊ
	static CBlock *m_pBlockArray[MAX_BLOCK_Y][MAX_BLOCK_X];

	// CSV�̏����󂯎��ꏊ�H
};

// End of file