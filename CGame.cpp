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
#include "CBeecon.h"
#include "CLaser.h"
#include "CBackGround.h"
#include "CAntonLifeUI.h"
#include "CScrollManager.h"
#include "CLaserManager.h"
#include "CInputCommand.h"
#include "CPseudoLight.h"
#include "C2DLogo.h"
#include "CConfigRecorder.h"
#include "CStageConfig.h"
#include "CPause.h"
#include "CSoundXAudio2.h"
#include "CLifeConfig.h"

/*-----------------------------------------------------------------------------
	テクスチャ読み込み先のパス設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_FONT_CLEAR = "data/texture/font/clear.png";
static const char* TEXTUREPATH_PSEUDOLIGHT = "data/texture/pseudo_light/pseudo_light.png";
static const char* TEXTUREPATH_BG_STAGE1 = "data/texture/game_bg/s1_bg.jpg";
static const char* TEXTUREPATH_BG_STAGE2 = "data/texture/game_bg/s2_bg.jpg";
static const char* TEXTUREPATH_BG_STAGE3 = "data/texture/game_bg/s3_bg.jpg";
static const char* TEXTUREPATH_BG_STAGE4 = "data/texture/game_bg/s4_bg.jpg";

/*-----------------------------------------------------------------------------
	ステージデータ読み込み先のパス設定
-----------------------------------------------------------------------------*/
static char* MAPDATAPATH_CSV_STAGE1 = "data/stage_info/stage1.csv";
static char* MAPDATAPATH_CSV_STAGE2 = "data/stage_info/stage2ex.csv";
static char* MAPDATAPATH_CSV_STAGE3 = "data/stage_info/stage3.csv";
static char* MAPDATAPATH_CSV_STAGE4 = "data/stage_info/stage4.csv";

/*-----------------------------------------------------------------------------
	ゲージUIのベース値(これが最大値になる)
-----------------------------------------------------------------------------*/
static const float GAUGE_BASE_VALUE = (100.0f);

/*-----------------------------------------------------------------------------
	ゲームオーバーＢＧの生成設定
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_GAMEOVERBG = "data/texture/gameover_bg/gameover_bg.png";
static const D3DXVECTOR3 POS_GAMEOVERBG = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
static const float WIDTH_GAMEOVERBG = 1280.0f;
static const float HEIGHT_GAMEOVERBG = 720.0f;

/*-----------------------------------------------------------------------------
	ゲームオーバーのタイトルへの遷移
-----------------------------------------------------------------------------*/
static const float TIME_AUTOCHANGE_GAMEOVER = 30.0f * 6.0f;

/*-----------------------------------------------------------------------------
静的メンバ変数の初期化
-----------------------------------------------------------------------------*/
CPlayer *CGame::m_pPlayer = nullptr;
CBlockManager *CGame::m_pBlockManager = nullptr;
CLaserManager *CGame::m_pLaserManager = nullptr;
CPause* CGame::m_pPause = nullptr;

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
	m_pInputCommand = nullptr;
	m_fScore = 0.0f;
	m_pPseudoLight = nullptr;
	m_pEndLogo = nullptr;
	m_pPause = nullptr;
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
	m_pPseudoLight = CPseudoLight::Create(TEXTUREPATH_PSEUDOLIGHT);

	InitStage();

	m_pPlayer = CPlayer::Create(VEC3_ZERO, VEC3_ZERO);
	
	InitGauge();

	m_pLifeUI = CAntonLifeUI::Create(D3DXVECTOR3(350.0f, 50.0f, 0.0f));

	// 現在の設定記録されているライフの値に初期化する
	int nowLife = CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE);
	nowLife = MAX_RETRYLIFE - nowLife;
	m_pLifeUI->AddLife(-nowLife);
	
	m_pScrollManager = new CScrollManager();
	m_pScrollManager->Init();

	m_pLaserManager = CLaserManager::Create();

	m_pInputCommand = new CInputCommand(CManager::GetInputKeyboard(), CManager::GetInputJoypad());
	m_pInputCommand->Init();

	m_pPause = new CPause();
	m_pPause->Init();

	// ポーズの選択情報を初期化して、ループしたときにいきなり画面遷移みたいなのを防止
	CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_PAUSESLECT, 0);

	m_transitionID = TRANSITIONID_NONE;
	m_bTransition = false;

	InitGameOverBG();

	PlayBGM();

	// 1秒間のフェードイン
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 30.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
終了
-----------------------------------------------------------------------------*/
void CGame::Uninit(void)
{
	StopBGM();

	// 描画対象オブジェクトの解放
	CScene::ReleaseAll();

	m_pScrollManager->SetScrollWorld(VEC2_ZERO);
	delete m_pScrollManager;

	m_pPlayer->Uninit();
	delete m_pPlayer;

	m_pBlockManager->Uninit();
	delete m_pBlockManager;

	m_pLaserManager->Uninit();
	delete m_pLaserManager;

	m_pInputCommand->Uninit();
	delete m_pInputCommand;

	m_pPause->Uninit();
	delete m_pPause;
}

