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

	void SetWarpFlag(bool b_warp_flag){ m_bWarpFlag = b_warp_flag; }
	bool GetWarpFlag(){ return m_bWarpFlag; }

	CGimmickBlock *GetWarpPoint(){ return m_pWarpPoint; }	// 転移先のアドレスゲッター

private:
	CGimmickBlock *m_pWarpPoint;	// このワープゾーンの転移先へのアドレス
	bool m_bWarpFlag;				// ワープフラグ

};

// End of file