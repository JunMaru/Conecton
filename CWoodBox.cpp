//=============================================================================
// �ؔ��N���X
// AUTHOR : Yutaka Ichikawa
//  SINCE : 20150724
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================

#include "CWoodBox.h"
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

//=============================================================================
// ������
//=============================================================================
HRESULT CWoodBox::Init()
{
	m_posOld = D3DXVECTOR2( 0.0f, 0.0f );

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWoodBox::Uninit()
{
	CBlock::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CWoodBox::Update()
{
	m_posOld = m_pos;

	// �A���g���̃A�h���X������Ă���
	CAnton *pAnton = CGame::GetPlayer()->GetAnton();
	CBeecon *pBeecon = CGame::GetPlayer()->GetBeecon();
	CBlockManager *pBlockManager = CGame::GetBlockManager();

	// �A���g���̍��W�𒲂ׂ�B
	D3DXVECTOR3 antonPos = pAnton->GetPosition();
	D3DXVECTOR3 antonPosOld = pAnton->GetPositionOld();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(50.0f, 60.0f, 0.0f);

	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);
	D3DXVECTOR3 blockPos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f) + blockSize;

	// �ؔ��̔���ɕK�v�ȕϐ�
	int nArrayNumX = 0, nArrayNumY = 0;
	D3DXVECTOR3 underBlockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR2 boxMoveLength = D3DXVECTOR2(0.0f, 0.0f);
	CBlock *pBlock = nullptr;
	CWoodBox* pWoodBox = nullptr;

	// �Ԃ����Ă���A�p���t���A���g���ɕω����Ă����珈�����s��
	if (blockPos.y - blockSize.y < antonPos.y + antonSize.y && blockPos.y + blockSize.y > antonPos.y - antonSize.y
		&& blockPos.x - blockSize.x < antonPos.x + (antonSize.x * 1.65f) && blockPos.x + blockSize.x > antonPos.x
		&& (blockPos.x - blockSize.x) - (antonPos.x + antonSize.x) < BLOCK_WIDTH
		&& (blockPos.x - blockSize.x) - (antonPos.x + antonSize.x) >= WOODBOX_MOVE
		&& pAnton->GetState() == CAnton::STATE_POWERFUL)
	{
		blockPos.x += WOODBOX_MOVE;

		PlaySeBoxSlip();
	}

	else if (blockPos.y - blockSize.y < antonPos.y + antonSize.y && blockPos.y + blockSize.y > antonPos.y - antonSize.y
		&& blockPos.x - blockSize.x < antonPos.x + (antonSize.x * 1.65f) && blockPos.x + blockSize.x > antonPos.x
		&& (blockPos.x + blockSize.x) - (antonPos.x) > -BLOCK_WIDTH
		&& (blockPos.x - blockSize.x) - (antonPos.x + antonSize.x) <= -WOODBOX_MOVE
		&& pAnton->GetState() == CAnton::STATE_POWERFUL)
	{
		blockPos.x -= WOODBOX_MOVE;

		PlaySeBoxSlip();
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
	else if (pBlock->GetBlockId() > CBlock::BLOCKID_WARP_PINK && pBlock->GetBlockId() < CBlock::BLOCKID_NO_METAMOR)
	{
		blockPos.y += WOODBOX_MOVE;
	}
	else
	{
		underBlockPos = pBlock->GetPosition() + blockSize;

		if ((underBlockPos.y - blockSize.y) - (blockPos.y + blockSize.y) >= WOODBOX_MOVE)
		{
			blockPos.y += WOODBOX_MOVE;
		}
		else if (underBlockPos.x - blockSize.x < blockPos.x + blockSize.x && underBlockPos.x + blockSize.x > blockPos.x - blockSize.x
			&& ((underBlockPos.y - blockSize.y) - (blockPos.y + blockSize.y)) < 0.0f)
		{
			blockPos.y = underBlockPos.y - BLOCK_HEIGHT;
		}
	}

	m_pos = D3DXVECTOR2( blockPos.x - blockSize.x, blockPos.y - blockSize.y );

	boxMoveLength = m_pos - m_posOld;
	int nWoodBoxCnt = pBlockManager -> GetWoodBoxCnt();

	if ( boxMoveLength.x != 0.0f || boxMoveLength.y != 0.0f )
	{
		// �ؔ��z��ɂ���S�ؔ��Ƃ̓����蔻��
		for ( int nCnt = 0; nCnt < nWoodBoxCnt; nCnt++ )
		{
			pWoodBox = CBlockManager::GetWoodBoxAddress( nCnt );

			if (pWoodBox != nullptr && pWoodBox != this)
			{
				D3DXVECTOR3 woodBoxPos = pWoodBox->GetPosition();
				D3DXVECTOR3 workWoodBoxPos = woodBoxPos + blockSize;

				if ( blockPos.y - blockSize.y < workWoodBoxPos.y + blockSize.y && blockPos.y + blockSize.y > workWoodBoxPos.y - blockSize.y
				 &&  blockPos.x - blockSize.x < workWoodBoxPos.x + blockSize.x && blockPos.x + blockSize.x > workWoodBoxPos.x - blockSize.x )
				{
					woodBoxPos.x += boxMoveLength.x;
					woodBoxPos.y += boxMoveLength.y;

					pWoodBox -> SetPosition( woodBoxPos );

					break;
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWoodBox::Draw()
{
	CBlock::Draw();
}

/*-----------------------------------------------------------------------------
�ؔ������r�d�Đ�
-----------------------------------------------------------------------------*/
void CWoodBox::PlaySeBoxSlip(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_SE_BOX_SLIP);
}