/*-----------------------------------------------------------------------------
更新
-----------------------------------------------------------------------------*/
void CGame::Update(void)
{
	m_pInputCommand->Update();

	// フェードしていなければ更新
	if (CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		PauseTo();

		CheckGameOver();

		// 無限フェードアウトにしないようにここで発生させる
		if(m_bTransition)
		{
			CManager::GetPhaseFade()->Start(
												CFade::FADETYPE_OUT,
												30.0f,
												COL_WHITE);
		}
	}

	CheckTransition();

	// ポーズの更新
	if(m_pPause->GetPause())
	{
		m_pPause->UpdateInputEvent(m_pInputCommand);

		m_pPause->Update();
	}
	else
	{
		m_pPlayer->Update();

		m_pLaserManager->Update();

		CheckConnectAction();
	}

	HitCheckItem();

	CAnton *ant = m_pPlayer->GetAnton();

	if(ant->GetState() == CAnton::STATE_MINIMUM)
	{
		HitCheckMinimumAnton();
	}
	else if(ant->GetState() == CAnton::STATE_METAL)
	{
		HitCheckMetalAnton();
	}
	else
	{
		HitCheckAnton();
	}

	CheckGameEnd();

	CheckPauseSelect();

#ifdef _DEBUG
	CDebugProcDX9::Print("[CGame.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:フェーズ遷移\n");
#endif
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

		if (pBlock != nullptr
		 && pBlock ->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
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

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
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

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
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

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
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

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
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

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
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

//=============================================================================
// ゲージ初期化処理
//=============================================================================
void CGame::InitGauge(void)
{
	m_pGauge = new CGauge(6);
	m_pGauge->Init();
	m_pGauge->SetPosition(D3DXVECTOR3(1200.0f, 100.0f, 0.0f));
	m_pGauge->SetScling(D3DXVECTOR2(116.0f, 116.0f));
	m_pGauge->SetGaugeBaseVal(GAUGE_BASE_VALUE);
	m_pGauge->SetGaugeVal(0.0f);

	const char *pFilePathTable[] = { "data/texture/ui/gauge_base.png", "data/texture/ui/gauge.png", "data/texture/ui/gauge_frame.png", };
	const int nLoadFileNum = 3;
	for (int nCnt = 0; nCnt < nLoadFileNum; ++nCnt)
	{
		m_pGauge->LoadTexture(nCnt, pFilePathTable[nCnt]);
	}
}

//=============================================================================
// コネクトアクション処理
//=============================================================================
void CGame::CheckConnectAction(void)
{
	const bool bConnect = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CONNECT);

	if (bConnect == false)
	{
		return;
	}

	const float fOffsetX = 25.0f;
	const float fOffsetY = 25.0f;

	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x + fOffsetX, beeconPos.y + fOffsetY, 0.0f);
	CBlock::BLOCKID blockIDFromBlockManager = CBlock::BLOCKID_NONE;

	// ここでブロックマネージャーからブロック情報を取得
	blockIDFromBlockManager = m_pBlockManager->GetBlockID(workPos);

	// コネクト先が何もなかった場合
	if (blockIDFromBlockManager == CBlock::BLOCKID_NONE)
	{
		// コネクトなんてなかった
		return;
	}

	// ビーコンは今コネクトモーション中？
	if (pBeecon->GetAction() == CBeecon::ACTION_CONNECT)
	{
		// コネクト中のコネクト。ダメゼッタイ
		return;
	}

	// 取ってきたのは変身ブロックか？
	if (ConnectChangeAntonBlock())
	{
		// 選ばれたのは変身ブロックでした
		return;
	}

	// 取ってきたのはギミックブロックか？
	if (ConnectGimmickBlock())
	{
		// 選ばれたのはギミックブロックでした
		return;
	}

	// 取ってきたのはノーマルブロックか？
	if (ConnectNormalBlock())
	{
		// 選ばれたのはノーマルブロックでした
		return;
	}

	// 変身でもギミックでもノーマルブロックでもないです
	return;
}

