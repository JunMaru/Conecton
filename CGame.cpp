/*-----------------------------------------------------------------------------
ゲーム画面フェーズクラス
author	OkadaMoeto
since	20140713
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
インクルードファイル
-----------------------------------------------------------------------------*/
#include "CGame.h"
#include "Define.h"
#include "CManager.h"
#include "CInput.h"
#include "CPlayer.h"
#include "CFade.h"
#include "CScene.h"
#include "CScene2D.h"
#include "collisionDetection.h"
#include "CBlockManager.h"
#include "CGauge.h"
#include "CInstancingObject.h"
#include "CBlock.h"
#include "CAnton.h"
#include "CLaser.h"
#include "CBackGround.h"
#include "CAntonLifeUI.h"
#include "CScrollManager.h"
#include "CLaserManager.h"

/*-----------------------------------------------------------------------------
静的メンバ変数の初期化
-----------------------------------------------------------------------------*/
CPlayer *CGame::m_pPlayer = nullptr;
CBlockManager *CGame::m_pBlockManager = nullptr;
CLaserManager *CGame::m_pLaserManager = nullptr;

/*-----------------------------------------------------------------------------
グローバル変数
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
コンストラクタ
-----------------------------------------------------------------------------*/
CGame::CGame()
{
	m_pPlayer = nullptr;
	m_pLifeUI = nullptr;
	m_pGauge = nullptr;
	m_pScrollManager = nullptr;
	m_pBackGround = nullptr;
}

/*-----------------------------------------------------------------------------
デストラクタ
-----------------------------------------------------------------------------*/
CGame::~CGame()
{
}

/*-----------------------------------------------------------------------------
初期化
-----------------------------------------------------------------------------*/
void CGame::Init(void)
{
	m_pBlockManager = CBlockManager::Create( "data/stage_info/stage1.csv" );
	m_pPlayer = CPlayer::Create(VEC3_ZERO, VEC3_ZERO);
	InitGauge();
	m_pBackGround = CBackGround::Create("data/texture/game_bg/game_bg.jpg");
	m_pLifeUI = CAntonLifeUI::Create(D3DXVECTOR3(350.0f, 50.0f, 0.0f));

	m_pScrollManager = new CScrollManager();
	m_pScrollManager->Init();

	// レーザー動作テスト
	//CLaser::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), CLaser::DIRECTION_RIGHT);
	m_pLaserManager = CLaserManager::Create();

	// 1秒間のフェードイン
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 60.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
終了
-----------------------------------------------------------------------------*/
void CGame::Uninit(void)
{
	// 描画対象オブジェクトの解放
	CScene::ReleaseAll();

	m_pScrollManager->SetScrollWorld(VEC2_ZERO);
	delete m_pScrollManager;

	m_pPlayer->Uninit();
	delete m_pPlayer;

	m_pBlockManager->Uninit();
	delete m_pBlockManager;

	m_pGauge->Uninit();
	delete m_pGauge;

	m_pLaserManager->Uninit();
	delete m_pLaserManager;
}

/*-----------------------------------------------------------------------------
更新
-----------------------------------------------------------------------------*/
void CGame::Update(void)
{
	// キーボード入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoyPad = CManager::GetInputJoypad();

	// フェードしていなければ更新
	if (CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
#ifdef _DEBUG
		if (pKeyboard->GetKeyTrigger(DIK_RETURN))
		{
			CManager::GetPhaseFade()->Start(
				CFade::FADETYPE_OUT,
				60.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
#endif
	}

	// フェードアウト完了後に画面遷移
	if (CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		CManager::SetPhase(CManager::PHASE_RESULT);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CGame.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:フェーズ遷移\n");
#endif

	m_pPlayer->Update();

	m_pLaserManager->Update();

	CAnton *ant = m_pPlayer->GetAnton();

	if (ant->GetState() == CAnton::STATE_MINIMUM)
	{
		HitCheckMinimumAnton();
	}
	else if (ant->GetState() == CAnton::STATE_METAL)
	{
		HitCheckMetalAnton();
	}
	else
	{
		HitCheckAnton();
	}

	// ゲージテスト
	static float fTestGaugeVal = 50.0f;
	if (pKeyboard->GetKeyTrigger(DIK_8))
	{
		fTestGaugeVal -= 1.0f;
		m_pGauge->SetGaugeVal(fTestGaugeVal);
	}
	if (pKeyboard->GetKeyTrigger(DIK_9))
	{
		fTestGaugeVal += 1.0f;
		m_pGauge->SetGaugeVal(fTestGaugeVal);
	}
}

// ミニマム用のあたり判定。多分普通のと同じソースで実装できる
void CGame::HitCheckMinimumAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();
	CBlock *pBlock = nullptr;

	D3DXVECTOR3 antonPos = ant->GetPosition();
	D3DXVECTOR3 antonPosOld = ant->GetPositionOld();
	D3DXVECTOR3 antonPosTar = ant->GetTargetPosition();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(25.0f, 32.5f, 0.0f);
	D3DXVECTOR3 setAntonPos = antonPos;

	D3DXVECTOR3 blockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);

	int nBlockIdxX = (int)(antonPos.x / BLOCK_WIDTH);
	int nBlockIdxY = (int)(antonPos.y / BLOCK_HEIGHT);

	// 中心にずらす
	antonPos.x += antonSize.x * 2.0f;
	antonPos.y += antonSize.y * 3.0f;

	// アントンを地面の上に乗せるための当たり判定
	for (int nCntX = 0; nCntX < 2; nCntX++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + nCntX, nBlockIdxY + 2);

		if (pBlock != nullptr)
		{
			// 座標を中心に変更
			blockPos = pBlock->GetPosition() + blockSize;

			// X方向の当たり判定
			if ((antonPos.x + antonSize.x) > (blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x) < (blockPos.x + blockSize.x))
			{
				// Y方向の当たり判定
				if ((antonPos.y + antonSize.y) > (blockPos.y - blockSize.y)
					&& (antonPos.y - antonSize.y) < (blockPos.y + blockSize.y))
				{
					setAntonPos.y = ( blockPos.y ) - ( antonSize.y * 4.0f ) + ( 5.0f * 2.0f );
				}
			}
		}
	}

	// 壁へのめり込みを防ぐループ( 未完成 )
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + (nCnt * 2) - 1, nBlockIdxY + 1);

		if (pBlock != nullptr)
		{
			blockPos = pBlock->GetPosition() + blockSize;

			if ((antonPos.y + antonSize.y) - (blockPos.y - blockSize.y) >= BLOCK_HEIGHT
				&& (antonPos.x + antonSize.x - 20.0f) >(blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x + 20.0f) < (blockPos.x + blockSize.x))
			{
				// 進んでいる方向に壁がある場合は目的の座標と現在の座標を１つ前に戻す
				if ((antonPosTar.x - antonPos.x) * (blockPos.x - antonPos.x) > 0)
				{
					setAntonPos = antonPosOld;

					antonPosTar = setAntonPos;

					ant->SetTargetPosition(antonPosTar.x, antonPosTar.y);
				}

			}
		}
	}

	ant->SetPosition(setAntonPos);
}

