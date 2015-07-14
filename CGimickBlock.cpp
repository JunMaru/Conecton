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
#include "CBeecon.h"
#include "CPlayer.h"
#include "CGame.h"

//=============================================================================
// 静的メンバ
//=============================================================================
bool CGimmickBlock::m_bWarpFlag = false;
int CGimmickBlock::m_nRetryWarpWaitTime = WARP_WAIT_TIME;

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGimmickBlock::Init()
{
	m_pWarpPoint = nullptr;

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
	CBeecon *pBeecon = CGame::GetPlayer()->GetBeecon();

	// アントンの座標を調べる。
	D3DXVECTOR3 antonPos = pAnton->GetPosition();
	D3DXVECTOR3 antonPosOld = pAnton->GetPositionOld();
	D3DXVECTOR3 workPos = antonPos - antonPosOld;

	// ギミック系のものを更新する
	switch (m_blockType)
	{
	case CBlock::BLOCKID_WARP_BLUE:

		// ワープするタイミングになったら
		if (m_bWarpFlag
		 && m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
		{
			// アントンを転移先の座標へ(ビーコンも移動する可能性あり)
			D3DXVECTOR3 warpPos = m_pWarpPoint -> GetPosition();
			warpPos.x = warpPos.x + BLOCK_WIDTH / 2.0f;
			warpPos.y = warpPos.y - ( 65.0f - BLOCK_HEIGHT );

			pAnton -> SetPosition( warpPos );
			pAnton -> SetPositionOld( warpPos );
			pAnton->SetTargetPosition( warpPos.x, warpPos.y );

			warpPos.y -= BLOCK_HEIGHT;

			pBeecon -> SetPosition( warpPos );
			pBeecon -> SetPositionOld( warpPos );
			pBeecon->SetTargetPosition( warpPos.x, warpPos.y );

			// ワープ終了
			m_bWarpFlag = false;
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if ( m_nRetryWarpWaitTime == 0 )
		{
			// 前回と今回の座標の距離が一定の距離内だったら転移する
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 5.0f
			 && m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x )
			{
				m_bWarpFlag = true;
			}
		}
		else
		{
			// 待ち時間がまだあれば減らす
			if (m_nRetryWarpWaitTime > 0)
			{
				m_nRetryWarpWaitTime--;
			}
		}

		break;

	case CBlock::BLOCKID_WARP_GREEN:

		// ワープするタイミングになったら
		if (m_bWarpFlag
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
		{
			// アントンを転移先の座標へ(ビーコンも移動する可能性あり)
			D3DXVECTOR3 warpPos = m_pWarpPoint->GetPosition();
			warpPos.x = warpPos.x + BLOCK_WIDTH / 2.0f;
			warpPos.y = warpPos.y - (65.0f - BLOCK_HEIGHT);

			pAnton->SetPosition(warpPos);
			pAnton->SetPositionOld(warpPos);
			pAnton->SetTargetPosition(warpPos.x, warpPos.y);

			warpPos.y -= BLOCK_HEIGHT;

			pBeecon->SetPosition(warpPos);
			pBeecon->SetPositionOld(warpPos);
			pBeecon->SetTargetPosition(warpPos.x, warpPos.y);

			// ワープ終了
			m_bWarpFlag = false;
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// 前回と今回の座標の距離が一定の距離内だったら転移する
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 5.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
			{
				m_bWarpFlag = true;
			}
		}
		else
		{
			// 待ち時間がまだあれば減らす
			if (m_nRetryWarpWaitTime > 0)
			{
				m_nRetryWarpWaitTime--;
			}
		}

		break;

	case CBlock::BLOCKID_WARP_PINK:

		// ワープするタイミングになったら
		if (m_bWarpFlag
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
		{
			// アントンを転移先の座標へ(ビーコンも移動する可能性あり)
			D3DXVECTOR3 warpPos = m_pWarpPoint->GetPosition();
			warpPos.x = warpPos.x + BLOCK_WIDTH / 2.0f;
			warpPos.y = warpPos.y - (65.0f - BLOCK_HEIGHT);

			pAnton->SetPosition(warpPos);
			pAnton->SetPositionOld(warpPos);
			pAnton->SetTargetPosition(warpPos.x, warpPos.y);

			warpPos.y -= BLOCK_HEIGHT;

			pBeecon->SetPosition(warpPos);
			pBeecon->SetPositionOld(warpPos);
			pBeecon->SetTargetPosition(warpPos.x, warpPos.y);

			// ワープ終了
			m_bWarpFlag = false;
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// 前回と今回の座標の距離が一定の距離内だったら転移する
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 5.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
			{
				m_bWarpFlag = true;
			}
		}
		else
		{
			// 待ち時間がまだあれば減らす
			if (m_nRetryWarpWaitTime > 0)
			{
				m_nRetryWarpWaitTime--;
			}
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