//=============================================================================
// コネクトアクション(変身ブロック)処理
//=============================================================================
bool CGame::ConnectChangeAntonBlock(void)
{
	const float fOffsetX = 25.0f;
	const float fOffsetY = 25.0f;

	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x + fOffsetX, beeconPos.y + fOffsetY, 0.0f);
	CBlock::BLOCKID blockIDFromBlockManager = m_pBlockManager->GetBlockID(workPos);

	const CBlock::BLOCKID aChangeAntonBlockTable[] = { CBlock::BLOCKID_METAL, CBlock::BLOCKID_MINIMUM, CBlock::BLOCKID_POWERFUL, };
	const CPlayer::ANTON_STATE aAntonStateTable[] = { CPlayer::ANTON_STATE_METAL, CPlayer::ANTON_STATE_MINIMUM, CPlayer::ANTON_STATE_POWERFUL, };

	// 取得してきたブロック情報がアントンを変身させるものか？
	for (int nCnt = 0; nCnt < sizeof(aChangeAntonBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		// テーブル内検索
		if (blockIDFromBlockManager != aChangeAntonBlockTable[nCnt])
		{
			continue;
		}

		// 変身ブロックだった場合
		// アントンを変身させ、ブロックをノーマルにする
		m_pPlayer->SetAntonState(aAntonStateTable[nCnt]);

		// ブロックマネージャーのブロックをノーマルに上書き
		m_pBlockManager->OverwriteGimmickBlock(CBlock::BLOCKID_SOIL, workPos);

		pBeecon->SetAction(CBeecon::ACTION_CONNECT);
		return true;
	}

	return false;
}

//=============================================================================
// コネクトアクション(ギミックブロック)処理
//=============================================================================
bool CGame::ConnectGimmickBlock(void)
{
	const float fOffsetX = 25.0f;
	const float fOffsetY = 25.0f;

	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x + fOffsetX, beeconPos.y + fOffsetY, 0.0f);
	CBlock::BLOCKID blockIDFromBlockManager = m_pBlockManager->GetBlockID(workPos);

	const CBlock::BLOCKID aGimmickBlockTable[] = { CBlock::BLOCKID_LASER_CONTROL_DOWN, CBlock::BLOCKID_LASER_CONTROL_LEFT,
													CBlock::BLOCKID_LASER_CONTROL_RIGHT, CBlock::BLOCKID_LASER_CONTROL_UP,
													CBlock::BLOCKID_MAGNET, };
	const int nGimmickBlock2BeeconIconUIOffset = CBlock::BLOCKID_LASER_CONTROL_DOWN - 1;

	CBlock::BLOCKID blockIDFromBeecon = static_cast<CBlock::BLOCKID>(pBeecon->GetBlockID());

	// 取得してきたブロック情報はギミックなのか？
	for (int nCnt = 0; nCnt < sizeof(aGimmickBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		// テーブル内検索
		if (blockIDFromBlockManager != aGimmickBlockTable[nCnt])
		{
			continue;
		}

		// ビーコンに対するアクション設定
		pBeecon->SetAction(CBeecon::ACTION_CONNECT);
		m_pPlayer->SetBeeconIconUIType(static_cast<CPlayer::BEECON_ICONTYPE>(aGimmickBlockTable[nCnt] - nGimmickBlock2BeeconIconUIOffset));

		// ギミックブロックだった場合
		// ビーコンがギミックブロックのストックを持っていない場合
		if (blockIDFromBeecon == CBlock::BLOCKID_NONE)
		{
			int nLaserNo = m_pBlockManager->GetBlockHitLaserNo(workPos);
			m_pLaserManager->ReplaceLaser(nLaserNo);

			// ビーコンにギミックブロックIDをセットし、ブロックマネージャーのブロックにはノーマルに上書き
			m_pPlayer->SetBeeconBlockID(static_cast<CPlayer::BEECON_BLOCKID>(blockIDFromBlockManager));
			m_pBlockManager->OverwriteGimmickBlock(CBlock::BLOCKID_SOIL, workPos);

			return true;
		}

		// ビーコンがなんかしらのブロックを持っていた場合
		else
		{
			int nLaserNo = m_pBlockManager->GetBlockHitLaserNo(workPos);
			m_pLaserManager->ReplaceLaser(nLaserNo);

			// ビーコンにギミックブロックIDをセットし、ブロックマネージャーのブロックにはビーコンが持っていたのを上書き
			m_pPlayer->SetBeeconBlockID(static_cast<CPlayer::BEECON_BLOCKID>(blockIDFromBlockManager));
			m_pBlockManager->OverwriteGimmickBlock(blockIDFromBeecon, workPos);

			return true;
		}
	}

	return false;
}

