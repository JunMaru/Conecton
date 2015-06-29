/*-----------------------------------------------------------------------------
	�����Q�[�W�e�X�g
	author	HaradaYuto
	since	20150623
-----------------------------------------------------------------------------*/

#include "CScene.h"

class CGauge : public CScene
{
	public:
		CGauge(const int layer = 0 ) : CScene(layer),m_gaugeBaseVal(0),m_gaugeVal(0),m_gaugeSclY(0),m_pVtxBuff(NULL){}
		~CGauge(){}

		//������
		HRESULT Init();
		void Uninit();
		void Update();
		void Draw();

		//�Ƃ肠�����̃Z�b�^�[�Q�b�^�[
		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x,m_pos.y,0); }
		void SetPosition( D3DXVECTOR3 pos ){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition( float x,float y,float z ){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){return m_rot;}
		void SetRotation( D3DXVECTOR3 rot ){ m_rot = rot; }
		void SetRotation( float x,float y,float z ){ m_rot = D3DXVECTOR3(x,y,z); }

		//�X�P�[�����O
		void SetScling( const D3DXVECTOR2 scl ){ m_scl = scl; }
		D3DXVECTOR2 GetScling(){ return m_scl ; }

		/*------------------------------------------------------------------------
			�Q�[�W�̐��l�̍ő�l�ƌ��ݒl���Z�b�g
			param ���l
		-------------------------------------------------------------------------*/
		void SetGaugeBaseVal( const float val ){ m_gaugeBaseVal = val; }
		void SetGaugeVal    ( const float val ){ m_gaugeVal = val; }

		/*------------------------------------------------------------------------
			�e�N�X�`�������[�h����
			param �z��Idx �t�@�C���p�X
		-------------------------------------------------------------------------*/
		void LoadTexture( const int idx ,const char *pfile_path );

	private:
		//��{�I�ȏ��
		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;
		
		//�Q�[�W�̐��l
		float m_gaugeBaseVal;
		float m_gaugeVal;

		//�Q�[�W�̑傫��
		float m_gaugeSclY;

		//�Ƃ肠�������ʂɕ`��
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;

		//���n�@�g�@���g�̃e�N�X�`��
		static const int MAX_TEX = 3;
		LPDIRECT3DTEXTURE9      m_pTex[MAX_TEX];
};