void CGame::HitCheckMetalAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();
	CBlock *pBlock = nullptr;

	D3DXVECTOR3 antonPos = ant->GetPosition();
	D3DXVECTOR3 antonPosOld = ant->GetPositionOld();
	D3DXVECTOR3 antonPosTar = ant->GetTargetPosition();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(50.0f, 65.0f, 0.0f);
	D3DXVECTOR3 setAntonPos = antonPos;

	int nBlockIdxX = (int)(antonPos.x / BLOCK_WIDTH);
	int nBlockIdxY = (int)(antonPos.y / BLOCK_HEIGHT);

	D3DXVECTOR3 blockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);

	// 中心にずらす
	antonPos += antonSize;

	// アントンを地面の上に乗せるための当たり判定
	for (int nCntX = 0; nCntX < 2; nCntX++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + nCntX, nBlockIdxY + 2);

		if (pBlock != nullptr)
		{
			// 座標を中心に変更
			blockPos = pBlock->GetPosition() + blockSize;

			// X方向の当たり判定
			if ((antonPos.x + antonSize.x) >(blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x) < (blockPos.x + blockSize.x))
			{
				// Y方向の当たり判定
				if ( (antonPos.y + antonSize.y) > (blockPos.y - blockSize.y)
				  && (antonPos.y - antonSize.y) < (blockPos.y + blockSize.y) )
				{
					if ( pBlock -> GetBlockId() == CBlock::BLOCKID_SOIL_CRACK
					  || pBlock -> GetBlockId() == CBlock::BLOCKID_GRASS_CRACK )
					{
						pBlock -> Uninit();
						pBlock = NULL;
						CBlockManager::SetBlock( nBlockIdxX + nCntX, nBlockIdxY + 2, nullptr );
					}
					else
					{
						// 30.0fは、地面にめり込まないための補正値
						setAntonPos.y = ( blockPos.y ) - ( ( antonSize.y - 5.0f ) * 2 ) - 30.0f;
					}
				}
			}
		}
	}

	// 壁へのめり込みを防ぐループ( 未完成 )
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + (nCnt * 3) - 1, nBlockIdxY + nCnt);

		if (pBlock != nullptr)
		{
			blockPos = pBlock->GetPosition() + blockSize;

			if ((antonPos.y + antonSize.y) - (blockPos.y - blockSize.y) >= BLOCK_HEIGHT
				&& (antonPos.x + antonSize.x - 20.0f) >(blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x + 20.0f) < (blockPos.x + blockSize.x))
			{
				// 進んでいる方向に壁がある場合は目的の座標と現在の座標を１つ前に戻す
				if ((antonPosTar.x - antonPos.x) * (blockPos.x - antonPos.x) > 0)
				{
					setAntonPos.x = antonPosOld.x;

					antonPosTar = setAntonPos;

					ant->SetTargetPosition(antonPosTar.x, antonPosTar.y);
				}

			}
		}
	}

	ant->SetPosition(setAntonPos);
}

