/*-------------------------------------------------------------------------------------------
		CInstancingObject.h
		author HaradaYuto
		since 2015512
--------------------------------------------------------------------------------------------*/

//�Ƃ肠����2D�ō���Ă݂܂�
//���j�@���܂ł́A�z�u�p�f�[�^���|�C���^�œn���Ă��܂������A����͖���o�^�������s���A�o�b�t�@�̌������x�����コ���܂��B
//�e�N�X�`���̃��[�h��Init���O�ɍs���Ă��������B
//Init��̃e�N�X�`�����[�h�̂ق����s���������Ȃ��ō��܂�

// �Q�d�C���N���[�h�h�~
#pragma once

/*-------------------------------------------------------------------------------------------
include
--------------------------------------------------------------------------------------------*/

#include "CScene.h"

/*--------------------------------------------------------------------------------------------
	�C���X�^���V���O�I�u�W�F�N�g�N���X
--------------------------------------------------------------------------------------------*/

class CInstancingObject : public CScene
{
	public:

		//�R���X�g���N�^�@�Ƃ肠�����������Ȃ�
		CInstancingObject( const int Layer = 7 ) :	m_pVsConstantTable(NULL),m_pPsConstantTable(NULL),m_pVertexShader(NULL),
								m_pPixelShader(NULL),m_pDecl(NULL),m_pTex(NULL),m_pBasePoly(NULL),
								m_pInsBuff(NULL),m_pIndexBuff(NULL),
								m_arrayCursol(0),m_texSize(D3DXVECTOR2(0,0)),m_oneTexSize(D3DXVECTOR2(0,0)),CScene(Layer){}
		~CInstancingObject(){}

		/*-----------------------------------------------------------------------------
			�e�N�X�`�������[�h���� �K��Init�O�ɌĂԂ���
			param �e�N�X�`���T�C�Y
			param ��v�f�T�C�Y
			return ����
		-----------------------------------------------------------------------------*/
		HRESULT LoadTexture( const char *file_path ,const D3DXVECTOR2 &tex_size,const D3DXVECTOR2 &tex_one_size);


		/*-----------------------------------------------------------------------------
			������
			return	HRESULT		����������
		-----------------------------------------------------------------------------*/
		HRESULT Init(void);
		/*-----------------------------------------------------------------------------
			�I��
		-----------------------------------------------------------------------------*/
		void Uninit(void);
		/*-----------------------------------------------------------------------------
			�X�V
		-----------------------------------------------------------------------------*/
		void Update(void);
		/*-----------------------------------------------------------------------------
			�`��
		-----------------------------------------------------------------------------*/
		void Draw(void);

		/*-----------------------------------------------------------------------------
			�C���X�^���V���O�p�f�[�^�z��(m_insBufInfo[ ARRAY_MAX ])�ɔz�u�f�[�^�𑗂�܂�
			param scl rot pos �z�u����I�u�W�F�N�g�̃X�P�[�� ��] �ʒu ���
			param tex_id �e�N�X�`��id x = �� y = �c �e�N�X�`�����ꖇ������̑傫���Ő؂蕪�����Ƃ��̂h�c
			----------------
			| 00 | 10 | 20 |
			----------------
			| 01 | 11 | 11 |
			----------------����Ȋ���

			return ���۔���

			*AddDataScreen�͔z�u��������2D�|���S���Ɠ����悤�ɃI�t�Z�b�g���Ă��܂��B
			����00 ����+
		-----------------------------------------------------------------------------*/
		bool AddData( const D3DXVECTOR2 &scl,const D3DXVECTOR3 &rot,const D3DXVECTOR2 &pos,
						const int tex_id_x,const int tex_id_y );

		bool AddDataScreen( const D3DXVECTOR2 &scl,const D3DXVECTOR3 &rot,const D3DXVECTOR2 &pos,
							const int tex_id_x,const int tex_id_y );


		/*-----------------------------------------------------------------------------
			���̃N���X�̃|�C���^���擾����@�O���̃N���X�͂��̃N���X�|�C���^��AddData�̂�
			�ĂԂ���
			return ���̃N���X�̃|�C���^
		-----------------------------------------------------------------------------*/
		CInstancingObject *GetThis(){ return this; }


		//�䏃���֐��΍�
		D3DXVECTOR3 GetPosition(){return D3DXVECTOR3(0,0,0);}
		void SetPosition( D3DXVECTOR3 pos ){} 
		void SetPosition( float x,float y,float z ){}
		D3DXVECTOR3 GetRotation(){return D3DXVECTOR3(0,0,0);}
		void SetRotation( D3DXVECTOR3 rot ){}
		void SetRotation( float x,float y,float z ){}

	private:

		/*-----------------------------------------------------------------------------
			�C���X�^���V���O�p�̏����o�b�t�@�ɋl�߂܂�
		-----------------------------------------------------------------------------*/
		void ApplyBuffer(void);

		//�萔�o�b�t�@
		LPD3DXCONSTANTTABLE m_pVsConstantTable;
		LPD3DXCONSTANTTABLE m_pPsConstantTable;

		//���_�V�F�[�_�[
		IDirect3DVertexShader9 *m_pVertexShader;

		//�s�N�Z���V�F�[�_�[
		IDirect3DPixelShader9  *m_pPixelShader;

		//�����\��
		LPDIRECT3DVERTEXDECLARATION9 m_pDecl;

		//�e�N�X�`��
		LPDIRECT3DTEXTURE9 m_pTex;

		//�ő�o�b�t�@��
		static const int ARRAY_MAX = 12000;

		//�o�b�t�@
		typedef struct
		{
			D3DXVECTOR2 m_pos;
			D3DXVECTOR3 m_rot;
			D3DXVECTOR2 m_scl;
			D3DXVECTOR2 m_uv_pos;
		}InsInfo;

		//���_���
		typedef struct
		{
			D3DXVECTOR2 m_pos;
			D3DXCOLOR m_col;
			D3DXVECTOR2 m_uv;
		}Vertex2DIns;

		//�C���X�^���V���O���
		typedef struct
		{
			D3DXVECTOR4  m_worldMtx[4];
			D3DXVECTOR2 m_texPos;
		}InsBuff;


		//�C���X�^���V���O�p��{�}�`
		//1��1�̐����`
		LPDIRECT3DVERTEXBUFFER9 m_pBasePoly;

		//�C���X�^���V���O�p�z�u���o�b�t�@
		LPDIRECT3DVERTEXBUFFER9 m_pInsBuff;

		//�C���f�b�N�X�o�b�t�@�@�C���X�^���V���O��DrawIndexedPrimitive����Ȃ��ƌx�����o�����ۂ�
		LPDIRECT3DINDEXBUFFER9 m_pIndexBuff;

		//�o�b�t�@�z��
		InsInfo m_insBuffInfo[ ARRAY_MAX ];

		unsigned int m_arrayCursol;//�z��̎w��idx

		//�e�N�X�`���̑傫��
		D3DXVECTOR2 m_texSize;
		//�e�N�X�`��1���̑傫��
		D3DXVECTOR2 m_oneTexSize;

};

//�ȈՃl�[��
typedef CInstancingObject CInsObj;

