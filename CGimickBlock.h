//=============================================================================
// ギミックブロッククラス
// AUTHOR : Yutaka Ichikawa
// SINCE  : 20150707
//=============================================================================

// ２重インクルード防止
#pragma once

//=============================================================================
// インクルードファイル
//=============================================================================

#include "CScene.h"
#include "CBlock.h"

//=============================================================================
// マクロ
//=============================================================================
#define WARP_WAIT_TIME ( 500 )

//=============================================================================
// class
//=============================================================================
class CInstancingObject;
class CBlock;

//=============================================================================
// ギミックブロッククラス
//=============================================================================
class CGimmickBlock : public CBlock
{
public:
	typedef enum
	{
		SWITCH_ACTION_CREATE_GRASS = 0,		// 草ブロック生成ギミック
		SWITCH_ACTION_CREATE_
	}SWITCH_ACTION;

	CGimmickBlock(const int Layer = 0){}
	virtual ~CGimmickBlock(){}

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetWarpFlag(bool b_warp_flag){ m_bWarpFlag = b_warp_flag; }
	static bool GetWarpFlag(){ return m_bWarpFlag; }

	static void SetRetryWarpWaitTime(int n_retry_warp_wait_time){ m_nRetryWarpWaitTime = n_retry_warp_wait_time; }

	CGimmickBlock *GetWarpPoint(){ return m_pWarpPoint; }	// 転移先のアドレスゲッター
	void SetWarpPoint(CGimmickBlock *p_warp_point){ m_pWarpPoint = p_warp_point; }

private:
	CGimmickBlock *m_pWarpPoint;	// このワープゾーンの転移先へのアドレス
	static bool m_bWarpFlag;				// ワープフラグ
	static int m_nRetryWarpWaitTime;		// 再びワープ判定を取るまでの待ち時間

};

// End of file