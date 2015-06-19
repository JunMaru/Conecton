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
#include "CScene2D.h"
#include "collisionDetection.h"

//テスト
#include "CInstancingObject.h"
#include "CBlock.h"
#include "CAnton.h"
CInstancingObject *ins;

// temp(maru)
#include "CLaser.h"
#include "CBackGround.h"
#include "CScrollManager.h"
CScrollManager *pScrollManager;
CBackGround *pBackGround;

CBlock *Block[63];
CBlock *SpBlock[ 20 ];

/*-----------------------------------------------------------------------------
	静的メンバ変数の初期化
-----------------------------------------------------------------------------*/
CPlayer *CGame::m_pPlayer = nullptr;

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CGame::CGame()
{
	m_pPlayer = nullptr;
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
	m_pPlayer = CPlayer::Create(VEC3_ZERO,VEC3_ZERO);

	//test
	ins = new CInstancingObject(6);
	ins->LoadTexture( "data/texture/block/block.jpg",D3DXVECTOR2(300,100),D3DXVECTOR2(100,100) );
	ins->Init();
	
	for( int cnt = 0 ; cnt < 10 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( cnt * 100.0f,200.0f,0.0f );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 0,0 );

		Block[ cnt ] = b;
	}

	for( int cnt = 10 ; cnt < 20 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( cnt * 100.0f,700,0 );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 1,0 );

		Block[ cnt ] = b;
	}

	for( int cnt = 20 ; cnt < 40 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( 900,700  + 100 *(-20.0f +  cnt),0 );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 1,0 );

		Block[ cnt ] = b;
	}

	for( int cnt = 40 ; cnt < 60 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( 700.0f,700.0f + 100.0f *(-40 +  cnt),0.0f );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 1,0 );

		Block[ cnt ] = b;
	}
	
	for( int cnt = 60 ; cnt < 63 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( 700 + ( -60.0f +cnt) * 100,700 + 20 * 100,0 );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 1,0 );

		Block[ cnt ] = b;
	}
	
	for( int cnt = 0 ; cnt < 20 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( 800,700.0f + 100.0f * cnt,0 );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 2,0 );

		SpBlock[ cnt ] = b;		
	}
	
	// temp(maru)
	pBackGround = CBackGround::Create("data/texture/anton/anton_t.png");
	
	pScrollManager = new CScrollManager();
	pScrollManager->Init();

	// レーザー動作テスト
	CLaser::Create(D3DXVECTOR3(0.0f,100.0f,0.0f), CLaser::DIRECTION_RIGHT);

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

	pScrollManager->SetScrollWorld(VEC2_ZERO);
	delete pScrollManager;

	m_pPlayer->Uninit();
	delete m_pPlayer;
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
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
#ifdef _DEBUG
		if(pKeyboard->GetKeyTrigger(DIK_RETURN))
		{
			CManager::GetPhaseFade()->Start(
												CFade::FADETYPE_OUT,
												60.0f,
												D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
#endif
	}
	
	// フェードアウト完了後に画面遷移
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		CManager::SetPhase(CManager::PHASE_RESULT);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CGame.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:フェーズ遷移\n");
#endif

	m_pPlayer->Update();

	CAnton *ant = m_pPlayer->GetAnton();

	if (ant->GetState() == CAnton::STATE_MINIMUM)
	{
		HitCheckMinimumAnton();
	}
	else
	{
		HitCheckAnton();
	}
}

