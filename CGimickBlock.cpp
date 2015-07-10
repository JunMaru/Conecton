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
#include "CPlayer.h"
#include "CGame.h"

//=============================================================================
// ������
//=============================================================================
HRESULT CGimmickBlock::Init()
{
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

	// �M�~�b�N�n�̂��̂��X�V����
	switch (m_blockType)
	{
	case CBlock::BLOCKID_WARP:

		// ���[�v����^�C�~���O�ɂȂ�����
		if (m_bWarpFlag)
		{
			// �A���g����]�ڐ�̍��W��(�r�[�R�����ړ�����\������)
			D3DXVECTOR3 warpPos = m_pWarpPoint -> GetPosition();
			warpPos.x = warpPos.x + BLOCK_WIDTH;
			warpPos.y = warpPos.y - ( 65.0f - BLOCK_HEIGHT );

			pAnton -> SetPosition( warpPos );

			// ���[�v�I��
			m_bWarpFlag = false;
		}
		else
		{
			// �A���g���̍��W�𒲂ׂāA��~���Ă����烏�[�v����B
			D3DXVECTOR3 antonPos = pAnton -> GetPosition() - pAnton -> GetPositionOld();

			float fDot = sqrtf(antonPos.x * antonPos.x + antonPos.y * antonPos.y);
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