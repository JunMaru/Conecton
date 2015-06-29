/*-----------------------------------------------------------------------------
	満腹ゲージテスト
	author	HaradaYuto
	since	20150623
-----------------------------------------------------------------------------*/

#include "CScene.h"

class CGauge : public CScene
{
	public:
		CGauge(const int layer = 0 ) : CScene(layer),m_gaugeBaseVal(0),m_gaugeVal(0),m_gaugeSclY(0),m_pVtxBuff(NULL){}
		~CGauge(){}

		//いつもの
		HRESULT Init();
		void Uninit();
		void Update();
		void Draw();

		//とりあえずのセッターゲッター
		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x,m_pos.y,0); }
		void SetPosition( D3DXVECTOR3 pos ){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition( float x,float y,float z ){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){return m_rot;}
		void SetRotation( D3DXVECTOR3 rot ){ m_rot = rot; }
		void SetRotation( float x,float y,float z ){ m_rot = D3DXVECTOR3(x,y,z); }

		//スケーリング
		void SetScling( const D3DXVECTOR2 scl ){ m_scl = scl; }
		D3DXVECTOR2 GetScling(){ return m_scl ; }

		/*------------------------------------------------------------------------
			ゲージの数値の最大値と現在値をセット
			param 数値
		-------------------------------------------------------------------------*/
		void SetGaugeBaseVal( const float val ){ m_gaugeBaseVal = val; }
		void SetGaugeVal    ( const float val ){ m_gaugeVal = val; }

		/*------------------------------------------------------------------------
			テクスチャをロードする
			param 配列Idx ファイルパス
		-------------------------------------------------------------------------*/
		void LoadTexture( const int idx ,const char *pfile_path );

	private:
		//基本的な情報
		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;
		
		//ゲージの数値
		float m_gaugeBaseVal;
		float m_gaugeVal;

		//ゲージの大きさ
		float m_gaugeSclY;

		//とりあえず普通に描画
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;

		//下地　枠　中身のテクスチャ
		static const int MAX_TEX = 3;
		LPDIRECT3DTEXTURE9      m_pTex[MAX_TEX];
};