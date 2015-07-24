//=============================================================================
// �M�~�b�N�u���b�N�N���X
// AUTHOR : Yutaka Ichikawa
//  SINCE : 20150707
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
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
// �ÓI�����o
//=============================================================================
int CGimmickBlock::m_nRetryWarpWaitTime = WARP_WAIT_TIME;

//=============================================================================
// ������
//=============================================================================
HRESULT CGimmickBlock::Init()
{
	m_pWarpPoint = nullptr;
	m_bWarpFlag = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGimmickBlock::Uninit()
{
	CBlock::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGimmickBlock::Update()
{
	// �A���g���̃A�h���X������Ă���
	CAnton *pAnton = CGame::GetPlayer()->GetAnton();
	CBeecon *pBeecon = CGame::GetPlayer()->GetBeecon();

	// �A���g���̍��W�𒲂ׂ�B
	D3DXVECTOR3 antonPos = pAnton->GetPosition();
	D3DXVECTOR3 antonPosOld = pAnton->GetPositionOld();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(50.0f, 60.0f, 0.0f);
	D3DXVECTOR3 workPos = antonPos - antonPosOld;
	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);
	D3DXVECTOR3 blockPos = D3DXVECTOR3( m_pos.x, m_pos.y, 0.0f ) + blockSize;

	D3DXVECTOR3 checkPos = blockPos;

	// �ؔ��̔���ɕK�v�ȕϐ�
	CBlock* pBlock = nullptr;
	int nArrayNumX = 0, nArrayNumY = 0;
	int nWorkBlockArrayNumX = 0, nWorkBlockArrayNumY = 0;
	D3DXVECTOR3 underBlockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 workBlockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 workBlockPos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bCheckRightHit = false, bCheckLeftHit = false;

	// �M�~�b�N�n�̂��̂��X�V����
	switch (m_blockType)
	{
	case CBlock::BLOCKID_MAGNET:

		// �A���g�������^���̏�ԂȂ画����s��
		if (pAnton->GetState() == CAnton::STATE_METAL)
		{
			// �}�O�l�b�g�u���b�N�ƃ��^���A���g���̋��������͈͓̔��ɂ�����
			// ���^���A���g�����z���t������B
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

		// �Ԃ����Ă���A�p���t���A���g���ɕω����Ă����珈�����s��
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

		// �����̔���́A�ω��O�̍��W����u���b�NID�����߂�
		// �P���̃u���b�N�̃A�h���X�����߂�Bnull�������ꍇ�́A�����B
		// null�łȂ��Ă��n�ʂƐڂ��Ă��Ȃ������ꍇ����������B
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
			// �����A�E�ɖؔ����������炻������炵�Ă���
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
			// �����A���ɖؔ����������炻������炵�Ă���
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

		// �������Ă����A�z��ԍ����ω�����ʒu�ɂȂ����ꍇ�́A
		// BlockManager�ɓo�^���Ă���ꏊ��؂�ւ���B���̎��A
		// �O�񂢂��ꏊ�����������Ă����B
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

		// ���[�v����^�C�~���O�ɂȂ�����
		if (m_bWarpFlag && m_pWarpPoint->GetWarpFlag()
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x
			&& m_pos.y < antonPos.y + BLOCK_HEIGHT && m_pos.y + BLOCK_HEIGHT > antonPos.y + BLOCK_HEIGHT)
		{
			// �A���g����]�ڐ�̍��W��(�r�[�R�����ړ�����\������)
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

			// ���[�v�I��
			m_bWarpFlag = false;
			m_pWarpPoint->SetWarpFlag(false);
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// �O��ƍ���̍��W�̋��������̋�������������]�ڂ���
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 1.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
			{
				m_bWarpFlag = true;
			}
			// �]���������[�v��ԂɂȂ��Ă�����A�]����ł��邱�����
			// ���[�v�t���O�𗧂Ă�
			else if (m_pWarpPoint->GetWarpFlag() == true)
			{
				m_bWarpFlag = true;
			}
		}
		else
		{
			// �҂����Ԃ��܂�����Ό��炷
			if (m_nRetryWarpWaitTime > 0)
			{
				m_nRetryWarpWaitTime--;
			}
		}

		break;

	case CBlock::BLOCKID_WARP_GREEN:

		// ���[�v����^�C�~���O�ɂȂ�����
		if (m_bWarpFlag && m_pWarpPoint->GetWarpFlag()
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x
			&& m_pos.y < antonPos.y + BLOCK_HEIGHT && m_pos.y + BLOCK_HEIGHT > antonPos.y + BLOCK_HEIGHT)
		{
			// �A���g����]�ڐ�̍��W��(�r�[�R�����ړ�����\������)
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

			// ���[�v�I��
			m_bWarpFlag = false;
			m_pWarpPoint->SetWarpFlag(false);
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// �O��ƍ���̍��W�̋��������̋�������������]�ڂ���
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 1.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
			{
				m_bWarpFlag = true;
			}
			// �]���������[�v��ԂɂȂ��Ă�����A�]����ł��邱�����
			// ���[�v�t���O�𗧂Ă�
			else if (m_pWarpPoint->GetWarpFlag() == true)
			{
				m_bWarpFlag = true;
			}
		}
		else
		{
			// �҂����Ԃ��܂�����Ό��炷
			if (m_nRetryWarpWaitTime > 0)
			{
				m_nRetryWarpWaitTime--;
			}
		}

		break;

	case CBlock::BLOCKID_WARP_PINK:

		// ���[�v����^�C�~���O�ɂȂ�����
		if (m_bWarpFlag && m_pWarpPoint->GetWarpFlag()
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x
			&& m_pos.y < antonPos.y + BLOCK_HEIGHT && m_pos.y + BLOCK_HEIGHT > antonPos.y + BLOCK_HEIGHT)
		{
			// �A���g����]�ڐ�̍��W��(�r�[�R�����ړ�����\������)
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

			// ���[�v�I��
			m_bWarpFlag = false;
			m_pWarpPoint->SetWarpFlag(false);
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// �O��ƍ���̍��W�̋��������̋�������������]�ڂ���
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 1.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
			{
				m_bWarpFlag = true;
			}
			// �]���������[�v��ԂɂȂ��Ă�����A�]����ł��邱�����
			// ���[�v�t���O�𗧂Ă�
			else if (m_pWarpPoint->GetWarpFlag() == true)
			{
				m_bWarpFlag = true;
			}
		}
		else
		{
			// �҂����Ԃ��܂�����Ό��炷
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
// �`�揈��
//=============================================================================
void CGimmickBlock::Draw()
{
	CBlock::Draw();
}

/*-----------------------------------------------------------------------------
	�ؔ������r�d�Đ�
-----------------------------------------------------------------------------*/
void CGimmickBlock::PlaySeBoxSlip(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_SE_BOX_SLIP);
}