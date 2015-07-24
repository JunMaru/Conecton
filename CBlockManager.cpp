//=============================================================================
//
// CBlockManager 処理部分[ CBlockManager.cpp ]
// Author : Yutaka Ichikawa
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "CInstancingObject.h"
#include "CBlockManager.h"
#include "CBlock.h"
#include "CGimickBlock.h"
#include "CWoodBox.h"
#include "Utility.h"
#include <stdio.h>

//=============================================================================
// グローバル
//=============================================================================
CInstancingObject *ins;

//=============================================================================
// 静的メンバ変数
//=============================================================================
CBlock *CBlockManager::m_pBlockArray[MAX_BLOCK_Y * MAX_BLOCK_X] = { nullptr };
CBlock *CBlockManager::m_pLaserStart = nullptr;
CBlock *CBlockManager::m_pLaserGoal = nullptr;

CWoodBox *CBlockManager::m_pWoodBoxArray[MAX_WOOD_BOX] = { nullptr };

//=============================================================================
// 生成処理
//=============================================================================
CBlockManager* CBlockManager::Create(char *pFileName)
{
	CBlockManager *pBlockManager = new CBlockManager;
	pBlockManager->m_pFileName = pFileName;
	pBlockManager->Init();
	return pBlockManager;
}

//=============================================================================
// 初期化処理
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
// 終了処理
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
// 更新処理
//=============================================================================
void CBlockManager::Update()
{

}

//=============================================================================
// ステージ生成処理
// 引数の説明
// p_stagemap_filename :ステージ情報が書かれたファイルのパス
// 返り値:bool型 成功 > true, 失敗 > false
//=============================================================================
bool CBlockManager::CreateBlockMap(char *p_stagemap_filename)
{
	int nBlockID = 0;
	CBlock *pBlock = nullptr;

	int **ppCreateBlockMapArray = nullptr;

	// この関数の説明。
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

			// 何もないところをあらかじめはじくことで、コード量を短くする
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

				// ここでブロックの生成
				pBlock->SetInsObj(ins);
				pBlock->Init();
				pBlock->SetPosition(D3DXVECTOR3(nCntX * BLOCK_WIDTH, nCntY * BLOCK_HEIGHT, 0.0f));
				pBlock->SetRotation(0.0f, 0.0f, 0.0f);
				pBlock->SetScaling(50.0f, 50.0f);
				pBlock->SetBlockId((CBlock::BLOCKID)nBlockID);

				// 変身アイテムの下地のテクスチャを設定。上に別のブロックがある場合は
				// 土ブロックを設定。無い場合は草ブロックを設定。
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

	// CSVファイルの後始末
	Utility::Delete2DArrayInt(ppCreateBlockMapArray, m_nMaxArrayNumY);

	m_pLaserStart->SetPosition(laserStartPos);

	// ワープポイントをリンクさせる関数
	SetWarpPoint();

	return true;
}

//=============================================================================
// 配列に登録されたブロックのアドレスゲッター
// 引数の説明
// n_block_idx_x, n_block_idx_y :それぞれの配列番号
//
// 返り値:CBlock*型
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
// ブロック配列番号算出関数
// 引数の説明
//    pos    :対象オブジェクトの中心座標
// n_array_x :配列番号-Xを入れる場所
// n_array_y :配列番号-Yを入れる場所
//=============================================================================
void CBlockManager::CalculateBlockArrayNum(D3DXVECTOR3 pos, int *p_array_x, int *p_array_y)
{
	*p_array_x = (int)(pos.x / BLOCK_WIDTH);
	*p_array_y = (int)(pos.y / BLOCK_HEIGHT);
}

//=============================================================================
// ブロックIDゲッター
// 引数の説明
// pos :対象オブジェクトの中心座標
//=============================================================================
CBlock::BLOCKID CBlockManager::GetBlockID(D3DXVECTOR3 pos)
{
	int nArrayX = 0, nArrayY = 0;

	// 現在位置のブロックアドレスを求め
	CalculateBlockArrayNum(pos, &nArrayX, &nArrayY);
	CBlock *pBlock = GetBlock(nArrayX, nArrayY);

	// ブロックの種類を返す
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

	// 現在位置のブロックアドレスを求め
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
// ギミックブロックへ上書きする関数
// 引数の説明
//  block_type :ブロックの種類
//     pos     :対象オブジェクトの中心座標
//=============================================================================
void CBlockManager::OverwriteGimmickBlock(CBlock::BLOCKID block_type, D3DXVECTOR3 pos)
{
	int nArrayX = 0, nArrayY = 0;

	// 現在位置のブロックのアドレスを求め
	CalculateBlockArrayNum(pos, &nArrayX, &nArrayY);
	CBlock *pBlock = GetBlock(nArrayX, nArrayY);

	// アドレスがあるとき
	if (pBlock != nullptr)
	{
		// 前のアドレスのところを破棄
		pBlock->Uninit();
		pBlock = nullptr;
	}

	// 上書きタイプがBLOCK_NONEだった場合
	if (block_type == CBlock::BLOCKID_NONE)
	{
		m_pBlockArray[nArrayY * MAX_BLOCK_X + nArrayX] = nullptr;
		return;
	}

	// ギミックブロックを生成し、配列に登録する
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

	// 上にブロックが存在するかどうか
	bool bExistOnOneBlock = m_pBlockArray[(nArrayY - 1) * MAX_BLOCK_X + nArrayX] != nullptr;
	bool bNotExistOnOneBlock = m_pBlockArray[(nArrayY - 1) * MAX_BLOCK_X + nArrayX] == nullptr;

	// 下地ブロックの設定
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
// ワープブロック転移先設定関数
//=============================================================================
void CBlockManager::SetWarpPoint()
{
	CGimmickBlock *pWarpBlue1 = nullptr, *pWarpBlue2 = nullptr,
				  *pWarpGreen1 = nullptr, *pWarpGreen2 = nullptr,
				  *pWarpPink1 = nullptr, *pWarpPink2 = nullptr;

	// ワープブロック検索
	for (int nCntY = 0; nCntY < MAX_BLOCK_Y; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_BLOCK_X; nCntX++)
		{
			if (m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX] != nullptr)
			{
				// ブロックの種類がワープ青だった場合
				if (m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX]->GetBlockId() == CBlock::BLOCKID_WARP_BLUE)
				{
					// １つ目のアドレスが設定されていなかったら１つ目に。
					// 設定済みなら２つ目に設定する。
					if (pWarpBlue1 == nullptr)
					{
						pWarpBlue1 = ( CGimmickBlock* )m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX];
					}
					else if ( pWarpBlue1 != nullptr && pWarpBlue2 == nullptr )
					{
						pWarpBlue2 = ( CGimmickBlock* )m_pBlockArray[nCntY * MAX_BLOCK_X + nCntX];
					}
				}
				// ブロックの種類がワープ緑だった場合
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
				// ブロックの種類がワープ桃だった場合
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

			// 対応する２つのワープにアドレスが登録されていたら、その２つで
			// つなげ、nullで初期化する。
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