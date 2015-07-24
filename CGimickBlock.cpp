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

	// アントンの座標を調べる。
	D3DXVECTOR3 antonPos = pAnton->GetPosition();
	D3DXVECTOR3 antonPosOld = pAnton->GetPositionOld();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(50.0f, 60.0f, 0.0f);
	D3DXVECTOR3 workPos = antonPos - antonPosOld;
	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);
	D3DXVECTOR3 blockPos = D3DXVECTOR3( m_pos.x, m_pos.y, 0.0f ) + blockSize;

	D3DXVECTOR3 checkPos = blockPos;

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

	case CBlock::BLOCKID_WOODBOX:

		// ぶつかっており、パワフルアントンに変化していたら処理を行う
		if (blockPos.y - blockSize.y < antonPos.y + antonSize.y && blockPos.y + blockSize.y > antonPos.y - antonSize.y
		&& blockPos.x - blockSize.x < antonPos.x + ( antonSize.x * 1.65f ) && blockPos.x + blockSize.x > antonPos.x
		&& (blockPos.x - blockSize.x ) - ( antonPos.x + antonSize.x ) < BLOCK_WIDTH
		&& (blockPos.x - blockSize.x) - (antonPos.x + antonSize.x) >= WOODBOX_MOVE
		&& pAnton->GetState() == CAnton::STATE_POWERFUL)
		{
			blockPos.x += WOODBOX_MOVE;

			bCheckRightHit = true;

			PlaySeBoxSlip();
		}

		else if (blockPos.y - blockSize.y < antonPos.y + antonSize.y && blockPos.y + blockSize.y > antonPos.y - antonSize.y
			 && blockPos.x - blockSize.x < antonPos.x + ( antonSize.x * 1.65f ) && blockPos.x + blockSize.x > antonPos.x
			 && (blockPos.x + blockSize.x) - (antonPos.x) > -BLOCK_WIDTH
			 && (blockPos.x - blockSize.x) - (antonPos.x + antonSize.x) <= -WOODBOX_MOVE
			 && pAnton->GetState() == CAnton::STATE_POWERFUL)
		{
			blockPos.x -= WOODBOX_MOVE;

			bCheckLeftHit = true;

			PlaySeBoxSlip();
		}
		
		if(checkPos != blockPos)
		{
			pAnton->SetAction(CAnton::ACTION_PUSH);
		}

		// 落下の判定は、変化前の座標からブロックIDを求めて
		// １つ下のブロックのアドレスを求める。nullだった場合は、落下。
		// nullでなくても地面と接していなかった場合も落下する。
		CBlockManager::CalculateBlockArrayNum(D3DXVECTOR3(m_pos.x + BLOCK_WIDTH / 2.0f, m_pos.y + BLOCK_HEIGHT / 2.0f, 0), &nArrayNumX, &nArrayNumY);

		pBlock = CBlockManager::GetBlock(nArrayNumX, nArrayNumY + 1);

		if (pBlock == nullptr)
		{
			blockPos.y += WOODBOX_MOVE;
		}
		else
		{
			underBlockPos = pBlock -> GetPosition() + blockSize;

			if ( ( underBlockPos.y - blockSize.y ) - ( blockPos.y + blockSize.y ) >= WOODBOX_MOVE )
			{
				blockPos.y += WOODBOX_MOVE;
			}
			else if ( underBlockPos.x - blockSize.x < blockPos.x + blockSize.x && underBlockPos.x + blockSize.x > blockPos.x - blockSize.x
			   && ( ( underBlockPos.y - blockSize.y ) - ( blockPos.y + blockSize.y ) ) < 0.0f )
			{
				blockPos.y = underBlockPos.y - blockSize.y - BLOCK_HEIGHT;
			}
		}

		if ( bCheckRightHit == true )
		{
			// もし、右に木箱があったらそれもずらしておく
			pBlock = CBlockManager::GetBlock(nArrayNumX + 1, nArrayNumY);

			if (pBlock != nullptr && pBlock->GetBlockId() == CBlock::BLOCKID_WOODBOX)
			{
				workBlockPos2 = pBlock->GetPosition() + blockSize;

				if (workBlockPos2.y - blockSize.y < blockPos.y + blockSize.y && workBlockPos2.y + blockSize.y > blockPos.y - blockSize.y
					&& workBlockPos2.x - blockSize.x < blockPos.x + blockSize.x && workBlockPos2.x + blockSize.x > blockPos.x - blockSize.x
					&& workBlockPos2.x - blockPos.x < BLOCK_WIDTH && workBlockPos2.x - blockPos.x > 0.0f)
				{
					workBlockPos2.x = blockPos.x + blockSize.x;
					workBlockPos2.y = blockPos.y - blockSize.y;

					pBlock->SetPosition(workBlockPos2);

					CBlockManager::CalculateBlockArrayNum(workBlockPos2, &nWorkBlockArrayNumX, &nWorkBlockArrayNumY);

					if (nWorkBlockArrayNumX != nArrayNumX + 1)
					{
						CBlockManager::SetBlock(nWorkBlockArrayNumX, nWorkBlockArrayNumY, pBlock);
						CBlockManager::SetBlock(nArrayNumX + 1, nArrayNumY, nullptr);
					}
				}
			}
		}
		else if ( bCheckLeftHit == true )
		{
			// もし、左に木箱があったらそれもずらしておく
			pBlock = CBlockManager::GetBlock(nArrayNumX - 1, nArrayNumY);

			if (pBlock != nullptr && pBlock->GetBlockId() == CBlock::BLOCKID_WOODBOX)
			{
				workBlockPos2 = pBlock->GetPosition() + blockSize;

				if (workBlockPos2.y - blockSize.y < blockPos.y + blockSize.y && workBlockPos2.y + blockSize.y > blockPos.y - blockSize.y
					&& workBlockPos2.x - blockSize.x < blockPos.x + blockSize.x && workBlockPos2.x + blockSize.x > blockPos.x - blockSize.x
					&& workBlockPos2.x - blockPos.x > -BLOCK_WIDTH && workBlockPos2.x - blockPos.x < 0.0f)
				{
					workBlockPos2.x = blockPos.x - blockSize.x - BLOCK_WIDTH;
					workBlockPos2.y = blockPos.y - blockSize.y;

					pBlock->SetPosition(workBlockPos2);

					CBlockManager::CalculateBlockArrayNum(workBlockPos2, &nWorkBlockArrayNumX, &nWorkBlockArrayNumY);

					if (nWorkBlockArrayNumX != nArrayNumX - 1)
					{
						CBlockManager::SetBlock(nWorkBlockArrayNumX, nWorkBlockArrayNumY, pBlock);
						CBlockManager::SetBlock(nArrayNumX - 1, nArrayNumY, nullptr);
					}
				}
			}
		}

		// 落下していき、配列番号が変化する位置になった場合は、
		// BlockManagerに登録してある場所を切り替える。その時、
		// 前回いた場所を初期化しておく。
		workBlockPos = blockPos - blockSize;

		CBlockManager::CalculateBlockArrayNum(workBlockPos, &nWorkBlockArrayNumX, &nWorkBlockArrayNumY);

		if ( nArrayNumX != nWorkBlockArrayNumX || nArrayNumY != nWorkBlockArrayNumY )
		{
			CBlockManager::SetBlock(nWorkBlockArrayNumX, nWorkBlockArrayNumY, ( CBlock* )this);
			CBlockManager::SetBlock(nArrayNumX, nArrayNumY, nullptr);
		}

		m_pos = D3DXVECTOR2(workBlockPos.x, workBlockPos.y);

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

/*-----------------------------------------------------------------------------
	木箱押しＳＥ再生
-----------------------------------------------------------------------------*/
void CGimmickBlock::PlaySeBoxSlip(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_SE_BOX_SLIP);
}