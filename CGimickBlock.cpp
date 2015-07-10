//=============================================================================
// ギミックブロッククラス
// AUTHOR : Yutaka Ichikawa
//  SINCE : 20150707
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================

#include "CGimickBlock.h"
#include "CBlock.h"
#include "CBlockManager.h"
#include "CAnton.h"
#include "CPlayer.h"
#include "CGame.h"

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGimmickBlock::Init()
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGimmickBlock::Uninit()
{
	CBlock::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGimmickBlock::Update()
{
	// アントンのアドレスを取ってくる
	CAnton *pAnton = CGame::GetPlayer()->GetAnton();

	// ギミック系のものを更新する
	switch (m_blockType)
	{
	case CBlock::BLOCKID_WARP:

		// ワープするタイミングになったら
		if (m_bWarpFlag)
		{
			// アントンを転移先の座標へ(ビーコンも移動する可能性あり)
			D3DXVECTOR3 warpPos = m_pWarpPoint -> GetPosition();
			warpPos.x = warpPos.x + BLOCK_WIDTH;
			warpPos.y = warpPos.y - ( 65.0f - BLOCK_HEIGHT );

			pAnton -> SetPosition( warpPos );

			// ワープ終了
			m_bWarpFlag = false;
		}
		else
		{
			// アントンの座標を調べて、停止していたらワープする。
			D3DXVECTOR3 antonPos = pAnton -> GetPosition() - pAnton -> GetPositionOld();

			float fDot = sqrtf(antonPos.x * antonPos.x + antonPos.y * antonPos.y);
		}

		break;

	default:
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGimmickBlock::Draw()
{
	CBlock::Draw();
}