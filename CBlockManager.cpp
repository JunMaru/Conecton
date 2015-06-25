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
#include <stdio.h>

//=============================================================================
// グローバル
//=============================================================================
CInstancingObject *ins;

//=============================================================================
// 静的メンバ変数
//=============================================================================
CBlock *CBlockManager::m_pBlockArray[MAX_BLOCK_Y][MAX_BLOCK_X] = { nullptr };

//=============================================================================
// 生成処理
//=============================================================================
CBlockManager* CBlockManager::Create(void)
{
	CBlockManager *pBlockManager = new CBlockManager;
	pBlockManager -> Init();
	return pBlockManager;
}

//=============================================================================
// 初期化処理
// この初期化は、ゲームでの初期化のときだけ呼んでください。
//=============================================================================
HRESULT CBlockManager::Init()
{
	// テキストか何かにステージ情報を書き込んでおき、それを読み込んで
	// ステージを作成する
	if ( !CreateBlockMap("data/stage_info/stage1.bin") )
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
				// ここでブロックの生成
				CBlock *pBlock = new CBlock;
				pBlock -> SetInsObj( ins );
				pBlock -> Init();
				pBlock -> SetPosition( D3DXVECTOR3( nCntX * BLOCK_WIDTH, nCntY * BLOCK_HEIGHT, 0.0f ) );
				pBlock -> SetRotation( 0.0f, 0.0f, 0.0f );
				pBlock -> SetScaling( 50.0f, 50.0f );

				switch ( nBlockID )
				{
				case 1:		// 土ブロック
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
// 当たり判定用のブロックのアドレスゲッター
// 引数の説明
// character_pos....当たり判定を取るオブジェクトの座標。ここから判定を取るべき
// ブロックの配列番号を求める。
//
// character_size...配列番号を求める際に必要。今までのままなら座標は左上なので、
// sizeの半分ずらし、中心として判定する。
//
// 返り値:CBlock*型
//=============================================================================
CBlock* CBlockManager::GetBlock(D3DXVECTOR3 character_pos, D3DXVECTOR2 character_size)
{
	// Xは中心で、Yは足元で判定を取るので、それぞれずらす
	int nBlockArrayIdxNumX = ( character_pos.x + ( character_size.x / 2.0f ) ) / BLOCK_WIDTH;
	int nBlockArrayIdxNumY = ( character_pos.y + character_size.y ) / BLOCK_HEIGHT;

	// 足元の座標にあるブロックのアドレスを返す
	return m_pBlockArray[ nBlockArrayIdxNumY ][ nBlockArrayIdxNumY ];
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
	return m_pBlockArray[ n_block_idx_x][n_block_idx_y];
}

// End of file