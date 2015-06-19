/*-----------------------------------------------------------------------------
	X�t�@�C�����f���N���X
	author	OkadaMoeto
	since	20140423
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "Main.h"
#include "CScene.h"

/*-----------------------------------------------------------------------------
	X�t�@�C�����f���N���X
-----------------------------------------------------------------------------*/
class CSceneX : public CScene
{
	public:
		CSceneX(
					int nPriority = 3,
					CScene::OBJTYPE objtype = CScene::OBJTYPE_X);
		virtual ~CSceneX();

		/*-----------------------------------------------------------------------------
			����
			return	CSceneX*	CSceneX�̃C���X�^���X
			param	pos			���W
			param	rot			����
			param	col			�F
		-----------------------------------------------------------------------------*/
		static CSceneX* Create(
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
			�e�N�X�`���ւ̐ݒ�
			param	pTexture		LPDIRECT3DTEXTURE9�̐ݒ�l
		-----------------------------------------------------------------------------*/
		void SetTexture(LPDIRECT3DTEXTURE9 pTexture){ m_pD3DTexture = pTexture; }

	protected:
		// �e�N�X�`���ւ̃|�C���^
		LPDIRECT3DTEXTURE9	m_pD3DTexture;
		// ���b�V�����ւ̃|�C���^
		LPD3DXMESH m_pD3DXMesh;
		// �}�e���A�����ւ̃|�C���^
		LPD3DXBUFFER m_pD3DXBuffMat;
		// �}�e���A�����̑���
		DWORD m_nNumMat;
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
};
