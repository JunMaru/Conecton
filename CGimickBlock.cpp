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
#include "CSoundXAudio2.h"

//=============================================================================
// 静的メンバ
//=============================================================================
int CGimmickBlock::m_nRetryWarpWaitTime = WARP_WAIT_TIME;

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGimmickBlock::Init()
{
	m_pWarpPoint = nullptr;
	m_bWarpFlag = false;

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
	CBlockManager *pBlockManager = CGame::GetBlockManager();

	// アントンの座標を調べる。
	D3DXVECTOR3 antonPos = pAnton->GetPosition();
	D3DXVECTOR3 antonPosOld = pAnton->GetPositionOld();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(50.0f, 60.0f, 0.0f);
	D3DXVECTOR3 workPos = antonPos - antonPosOld;
	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);
	D3DXVECTOR3 blockPos = D3DXVECTOR3( m_pos.x, m_pos.y, 0.0f ) + blockSize;

	// 木箱の判定に必要な変数
	CBlock* pBlock = nullptr;
	int nArrayNumX = 0, nArrayNumY = 0;
	int nWorkBlockArrayNumX = 0, nWorkBlockArrayNumY = 0;
	D3DXVECTOR3 underBlockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 workBlockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 workBlockPos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bCheckRightHit = false, bCheckLeftHit = false;

	// ギミック系のものを更新する
	switch (m_blockType)
	{
	case CBlock::BLOCKID_MAGNET:

		// アントンがメタルの状態なら判定を行う
		if (pAnton->GetState() == CAnton::STATE_METAL)
		{
			// マグネットブロックとメタルアントンの距離が一定の範囲内にいたら
			// メタルアントンを吸い付かせる。
			float fLength = sqrtf( ( blockPos.x - antonPos.x ) * ( blockPos.x - antonPos.x ) + ( blockPos.y - antonPos.y ) * ( blockPos.y - antonPos.y ) );

			if ( fLength <= STICK_LENGTH )
			{
				if (antonPos.y + antonSize.y > blockPos.y - blockSize.y && antonPos.y < blockPos.y + blockSize.y)
				{
					if ( antonPos.x > blockPos.x )
					{
						antonPos.x += ( ( blockPos.x + blockSize.x ) - antonPos.x ) * 0.3f;
					}
					else if ( antonPos.x < blockPos.x )
					{
						antonPos.x += ( ( blockPos.x - blockSize.x ) - antonPos.x ) * 0.3f;
					}
				}
				else if ( antonPos.y > blockPos.y )
				{
					antonPos.x += (blockPos.x - antonPos.x) * 0.3f;
					antonPos.y += (blockPos.y - antonPos.y) * 0.3f;
				}
				else if (antonPos.y < blockPos.y)
				{
					antonPos.x += (blockPos.x - antonPos.x) * 0.3f;
				}

				pAnton -> SetPosition( antonPos );

				//CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_SE_MAGNET);
			}
		}

		break;

	case CBlock::BLOCKID_WARP_BLUE:

		// ワープするタイミングになったら
		if (m_bWarpFlag && m_pWarpPoint->GetWarpFlag()
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x
			&& m_pos.y < antonPos.y + BLOCK_HEIGHT && m_pos.y + BLOCK_HEIGHT > antonPos.y + BLOCK_HEIGHT)
		{
			// アントンを転移先の座標へ(ビーコンも移動する可能性あり)
			D3DXVECTOR3 warpPos = m_pWarpPoint->GetPosition();
			warpPos.x = warpPos.x + BLOCK_WIDTH / 2.0f;
			warpPos.y = warpPos.y - (65.0f);

			pAnton->SetPosition(warpPos);
			pAnton->SetPositionOld(warpPos);
			pAnton->SetTargetPosition(warpPos.x, warpPos.y);

			warpPos.y -= BLOCK_HEIGHT;

			pBeecon->SetPosition(warpPos);
			pBeecon->SetPositionOld(warpPos);
			pBeecon->SetTargetPosition(warpPos.x, warpPos.y);

			// ワープ終了
			m_bWarpFlag = false;
			m_pWarpPoint->SetWarpFlag(false);
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// 前回と今回の座標の距離が一定の距離内だったら転移する
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 1.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
			{
				m_bWarpFlag = true;
			}
			// 転送元がワープ状態になっていたら、転送先であるこちらも
			// ワープフラグを立てる
			else if (m_pWarpPoint->GetWarpFlag() == true)
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
		if (m_bWarpFlag && m_pWarpPoint->GetWarpFlag()
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x
			&& m_pos.y < antonPos.y + BLOCK_HEIGHT && m_pos.y + BLOCK_HEIGHT > antonPos.y + BLOCK_HEIGHT)
		{
			// アントンを転移先の座標へ(ビーコンも移動する可能性あり)
			D3DXVECTOR3 warpPos = m_pWarpPoint->GetPosition();
			warpPos.x = warpPos.x + BLOCK_WIDTH / 2.0f;
			warpPos.y = warpPos.y - (65.0f);

			pAnton->SetPosition(warpPos);
			pAnton->SetPositionOld(warpPos);
			pAnton->SetTargetPosition(warpPos.x, warpPos.y);

			warpPos.y -= BLOCK_HEIGHT;

			pBeecon->SetPosition(warpPos);
			pBeecon->SetPositionOld(warpPos);
			pBeecon->SetTargetPosition(warpPos.x, warpPos.y);

			// ワープ終了
			m_bWarpFlag = false;
			m_pWarpPoint->SetWarpFlag(false);
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// 前回と今回の座標の距離が一定の距離内だったら転移する
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 1.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
			{
				m_bWarpFlag = true;
			}
			// 転送元がワープ状態になっていたら、転送先であるこちらも
			// ワープフラグを立てる
			else if (m_pWarpPoint->GetWarpFlag() == true)
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
		if (m_bWarpFlag && m_pWarpPoint->GetWarpFlag()
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x
			&& m_pos.y < antonPos.y + BLOCK_HEIGHT && m_pos.y + BLOCK_HEIGHT > antonPos.y + BLOCK_HEIGHT)
		{
			// アントンを転移先の座標へ(ビーコンも移動する可能性あり)
			D3DXVECTOR3 warpPos = m_pWarpPoint->GetPosition();
			warpPos.x = warpPos.x + BLOCK_WIDTH / 2.0f;
			warpPos.y = warpPos.y - (65.0f);

			pAnton->SetPosition(warpPos);
			pAnton->SetPositionOld(warpPos);
			pAnton->SetTargetPosition(warpPos.x, warpPos.y);

			warpPos.y -= BLOCK_HEIGHT;

			pBeecon->SetPosition(warpPos);
			pBeecon->SetPositionOld(warpPos);
			pBeecon->SetTargetPosition(warpPos.x, warpPos.y);

			// ワープ終了
			m_bWarpFlag = false;
			m_pWarpPoint->SetWarpFlag(false);
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// 前回と今回の座標の距離が一定の距離内だったら転移する
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 1.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
			{
				m_bWarpFlag = true;
			}
			// 転送元がワープ状態になっていたら、転送先であるこちらも
			// ワープフラグを立てる
			else if (m_pWarpPoint->GetWarpFlag() == true)
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

// End of file