void CGame::HitCheckAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();
	CBlock *pBlock = nullptr;

	D3DXVECTOR3 antonPos = ant->GetPosition();
	D3DXVECTOR3 antonPosOld = ant->GetPositionOld();
	D3DXVECTOR3 antonPosTar = ant->GetTargetPosition();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(50.0f, 65.0f, 0.0f);
	D3DXVECTOR3 setAntonPos = antonPos;

	int nBlockIdxX = (int)(antonPos.x / BLOCK_WIDTH);
	int nBlockIdxY = (int)(antonPos.y / BLOCK_HEIGHT);

	D3DXVECTOR3 blockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);

	// 中心にずらす
	antonPos += antonSize;

	// アントンを地面の上に乗せるための当たり判定
	for (int nCntX = 0; nCntX < 2; nCntX++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + nCntX, nBlockIdxY + 2);

		if (pBlock != nullptr)
		{
			// 座標を中心に変更
			blockPos = pBlock->GetPosition() + blockSize;

			// X方向の当たり判定
			if ((antonPos.x + antonSize.x) > (blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x) < (blockPos.x + blockSize.x))
			{
				// Y方向の当たり判定
				if ((antonPos.y + antonSize.y) > (blockPos.y - blockSize.y)
					&& (antonPos.y - antonSize.y) < (blockPos.y + blockSize.y))
				{
					setAntonPos.y = (blockPos.y) - ((antonSize.y - 5.0f) * 2);
				}
			}
		}
	}

	// 壁へのめり込みを防ぐループ( 未完成 )
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + (nCnt * 3) - 1, nBlockIdxY + nCnt);

		if (pBlock != nullptr)
		{
			blockPos = pBlock->GetPosition() + blockSize;

			if ((antonPos.y + antonSize.y) - (blockPos.y - blockSize.y) >= BLOCK_HEIGHT
				&& (antonPos.x + antonSize.x - 20.0f) > (blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x + 20.0f) < (blockPos.x + blockSize.x))
			{
				// 進んでいる方向に壁がある場合は目的の座標と現在の座標を１つ前に戻す
				if ((antonPosTar.x - antonPos.x) * (blockPos.x - antonPos.x) > 0)
				{
					setAntonPos = antonPosOld;

					antonPosTar = setAntonPos;

					ant->SetTargetPosition(antonPosTar.x, antonPosTar.y);
				}
				
			}
		}
	}

	ant->SetPosition(setAntonPos);
}

/*-----------------------------------------------------------------------------
 ゲージの初期化
-----------------------------------------------------------------------------*/
void CGame::InitGauge(void)
{
	m_pGauge = new CGauge(6);
	m_pGauge->Init();
	m_pGauge->SetPosition(D3DXVECTOR3(1200.0f, 100.0f, 0.0f));
	m_pGauge->SetScling(D3DXVECTOR2(116.0f, 116.0f));
	m_pGauge->SetGaugeBaseVal(100.0f);
	m_pGauge->SetGaugeVal(50.0f);

	const char *pFilePathTable[] = { "data/texture/ui/gauge_base.png", "data/texture/ui/gauge.png", "data/texture/ui/gauge_frame.png", };
	const int nLoadFileNum = 3;
	for (int nCnt = 0; nCnt < nLoadFileNum; ++nCnt)
	{
		m_pGauge->LoadTexture(nCnt, pFilePathTable[nCnt]);
	}
}