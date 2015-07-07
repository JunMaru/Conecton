/*-----------------------------------------------------------------------------
	2D�`�惉�C��
	author	HaradaYuto
	since	20150703
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

//������2D�|���S�����ꊇ�`�悵�܂��B

/*----------------------------------------------------------------------------
	�C���N���[�h
-----------------------------------------------------------------------------*/

#include "CScene.h"
#include "TextureLoader.h"

/*----------------------------------------------------------------------------	
	�`�惉�C���N���X
-----------------------------------------------------------------------------*/

class CDrawLine2D : public CScene
{
	public:
		CDrawLine2D(const int layer) : CScene(layer),m_pVtxBuff(NULL),m_pIdxBuff(NULL),m_pTex(NULL),
													 m_polygonArray(NULL),m_pTexUvInfo(NULL),m_drawPolygonSum(0){}
		~CDrawLine2D(){}

		//������
		HRESULT Init();

		/*--------------------------------------------------------------------------
			������
			param �`��|���S���ő吔
			return ���۔���
		---------------------------------------------------------------------------*/
		HRESULT Init( const char *file_path ,const int polygon_max );
		void Uninit();
		void Update();
		void Draw();

		/*-------------------------------------------------------------------------
			�`��|���S���ǉ�
		---------------------------------------------------------------------------*/
		void AddPolygon( D3DXVECTOR2 scl, float rot, D3DXVECTOR2 pos, const int tex_idx );


		//�Ƃ肠�����̃Z�b�^�[�Q�b�^�[
		//����K�v�Ȃ�
		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x, m_pos.y, 0); }
		void SetPosition(D3DXVECTOR3 pos){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition(float x, float y, float z){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){ return m_rot; }
		void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
		void SetRotation(float x, float y, float z){ m_rot = D3DXVECTOR3(x, y, z); }

	private:

		/*--------------------------------------------------------------------------
			�`��|���S���f�[�^���o�b�t�@�Ɋi�[����
		----------------------------------------------------------------------------*/
		void ApplyData();


		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;
		LPDIRECT3DTEXTURE9      m_pTex;

		int m_drawPolygonSum;//���ݕ`��|���S����

		typedef struct
		{
			D3DXVECTOR3 pos;//z�\�[�g���ǂ����邩���Ȃ̂�z���p��
			float rot;//�ǂ���z����]�������Ȃ�
			D3DXVECTOR2 scl;//xy�̂�
			int texIdx;//�e�N�X�`���ւ�idx
		}PolygonInfo;

		PolygonInfo *m_polygonArray;


		TexLoader::UvInfo *m_pTexUvInfo;

		//��{�I�ȏ��
		//����K�v�Ȃ�
		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;
};