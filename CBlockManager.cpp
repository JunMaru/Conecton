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
#include "Utility.h"
#include <stdio.h>

//=============================================================================
// �O���[�o��
//=============================================================================
CInstancingObject *ins;

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CBlock *CBlockManager::m_pBlockArray[MAX_BLOCK_Y * MAX_BLOCK_X] = { nullptr };
CBlock *CBlockManager::m_pLaserStart = nullptr;
CBlock *CBlockManager::m_pLaserGoal = nullptr;

//=============================================================================
// ��������
//=============================================================================
CBlockManager* CBlockManager::Create( char *pFileName )
{
	CBlockManager *pBlockManager = new CBlockManager;
	pBlockManager -> m_pFileName = pFileName;
	pBlockManager -> Init();
	return pBlockManager;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBlockManager::Init()
{
	if ( !CreateBlockMap( m_pFileName ) )
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
				m_pBlockArray[ nLoopY * MAX_BLOCK_X + nLoopX ] = nullptr;
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
	int nBlockID = 0;

	int **ppCreateBlockMapArray = nullptr;

	// ���̊֐��̐����B
	Utility::LoadCsv( p_stagemap_filename, ppCreateBlockMapArray, &m_nMaxArrayNumX, &m_nMaxArrayNumY );

	ins = new CInstancingObject( 6 );
	ins -> LoadTexture("data/texture/block/block.jpg", D3DXVECTOR2( 1000.0f, 1500.0f ), D3DXVECTOR2( 100.0f, 100.0f ) );
	ins -> Init();

	for ( int nCntY = 0; nCntY < m_nMaxArrayNumY; nCntY++ )
	{

		for ( int nCntX = 0; nCntX < m_nMaxArrayNumX; nCntX++ )
		{
			nBlockID = ppCreateBlockMapArray[nCntY][nCntX];

			// �����Ȃ��Ƃ�������炩���߂͂������ƂŁA�R�[�h�ʂ�Z������
			if ( nBlockID != 0 )
			{
				// �����Ńu���b�N�̐���
				CBlock *pBlock = new CBlock;
				pBlock -> SetInsObj( ins );
				pBlock -> Init();
				pBlock -> SetPosition( D3DXVECTOR3( nCntX * BLOCK_WIDTH, nCntY * BLOCK_HEIGHT, 0.0f ) );
				pBlock -> SetRotation( 0.0f, 0.0f, 0.0f );
				pBlock -> SetScaling( 50.0f, 50.0f );
				pBlock -> SetBlockId( ( CBlock::BLOCKID )nBlockID );
				pBlock->SetSecondTexID( -1, -1 );

				switch ( nBlockID )
				{
				case CBlock::BLOCKID_SOIL:
					pBlock -> SetBlockTexID( 0, 0 );
					break;

				case CBlock::BLOCKID_GRASS:
					pBlock -> SetBlockTexID( 1, 0 );
					break;

				case CBlock::BLOCKID_SOIL_CRACK:
					pBlock -> SetBlockTexID( 2, 0 );
					break;

				case CBlock::BLOCKID_GRASS_CRACK:
					pBlock -> SetBlockTexID( 3, 0 );
					break;

				case CBlock::BLOCKID_METAL:
					pBlock -> SetBlockTexID( 0, 1 );
					break;

				case CBlock::BLOCKID_MINIMUM:
					pBlock -> SetBlockTexID( 0, 2 );
					break;

				case CBlock::BLOCKID_POWERFUL:
					pBlock -> SetBlockTexID( 0, 3 );
					break;

				case CBlock::BLOCKID_LASER_GOAL_TOP:
					pBlock -> SetBlockTexID( 0, 8 );
					m_pLaserGoal = pBlock;
					break;

				case CBlock::BLOCKID_LASER_GOAL_BOTTOM:
					pBlock -> SetBlockTexID( 1, 8 );
					m_pLaserGoal = pBlock;
					break;

				case CBlock::BLOCKID_LASER_GOAL_LEFT:
					pBlock -> SetBlockTexID( 2, 8 );
					m_pLaserGoal = pBlock;
					break;

				case CBlock::BLOCKID_LASER_GOAL_RIGHT:
					pBlock -> SetBlockTexID( 3, 8 );
					m_pLaserGoal = pBlock;
					break;

				case CBlock::BLOCKID_LASER_START_TOP:
					pBlock -> SetBlockTexID( 4, 8 );
					m_pLaserStart = pBlock;
					break;

				case CBlock::BLOCKID_LASER_START_BOTTOM:
					pBlock -> SetBlockTexID( 5, 8 );
					m_pLaserStart = pBlock;
					break;

				case CBlock::BLOCKID_LASER_START_LEFT:
					pBlock -> SetBlockTexID( 6, 8 );
					m_pLaserStart = pBlock;
					m_pLaserStart->SetPosition(D3DXVECTOR3(nCntX * BLOCK_WIDTH + 25.0f, nCntY * BLOCK_HEIGHT, 0.0f));
					break;

				case CBlock::BLOCKID_LASER_START_RIGHT:
					pBlock -> SetBlockTexID( 7, 8 );
					m_pLaserStart = pBlock;
					break;

				case CBlock::BLOCKID_LASER_CONTROL_DOWN:
					pBlock -> SetBlockTexID( 0, 10 );
					break;

				case CBlock::BLOCKID_LASER_CONTROL_UP:
					pBlock -> SetBlockTexID( 1, 10 );
					break;

				case CBlock::BLOCKID_LASER_CONTROL_RIGHT:
					pBlock -> SetBlockTexID( 2, 10 );
					break;

				case CBlock::BLOCKID_LASER_CONTROL_LEFT:
					pBlock -> SetBlockTexID( 3, 10 );
					break;

				case CBlock::BLOCKID_MAGNET:
					pBlock -> SetBlockTexID( 0, 11 );
					break;

				case CBlock::BLOCKID_WOODBOX:
					pBlock -> SetBlockTexID( 0, 12 );
					break;

				case CBlock::BLOCKID_SWITCH:
					pBlock -> SetBlockTexID( 0, 13 );
					break;

				case CBlock::BLOCKID_WARP:
					pBlock -> SetBlockTexID( 0, 14 );
					break;

				default:
					break;
				}

				// �ϐg�A�C�e���̉��n�̃e�N�X�`����ݒ�B��ɕʂ̃u���b�N������ꍇ��
				// �y�u���b�N��ݒ�B�����ꍇ�͑��u���b�N��ݒ�B
				if ( pBlock -> GetBlockId() > CBlock::BLOCKID_GRASS_CRACK
				  && pBlock -> GetBlockId() < CBlock::BLOCKID_LASER_GOAL_TOP
				  && m_pBlockArray[ ( nCntY - 1 ) * MAX_BLOCK_X + nCntX ] != nullptr )
				{
					pBlock -> SetSecondTexID( 0, 0 );
				}
				else if ( pBlock -> GetBlockId() > CBlock::BLOCKID_GRASS_CRACK
					   && pBlock -> GetBlockId() < CBlock::BLOCKID_LASER_GOAL_TOP
					   && m_pBlockArray[ ( nCntY - 1 ) * MAX_BLOCK_X + nCntX ] == nullptr )
				{
					pBlock->SetSecondTexID( 1, 0 );
				}

				m_pBlockArray[ nCntY * MAX_BLOCK_X + nCntX ] = pBlock;
			}
		}
	}

	Utility::Delete2DArrayInt( ppCreateBlockMapArray, m_nMaxArrayNumY );

	return true;
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
	if ( n_block_idx_x < MAX_BLOCK_X && n_block_idx_x >= 0
	  && n_block_idx_y < MAX_BLOCK_Y && n_block_idx_y >= 0
	  && m_pBlockArray[n_block_idx_y * MAX_BLOCK_X + n_block_idx_x] != nullptr)
	{
		return m_pBlockArray[n_block_idx_y * MAX_BLOCK_X + n_block_idx_x];
	}

	return nullptr;
}

// End of file