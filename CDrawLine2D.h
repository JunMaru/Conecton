/*-----------------------------------------------------------------------------
	2D描画ライン
	author	HaradaYuto
	since	20150703
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

//複数の2Dポリゴンを一括描画します。

/*----------------------------------------------------------------------------
	インクルード
-----------------------------------------------------------------------------*/

#include "CScene.h"
#include "TextureLoader.h"

/*----------------------------------------------------------------------------	
	描画ラインクラス
-----------------------------------------------------------------------------*/

class CDrawLine2D : public CScene
{
	public:
		CDrawLine2D(const int layer) : CScene(layer),m_pVtxBuff(NULL),m_pIdxBuff(NULL),m_pTex(NULL),
													 m_polygonArray(NULL),m_pTexUvInfo(NULL),m_drawPolygonSum(0){}
		~CDrawLine2D(){}

		//いつもの
		HRESULT Init();

		/*--------------------------------------------------------------------------
			初期化
			param 描画ポリゴン最大数
			return 成否判定
		---------------------------------------------------------------------------*/
		HRESULT Init( const char *file_path ,const int polygon_max );
		void Uninit();
		void Update();
		void Draw();

		/*-------------------------------------------------------------------------
			描画ポリゴン追加
		---------------------------------------------------------------------------*/
		void AddPolygon( D3DXVECTOR2 scl, float rot, D3DXVECTOR2 pos, const int tex_idx );


		//とりあえずのセッターゲッター
		//今回必要なし
		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x, m_pos.y, 0); }
		void SetPosition(D3DXVECTOR3 pos){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition(float x, float y, float z){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){ return m_rot; }
		void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
		void SetRotation(float x, float y, float z){ m_rot = D3DXVECTOR3(x, y, z); }

	private:

		/*--------------------------------------------------------------------------
			描画ポリゴンデータをバッファに格納する
		----------------------------------------------------------------------------*/
		void ApplyData();


		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;
		LPDIRECT3DTEXTURE9      m_pTex;

		int m_drawPolygonSum;//現在描画ポリゴン数

		typedef struct
		{
			D3DXVECTOR3 pos;//zソートもどきあるかもなのでzも用意
			float rot;//どうせz軸回転しかしない
			D3DXVECTOR2 scl;//xyのみ
			int texIdx;//テクスチャへのidx
		}PolygonInfo;

		PolygonInfo *m_polygonArray;


		TexLoader::UvInfo *m_pTexUvInfo;

		//基本的な情報
		//今回必要なし
		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;
};