//=============================================================================
// コネクトアクション(ノーマルブロック)処理
//=============================================================================
bool CGame::ConnectNormalBlock(void)
{
	const float fOffsetX = 25.0f;
	const float fOffsetY = 25.0f;

	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x + fOffsetX, beeconPos.y + fOffsetY, 0.0f);
	CBlock::BLOCKID blockIDFromBlockManager = m_pBlockManager->GetBlockID(workPos);

	const CBlock::BLOCKID aGimmickBlockTable[] = { CBlock::BLOCKID_LASER_CONTROL_DOWN, CBlock::BLOCKID_LASER_CONTROL_LEFT,
													CBlock::BLOCKID_LASER_CONTROL_RIGHT, CBlock::BLOCKID_LASER_CONTROL_UP,
													CBlock::BLOCKID_MAGNET, };

	CBlock::BLOCKID blockIDFromBeecon = static_cast<CBlock::BLOCKID>(pBeecon->GetBlockID());
	bool bGimmick = false;

	// ビーコンは何かしらのギミックブロックをもっているか？
	for (int nCnt = 0; nCnt < sizeof(aGimmickBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		// テーブル内検索
		if (blockIDFromBeecon != aGimmickBlockTable[nCnt])
		{
			continue;
		}

		bGimmick = true;
		break;
	}

	// ビーコンがギミックを持っていない→ノーマルブロックに置けないのでさようなら
	if (bGimmick == false)
	{
		return false;
	}

	// ギミックブロック判定テーブル
	const CBlock::BLOCKID aNormalBlockTable[] = { CBlock::BLOCKID_GRASS, CBlock::BLOCKID_SOIL, };

	// 取得してきたブロック情報はノーマルなのか？
	for (int nCnt = 0; nCnt < sizeof(aNormalBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		// テーブル内検索
		if (blockIDFromBlockManager != aNormalBlockTable[nCnt])
		{
			continue;
		}

		int nLaserNo = m_pBlockManager->GetBlockHitLaserNo(workPos);
		m_pLaserManager->ReplaceLaser(nLaserNo);

		// ノーマルだった場合
		// ビーコンに入っていたギミックを、ブロックマネージャーのブロックに上書き。ビーコンのは空
		m_pPlayer->SetBeeconBlockID(CPlayer::BEECON_BLOCKID_NONE);
		m_pBlockManager->OverwriteGimmickBlock(blockIDFromBeecon, workPos);

		pBeecon->SetAction(CBeecon::ACTION_CONNECT);
		m_pPlayer->SetBeeconIconUIType(CPlayer::BEECON_ICONTYPE_NONE);
		return true;
	}

	return false;
}

//=============================================================================
// アイテムとの当たり判定
//=============================================================================
void CGame::HitCheckItem(void)
{
	const float fAntonOffsetX = 50.0f;
	const float fAntonOffsetY = 50.0f;

	D3DXVECTOR3 antonPos = m_pPlayer->GetAnton()->GetPosition();
	antonPos.x += fAntonOffsetX;
	antonPos.y += fAntonOffsetY;

	CBlock::BLOCKID blockIDFromBlockManager = m_pBlockManager->GetBlockID(antonPos);

	// 何かしらのブロックか？
	if (blockIDFromBlockManager == CBlock::BLOCKID_NONE)
	{
		return;
	}

	const CBlock::BLOCKID aItemBlockTable[] = { CBlock::BLOCKID_FOOD_ACORN, CBlock::BLOCKID_FOOD_APPLE, CBlock::BLOCKID_FOOD_MUSHROOM, };
	const int nFoodNum = m_pBlockManager->GetFoodNum();
	const float fFoodScore = GAUGE_BASE_VALUE / nFoodNum;

	// アイテムチェック
	for (int nCnt = 0; nCnt < sizeof(aItemBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		if (blockIDFromBlockManager != aItemBlockTable[nCnt])
		{
			continue;
		}

		m_fScore += fFoodScore;
		m_pGauge->SetGaugeVal(m_fScore);

		m_pBlockManager->OverwriteGimmickBlock(CBlock::BLOCKID_NONE, antonPos);
	}
}

//=============================================================================
// ゲーム終了チェック処理
//=============================================================================
void CGame::CheckGameEnd(void)
{
	const bool bFadeNone = (CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE);

	if (bFadeNone == false)
	{
		return;
	}

	const bool bEnd = m_pLaserManager->GetLaserGoalFlag();

	if (bEnd == false)
	{
		return;
	}

	const bool bMoveHand = (m_pPlayer->GetAntonAction() == CPlayer::ANTON_ACTION_FRONT) && (m_pPlayer->GetAntonState() == CPlayer::ANTON_STATE_NORMAL);

	if (bMoveHand == false)
	{
		m_pPlayer->SetAntonState(CPlayer::ANTON_STATE_NORMAL);
		m_pPlayer->SetAntonAction(CPlayer::ANTON_ACTION_FRONT);
		m_pPlayer->SetStopMove(true);
	}

	if (m_pEndLogo == nullptr)
	{
		D3DXVECTOR2 texSize = D3DXVECTOR2(1076.0f, 237.0f);
		const float fLogoOffsetY = -200.0f;
		m_pEndLogo = new C2DLogo();
		m_pEndLogo->LoadTexture(TEXTUREPATH_FONT_CLEAR);
		m_pEndLogo->Init();
		m_pEndLogo->SetPosition(D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y + fLogoOffsetY,0.0f));
		m_pEndLogo->SetScling(texSize / 2);
		m_pEndLogo->StartSclAnimation(true, texSize / 4, texSize / 2, 0.01f);

		PlayJingleClear();
	}

	const bool bEnter = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_ENTER);

	if (bEnter == true)
	{
		m_bTransition = true;
		SetTransitionID(TRANSITIONID_STAGESELECT);

		// クリア特典でライフを最大値まで上げる
		CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_RETRYLIFE, MAX_RETRYLIFE);
	}
}

