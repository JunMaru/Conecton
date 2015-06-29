/*-----------------------------------------------------------------------------
	�u���b�N�N���X
	author	HaradaYuto
	since	20140519
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/

#include "CBlock.h"
#include "CInstancingObject.h"
#include "Define.h"
#include "CScrollManager.h"

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/

HRESULT CBlock::Init()
{
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
			m_pInsObj->AddDataScreen( m_scl, m_rot, scroll_pos, m_blockIdX, m_blockIdY );
		}
	}
	
}