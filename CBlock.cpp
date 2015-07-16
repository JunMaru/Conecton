/*-----------------------------------------------------------------------------
	�u���b�N�N���X
	author	HaradaYuto
	since	20140519
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/

#include "CBlock.h"
#include "CBlockManager.h"
#include "CInstancingObject.h"
#include "Define.h"
#include "CScrollManager.h"

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/

HRESULT CBlock::Init()
{
	m_nHitLaserNo = -1;
	return S_OK;
}

/*-----------------------------------------------------------------------------
	�㏈��
-----------------------------------------------------------------------------*/

void CBlock::Uninit()
{
	Release();
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/

void CBlock::Update()
{

}

/*-----------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------*/

void CBlock::Draw()
{
	D3DXVECTOR2 scroll_val = CScrollManager::GetScrollWorld();
	D3DXVECTOR2 scroll_pos;

	//�X�N���[�����̈ʒu
	scroll_pos = m_pos - scroll_val;

	//��ʊO���ǂ����̔���
	if( scroll_pos.x > -m_scl.x/2 && scroll_pos.x < SCREEN_WIDTH + m_scl.x/2 )
	{
		if( scroll_pos.y > -m_scl.y/2 && scroll_pos.y < SCREEN_HEIGHT + m_scl.y/2 )
		{
			// ��ɉ��n�ƂȂ�e�N�X�`����ݒu
			if (m_secondTexIdX != -1 && m_secondTexIdY != -1)
			{
				m_pInsObj->AddDataScreen(m_scl, m_rot, scroll_pos, m_secondTexIdX, m_secondTexIdY);
			}

			//�z�u
			if ( m_blockIdX != -1 && m_blockIdY != -1 )
			{
				m_pInsObj->AddDataScreen(m_scl, m_rot, scroll_pos, m_blockIdX, m_blockIdY);
			}
		}
	}
	
}

//=============================================================================
// �e�N�X�`������ւ�����
// param �ݒ肵�����e�N�X�`���̃u���b�NID
//=============================================================================
void CBlock::ChangeTexture(BLOCKID block_type)
{
	// �ϐg�A�C�e���֌W�Ȃ����n�̃e�N�X�`�������������A
	// �u���b�N��ID��ς���
	SetSecondTexID(-1, -1);
	SetBlockId(block_type);

	CBlock *pBlock = CBlockManager::GetBlock(( int )( m_pos.x / BLOCK_WIDTH ), ( int )( m_pos.y / BLOCK_HEIGHT ) - 1 );

	// �u���b�N�̎�ނɂ���ăe�N�X�`���̖�����ς��Đݒ�
	switch (block_type)
	{
	case BLOCKID_SOIL:
		SetBlockTexID(0, 0);
		break;

	case BLOCKID_GRASS:
		SetBlockTexID(1, 0);
		break;

	case BLOCKID_SOIL_CRACK:
		SetBlockTexID(2, 0);
		break;

	case BLOCKID_GRASS_CRACK:
		SetBlockTexID(3, 0);
		break;

	case BLOCKID_METAL:
		SetBlockTexID(0, 1);
		
		// �n�\�ɏo�Ă��邩�ǂ���
		if ( pBlock == nullptr )
		{
			SetSecondTexID(1, 0);
		}
		else
		{
			SetSecondTexID(0, 0);
		}

		break;

	case BLOCKID_MINIMUM:
		SetBlockTexID(0, 2);

		// �n�\�ɏo�Ă��邩�ǂ���
		if (pBlock == nullptr)
		{
			SetSecondTexID(1, 0);
		}
		else
		{
			SetSecondTexID(0, 0);
		}

		break;

	case BLOCKID_POWERFUL:
		SetBlockTexID(0, 3);
		
		// �n�\�ɏo�Ă��邩�ǂ���
		if (pBlock == nullptr)
		{
			SetSecondTexID(1, 0);
		}
		else
		{
			SetSecondTexID(0, 0);
		}

		break;

	case BLOCKID_LASER_GOAL_TOP:
		SetBlockTexID(0, 8);
		break;

	case BLOCKID_LASER_GOAL_BOTTOM:
		SetBlockTexID(1, 8);
		break;

	case BLOCKID_LASER_GOAL_LEFT:
		SetBlockTexID(2, 8);
		break;

	case BLOCKID_LASER_GOAL_RIGHT:
		SetBlockTexID(3, 8);
		break;

	case BLOCKID_LASER_START_TOP:
		SetBlockTexID(4, 8);
		break;

	case BLOCKID_LASER_START_BOTTOM:
		SetBlockTexID(5, 8);
		break;

	case BLOCKID_LASER_START_LEFT:
		SetBlockTexID(6, 8);
		break;

	case BLOCKID_LASER_START_RIGHT:
		SetBlockTexID(7, 8);
		break;

	case BLOCKID_LASER_CONTROL_DOWN:
		SetBlockTexID(0, 10);
		break;

	case BLOCKID_LASER_CONTROL_UP:
		SetBlockTexID(1, 10);
		break;

	case BLOCKID_LASER_CONTROL_RIGHT:
		SetBlockTexID(2, 10);
		break;

	case BLOCKID_LASER_CONTROL_LEFT:
		SetBlockTexID(3, 10);
		break;

	case BLOCKID_MAGNET:
		SetBlockTexID(0, 11);
		break;

	case BLOCKID_WOODBOX:
		SetBlockTexID(0, 12);
		break;

	case BLOCKID_SWITCH:
		SetBlockTexID(0, 13);
		break;

	case BLOCKID_WARP_BLUE:
		SetBlockTexID(0, 14);
		break;

	case BLOCKID_WARP_GREEN:
		SetBlockTexID(0, 15);
		break;

	case BLOCKID_WARP_PINK:
		SetBlockTexID(0, 16);
		break;

	case BLOCKID_NO_METAMOR:
		SetBlockTexID(0, 5);

		// �n�\�ɏo�Ă��邩�ǂ���
		if (pBlock == nullptr)
		{
			SetSecondTexID(1, 0);
		}
		else
		{
			SetSecondTexID(0, 0);
		}

		break;

	default:
		break;
	}
}