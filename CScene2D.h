/*-----------------------------------------------------------------------------
	2Dポリゴンクラス
	author	OkadaMoeto
	since	20140617
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"
#include "CScene.h"

/*-----------------------------------------------------------------------------
	2Dポリゴンクラス
-----------------------------------------------------------------------------*/
class CScene2D : public CScene
{
	public:
		CScene2D(
					int nPriority = 6,
					CScene::OBJTYPE objtype = CScene::OBJTYPE_2D);
		virtual ~CScene2D();

		/*-----------------------------------------------------------------------------
			2Dポリゴンの生成(1)
			return	CScene2D*		CScene2D*のインスタンス
			param	pos				ポリゴンの中心表示座標
			param	rot				向き
			param	col				色
		-----------------------------------------------------------------------------*/
		static CScene2D* Create(
									D3DXVECTOR3 pos,
									D3DXVECTOR3 rot,
									D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		/*-----------------------------------------------------------------------------
			2Dポリゴンの生成(2)
			return	CScene2D*		CScene2D*のインスタンス
			param	file_name		テクスチャファイル名
			param	pos				ポリゴンの中心表示座標
			param	rot				向き
			param	width			幅
			param	height			高さ
			param	col				色
		-----------------------------------------------------------------------------*/
		static CScene2D* Create(
									const char* file_name,
									D3DXVECTOR3 pos,
									D3DXVECTOR3 rot,
									float width,
									float height,
									D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		

		HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);
		HRESULT Init(
						const char* file_name,
						D3DXVECTOR3 pos,
						D3DXVECTOR3 rot,
						D3DXCOLOR col,
						float width,
						float height);
		void Uninit(void);
		void Update(void);
		void Draw(void);

		D3DXVECTOR3 GetPosition(void){ return m_pos; }
		void SetPosition(D3DXVECTOR3 pos){ m_pos = pos; }
		void SetPosition(float x, float y, float z){ m_pos.x = x; m_pos.y = y; m_pos.z = z; }

		D3DXVECTOR3 GetRotation(void){ return m_rot; }
		void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
		void SetRotation(float x, float y, float z){ m_rot.x = x; m_rot.y = y; m_rot.z = z; }

		/*-----------------------------------------------------------------------------
			テクスチャへの設定
			param	pTexture		LPDIRECT3DTEXTURE9の設定値
		-----------------------------------------------------------------------------*/
		void SetTexture(LPDIRECT3DTEXTURE9 pTexture){ m_pD3DTexture = pTexture; }

		/*-----------------------------------------------------------------------------
			頂点座標を設定
		-----------------------------------------------------------------------------*/
		void SetVertex(void);

		/*-----------------------------------------------------------------------------
			頂点色を設定
			param	diffuse		D3DXCOLORの設定値
		-----------------------------------------------------------------------------*/
		void SetDiffuse(D3DXCOLOR diffuse);
		
		/*-----------------------------------------------------------------------------
			頂点テクスチャ座標を設定
			param	tex0		左上頂点のテクスチャ座標
			param	tex1		右上頂点のテクスチャ座標
			param	tex2		左下頂点のテクスチャ座標
			param	tex3		右下頂点のテクスチャ座標
		-----------------------------------------------------------------------------*/
		void SetTexcoord(
							D3DXVECTOR2 tex0,
							D3DXVECTOR2 tex1,
							D3DXVECTOR2 tex2,
							D3DXVECTOR2 tex3);

		/*-----------------------------------------------------------------------------
			ポリゴンのサイズ設定して拡大・縮小する
			param	width		ポリゴンの幅
			param	height		ポリゴンの高さ
		-----------------------------------------------------------------------------*/
		void SetScale(float width, float height);
		/*-----------------------------------------------------------------------------
			ポリゴンのサイズ設定して拡大・縮小する
			param	size		D3DXVECTOR2の設定値
		-----------------------------------------------------------------------------*/
		void SetScale(D3DXVECTOR2 size);

		/*-----------------------------------------------------------------------------
			ポリゴンサイズを取得
			return	D3DXVECTOR3		直径のサイズ（幅）が返る
		-----------------------------------------------------------------------------*/
		D3DXVECTOR3 GetSize(void){ return (m_size * 2.0f); }
		
		/*-----------------------------------------------------------------------------
			ポリゴンサイズを取得
			return	D3DXVECTOR3		半径のサイズ（幅）が返る
		-----------------------------------------------------------------------------*/
		D3DXVECTOR3 GetSizeHalf(void){ return m_size; }

		/*-----------------------------------------------------------------------------
			頂点の左上座標を取得
			return	D3DXVECTOR3		ポリゴン左上座標
		-----------------------------------------------------------------------------*/
		D3DXVECTOR3 GetPositionLT(void){ return D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f); }

	protected:
		// テクスチャオブジェクト
		LPDIRECT3DTEXTURE9 m_pD3DTexture;
		// 頂点情報格納ワーク
		LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;

	private:
		// 現在の座標
		D3DXVECTOR3 m_pos;
		// 現在の向き
		D3DXVECTOR3 m_rot;
		// ポリゴンの中心から頂点までの長さ
		float m_lengthDiagonal;
		// ポリゴンと頂点を結んだ直線の傾き
		float m_angleDiagonal;
		// 中心点からのサイズ
		D3DXVECTOR3 m_size;
};
