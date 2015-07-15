/*-------------------------------------------------------------------------------------------------------------
	2D���S�N���X
	author	HaradaYuto
	20150629
--------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------
	2D�|���S����\�����邾��
	�����ɃA�j���[�V���������܂��ł��Ă܂�
	�����A�j���[�V�����@�g��k��
---------------------------------------------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/

#include "CScene.h"

/*-----------------------------------------------------------------------------
	2D���S�N���X
------------------------------------------------------------------------------*/

class C2DLogo : public CScene
{
	public:
		C2DLogo() : CScene(6),m_pVtxBuff(NULL),m_pTex(NULL),m_bAnim(false),
					m_bAnimRoop(false),m_animMoveScl(D3DXVECTOR2(0,0)),m_animStartScl(D3DXVECTOR2(0,0)),m_animIdx(-1),
					m_animSpd(0),m_animNow(0){}
		~C2DLogo(){}
		HRESULT Init();
		void Uninit();
		void Update();
		void Draw();

		//�Ƃ肠�����̃Z�b�^�[�Q�b�^�[
		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x, m_pos.y, 0); }
		void SetPosition(D3DXVECTOR3 pos){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition(float x, float y, float z){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){ return m_rot; }
		void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
		void SetRotation(float x, float y, float z){ m_rot = D3DXVECTOR3(x, y, z); }

		//�X�P�[�����O
		void SetScling(const D3DXVECTOR2 scl){ m_scl = scl; }
		D3DXVECTOR2 GetScling(){ return m_scl; }
		
		/*----------------------------------------------------------------------------
			�X�P�[�����O�A�j���[�V�����֐�
			param ���[�v�@�J�n�X�P�[���@�ω��ʁ@���x
		-----------------------------------------------------------------------------*/
		void StartSclAnimation( const bool b_roop,const D3DXVECTOR2  &start_scl,const D3DXVECTOR2 &move_scl, float anim_spd );

		/*------------------------------------------------------------------------
			�e�N�X�`�������[�h����
			param �t�@�C���p�X
		-------------------------------------------------------------------------*/
		void LoadTexture( const char *pfile_path);


	private:

		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;
		D3DXVECTOR2 m_scl;

		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		LPDIRECT3DTEXTURE9      m_pTex;

		//�A�j���[�V�����p
		//�A�j���[�V�������邩
		bool m_bAnim;
		//�A�j���[�V����idx
		int m_animIdx;
		//�A�j���[�V�������[�v�t���O
		bool m_bAnimRoop;

		//�A�j���[�V�������x
		float m_animSpd;
		float m_animNow;

		//�A�j���[�V�������
		D3DXVECTOR2 m_animMoveScl;
		D3DXVECTOR2 m_animStartScl;

};