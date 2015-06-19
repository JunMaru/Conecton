/*-----------------------------------------------------------------------------
	�t�F�[�h�N���X
	author	OkadaMoeto
	since	20140709
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CScene2D.h"

/*-----------------------------------------------------------------------------
	�t�F�[�h�N���X
-----------------------------------------------------------------------------*/
class CFade : public CScene2D
{
	public:
		// �t�F�[�h���
		typedef enum
		{
			FADETYPE_NONE = 0,
			FADETYPE_IN,
			FADETYPE_UNIN,
			FADETYPE_OUT,
			FADETYPE_UNOUT,
			FADETYPE_INFINITY,
			FADETYPE_MAX
		}FADETYPE;

		CFade(
				int nPriority = 7,
				CScene::OBJTYPE objtype = CScene::OBJTYPE_FADE);
		virtual ~CFade();

		/*-----------------------------------------------------------------------------
			2D�|���S���̃t�F�[�h����
			return	CFade*	CFade�̃C���X�^���X��Ԃ�
			param	pos		�|���S�����S�\�����W
			param	rot		����
			param	col		�\���F
		-----------------------------------------------------------------------------*/
		static CFade* Create(
								D3DXVECTOR3 pos,
								D3DXVECTOR3 rot,
								D3DXCOLOR color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);
		void Uninit(void);
		void Update(void);
		void Draw(void);

		/*-----------------------------------------------------------------------------
			�t�F�[�h�̊J�n
			param	type		�t�F�[�h�^�C�v
			param	time_frame	�t�F�[�h����
			param	color		�t�F�[�h�F
		-----------------------------------------------------------------------------*/
		void Start(FADETYPE type, float time_frame, D3DXCOLOR color);

		/*-----------------------------------------------------------------------------
			�t�F�[�h��ʂ̎擾
			return	FADETYPE		���݂̃t�F�[�h���
		-----------------------------------------------------------------------------*/
		FADETYPE GetFadetype(void){ return m_fadetype; }
		/*-----------------------------------------------------------------------------
			�t�F�[�h��ʂ̐ݒ�
			param	type		�t�F�[�h��ʂ̐ݒ�l
		-----------------------------------------------------------------------------*/
		void GetFadetype(FADETYPE type){ m_fadetype = type; }

	private:
		// �t�F�[�h�F
		D3DXCOLOR m_color;
		// �t�F�[�h����(�t���[���P��)
		float m_timeFrame;
		// �t�F�[�h�̎g�p�t���O
		bool m_bUse;
		// �t�F�[�h�̎��
		FADETYPE m_fadetype;
		// �t�F�[�h�̓����x
		float m_alpha;
};