/*-----------------------------------------------------------------------------
	設定記録から選択されているステージを取得し、
	遊びたいステージのブロックと背景を生成 / 初期化する
-----------------------------------------------------------------------------*/
void CGame::InitStage(void)
{
	int selectStage = CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_STAGESELECT);

	switch(selectStage)
	{
		case STAGEID_1:
			m_pBlockManager = CBlockManager::Create(MAPDATAPATH_CSV_STAGE1);
			m_pBackGround = CBackGround::Create(TEXTUREPATH_BG_STAGE1);
			break;

		case STAGEID_2:
			m_pBlockManager = CBlockManager::Create(MAPDATAPATH_CSV_STAGE2);
			m_pBackGround = CBackGround::Create(TEXTUREPATH_BG_STAGE2);
			break;

		case STAGEID_3:
			m_pBlockManager = CBlockManager::Create(MAPDATAPATH_CSV_STAGE3);
			m_pBackGround = CBackGround::Create(TEXTUREPATH_BG_STAGE3);
			break;

		case STAGEID_4:
			m_pBlockManager = CBlockManager::Create(MAPDATAPATH_CSV_STAGE4);
			m_pBackGround = CBackGround::Create(TEXTUREPATH_BG_STAGE4);
			break;

		default:
			break;
	}
}

/*-----------------------------------------------------------------------------
	ポーズの入力処理をチェックし切り替える
-----------------------------------------------------------------------------*/
void CGame::PauseTo(void)
{
	const bool bPause = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_PAUSE);

	if (bPause == false)
	{
		return;
	}

	if(!m_pPause->GetPause())
	{
		m_pPause->Enable();
	}
	else
	{
		m_pPause->Disable();
	}
}

/*-----------------------------------------------------------------------------
	画面遷移のイベント更新
-----------------------------------------------------------------------------*/
void CGame::CheckPauseSelect(void)
{
	int selectPauseMenu = CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_PAUSESLECT);

	if(selectPauseMenu == 0)
	{
		return;
	}

	// １度だけ通るようにフラグＯＮならば返す
	if(m_bTransition)
	{
		return;
	}

	if(m_bGameOver)
	{
		return;
	}

	switch(selectPauseMenu)
	{
		case PAUSEID_RETRY:
			Retry();
			break;

		case PAUSEID_EXIT:
			ReturnToStageSelect();
			break;

		default:
			break;
	}
}

