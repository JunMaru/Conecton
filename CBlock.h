/*-----------------------------------------------------------------------------
	�u���b�N�N���X
	author	HaradaYuto
	since	20150519
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/

#include "CScene.h"

/*-----------------------------------------------------------------------------
	�}�N��
-----------------------------------------------------------------------------*/
#define BLOCK_WIDTH ( 50.0f )
#define BLOCK_HEIGHT ( 50.0f )

/*-----------------------------------------------------------------------------
	class
-----------------------------------------------------------------------------*/

class CInstancingObject;

/*-----------------------------------------------------------------------------
	�u���b�N�N���X
------------------------------------------------------------------------------*/

class CBlock : public CScene
{
	public:

		typedef enum
		{
			BLOCKTYPE_SOIL = 0,		// �y�u���b�N
			BLOCKTYPE_GRASS_SOIL,	// ���̐������y�u���b�N
			BLOCKTYPE_CRACK,		// �Ђъ���u���b�N
			BLOCKTYPE_ALL,
		}BLOCKTYPE;

		CBlock( const int Layer = 0 ) : m_scl(D3DXVECTOR2(0,0) ),m_rot( D3DXVECTOR3(0,0,0) ),m_pos( D3DXVECTOR2(0,0) ),
										m_blockIdX(0),m_blockIdY(0),CScene( Layer ){}
		virtual ~CBlock(){}

		HRESULT Init();
		void Uninit();
		void Update();
		void Draw();

		/*-----------------------------------------------------------------------------
			�C���X�^���V���O�I�u�W�F�N�g���Z�b�g
			param �C���X�^���V���O�I�u�W�F�N�g�ւ̃|�C���^
		------------------------------------------------------------------------------*/
		void SetInsObj( CInstancingObject *ins_obj ){ m_pInsObj = ins_obj; }

		//�Ƃ肠�����̃Z�b�^�[�Q�b�^�[
		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x,m_pos.y,0); }
		void SetPosition( D3DXVECTOR3 pos ){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition( float x,float y,float z ){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){return m_rot;}
		void SetRotation( D3DXVECTOR3 rot ){ m_rot = rot; }
		void SetRotation( float x,float y,float z ){ m_rot = D3DXVECTOR3(x,y,z); }

		void SetScaling( const float x,const float y ){ m_scl.x = x; m_scl.y = y; }

		/*------------------------------------------------------------------------------
			�u���b�N�̃e�N�X�`��ID���Z�b�g����
			param �e�N�X�`���ւ�ID
		-------------------------------------------------------------------------------*/
		void SetBlockTexID( const int id_x,const int id_y ){ m_blockIdX = id_x; m_blockIdY = id_y; }

	private:



		//�֐���AddData(Screen)�����ǂ�ł͂����Ȃ�
		CInstancingObject *m_pInsObj;

		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;

		//�u���b�N�̃e�N�X�`��ID
		int m_blockIdX;
		int m_blockIdY;







};