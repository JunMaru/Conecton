/*-----------------------------------------------------------------------------
	�t�F�[�h�N���X
	author	OkadaMoeto
	since	20140709
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CFade.h"
#include "Define.h"

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CFade::CFade(
				int nPriority,
				CScene::OBJTYPE objtype) : CScene2D(nPriority, objtype)
{
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f ,0.0f);
	m_timeFrame = 0.0f;
	m_bUse = false;
	m_alpha = 0.0f;
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CFade::~CFade()
{
}

/*-----------------------------------------------------------------------------
	2D�|���S���̃t�F�[�h����
-----------------------------------------------------------------------------*/
CFade* CFade::Create(
						D3DXVECTOR3 pos,
						D3DXVECTOR3 rot,
						D3DXCOLOR col)
{
	CFade *pFade;

	// �t�F�[�h�̐����Ə�����
	pFade = new CFade();
	pFade->Init(pos, rot, col);

	pFade->SetPosition(pos);
	pFade->SetRotation(rot);
	pFade->m_color = col;

	return pFade;
}

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
HRESULT CFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	// 2D�|���S���̏���������
	CScene2D::Init(
					NULL,
					pos,
					rot,
					col,
					(float)SCREEN_WIDTH,
					(float)SCREEN_HEIGHT);

	// �t�F�[�h�̏�����
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f , 0.0f);
	m_timeFrame = 0.0f;
	m_bUse = false;
	m_fadetype = FADETYPE_NONE;
	m_alpha = 0.0f;

	SetDiffuse(m_color);

	// �V�[���Ԃ��p��
	SetGlobal(true);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CFade::Uninit(void)
{
	// �V�[���Ԃ��p���𖳌�
	SetGlobal(false);

	// 2D�|���S���̏I������
	CScene2D::Uninit();
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CFade::Update(void)
{
	// �t�F�[�h���g�p���Ă��Ȃ���΁A�X�V���Ȃ�
	if(m_bUse == false){ return; }

	// 2D�|���S���̍X�V
	CScene2D::Update();

	if(m_bUse == true && m_fadetype != FADETYPE_NONE)
	{
		// �t�F�[�h������
		switch(m_fadetype)
		{
			case FADETYPE_OUT:
			{
				// �����l�����Z���ĉ�ʂ������Ă���
				m_alpha += 1.0f / m_timeFrame;
				if(m_alpha >= 1.0f)
				{
					// �t�F�[�h�A�E�g�I���ɐ؂�ւ���
					m_fadetype = FADETYPE_UNOUT;
				}
			}
			break;

			case FADETYPE_UNOUT:
			{
				// Nowloading�Ȃǂɍ��킹��ׂɃt�F�[�h�̒�~��Ԃɐ؂�ւ���
				m_alpha = 1.0f;
				m_fadetype = FADETYPE_INFINITY;
			}
			break;

			case FADETYPE_IN:
			{
				// ���l�����Z���ĉ�ʂ𕂂��オ�点��
				m_alpha -= 1.0f / m_timeFrame;
				if(m_alpha <= 0.0f)
				{
					// �t�F�[�h�C���I���ɐ؂�ւ���
					m_fadetype = FADETYPE_UNIN;
				}
			}
			break;

			case FADETYPE_UNIN:
			{
				m_alpha = 0.0f;
				m_bUse = false;
				m_fadetype = FADETYPE_NONE;
			}
			break;

			case FADETYPE_INFINITY:
			{
				m_alpha = 1.0f;

				if(m_fadetype != FADETYPE_INFINITY)
				{
					m_bUse = false;
				}
			}
			break;
		}

		// �����l��ݒ�
		m_color.a = m_alpha;
		// �|���S���F��ݒ�
		SetDiffuse(m_color);
	}
}

/*-----------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------*/
void CFade::Draw(void)
{
	// �t�F�[�h���g�p���Ă��Ȃ���΁A�`�悵�Ȃ�
	if(m_bUse == false){ return; }

	// 2D�|���S���̕`��
	CScene2D::Draw();
}

/*-----------------------------------------------------------------------------
	�t�F�[�h�̊J�n
-----------------------------------------------------------------------------*/
void CFade::Start(FADETYPE type, float time_frame, D3DXCOLOR color)
{
	// �t�F�[�hON
	m_bUse = true;

	// ��Ԃ�ݒ�
	m_fadetype = type;

	// ���Ԃ�ݒ�
	m_timeFrame = time_frame;

	// �F��ݒ�
	m_color = color;

	// �A���t�@�l�̐ݒ�
	if(m_fadetype == FADETYPE_IN)
	{
		m_alpha = 1.0f;
	}
	else if(m_fadetype == FADETYPE_OUT)
	{
		m_alpha = 0.0f;
	}

	m_color.a = m_alpha;

	// �F��ݒ�
	SetDiffuse(m_color);
}

// �t�F�[�h��������Ԃɖ߂��i�t�F�[�h�Ȃ�, FADETYPE_NONE�j
void CFade::Reset(void)
{
	m_alpha = 0.0f;
	m_bUse = false;
	m_fadetype = FADETYPE_NONE;
	m_color.a = m_alpha;
	SetDiffuse(m_color);
}

