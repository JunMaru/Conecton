//=============================================================================
//
// CBlockManager ��������[ CBlockManager.cpp ]
// Author : Yutaka Ichikawa
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CInstancingObject.h"
#include "CBlockManager.h"
#include "CBlock.h"
#include <stdio.h>

//=============================================================================
// �O���[�o��
//=============================================================================
CInstancingObject *ins;

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CBlock *CBlockManager::m_pBlockArray[MAX_BLOCK_Y][MAX_BLOCK_X] = { nullptr };

//=============================================================================
// ��������
//=============================================================================
CBlockManager* CBlockManager::Create(void)
{
	CBlockManager *pBlockManager = new CBlockManager;
	pBlockManager -> Init();
	return pBlockManager;
}

//=============================================================================
// ����������
// ���̏������́A�Q�[���ł̏������̂Ƃ������Ă�ł��������B
//=============================================================================
HRESULT CBlockManager::Init()
{
	// �e�L�X�g�������ɃX�e�[�W������������ł����A�����ǂݍ����
	// �X�e�[�W���쐬����
	if ( !CreateBlockMap("data/stage_info/stage1.bin") )
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBlockManager::Uninit()
{
	for ( int nLoopY = 0; nLoopY < MAX_BLOCK_Y; nLoopY++ )
	{
		for ( int nLoopX = 0; nLoopX < MAX_BLOCK_X; nLoopX++ )
		{
			if ( m_pBlockArray != nullptr )
			{
				m_pBlockArray[ nLoopY ][ nLoopX ] = nullptr;
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CBlockManager::Update()
{

}

//=============================================================================
// �X�e�[�W��������
// �����̐���
// p_stagemap_filename :�X�e�[�W��񂪏����ꂽ�t�@�C���̃p�X
// �Ԃ�l:bool�^ ���� > true, ���s > false
//=============================================================================
bool CBlockManager::CreateBlockMap(char *p_stagemap_filename)
{
	int nMaxStageWidth = 0, nMaxStageHeight = 0, nBlockID = 0;
	char cBlockID;

	FILE *file = fopen( p_stagemap_filename, "rb" );

	if ( file == nullptr )
	{
		return false;
	}

	ins = new CInstancingObject( 6 );
	ins -> LoadTexture("data/texture/block/block.jpg", D3DXVECTOR2( 1000.0f, 1500.0f ), D3DXVECTOR2( 100.0f, 100.0f ) );
	ins -> Init();

	for ( int nCntY = 0; nCntY < MAX_BLOCK_Y; nCntY++ )
	{

		for ( int nCntX = 0; nCntX < MAX_BLOCK_X; nCntX++ )
		{
			fread( &cBlockID, 1, 1, file );

			if ( cBlockID < 48 )
			{
				nCntX--;
			}

			nBlockID = atoi( &cBlockID );

			if ( nBlockID != 0 )
			{
				// �����Ńu���b�N�̐���
				CBlock *pBlock = new CBlock;
				pBlock -> SetInsObj( ins );
				pBlock -> Init();
				pBlock -> SetPosition( D3DXVECTOR3( nCntX * BLOCK_WIDTH, nCntY * BLOCK_HEIGHT, 0.0f ) );
				pBlock -> SetRotation( 0.0f, 0.0f, 0.0f );
				pBlock -> SetScaling( 50.0f, 50.0f );

				switch ( nBlockID )
				{
				case 1:		// �y�u���b�N
					pBlock -> SetBlockTexID( 0, 0 );
					break;

				case 2:
					pBlock -> SetBlockTexID( 1, 0 );
					break;

				case 3:
					pBlock -> SetBlockTexID( 2, 0 );
					break;

				case 4:
					pBlock -> SetBlockTexID( 3, 0 );
					break;

				case 5:
					break;

				default:
					break;
				}

				m_pBlockArray[nCntY][nCntX] = pBlock;
			}
		}
	}

	return true;
}

//=============================================================================
// �����蔻��p�̃u���b�N�̃A�h���X�Q�b�^�[
// �����̐���
// character_pos....�����蔻������I�u�W�F�N�g�̍��W�B�������画������ׂ�
// �u���b�N�̔z��ԍ������߂�B
//
// character_size...�z��ԍ������߂�ۂɕK�v�B���܂ł̂܂܂Ȃ���W�͍���Ȃ̂ŁA
// size�̔������炵�A���S�Ƃ��Ĕ��肷��B
//
// �Ԃ�l:CBlock*�^
//=============================================================================
CBlock* CBlockManager::GetBlock(D3DXVECTOR3 character_pos, D3DXVECTOR2 character_size)
{
	// X�͒��S�ŁAY�͑����Ŕ�������̂ŁA���ꂼ�ꂸ�炷
	int nBlockArrayIdxNumX = ( character_pos.x + ( character_size.x / 2.0f ) ) / BLOCK_WIDTH;
	int nBlockArrayIdxNumY = ( character_pos.y + character_size.y ) / BLOCK_HEIGHT;

	// �����̍��W�ɂ���u���b�N�̃A�h���X��Ԃ�
	return m_pBlockArray[ nBlockArrayIdxNumY ][ nBlockArrayIdxNumY ];
}

//=============================================================================
// �z��ɓo�^���ꂽ�u���b�N�̃A�h���X�Q�b�^�[
// �����̐���
// n_block_idx_x, n_block_idx_y :���ꂼ��̔z��ԍ�
//
// �Ԃ�l:CBlock*�^
//=============================================================================
CBlock* CBlockManager::GetBlock(int n_block_idx_x, int n_block_idx_y)
{
	return m_pBlockArray[ n_block_idx_x][n_block_idx_y];
}

// End of file