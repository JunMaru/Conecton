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

		float GetSpd(){ return m_spd; }
		void  SetSpd(const float spd){ m_spd = spd; }

		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x, m_pos.y, 0); }
		void SetPosition(D3DXVECTOR3 pos){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition(float x, float y, float z){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){ return m_rot; }
		void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
		void SetRotation(float x, float y, float z){ m_rot = D3DXVECTOR3(x, y, z); }

		void SetScaling(const float x, const float y){ m_scl.x = x; m_scl.y = y; }
		D3DXVECTOR3 GetTargetPosition(){ return D3DXVECTOR3(m_tarPos.x, m_tarPos.y, 0); }
		void SetTargetPosition(const float x, const float y){ m_tarPos.x = x; m_tarPos.y = y; }

		D3DXVECTOR2 GetPreviewPosition(void){ return m_prevPos; }

	private:

		void TemporaryInit();
		void TemporaryUninit();
		void TemporaryUpdate();
		void TemporaryDraw();

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

		typedef struct
		{
			int animWait;//�҂�����
			int animSum;
			int *texIdArray;
			bool bRoop;

		}AnimationInfo;

		AnimationInfo *m_animSet;

};
