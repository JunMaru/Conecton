//=============================================================================
//
// CLaserManager 処理部分[ CLaserManager.cpp ]
// Author : Yutaka Ichikawa
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "CLaserManager.h"
#include "CLaser.h"
#include "CBlockManager.h"
#include "CBlock.h"
#include "CDebugProcDX9.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
int CLaserManager::m_nCurrentLaser = 0;		// 現在更新しているレーザーの番号
CLaser *CLaserManager::m_pLaserArray[MAX_LASER_NUM] = { nullptr };

//=============================================================================
// 生成処理
//=============================================================================
CLaserManager* CLaserManager::Create()
{
	CLaserManager *pLaserManager = new CLaserManager;
	pLaserManager -> Init();
	return pLaserManager;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLaserManager::Init()
{
	CBlock * pBlockStartLaser = CBlockManager::GetLaserStart();

	if (pBlockStartLaser != nullptr)
	{
		D3DXVECTOR3 laserStartPos = CBlockManager::GetLaserStart()->GetPosition();

		// スタートの種類によって、レーザーの向きを変える
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

	ZeroMemory(m_bStopLaserFlag, sizeof(bool) * MAX_LASER_NUM);

	m_bLaserGoalFlag = false;

	return E_FAIL;
}

//=============================================================================
// 終了処理
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
// 更新処理
//=============================================================================
void CLaserManager::Update()
{
	D3DXVECTOR3 laserPos = m_pLaserArray[m_nCurrentLaser]->GetPosition();
	D3DXVECTOR3 laserEndPos = m_pLaserArray[m_nCurrentLaser]->GetWorldEndPosition();
	D3DXVECTOR3 laserRot = m_pLaserArray[m_nCurrentLaser]->GetRotation();
	D3DXVECTOR3 workLaserPos = laserEndPos;

	int nBlockIdxX = 0;
	int nBlockIdxY = 0;

	bool bBlockHitCheck = false;

	CBlock *pBlock = nullptr;

	int nLaserDir = ( int )( laserRot.z / (D3DX_PI / 2.0f ) );

	// レーザーの向きから取ってくるブロックのアドレスを変更
	if ( nLaserDir == CLaser::DIRECTION_RIGHT )
	{
		workLaserPos.y += 25.0f;

		nBlockIdxX = (int)(workLaserPos.x / BLOCK_WIDTH);
		nBlockIdxY = (int)(workLaserPos.y / BLOCK_HEIGHT);

		pBlock = CBlockManager::GetBlock(nBlockIdxX + 1, nBlockIdxY );
	}
	else if ( nLaserDir == CLaser::DIRECTION_DOWN )
	{
		workLaserPos.x += 25.0f;

		nBlockIdxX = (int)(workLaserPos.x / BLOCK_WIDTH);
		nBlockIdxY = (int)(workLaserPos.y / BLOCK_HEIGHT);

		pBlock = CBlockManager::GetBlock(nBlockIdxX, nBlockIdxY + 1 );
	}
	else if ( nLaserDir == CLaser::DIRECTION_LEFT )
	{
		workLaserPos.y += 25.0f;

		nBlockIdxX = (int)(workLaserPos.x / BLOCK_WIDTH);
		nBlockIdxY = (int)(workLaserPos.y / BLOCK_HEIGHT);

		pBlock = CBlockManager::GetBlock(nBlockIdxX, nBlockIdxY );
	}
	else if ( nLaserDir == CLaser::DIRECTION_UP )
	{
		workLaserPos.x += 25.0f;

		nBlockIdxX = (int)(workLaserPos.x / BLOCK_WIDTH);
		nBlockIdxY = (int)(workLaserPos.y / BLOCK_HEIGHT);

		pBlock = CBlockManager::GetBlock(nBlockIdxX, nBlockIdxY );
	}

	// ブロックのアドレスがなかったら、処理終了
	if (pBlock == nullptr)
	{
		if (m_bStopLaserFlag[m_nCurrentLaser] == true)
		{
			m_bStopLaserFlag[m_nCurrentLaser] = false;

			m_pLaserArray[m_nCurrentLaser] -> MoveRestart();

			return;
		}
		else
		{
			return;
		}
	}

	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);
	D3DXVECTOR3 blockPos = pBlock -> GetPosition() + blockSize;

	// 当たっていた場合は、レーザーを伸ばさない
	if (nLaserDir == CLaser::DIRECTION_RIGHT)
	{
		if ( workLaserPos.x > blockPos.x - BLOCK_WIDTH
		  && m_bStopLaserFlag[ m_nCurrentLaser ] == false )
		{
			m_pLaserArray[m_nCurrentLaser]->MoveStop();
			m_bStopLaserFlag[m_nCurrentLaser] = true;

			bBlockHitCheck = true;
		}
	}
	else if (nLaserDir == CLaser::DIRECTION_DOWN)
	{
		if (workLaserPos.y > blockPos.y - BLOCK_HEIGHT
		  && m_bStopLaserFlag[m_nCurrentLaser] == false )
		{
			m_pLaserArray[m_nCurrentLaser]->MoveStop();
			m_bStopLaserFlag[m_nCurrentLaser] = true;

			bBlockHitCheck = true;
		}
	}
	else if (nLaserDir == CLaser::DIRECTION_LEFT)
	{
		if (workLaserPos.x < blockPos.x + blockSize.x
		  && m_bStopLaserFlag[m_nCurrentLaser] == false )
		{
			m_pLaserArray[m_nCurrentLaser]->MoveStop();
			m_bStopLaserFlag[m_nCurrentLaser] = true;

			bBlockHitCheck = true;
		}
	}
	else if (nLaserDir == CLaser::DIRECTION_UP)
	{
		if (workLaserPos.y < blockPos.y + BLOCK_HEIGHT
		  && m_bStopLaserFlag[m_nCurrentLaser] == false )
		{
			m_pLaserArray[m_nCurrentLaser]->MoveStop();
			m_bStopLaserFlag[m_nCurrentLaser] = true;

			bBlockHitCheck = true;
		}
	}

	// 当たったらレーザーを制御ブロックの種類により向きを変更して生成
	if (bBlockHitCheck == true)
	{
		CBlock::BLOCKID blockId = pBlock -> GetBlockId();

		switch (blockId)
		{
		case CBlock::BLOCKID_LASER_CONTROL_DOWN:

			if ( nLaserDir == CLaser::DIRECTION_RIGHT )
			{
				laserEndPos = D3DXVECTOR3(laserEndPos.x - 25.0f, laserEndPos.y - 25.0f, 0.0f);
			}
			else if ( nLaserDir == CLaser::DIRECTION_LEFT )
			{
				laserEndPos = D3DXVECTOR3(laserEndPos.x, laserEndPos.y - 25.0f, 0.0f);
			}

			CreateLaser(laserEndPos, CLaser::DIRECTION_DOWN);
			break;

		case CBlock::BLOCKID_LASER_CONTROL_UP:

			if (nLaserDir == CLaser::DIRECTION_RIGHT)
			{
				laserEndPos = D3DXVECTOR3(laserEndPos.x - 25.0f, laserEndPos.y + 25.0f, 0.0f);
			}
			else if (nLaserDir == CLaser::DIRECTION_LEFT)
			{
				laserEndPos = D3DXVECTOR3(laserEndPos.x, laserEndPos.y + 25.0f, 0.0f);
			}

			CreateLaser(laserEndPos, CLaser::DIRECTION_UP);

			break;

		case CBlock::BLOCKID_LASER_CONTROL_RIGHT:

			if (nLaserDir == CLaser::DIRECTION_DOWN)
			{
				laserEndPos = D3DXVECTOR3(laserEndPos.x - 25.0f, laserEndPos.y - 25.0f, 0.0f);
			}
			else if (nLaserDir == CLaser::DIRECTION_UP)
			{
				laserEndPos = D3DXVECTOR3(laserEndPos.x - 25.0f, laserEndPos.y, 0.0f);
			}

			CreateLaser(laserEndPos, CLaser::DIRECTION_RIGHT);
			break;

		case CBlock::BLOCKID_LASER_CONTROL_LEFT:

			if (nLaserDir == CLaser::DIRECTION_DOWN)
			{
				laserEndPos = D3DXVECTOR3(laserEndPos.x + 25.0f, laserEndPos.y - 35.0f, 0.0f);
			}
			else if (nLaserDir == CLaser::DIRECTION_UP)
			{
				laserEndPos = D3DXVECTOR3(laserEndPos.x + 25.0f, laserEndPos.y, 0.0f);
			}

			CreateLaser(laserEndPos, CLaser::DIRECTION_LEFT);
			break;

		default:
			break;
		}
	}

	// レーザーとゴールの当たり判定
	pBlock = CBlockManager::GetLaserGoal();
	blockPos = pBlock->GetPosition() + blockSize;

	if (laserEndPos.x + blockSize.x > blockPos.x - blockSize.x
	 && laserEndPos.x + blockSize.x < blockPos.x + blockSize.x
	 && laserEndPos.y + blockSize.y > blockPos.y - blockSize.y
	 && laserEndPos.y + blockSize.y < blockPos.y + blockSize.y)
	{
		m_bLaserGoalFlag = true;
	}

	if (m_bLaserGoalFlag == true)
	{
		CDebugProcDX9::Print("終了\n");
	}
}

//=============================================================================
// レーザー生成処理
//=============================================================================
void CLaserManager::CreateLaser(const D3DXVECTOR3& start_pos, int direction)
{
	m_nCurrentLaser++;

	m_pLaserArray[m_nCurrentLaser] = CLaser::Create( start_pos, ( CLaser::DIRECTION )direction );
}

// End of file