// ミニマム用のあたり判定。多分普通のと同じソースで実装できる
void CGame::HitCheckMinimumAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();
	D3DXVECTOR3 workAntonSize;
	workAntonSize.x = 10.0f;
	workAntonSize.y = ant->GetParameter().size.y;
	workAntonSize.z = 0.0f;


	//Hack くっそ雑なあたり判定
	for (int cnt = 0; cnt < 63; cnt++)
	{
		//アントン
		D3DXVECTOR3 left_ant = ant->GetPosition();

		left_ant.x -= workAntonSize.x;
		left_ant.y -= workAntonSize.y / 2;

		//ブロック
		D3DXVECTOR3 left_b = Block[cnt]->GetPosition();
		left_b.x -= 50;
		left_b.y -= 50;

		if (CheckCollisionRectVsRect(left_ant, workAntonSize, left_b, D3DXVECTOR3(100, 100, 0)) && left_b.y - left_ant.y >= 0)
		{
			D3DXVECTOR3 set_pos = Block[cnt]->GetPosition();
			float fAntonSize = workAntonSize.y * 0.75f;			// 0.5fだと半分めり込むのでさらにもう半分引き上げる
			set_pos.x = ant->GetPosition().x;
			set_pos.y -= fAntonSize + 50;
			ant->SetPosition(set_pos);
		}
	}

	for (int cnt = 0; cnt < 20; cnt++)
	{
		if (SpBlock[cnt] == NULL)
		{
			continue;
		}
		//アントン
		D3DXVECTOR3 left_ant = ant->GetPosition();

		left_ant.x -= workAntonSize.x;
		left_ant.y -= workAntonSize.y / 2;

		//ブロック
		D3DXVECTOR3 left_b = SpBlock[cnt]->GetPosition();
		left_b.x -= 50;
		left_b.y -= 50;

		if (CheckCollisionRectVsRect(left_ant, workAntonSize, left_b, D3DXVECTOR3(100, 100, 0)) && left_b.y - left_ant.y >= 0)
		{
				D3DXVECTOR3 set_pos = SpBlock[cnt]->GetPosition();
				float fAntonSize = workAntonSize.y * 0.75f;			// 0.5fだと半分めり込むのでさらにもう半分引き上げる
				set_pos.x = ant->GetPosition().x;
				set_pos.y -= fAntonSize + 50;
				ant->SetPosition(set_pos);
		}
	}

}
void CGame::HitCheckAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();

	//Hack くっそ雑なあたり判定
	for (int cnt = 0; cnt < 63; cnt++)
	{
		//アントン
		D3DXVECTOR3 left_ant = ant->GetPosition();

		left_ant.x -= 10;
		left_ant.y -= 60;

		//ブロック
		D3DXVECTOR3 left_b = Block[cnt]->GetPosition();
		left_b.x -= 50;
		left_b.y -= 50;

		if (CheckCollisionRectVsRect(left_ant, D3DXVECTOR3(10, 120, 0), left_b, D3DXVECTOR3(100, 100, 0)) && left_b.y - left_ant.y >= 0)
		{
			D3DXVECTOR3 set_pos = Block[cnt]->GetPosition();
			set_pos.x = ant->GetPosition().x;
			set_pos.y -= 60 + 50;
			ant->SetPosition(set_pos);
		}
	}

	for (int cnt = 0; cnt < 20; cnt++)
	{
		if (SpBlock[cnt] == NULL)
		{
			continue;
		}
		//アントン
		D3DXVECTOR3 left_ant = ant->GetPosition();

		left_ant.x -= 10;
		left_ant.y -= 60;

		//ブロック
		D3DXVECTOR3 left_b = SpBlock[cnt]->GetPosition();
		left_b.x -= 50;
		left_b.y -= 50;

		if (CheckCollisionRectVsRect(left_ant, D3DXVECTOR3(10, 120, 0), left_b, D3DXVECTOR3(100, 100, 0)) && left_b.y - left_ant.y >= 0)
		{
			if (ant->GetMass() >= 50)
			{
				SpBlock[cnt]->Uninit();
				SpBlock[cnt] = NULL;
			}
			else
			{
				D3DXVECTOR3 set_pos = SpBlock[cnt]->GetPosition();
				set_pos.x = ant->GetPosition().x;
				set_pos.y -= 60 + 50;
				ant->SetPosition(set_pos);
			}
		}
	}

}
