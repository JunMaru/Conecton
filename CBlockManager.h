//=============================================================================
//
// CBlockManager 定義部分[ CBlockManager.h ]
// Author : Yutaka Ichikawa
//
//=============================================================================

// 二重インクルード防止
#pragma once

//=============================================================================
// インクルード
//=============================================================================
#include "Main.h"

//=============================================================================
// マクロ
//=============================================================================
#define MAX_BLOCK_X ( 10 )
#define MAX_BLOCK_Y ( 10 )

//=============================================================================
// 前方定義
//=============================================================================
class CBlock;

//=============================================================================
// クラス定義
//=============================================================================
class CBlockManager
{
public:
	CBlockManager(){}
	~CBlockManager(){}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CBlockManager* Create( void );

	// ステージ生成処理
	bool CreateBlockMap(char *p_stagemap_filename);

	// ブロックへのアドレスゲッター
	static CBlock* GetBlock(D3DXVECTOR3 character_pos, D3DXVECTOR2 character_size);		// 座標から
	static CBlock* GetBlock(int n_block_idx_x, int n_block_idx_y);						// 配列番号指定

private:
	// ステージ上のブロックへのアドレス管理場所
	static CBlock *m_pBlockArray[MAX_BLOCK_Y][MAX_BLOCK_X];

	// CSVの情報を受け取る場所？
};

// End of file