/*-----------------------------------------------------------------------------
	画面遷移のイベント更新
-----------------------------------------------------------------------------*/
void CGame::CheckTransition(void)
{
	if(m_transitionID == TRANSITIONID_NONE)
	{
		return;
	}

	// フェードアウト完了後に画面遷移
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		switch(m_transitionID)
		{
			case TRANSITIONID_GAME_RETRY:
				CManager::SetPhase(CManager::PHASE_GAME_RETRY);
				break;

			case TRANSITIONID_STAGESELECT:
				CManager::SetPhase(CManager::PHASE_STAGESELECT);
				break;

			case TRANSITIONID_TITLE:
				CManager::SetPhase(CManager::PHASE_TITLE);
				break;

			default:
				break;
		}
	}
}

/*-----------------------------------------------------------------------------
	ゲームオーバーＢＧの初期化
-----------------------------------------------------------------------------*/
void CGame::InitGameOverBG(void)
{
	m_pGameOverBG = CScene2D::Create(
										TEXTUREPATH_GAMEOVERBG,
										POS_GAMEOVERBG,
										VEC3_ZERO,
										WIDTH_GAMEOVERBG,
										HEIGHT_GAMEOVERBG);

	m_pGameOverBG->SetDraw(false);

	m_bGameOver = false;

	m_autoChange = 0.0f;
}

/*-----------------------------------------------------------------------------
	ポーズにてＲＥＴＲＹ選択時の処理
-----------------------------------------------------------------------------*/
void CGame::Retry(void)
{
	// ライフ表示などに対応させるために、リトライライフ値を１減少
	int nowLife = CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE);
	CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_RETRYLIFE, nowLife - 1);

	m_bTransition = true;
	SetTransitionID(TRANSITIONID_GAME_RETRY);
}

/*-----------------------------------------------------------------------------
	ポーズにてＥＸＩＴ選択時の処理
-----------------------------------------------------------------------------*/
void CGame::ReturnToStageSelect(void)
{
	if(CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE) == 0)
	{
		m_pGameOverBG->SetDraw(true);
		m_bGameOver = true;

		PlayJingleOver();
	}
	else
	{
		m_bTransition = true;
		SetTransitionID(TRANSITIONID_STAGESELECT);
	}
}

/*-----------------------------------------------------------------------------
	ゲームオーバー表示
-----------------------------------------------------------------------------*/
void CGame::CheckGameOver(void)
{
	if(m_bGameOver == false)
	{
		return;
	}

	bool bSkip = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CONNECT);
	if(bSkip)
	{
		m_autoChange = TIME_AUTOCHANGE_GAMEOVER;
	}

	m_autoChange++;

	bool bAutoChange = m_autoChange > TIME_AUTOCHANGE_GAMEOVER;
	if(bAutoChange)
	{
		m_bTransition = true;
		SetTransitionID(TRANSITIONID_TITLE);
	}
}


/*-----------------------------------------------------------------------------
	ゲームBGM再生
-----------------------------------------------------------------------------*/
void CGame::PlayBGM(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_BGM_GAME);
}

/*-----------------------------------------------------------------------------
	ゲームBGM停止
-----------------------------------------------------------------------------*/
void CGame::StopBGM(void)
{
	CManager::GetSoundXAudio2()->Stop(CSoundXAudio2::SL_BGM_GAME);
}

/*-----------------------------------------------------------------------------
	ゲームクリアジングル再生
-----------------------------------------------------------------------------*/
void CGame::PlayJingleClear(void)
{
	// ステージクリアジングル再生の前にBGMは止めておく
	CManager::GetSoundXAudio2()->Stop(CSoundXAudio2::SL_BGM_GAME);

	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_JINGLE_CLEAR);
}

/*-----------------------------------------------------------------------------
	ゲームオーバージングル再生
-----------------------------------------------------------------------------*/
void CGame::PlayJingleOver(void)
{
	// ゲームオーバージングル再生の前にBGMは止めておく
	CManager::GetSoundXAudio2()->Stop(CSoundXAudio2::SL_BGM_GAME);

	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_JINGLE_OVER);
}

