/*-----------------------------------------------------------------------------
	�r���{�[�h�N���X
	author	OkadaMoeto
	since	20140617
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "Main.h"
#include "CScene.h"

/*-----------------------------------------------------------------------------
	�r���{�[�h�N���X
-----------------------------------------------------------------------------*/
class CSceneBillboard : public CScene
{
	public:
		CSceneBillboard(
							int nPriority = 4,
							CScene::OBJTYPE objtype = CScene::OBJTYPE_BILLBOARD);
		virtual ~CSceneBillboard();

		/*-----------------------------------------------------------------------------
			����
			return	pSceneBillboard*	pSceneBillboard*�̃C���X�^���X
			param	pos					�|���S�����S�\�����W
			param	rot					����
			param	col					�F
		-----------------------------------------------------------------------------*/
		static CSceneBillboard* Create(
										D3DXVECTOR3 pos,
										D3DXVECTOR3 rot,
										D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);
		void Uninit(void);
		void Update(void);
		void Draw(void);

		D3DXVECTOR3 GetPosition(void){ return m_pos; }
		void SetPosition(D3DXVECTOR3 pos){ m_pos = pos; }
		void SetPosition(float x, float y, float z){ m_pos.x = x; m_pos.y = y; m_pos.z = z; }

		D3DXVECTOR3 GetRotation(void){ return m_rot; }
		void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
		void SetRotation(float x, float y, float z){ m_rot.x = x; m_rot.y = y; m_rot.z = z; }

		/*-----------------------------------------------------------------------------
			�|���S���T�C�Y���擾
			return	D3DXVECTOR3		�|���S���i���j���Ԃ�
		-----------------------------------------------------------------------------*/
		D3DXVECTOR3 GetSize(void){ return m_size; }
		/*-----------------------------------------------------------------------------
			�|���S���T�C�Y���擾
			return	size		D3DXVECTOR3�̐ݒ�l
		-----------------------------------------------------------------------------*/
		void SetSize(D3DXVECTOR3 size){ m_size = size; }

		/*-----------------------------------------------------------------------------
			�e�N�X�`���ւ̐ݒ�
			param	pTexture		LPDIRECT3DTEXTURE9�̐ݒ�l
		-----------------------------------------------------------------------------*/
		void SetTexture(LPDIRECT3DTEXTURE9 pTexture){ m_pD3DTexture = pTexture; }

		/*-----------------------------------------------------------------------------
			���_���W��ݒ�
			param	vtx0		���㒸�_���W�̐ݒ�l
			param	vtx1		�E�㒸�_���W�̐ݒ�l
			param	vtx2		�������_���W�̐ݒ�l
			param	vtx3		�E�����_���W�̐ݒ�l
		-----------------------------------------------------------------------------*/
		void SetVertex(
						D3DXVECTOR3 vtx0,
						D3DXVECTOR3 vtx1,
						D3DXVECTOR3 vtx2,
						D3DXVECTOR3 vtx3);
		
		/*-----------------------------------------------------------------------------
			���_�@����ݒ�
			param	nor0		���㒸�_�@���̐ݒ�l
			param	nor1		�E�㒸�_�@���̐ݒ�l
			param	nor2		�������_�@���̐ݒ�l
			param	nor3		�E�����_�@���̐ݒ�l
		-----------------------------------------------------------------------------*/
		void SetNormal(D3DXVECTOR3 nor0, D3DXVECTOR3 nor1, D3DXVECTOR3 nor2, D3DXVECTOR3 nor3);
	
		/*-----------------------------------------------------------------------------
			���_�F��ݒ�
			param	diffuse		D3DXCOLOR�̐ݒ�l
		-----------------------------------------------------------------------------*/
		void SetDiffuse(D3DXCOLOR diffuse);
		
		/*-----------------------------------------------------------------------------
			���_�e�N�X�`�����W��ݒ�
			param	tex0		���㒸�_�̃e�N�X�`�����W
			param	tex1		�E�㒸�_�̃e�N�X�`�����W
			param	tex2		�������_�̃e�N�X�`�����W
			param	tex3		�E�����_�̃e�N�X�`�����W
		-----------------------------------------------------------------------------*/
		void SetTexcoord(D3DXVECTOR2 tex0, D3DXVECTOR2 tex1, D3DXVECTOR2 tex2, D3DXVECTOR2 tex3);

	protected:
		// �e�N�X�`���ւ̃|�C���^
		LPDIRECT3DTEXTURE9 m_pD3DTexture;
		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;
		// ���[���h�}�g���b�N�X
		D3DXMATRIX m_mtxWorld;

	private:
		// ���݂̍��W
		D3DXVECTOR3 m_pos;
		// ���݂̌���
		D3DXVECTOR3 m_rot;
		// �傫��
		D3DXVECTOR3 m_scale;
		// �F
		D3DXCOLOR m_color;
		// �T�C�Y
		D3DXVECTOR3 m_size;
};
