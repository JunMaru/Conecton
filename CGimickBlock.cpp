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
	D3DXVECTOR3 workPos = antonPos - antonPosOld;

	// �M�~�b�N�n�̂��̂��X�V����
	switch (m_blockType)
	{
	case CBlock::BLOCKID_WARP_BLUE:

		// ���[�v����^�C�~���O�ɂȂ�����
		if (m_bWarpFlag
		 && m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
		{
			// �A���g����]�ڐ�̍��W��(�r�[�R�����ړ�����\������)
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

			// ���[�v�I��
			m_bWarpFlag = false;
			m_pWarpPoint -> SetWarpFlag( false );
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if ( m_nRetryWarpWaitTime == 0 )
		{
			// �O��ƍ���̍��W�̋��������̋�������������]�ڂ���
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 1.0f
			 && m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x )
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
		if (m_bWarpFlag
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
		{
			// �A���g����]�ڐ�̍��W��(�r�[�R�����ړ�����\������)
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

			// ���[�v�I��
			m_bWarpFlag = false;
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// �O��ƍ���̍��W�̋��������̋�������������]�ڂ���
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 5.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
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
		if (m_bWarpFlag
			&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
		{
			// �A���g����]�ڐ�̍��W��(�r�[�R�����ړ�����\������)
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

			// ���[�v�I��
			m_bWarpFlag = false;
			m_nRetryWarpWaitTime = WARP_WAIT_TIME;
		}
		else if (m_nRetryWarpWaitTime == 0)
		{
			// �O��ƍ���̍��W�̋��������̋�������������]�ڂ���
			if (sqrtf(workPos.x * workPos.x + workPos.y * workPos.y) <= 5.0f
				&& m_pos.x < antonPos.x && m_pos.x + BLOCK_WIDTH > antonPos.x)
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