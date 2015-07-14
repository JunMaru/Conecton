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
#include "CBlock.h"
#include < stdio.h >

//=============================================================================
// マクロ
//=============================================================================
#define MAX_BLOCK_X ( 30 )
#define MAX_BLOCK_Y ( 50 )

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

	static CBlockManager* Create( char *pFileName );

	// ステージ生成処理
	bool CreateBlockMap(char *p_stagemap_filename);

	// ブロックへのアドレスゲッター
	// 全てのオブジェクトが左上基準なので、ゲットする際は一度中心座標にして
	// ブロックの大きさで割ると正確な配列番号が取れる。
	static CBlock* GetBlock(int n_block_idx_x, int n_block_idx_y);		// 配列番号指定
	static void SetBlock(int n_block_idx_x, int n_block_idx_y, CBlock* p_block){ m_pBlockArray[n_block_idx_y * MAX_BLOCK_X + n_block_idx_x] = p_block; }
	static CBlock* GetLaserStart(){ return m_pLaserStart; }
	static CBlock* GetLaserGoal(){ return m_pLaserGoal; }

	// ブロック配列番号算出関数
	static void CalculateBlockArrayNum(D3DXVECTOR3 pos, int *p_array_x, int *p_array_y);

	// ブロックIDゲッター
	CBlock::BLOCKID GetBlockID( D3DXVECTOR3 pos );

	// ギミックブロックへ上書きする関数
	void OverwriteGimmickBlock( CBlock::BLOCKID block_type, D3DXVECTOR3 pos );

	// ワープブロック転移先設定関数( CreateBlockMap終了時に呼ぶ )
	void SetWarpPoint();

private:
	// ステージ上のブロックへのアドレス管理場所
	static CBlock *m_pBlockArray[MAX_BLOCK_Y * MAX_BLOCK_X];
	static CBlock *m_pLaserStart;		// レーザースタート位置へのアドレス
	static CBlock *m_pLaserGoal;		// レーザーゴール位置へのアドレス

	// CSVの情報を受け取る場所？
	int m_nMaxArrayNumX;	// ブロック配列最大数-X
	int m_nMaxArrayNumY;	// ブロック配列最大数-Y

	char *m_pFileName;
};

// End of file