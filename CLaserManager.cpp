//=============================================================================
//
// CLaserManager ��������[ CLaserManager.cpp ]
// Author : Yutaka Ichikawa
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CLaserManager.h"
#include "CLaser.h"
#include "CBlockManager.h"
#include "CBlock.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
int CLaserManager::m_nCurrentLaser = 0;		// ���ݍX�V���Ă��郌�[�U�[�̔ԍ�
CLaser *CLaserManager::m_pLaserArray[MAX_LASER_NUM] = { nullptr };

//=============================================================================
// ��������
//=============================================================================
CLaserManager* CLaserManager::Create()
{
	CLaserManager *pLaserManager = new CLaserManager;
	pLaserManager -> Init();
	return pLaserManager;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLaserManager::Init()
{
	CBlock * pBlockStartLaser = CBlockManager::GetLaserStart();

	if (pBlockStartLaser != nullptr)
	{
		D3DXVECTOR3 laserStartPos = CBlockManager::GetLaserStart()->GetPosition();

		// �X�^�[�g�̎�ނɂ���āA���[�U�[�̌�����ς���
		CLaser::DIRECTION direction;

		CBlock::BLOCKID blockId = CBlockManager::GetLaserStart()->GetBlockId();

		switch (blockId)
		{
		case CBlock::BLOCKID_LASER_START_TOP:
			direction = CLaser::DIRECTION_DOWN;
			break;
		case CBlock::BLOCKID_LASER_START_BOTTOM:
			direction = CLaser::DIRECTION_UP;
			break;
		case CBlock::BLOCKID_LASER_START_LEFT:
			direction = CLaser::DIRECTION_RIGHT;
			break;
		case CBlock::BLOCKID_LASER_START_RIGHT:
			direction = CLaser::DIRECTION_LEFT;
			break;
		default:
			break;
		}

		m_pLaserArray[m_nCurrentLaser] = CLaser::Create(laserStartPos, direction);
	}
	else
	{
		m_pLaserArray[m_nCurrentLaser] = CLaser::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f ), CLaser::DIRECTION_RIGHT);
	}

	return E_FAIL;
}

//=============================================================================
// �I������
//=============================================================================
void CLaserManager::Uninit()
{
	for (int nCnt = 0; nCnt < MAX_LASER_NUM; nCnt++)
	{
		if (m_pLaserArray[nCnt] != nullptr)
		{
			m_pLaserArray[nCnt] = nullptr;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CLaserManager::Update()
{
	D3DXVECTOR3 laserPos = m_pLaserArray[m_nCurrentLaser]->GetPosition();
	D3DXVECTOR3 laserEndPos = m_pLaserArray[m_nCurrentLaser]->GetWorldEndPosition();
	D3DXVECTOR3 laserRot = m_pLaserArray[m_nCurrentLaser]->GetRotation();

	int nBlockIdxX = (int)(laserEndPos.x / BLOCK_WIDTH);
	int nBlockIdxY = (int)(laserEndPos.y / BLOCK_HEIGHT);

	bool bBlockHitCheck = false;

	CBlock *pBlock = nullptr;

	int nLaserDir = ( int )( laserRot.z / (D3DX_PI / 2.0f ) );

	// ���[�U�[�̌����������Ă���u���b�N�̃A�h���X��ύX
	if ( nLaserDir == CLaser::DIRECTION_RIGHT )
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + 1, nBlockIdxY );
	}
	else if ( nLaserDir == CLaser::DIRECTION_DOWN )
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX, nBlockIdxY + 1 );
	}
	else if ( nLaserDir == CLaser::DIRECTION_LEFT )
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX - 1, nBlockIdxY );
	}
	else if ( nLaserDir == CLaser::DIRECTION_UP )
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX, nBlockIdxY - 1 );
	}

	// �A�h���X���Ȃ������ꍇ�́A�����I��
	if (pBlock == nullptr)
	{
		return;
	}

	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);
	D3DXVECTOR3 blockPos = pBlock -> GetPosition() + blockSize;

	// �������Ă����ꍇ�́A���[�U�[��L�΂��Ȃ�
	if (nLaserDir == CLaser::DIRECTION_RIGHT)
	{
		if ( laserEndPos.x + 25.0f > blockPos.x - blockSize.x
		  && laserEndPos.x + 25.0f < blockPos.x + blockSize.x
		  && laserEndPos.y > blockPos.y - blockSize.y
		  && laserEndPos.y < blockPos.y + blockSize.y )
		{
			m_pLaserArray[m_nCurrentLaser]->MoveStop();

			bBlockHitCheck = true;
		}
	}
	else if (nLaserDir == CLaser::DIRECTION_DOWN)
	{
		if ( laserEndPos.x > blockPos.x - blockSize.x
		  && laserEndPos.x < blockPos.x + blockSize.x
		  && laserEndPos.y + 25.0f > blockPos.y - blockSize.y
		  && laserEndPos.y + 25.0f < blockPos.y + blockSize.y)
		{
			m_pLaserArray[m_nCurrentLaser]->MoveStop();

			bBlockHitCheck = true;
		}
	}
	else if (nLaserDir == CLaser::DIRECTION_LEFT)
	{
		if ( laserEndPos.x - 25.0f > blockPos.x - blockSize.x
		  && laserEndPos.x - 25.0f < blockPos.x + blockSize.x
		  && laserEndPos.y > blockPos.y - blockSize.y
		  && laserEndPos.y < blockPos.y + blockSize.y)
		{
			m_pLaserArray[m_nCurrentLaser]->MoveStop();

			bBlockHitCheck = true;
		}
	}
	else if (nLaserDir == CLaser::DIRECTION_UP)
	{
		if ( laserEndPos.x > blockPos.x - blockSize.x
		  && laserEndPos.x < blockPos.x + blockSize.x
		  && laserEndPos.y - 25.0f > blockPos.y - blockSize.y
		  && laserEndPos.y - 25.0f < blockPos.y + blockSize.y)
		{
			m_pLaserArray[m_nCurrentLaser]->MoveStop();

			bBlockHitCheck = true;
		}
	}

	// ���������烌�[�U�[�𐧌�u���b�N�̎�ނɂ�������ύX���Đ���
	if (bBlockHitCheck == true)
	{
		CBlock::BLOCKID blockId = pBlock -> GetBlockId();

		switch (blockId)
		{
		case CBlock::BLOCKID_LASER_CONTROL_DOWN:

			CreateLaser(laserEndPos, CLaser::DIRECTION_DOWN);
			break;

		case CBlock::BLOCKID_LASER_CONTROL_UP:

			CreateLaser(laserEndPos, CLaser::DIRECTION_UP);
			break;

		case CBlock::BLOCKID_LASER_CONTROL_RIGHT:

			CreateLaser(laserEndPos, CLaser::DIRECTION_RIGHT);
			break;

		case CBlock::BLOCKID_LASER_CONTROL_LEFT:

			CreateLaser(laserEndPos, CLaser::DIRECTION_LEFT);
			break;

		default:
			break;
		}
	}
}

//=============================================================================
// ���[�U�[��������
//=============================================================================
void CLaserManager::CreateLaser(const D3DXVECTOR3& start_pos, const int & direction)
{

}

// End of file