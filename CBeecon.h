/*-----------------------------------------------------------------------------
	�r�[�R��(��l��)�N���X
	author	HaradaYuto
	since	20150519
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/

#include "CScene.h"
#include "TextureLoader.h"

/*-----------------------------------------------------------------------------
	class
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
	�r�[�R���N���X
------------------------------------------------------------------------------*/

class CBeecon : public CScene
{
	public:
		typedef enum
		{
			BLOCKID_NONE = 0,				// �Ȃ�
			BLOCKID_SOIL,					// �y�u���b�N
			BLOCKID_GRASS,					// ���u���b�N
			BLOCKID_SOIL_CRACK,				// �Ђъ���y�u���b�N
			BLOCKID_GRASS_CRACK,			// �Ђъ��ꑐ�u���b�N
			BLOCKID_METAL,					// ���^���ϐg�u���b�N
			BLOCKID_MINIMUM,				// �~�j�}���ϐg�u���b�N
			BLOCKID_POWERFUL,				// �p���t���ϐg�u���b�N
			BLOCKID_LASER_GOAL_TOP,			// ���[�U�[�̃S�[��_��
			BLOCKID_LASER_GOAL_BOTTOM,		// ���[�U�[�̃S�[��_��
			BLOCKID_LASER_GOAL_LEFT,		// ���[�U�[�̃S�[��_��
			BLOCKID_LASER_GOAL_RIGHT,		// ���[�U�[�̃S�[��_�E
			BLOCKID_LASER_START_TOP,		// ���[�U�[�̃X�^�[�g_��
			BLOCKID_LASER_START_BOTTOM,		// ���[�U�[�̃X�^�[�g_��
			BLOCKID_LASER_START_LEFT,		// ���[�U�[�̃X�^�[�g_��
			BLOCKID_LASER_START_RIGHT,		// ���[�U�[�̃X�^�[�g_�E
			BLOCKID_LASER_CONTROL_DOWN,		// ���[�U�[�i�s�����ύX�u���b�N_��
			BLOCKID_LASER_CONTROL_UP,		// ���[�U�[�i�s�����ύX�u���b�N_��
			BLOCKID_LASER_CONTROL_RIGHT,	// ���[�U�[�i�s�����ύX�u���b�N_�E
			BLOCKID_LASER_CONTROL_LEFT,		// ���[�U�[�i�s�����ύX�u���b�N_��
			BLOCKID_MAGNET,					// ����
			BLOCKID_WOODBOX,				// �ؔ�
			BLOCKID_SWITCH,					// �X�C�b�`
			BLOCKID_WARP,					// ���[�v�]�[��
			BLOCKID_ALL,
		}BLOCKID;

		enum ACTION
		{
			ACTION_WALK = 0,
			ACTION_CONNECT,
			ACTION_WARP,
			ACTION_MAX,
		};

		CBeecon(const int Layer = 0) : m_pVtxBuff(NULL), m_pTex(NULL),
			m_scl(D3DXVECTOR2(0, 0)), m_rot(D3DXVECTOR3(0, 0, 0)), m_pos(D3DXVECTOR2(0, 0)),
			m_prevPos(D3DXVECTOR2(0, 0)), m_tarPos(D3DXVECTOR2(0, 0)), m_spd(0),
			m_bDirectionRight(true), CScene(Layer){}
		virtual ~CBeecon(){}

		/*-----------------------------------------------------------------------------
			������
			return ����
		------------------------------------------------------------------------------*/
		HRESULT Init();

		/*-----------------------------------------------------------------------------
			�㏈��
		------------------------------------------------------------------------------*/
		void Uninit();

		/*-----------------------------------------------------------------------------
			�X�V
		------------------------------------------------------------------------------*/
		void Update();

		/*-----------------------------------------------------------------------------
			�`��
		------------------------------------------------------------------------------*/
		void Draw();

		/*------------------------------------------------------------------------------
			�ړ�����
		-------------------------------------------------------------------------------*/
		void CommandRightMove();
		void CommandLeftMove();
		void CommandUpMove();
		void CommandDownMove();
		void CommandStopMove(){ m_bMoveOperated = false; }


		float GetSpd(){ return m_spd; }
		void  SetSpd(const float spd){ m_spd = spd; }

		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x, m_pos.y, 0); }
		void SetPosition(D3DXVECTOR3 pos){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition(float x, float y, float z){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){ return m_rot; }
		void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
		void SetRotation(float x, float y, float z){ m_rot = D3DXVECTOR3(x, y, z); }

		D3DXVECTOR3 GetPositionOld(){ return D3DXVECTOR3(m_prevPos.x, m_prevPos.y, 0); }
		void SetPositionOld(D3DXVECTOR3 pos){ m_prevPos.x = pos.x; m_prevPos.y = pos.y; }
		void SetPositionOld(float x, float y, float z){ m_prevPos.x = x; m_prevPos.y = y; }

		void SetScaling(const float x, const float y){ m_scl.x = x; m_scl.y = y; }
		D3DXVECTOR3 GetTargetPosition(){ return D3DXVECTOR3(m_tarPos.x, m_tarPos.y, 0); }
		void SetTargetPosition(const float x, const float y){ m_tarPos.x = x; m_tarPos.y = y; }

		D3DXVECTOR2 GetPreviewPosition(void){ return m_prevPos; }

		ACTION GetAction(void){ return m_action; }
		void SetAction(ACTION action){ m_action = action; ResetSelectAnimetionIndex(); }

		BLOCKID GetBlockID(void){ return m_blockID; }
		void SetBlockID(BLOCKID block_id){ m_blockID = block_id; }

	private:

		typedef struct
		{
			int animWait;//�҂�����
			int animSum;
			int *texIdArray;
			bool bRoop;

		}AnimationInfo;

		void TemporaryInit();
		void TemporaryUninit();
		void TemporaryUpdate();
		void TemporaryDraw();

		void InitAnimaton(void);
		void ResetSelectAnimetionIndex(void);
		void UpdateEightMove(void);

		//�Ƃ肠�������ʂɕ`��
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		LPDIRECT3DTEXTURE9      m_pTex;

		//��{�I�ȏ��
		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;

		D3DXVECTOR2 m_prevPos;
		D3DXVECTOR2 m_tarPos;//�ڕW���W
		float m_fPosRadius;

		float m_spd;//���x���
		bool m_bDirectionRight;//����

		//�e�N�X�`���̂t���l�ƃT�C�Y��ۑ����Ă���f�[�^�z��
		TexLoader::UvInfo *m_pTexInfoArray;

		//�����̓e�N�X�`����Ԃł̘b
		//�e�N�X�`���ʒu(0~1)
		D3DXVECTOR2 m_texPos;
		//�e�N�X�`���T�C�Y(0~1)
		D3DXVECTOR2 m_texSize;

		//�A�j���[�V�����J�E���g
		float m_animCnt;

		//�A�j���[�V�������x
		int m_animWait;

		//�A�j���[�V��������
		int m_animSum;

		//�A�j���[�V����idx
		int m_animIdx;

		//�A�j���[�V�����I�t�Z�b�g(�J�n�ʒu)
		int m_animOffset;

		int m_selectAnimIdx;

		AnimationInfo *m_animSet;
		ACTION m_action;

		D3DXVECTOR2 m_eightMovePos;
		float m_fEightMoveCount;
		bool m_bEightMoveUpsideCircle;
		bool m_bMoveOperated;

		BLOCKID m_blockID;
};
