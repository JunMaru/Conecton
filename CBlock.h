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
			BLOCKID_NONE = 0,				// �Ȃ�
			BLOCKID_SOIL,					// �y�u���b�N
			BLOCKID_GRASS,				// ���u���b�N
			BLOCKID_SOIL_CRACK,			// �Ђъ���y�u���b�N
			BLOCKID_GRASS_CRACK,			// �Ђъ��ꑐ�u���b�N
			BLOCKID_METAL,				// ���^���ϐg�u���b�N
			BLOCKID_MINIMUM,				// �~�j�}���ϐg�u���b�N
			BLOCKID_POWERFUL,				// �p���t���ϐg�u���b�N
			BLOCKID_LASER_GOAL_TOP,		// ���[�U�[�̃S�[��_��
			BLOCKID_LASER_GOAL_BOTTOM,	// ���[�U�[�̃S�[��_��
			BLOCKID_LASER_GOAL_LEFT,		// ���[�U�[�̃S�[��_��
			BLOCKID_LASER_GOAL_RIGHT,		// ���[�U�[�̃S�[��_�E
			BLOCKID_LASER_START_TOP,		// ���[�U�[�̃X�^�[�g_��
			BLOCKID_LASER_START_BOTTOM,	// ���[�U�[�̃X�^�[�g_��
			BLOCKID_LASER_START_LEFT,		// ���[�U�[�̃X�^�[�g_��
			BLOCKID_LASER_START_RIGHT,	// ���[�U�[�̃X�^�[�g_�E
			BLOCKID_LASER_CONTROL_DOWN,	// ���[�U�[�i�s�����ύX�u���b�N_��
			BLOCKID_LASER_CONTROL_UP,		// ���[�U�[�i�s�����ύX�u���b�N_��
			BLOCKID_LASER_CONTROL_RIGHT,	// ���[�U�[�i�s�����ύX�u���b�N_�E
			BLOCKID_LASER_CONTROL_LEFT,	// ���[�U�[�i�s�����ύX�u���b�N_��
			BLOCKID_MAGNET,				// ����
			BLOCKID_WOODBOX,				// �ؔ�
			BLOCKID_SWITCH,				// �X�C�b�`
			BLOCKID_WARP,					// ���[�v�]�[��
			BLOCKID_ALL,
		}BLOCKID;

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

		void SetBlockId(BLOCKID blockType){ m_blockType = blockType; }
		BLOCKID GetBlockId(void){ return m_blockType; }

		/*------------------------------------------------------------------------------
			�u���b�N�̃e�N�X�`��ID���Z�b�g����
			param �e�N�X�`���ւ�ID
		-------------------------------------------------------------------------------*/
		void SetBlockTexID( const int id_x,const int id_y ){ m_blockIdX = id_x; m_blockIdY = id_y; }

		/*------------------------------------------------------------------------------
			�Q���ڂ̃e�N�X�`��ID�̃Z�b�^�[
			param �e�N�X�`���ւ�ID
		------------------------------------------------------------------------------*/
		void SetSecondTexID( const int id_x, const int id_y ){ m_secondTexIdX = id_x; m_secondTexIdY = id_y; }

	private:



		//�֐���AddData(Screen)�����ǂ�ł͂����Ȃ�
		CInstancingObject *m_pInsObj;

		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;

		//�u���b�N�̃e�N�X�`��ID
		int m_blockIdX;
		int m_blockIdY;

		int m_secondTexIdX;
		int m_secondTexIdY;

		// �ǂݍ��񂾃e�N�X�`��ID�ۑ��ꏊ
		BLOCKID m_blockType;
};