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
#include "CGimickBlock.h"
#include "CWoodBox.h"
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

CWoodBox *CBlockManager::m_pWoodBoxArray[MAX_WOOD_BOX] = { nullptr };

//=============================================================================
// ��������
//=============================================================================
CBlockManager* CBlockManager::Create(char *pFileName)
{
	CBlockManager *pBlockManager = new CBlockManager;
	pBlockManager->m_pFileName = pFileName;
	pBlockManager->Init();
	return pBlockManager;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBlockManager::Init()
{
	m_nFoodNum = 0;
	m_nWoodBoxCnt = 0;

	if (!CreateBlockMap(m_pFileName))
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
	for (int nLoopY = 0; nLoopY < MAX_BLOCK_Y; nLoopY++)
	{
		for (int nLoopX = 0; nLoopX < MAX_BLOCK_X; nLoopX++)
		{
			if ( m_pBlockArray[ nLoopY * MAX_BLOCK_X + nLoopX ] != nullptr )
			{
				m_pBlockArray[ nLoopY * MAX_BLOCK_X + nLoopX ] = nullptr;
			}
		}
	}

	for ( int nLoop = 0; nLoop < MAX_WOOD_BOX; nLoop++ )
	{
		if ( m_pWoodBoxArray[ nLoop ] != nullptr )
		{
			m_pWoodBoxArray[ nLoop ] = nullptr;
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
	CBlock *pBlock = nullptr;

	int **ppCreateBlockMapArray = nullptr;

	// ���̊֐��̐����B
	Utility::LoadCsv(p_stagemap_filename, ppCreateBlockMapArray, &m_nMaxArrayNumX, &m_nMaxArrayNumY);

	ins = new CInstancingObject(3);
	ins->LoadTexture("data/texture/block/block.png", D3DXVECTOR2(1000.0f, 1900.0f), D3DXVECTOR2(100.0f, 100.0f));
	ins->Init();

	D3DXVECTOR3 laserStartPos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	for (int nCntY = 0; nCntY < m_nMaxArrayNumY; nCntY++)
	{

		for (int nCntX = 0; nCntX < m_nMaxArrayNumX; nCntX++)
		{
			nBlockID = ppCreateBlockMapArray[nCntY][nCntX];

			// �����Ȃ��Ƃ�������炩���߂͂������ƂŁA�R�[�h�ʂ�Z������
			if (nBlockID != 0)
			{
				switch (nBlockID)
				{
				case CBlock::BLOCKID_SOIL:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(0, 0);
					break;

				case CBlock::BLOCKID_GRASS:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(1, 0);
					break;

				case CBlock::BLOCKID_SOIL_CRACK:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(2, 0);
					break;

				case CBlock::BLOCKID_GRASS_CRACK:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(3, 0);
					break;

				case CBlock::BLOCKID_METAL:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(0, 1);
					break;

				case CBlock::BLOCKID_MINIMUM:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(0, 2);
					break;

				case CBlock::BLOCKID_POWERFUL:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(0, 3);
					break;

				case CBlock::BLOCKID_LASER_GOAL_TOP:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(4, 8);
					m_pLaserGoal = pBlock;
					break;

				case CBlock::BLOCKID_LASER_GOAL_BOTTOM:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(5, 8);
					m_pLaserGoal = pBlock;
					break;

				case CBlock::BLOCKID_LASER_GOAL_LEFT:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(6, 8);
					m_pLaserGoal = pBlock;
					break;

				case CBlock::BLOCKID_LASER_GOAL_RIGHT:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(7, 8);
					m_pLaserGoal = pBlock;
					break;

				case CBlock::BLOCKID_LASER_START_TOP:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(0, 9);
					m_pLaserStart = pBlock;
					break;

				case CBlock::BLOCKID_LASER_START_BOTTOM:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(1, 9);
					m_pLaserStart = pBlock;
					break;

				case CBlock::BLOCKID_LASER_START_LEFT:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(2, 9);
					m_pLaserStart = pBlock;
					laserStartPos = D3DXVECTOR3(nCntX * BLOCK_WIDTH + 25.0f, nCntY * BLOCK_HEIGHT, 0.0f);
					break;

				case CBlock::BLOCKID_LASER_START_RIGHT:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(3, 9);
					m_pLaserStart = pBlock;
					break;

				case CBlock::BLOCKID_LASER_CONTROL_DOWN:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(0, 11);
					break;

				case CBlock::BLOCKID_LASER_CONTROL_UP:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(1, 11);
					break;

				case CBlock::BLOCKID_LASER_CONTROL_RIGHT:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(2, 11);
					break;

				case CBlock::BLOCKID_LASER_CONTROL_LEFT:
					pBlock = new CBlock;
					pBlock->SetBlockTexID(3, 11);
					break;

				case CBlock::BLOCKID_MAGNET:
					pBlock = (CBlock*)new CGimmickBlock;
					pBlock->SetBlockTexID(0, 12);
					break;

				case CBlock::BLOCKID_WOODBOX:
					pBlock = (CBlock*)new CWoodBox;
					pBlock->SetBlockTexID(0, 13);
					break;

				case CBlock::BLOCKID_SWITCH:
					pBlock = (CBlock*)new CGimmickBlock;
					pBlock->SetBlockTexID(0, 14);
					break;

				case CBlock::BLOCKID_WARP_BLUE:
					pBlock = (CBlock*)new CGimmickBlock;
					pBlock->SetBlockTexID(0, 15);
					break;

				case CBlock::BLOCKID_WARP_GREEN:
					pBlock = (CBlock*)new CGimmickBlock;
					pBlock->SetBlockTexID(0, 16);
					break;

				case CBlock::BLOCKID_WARP_PINK:
					pBlock = (CBlock*)new CGimmickBlock;
					pBlock->SetBlockTexID(0, 17);
					break;

				case CBlock::BLOCKID_NO_METAMOR:
					pBlock = (CBlock*)new CGimmickBlock;
					pBlock->SetBlockTexID(0, 5);
					break;

				case CBlock::BLOCKID_FOOD_ACORN:
					pBlock = new CGimmickBlock;
					pBlock->SetBlockTexID(0, 18);
					++m_nFoodNum;
					break;

				case CBlock::BLOCKID_FOOD_APPLE:
					pBlock = new CGimmickBlock;
					pBlock->SetBlockTexID(1, 18);
					++m_nFoodNum;
					break;

				case CBlock::BLOCKID_FOOD_MUSHROOM:
					pBlock = new CGimmickBlock;
					pBlock->SetBlockTexID(2, 18);
					++m_nFoodNum;
					break;

				default:
					break;
				}

				// �����Ńu���b�N�̐���
				pBlock->SetInsObj(ins);
				pBlock->Init();
				pBlock->SetPosition(D3DXVECTOR3(nCntX * BLOCK_WIDTH, nCntY * BLOCK_HEIGHT, 0.0f));
				pBlock->SetRotation(0.0f, 0.0f, 0.0f);
				pBlock->SetScaling(50.0f, 50.0f);
				pBlock->SetBlockId((CBlock::BLOCKID)nBlockID);

				// �ϐg�A�C�e���̉��n�̃e�N�X�`����ݒ�B��ɕʂ̃u���b�N������ꍇ��
				// �y�u���b�N��ݒ�B�����ꍇ�͑��u���b�N��ݒ�B
				if (pBlock->GetBlockId() > CBlock::BLOCKID_GRASS_CRACK
					&& pBlock->GetBlockId() < CBlock::BLOCKID_LASER_GOAL_TOP
					&& m_pBlockArray[(nCntY - 1) * MAX_BLOCK_X + nCntX] != nullptr)
				{
					pBlock->SetSecondTexID(0, 0);
				}
				else if (pBlock->GetBlockId() > CBlock::BLOCKID_GRASS_CRACK
					&& pBlock->GetBlockId() < CBlock::BLOCKID_LASER_GOAL_TOP
					&& m_pBlockArray[(nCntY - 1) * MAX_BLOCK_X + nCntX] == nullptr)
				{
					pBlock->SetSecondTexID(1, 0);
				}
				else
				{
					pBlock->SetSecondTexID(-1, -1);
				}

				if ( nBlockID == CBlock::BLOCKID_WOODBOX )
				{
					m_pWoodBoxArray[ m_nWoodBoxCnt ] = ( CWoodBox* )pBlock;

					m_nWoodBoxCnt++;
				}
				else
				{
					m_pBlockArray[ nCntY * MAX_BLOCK_X + nCntX ] = pBlock;
				}
			}
		}
	}

	// CSV�t�@�C���̌�n��
	Utility::Delete2DArrayInt(ppCreateBlockMapArray, m_nMaxArrayNumY);

	m_pLaserStart->SetPosition(laserStartPos);

	// ���[�v�|�C���g�������N������֐�
	SetWarpPoint();

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
	if (n_block_idx_x < MAX_BLOCK_X && n_block_idx_x >= 0
		&& n_block_idx_y < MAX_BLOCK_Y && n_block_idx_y >= 0
		&& m_pBlockArray[n_block_idx_y * MAX_BLOCK_X + n_block_idx_x] != nullptr)
	{
		return m_pBlockArray[n_block_idx_y * MAX_BLOCK_X + n_block_idx_x];
	}

	return nullptr;
}

//=============================================================================
// �u���b�N�z��ԍ��Z�o�֐�
// �����̐���
//    pos    :�ΏۃI�u�W�F�N�g�̒��S���W
// n_array_x :�z��ԍ�-X������ꏊ
// n_array_y :�z��ԍ�-Y������ꏊ
//=============================================================================
void CBlockManager::CalculateBlockArrayNum(D3DXVECTOR3 pos, int *p_array_x, int *p_array_y)
{
	*p_array_x = (int)(pos.x / BLOCK_WIDTH);
	*p_array_y = (int)(pos.y / BLOCK_HEIGHT);
}

//=============================================================================
// �u���b�NID�Q�b�^�[
// �����̐���
// pos :�ΏۃI�u�W�F�N�g�̒��S���W
//=============================================================================
CBlock::BLOCKID CBlockManager::GetBlockID(D3DXVECTOR3 pos)
{
	int nArrayX = 0, nArrayY = 0;

	// ���݈ʒu�̃u���b�N�A�h���X������
	CalculateBlockArrayNum(pos, &nArrayX, &nArrayY);
	CBlock *pBlock = GetBlock(nArrayX, nArrayY);

	// �u���b�N�̎�ނ�Ԃ�
	if (pBlock != nullptr)
	{
		return pBlock->GetBlockId();
	}
	else
	{
		return CBlock::BLOCKID_NONE;
	}
}

int CBlockManager::GetBlockHitLaserNo(D3DXVECTOR3 pos)
{
	int nArrayX = 0, nArrayY = 0;

	// ���݈ʒu�̃u���b�N�A�h���X������
	CalculateBlockArrayNum(pos, &nArrayX, &nArrayY);
	CBlock *pBlock = GetBlock(nArrayX, nArrayY);

	if (pBlock != nullptr)
	{
		return pBlock->GetHitLaserNo();
	}
	else
	{
		return -1;
	}
}

//=============================================================================
// �M�~�b�N�u���b�N�֏㏑������֐�
// �����̐���
//  block_type :�u���b�N�̎��
//     pos     :�ΏۃI�u�W�F�N�g�̒��S���W
//=============================================================================
void CBlockManager::OverwriteGimmickBlock(CBlock::BLOCKID block_type, D3DXVECTOR3 pos)
{
	int nArrayX = 0, nArrayY = 0;

	// ���݈ʒu�̃u���b�N�̃A�h���X������
	CalculateBlockArrayNum(pos, &nArrayX, &nArrayY);
	CBlock *pBlock = GetBlock(nArrayX, nArrayY);

	// �A�h���X������Ƃ�
	if (pBlock != nullptr)
	{
		// �O�̃A�h���X�̂Ƃ����j��
		pBlock->Uninit();
		pBlock = nullptr;
	}

	// �㏑���^�C�v��BLOCK_NONE�������ꍇ
	if (block_type == CBlock::BLOCKID_NONE)
	{
		m_pBlockArray[nArrayY * MAX_BLOCK_X + nArrayX] = nullptr;
		return;
	}

	// �M�~�b�N�u���b�N�𐶐����A�z��ɓo�^����
	pBlock = (CBlock*)new CGimmickBlock();
	pBlock->SetInsObj(ins);
	pBlock->Init();
	pBlock->SetPosition(D3DXVECTOR3(nArrayX * BLOCK_WIDTH, nArrayY * BLOCK_HEIGHT, 0.0f));
	pBlock->SetRotation(0.0f, 0.0f, 0.0f);
	pBlock->SetScaling(50.0f, 50.0f);
	pBlock->SetBlockId(block_type);

	switch (block_type)
	{
		case CBlock::BLOCKID_SOIL:
			pBlock->SetBlockTexID(0, 0);
			break;

		case CBlock::BLOCKID_GRASS:
			pBlock->SetBlockTexID(1, 0);
			break;

		case CBlock::BLOCKID_SOIL_CRACK:
			pBlock->SetBlockTexID(2, 0);
			break;

		case CBlock::BLOCKID_GRASS_CRACK:
			pBlock->SetBlockTexID(3, 0);
			break;

		case CBlock::BLOCKID_METAL:
			pBlock->SetBlockTexID(0, 1);
			break;

		case CBlock::BLOCKID_MINIMUM:
			pBlock->SetBlockTexID(0, 2);
			break;

		case CBlock::BLOCKID_POWERFUL:
			pBlock->SetBlockTexID(0, 3);
			break;

		case CBlock::BLOCKID_LASER_GOAL_TOP:
			pBlock->SetBlockTexID(0, 8);
			break;

		case CBlock::BLOCKID_LASER_GOAL_BOTTOM:
			pBlock->SetBlockTexID(1, 8);
			break;

		case CBlock::BLOCKID_LASER_GOAL_LEFT:
			pBlock->SetBlockTexID(2, 8);
			break;

		case CBlock::BLOCKID_LASER_GOAL_RIGHT:
			pBlock->SetBlockTexID(3, 8);
			break;

		case CBlock::BLOCKID_LASER_START_TOP:
			pBlock->SetBlockTexID(0, 9);
			break;

		case CBlock::BLOCKID_LASER_START_BOTTOM:
			pBlock->SetBlockTexID(1, 9);
			break;

		case CBlock::BLOCKID_LASER_START_LEFT:
			pBlock->SetBlockTexID(2, 9);
			break;

		case CBlock::BLOCKID_LASER_START_RIGHT:
			pBlock->SetBlockTexID(3, 9);
			break;

		case CBlock::BLOCKID_LASER_CONTROL_DOWN:
			pBlock->SetBlockTexID(0, 11);
			break;

		case CBlock::BLOCKID_LASER_CONTROL_UP:
			pBlock->SetBlockTexID(1, 11);
			break;

		case CBlock::BLOCKID_LASER_CONTROL_RIGHT:
			pBlock->SetBlockTexID(2, 11);
			break;

		case CBlock::BLOCKID_LASER_CONTROL_LEFT:
			pBlock->SetBlockTexID(3, 11);
			break;

		case CBlock::BLOCKID_MAGNET:
			pBlock->SetBlockTexID(0, 12);
			break;

		case CBlock::BLOCKID_WOODBOX:
			pBlock->SetBlockTexID(0, 13);
			break;

		case CBlock::BLOCKID_SWITCH:
			pBlock->SetBlockTexID(0, 14);
			break;

		case CBlock::BLOCKID_WARP_BLUE:
			pBlock->SetBlockTexID(0, 15);
			break;

		case CBlock::BLOCKID_WARP_GREEN:
			pBlock->SetBlockTexID(0, 16);
			break;

		case CBlock::BLOCKID_WARP_PINK:
			pBlock->SetBlockTexID(0, 17);
			break;

		case CBlock::BLOCKID_NO_METAMOR:
			pBlock->SetBlockTexID(0, 5);
			break;

		case CBlock::BLOCKID_FOOD_ACORN:
			pBlock->SetBlockTexID(0, 18);
			break;

		case CBlock::BLOCKID_FOOD_APPLE:
			pBlock->SetBlockTexID(1, 18);
			break;

		case CBlock::BLOCKID_FOOD_MUSHROOM:
			pBlock->SetBlockTexID(2, 18);
			break;

		default:
			break;
	}

	m_pBlockArray[nArrayY * MAX_BLOCK_X + nArrayX] = pBlock;

	// ��Ƀu���b�N�����݂��邩�ǂ���
	bool bExistOnOneBlock = m_pBlockArray[(nArrayY - 1) * MAX_BLOCK_X + nArrayX] != nullptr;
	bool bNotExistOnOneBlock = m_pBlockArray[(nArrayY - 1) * MAX_BLOCK_X + nArrayX] == nullptr;

	// ���n�u���b�N�̐ݒ�
	if(pBlock->GetBlockId() == CBlock::BLOCKID_NO_METAMOR
	&& bExistOnOneBlock)
	{
		pBlock->SetSecondTexID(0, 0);
	}
	else if(pBlock->GetBlockId() == CBlock::BLOCKID_NO_METAMOR
	&& bNotExistOnOneBlock)
	{
		pBlock->SetSecondTexID(1, 0);
	}
	else
	{
		pBlock->SetSecondTexID(-1, -1);
	}
}

//=============================================================================
// ���[�v�u���b�N�]�ڐ�ݒ�֐�
//=============================================================================
void CBlockManager::SetWarpPoint()
{
	CGimmickBlock *pWarpBlue1 = nullptr, *pWarpBlue2 = nullptr,
				  *pWarpGreen1 = nullptr, *pWarpGreen2 = nullptr,
				  *pWarpPink1 = nullptr, *pWarpPink2 = nullptr;

	// ���[�v�u���b�N����
	for (int nCntY = 0; nCntY < MAX_BLOCK_Y; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_BLOCK_X; nCntX++)
		{
			if (m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX] != nullptr)
			{
				// �u���b�N�̎�ނ����[�v�������ꍇ
				if (m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX]->GetBlockId() == CBlock::BLOCKID_WARP_BLUE)
				{
					// �P�ڂ̃A�h���X���ݒ肳��Ă��Ȃ�������P�ڂɁB
					// �ݒ�ς݂Ȃ�Q�ڂɐݒ肷��B
					if (pWarpBlue1 == nullptr)
					{
						pWarpBlue1 = ( CGimmickBlock* )m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX];
					}
					else if ( pWarpBlue1 != nullptr && pWarpBlue2 == nullptr )
					{
						pWarpBlue2 = ( CGimmickBlock* )m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX];
					}
				}
				// �u���b�N�̎�ނ����[�v�΂������ꍇ
				else if (m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX]->GetBlockId() == CBlock::BLOCKID_WARP_GREEN)
				{
					if (pWarpGreen1 == nullptr)
					{
						pWarpGreen1 = (CGimmickBlock*)m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX];
					}
					else
					{
						pWarpGreen2 = (CGimmickBlock*)m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX];
					}
				}
				// �u���b�N�̎�ނ����[�v���������ꍇ
				else if (m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX]->GetBlockId() == CBlock::BLOCKID_WARP_PINK)
				{
					if (pWarpPink1 == nullptr)
					{
						pWarpPink1 = (CGimmickBlock*)m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX];
					}
					else
					{
						pWarpPink2 = (CGimmickBlock*)m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX];
					}
				}
			}

			// �Ή�����Q�̃��[�v�ɃA�h���X���o�^����Ă�����A���̂Q��
			// �Ȃ��Anull�ŏ���������B
			if (pWarpBlue1 != nullptr && pWarpBlue2 != nullptr)
			{
				pWarpBlue1->SetWarpPoint(pWarpBlue2);
				pWarpBlue2->SetWarpPoint(pWarpBlue1);

				pWarpBlue1 = nullptr;
				pWarpBlue2 = nullptr;
			}

			if (pWarpGreen1 != nullptr && pWarpGreen2 != nullptr)
			{
				pWarpGreen1->SetWarpPoint(pWarpGreen2);
				pWarpGreen2->SetWarpPoint(pWarpGreen1);

				pWarpGreen1 = nullptr;
				pWarpGreen2 = nullptr;
			}

			if (pWarpPink1 != nullptr && pWarpPink2 != nullptr)
			{
				pWarpPink1->SetWarpPoint(pWarpPink2);
				pWarpPink2->SetWarpPoint(pWarpPink1);

				pWarpPink1 = nullptr;
				pWarpPink2 = nullptr;
			}
		}
	}
}

// End of file