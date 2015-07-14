/*-----------------------------------------------------------------------------
	�A���g���i��l���j�N���X
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
	�A���g���N���X
------------------------------------------------------------------------------*/

class CAnton : public CScene
{
	public:

		CAnton( const int Layer = 0 ) : m_pVtxBuff(NULL),m_pTex(NULL),
										m_scl( D3DXVECTOR2(0,0) ),m_rot( D3DXVECTOR3(0,0,0) ),m_pos( D3DXVECTOR2(0,0) ),
										m_prevPos( D3DXVECTOR2(0,0) ),m_tarPos( D3DXVECTOR2(0,0) ),m_spd( 0 ),
										m_bDirectionRight(true),m_mass(10),CScene(Layer){}
		~CAnton(){}

		enum STATE
		{
			STATE_NORMAL = 0,
			STATE_METAL,
			STATE_MINIMUM,
			STATE_POWERFUL,
			STATE_MAX,
		};

		enum ACTION
		{
			ACTION_WAIT = 0,
			ACTION_WALK = 1,
			ACTION_FRONT = 2,
			ACTION_PUSH = 2,
			ACTION_METALPOSE = 3,
			ACTION_MAX = 5,
		};

		struct PARAMETER
		{
			int nMass;
			int nPower;
			D3DXVECTOR2 size;
		};

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


		/*-----------------------------------------------------------------------------
			�Q�b�^�[�Z�b�^�[
		------------------------------------------------------------------------------*/

		//�Ƃ肠�����̃Z�b�^�[�Q�b�^�[
		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x,m_pos.y,0); }
		void SetPosition( D3DXVECTOR3 pos ){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition( float x,float y,float z ){ m_pos.x = x; m_pos.y = y; }

		D3DXVECTOR3 GetPositionOld(){ return D3DXVECTOR3( m_prevPos.x, m_prevPos.y, 0.0f ); }
		void SetPositionOld(D3DXVECTOR3 pos){ m_prevPos.x = pos.x; m_prevPos.y = pos.y; }
		void SetPositionOld(float x, float y, float z){ m_prevPos.x = x; m_prevPos.y = y; }

		D3DXVECTOR3 GetRotation(){return m_rot;}
		void SetRotation( D3DXVECTOR3 rot ){ m_rot = rot; }
		void SetRotation( float x,float y,float z ){ m_rot = D3DXVECTOR3(x,y,z); }


		/*------------------------------------------------------------------------------

		-------------------------------------------------------------------------------*/
		void SetScaling( const float x,const float y ){ m_scl.x = x; m_scl.y = y; }
		D3DXVECTOR3 GetTargetPosition(){ return D3DXVECTOR3( m_tarPos.x,m_tarPos.y,0); }
		void SetTargetPosition( const float x,const float y ){ m_tarPos.x = x;m_tarPos.y = y;}

		/*------------------------------------------------------------------------------

		-------------------------------------------------------------------------------*/

		/*------------------------------------------------------------------------------
			�e�N�X�`�����[�h
			param �e�N�X�`���ւ̃p�X
			return ����
		-------------------------------------------------------------------------------*/
		HRESULT LoadTexture( const char *file_path );

		/*------------------------------------------------------------------------------
			���x�̃Z�b�^�[�@�Q�b�^�[
		-------------------------------------------------------------------------------*/

		float GetSpd(){ return m_spd; }
		void  SetSpd( const float spd ){ m_spd = spd; }

		/*------------------------------------------------------------------------------
			���ʃZ�b�g�Q�b�g
		-------------------------------------------------------------------------------*/

		int GetMass(){ return m_parameter.nMass; }
		void SetMass( const int mass ){ m_mass = mass; }

		/*------------------------------------------------------------------------------
			�ړ�����
		-------------------------------------------------------------------------------*/

		void CommandRightMove();
		void CommandLeftMove();

		/*------------------------------------------------------------------------------
			�ϐg����
		-------------------------------------------------------------------------------*/

		void CommandChangeNormal();
		void CommandChangeMetal();
		
		void SetState(const STATE state);
		STATE GetState(void){ return m_state; }
		void SetAction(const ACTION action){ m_action = action; ResetSelectAnimetionIndex();}
		ACTION GetAction(void){ return m_action; }
		PARAMETER GetParameter(void){ return m_parameter; }

	private:

		//����������������邩������Ȃ��̂ł���ł��낢�돀��
		void TemporaryInit();
		void TemporaryUninit();
		void TemporaryUpdate();
		void TemporaryDraw();

		//test
		void UpdateAnimation();
		void InitAnimation();
		void ResetSelectAnimetionIndex();

		//�Ƃ肠�������ʂɕ`��
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		LPDIRECT3DTEXTURE9      m_pTex;

		//��{�I�ȏ��
		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;

		D3DXVECTOR2 m_prevPos;
		D3DXVECTOR2 m_tarPos;//�ڕW���W

		float m_spd;//���x���
		bool m_bDirectionRight;//����

		int m_mass;//����
		
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

		typedef struct
		{
			int animWait;//�҂�����
			int animSum;
			int *texIdArray;
			bool bRoop;

		}AnimationInfo;

		AnimationInfo *m_animSet;
		PARAMETER m_parameter;
		STATE m_state;				// �A���g���̏��(�m�[�}��/���^��/�~�j�}��/�p���t��)
		ACTION m_action;			// �A���g���̍s��(�ҋ@/����/���ʂ�����)
};

