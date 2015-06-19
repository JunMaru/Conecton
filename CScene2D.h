/*-----------------------------------------------------------------------------
	2D�|���S���N���X
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
	2D�|���S���N���X
-----------------------------------------------------------------------------*/
class CScene2D : public CScene
{
	public:
		CScene2D(
					int nPriority = 6,
					CScene::OBJTYPE objtype = CScene::OBJTYPE_2D);
		virtual ~CScene2D();

		/*-----------------------------------------------------------------------------
			2D�|���S���̐���(1)
			return	CScene2D*		CScene2D*�̃C���X�^���X
			param	pos				�|���S���̒��S�\�����W
			param	rot				����
			param	col				�F
		-----------------------------------------------------------------------------*/
		static CScene2D* Create(
									D3DXVECTOR3 pos,
									D3DXVECTOR3 rot,
									D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		/*-----------------------------------------------------------------------------
			2D�|���S���̐���(2)
			return	CScene2D*		CScene2D*�̃C���X�^���X
			param	file_name		�e�N�X�`���t�@�C����
			param	pos				�|���S���̒��S�\�����W
			param	rot				����
			param	width			��
			param	height			����
			param	col				�F
		-----------------------------------------------------------------------------*/
		static CScene2D* Create(
									const char* file_name,
									D3DXVECTOR3 pos,
									D3DXVECTOR3 rot,
									float width,
									float height,
									D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		

		HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);
		HRESULT Init(
						const char* file_name,
						D3DXVECTOR3 pos,
						D3DXVECTOR3 rot,
						D3DXCOLOR col,
						float width,
						float height);
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
			�e�N�X�`���ւ̐ݒ�
			param	pTexture		LPDIRECT3DTEXTURE9�̐ݒ�l
		-----------------------------------------------------------------------------*/
		void SetTexture(LPDIRECT3DTEXTURE9 pTexture){ m_pD3DTexture = pTexture; }

		/*-----------------------------------------------------------------------------
			���_���W��ݒ�
		-----------------------------------------------------------------------------*/
		void SetVertex(void);

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
		void SetTexcoord(
							D3DXVECTOR2 tex0,
							D3DXVECTOR2 tex1,
							D3DXVECTOR2 tex2,
							D3DXVECTOR2 tex3);

		/*-----------------------------------------------------------------------------
			�|���S���̃T�C�Y�ݒ肵�Ċg��E�k������
			param	width		�|���S���̕�
			param	height		�|���S���̍���
		-----------------------------------------------------------------------------*/
		void SetScale(float width, float height);
		/*-----------------------------------------------------------------------------
			�|���S���̃T�C�Y�ݒ肵�Ċg��E�k������
			param	size		D3DXVECTOR2�̐ݒ�l
		-----------------------------------------------------------------------------*/
		void SetScale(D3DXVECTOR2 size);

		/*-----------------------------------------------------------------------------
			�|���S���T�C�Y���擾
			return	D3DXVECTOR3		���a�̃T�C�Y�i���j���Ԃ�
		-----------------------------------------------------------------------------*/
		D3DXVECTOR3 GetSize(void){ return (m_size * 2.0f); }
		
		/*-----------------------------------------------------------------------------
			�|���S���T�C�Y���擾
			return	D3DXVECTOR3		���a�̃T�C�Y�i���j���Ԃ�
		-----------------------------------------------------------------------------*/
		D3DXVECTOR3 GetSizeHalf(void){ return m_size; }

		/*-----------------------------------------------------------------------------
			���_�̍�����W���擾
			return	D3DXVECTOR3		�|���S��������W
		-----------------------------------------------------------------------------*/
		D3DXVECTOR3 GetPositionLT(void){ return D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f); }

	protected:
		// �e�N�X�`���I�u�W�F�N�g
		LPDIRECT3DTEXTURE9 m_pD3DTexture;
		// ���_���i�[���[�N
		LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;

	private:
		// ���݂̍��W
		D3DXVECTOR3 m_pos;
		// ���݂̌���
		D3DXVECTOR3 m_rot;
		// �|���S���̒��S���璸�_�܂ł̒���
		float m_lengthDiagonal;
		// �|���S���ƒ��_�����񂾒����̌X��
		float m_angleDiagonal;
		// ���S�_����̃T�C�Y
		D3DXVECTOR3 m_size;
};
