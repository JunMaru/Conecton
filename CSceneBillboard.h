/*-----------------------------------------------------------------------------
	ビルボードクラス
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
	ビルボードクラス
-----------------------------------------------------------------------------*/
class CSceneBillboard : public CScene
{
	public:
		CSceneBillboard(
							int nPriority = 4,
							CScene::OBJTYPE objtype = CScene::OBJTYPE_BILLBOARD);
		virtual ~CSceneBillboard();

		/*-----------------------------------------------------------------------------
			生成
			return	pSceneBillboard*	pSceneBillboard*のインスタンス
			param	pos					ポリゴン中心表示座標
			param	rot					向き
			param	col					色
		-----------------------------------------------------------------------------*/
		static CSceneBillboard* Create(
										D3DXVECTOR3 pos,
										D3DXVECTOR3 rot,
										D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);
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
			ポリゴンサイズを取得
			return	D3DXVECTOR3		ポリゴン（幅）が返る
		-----------------------------------------------------------------------------*/
		D3DXVECTOR3 GetSize(void){ return m_size; }
		/*-----------------------------------------------------------------------------
			ポリゴンサイズを取得
			return	size		D3DXVECTOR3の設定値
		-----------------------------------------------------------------------------*/
		void SetSize(D3DXVECTOR3 size){ m_size = size; }

		/*-----------------------------------------------------------------------------
			テクスチャへの設定
			param	pTexture		LPDIRECT3DTEXTURE9の設定値
		-----------------------------------------------------------------------------*/
		void SetTexture(LPDIRECT3DTEXTURE9 pTexture){ m_pD3DTexture = pTexture; }

		/*-----------------------------------------------------------------------------
			頂点座標を設定
			param	vtx0		左上頂点座標の設定値
			param	vtx1		右上頂点座標の設定値
			param	vtx2		左下頂点座標の設定値
			param	vtx3		右下頂点座標の設定値
		-----------------------------------------------------------------------------*/
		void SetVertex(
						D3DXVECTOR3 vtx0,
						D3DXVECTOR3 vtx1,
						D3DXVECTOR3 vtx2,
						D3DXVECTOR3 vtx3);
		
		/*-----------------------------------------------------------------------------
			頂点法線を設定
			param	nor0		左上頂点法線の設定値
			param	nor1		右上頂点法線の設定値
			param	nor2		左下頂点法線の設定値
			param	nor3		右下頂点法線の設定値
		-----------------------------------------------------------------------------*/
		void SetNormal(D3DXVECTOR3 nor0, D3DXVECTOR3 nor1, D3DXVECTOR3 nor2, D3DXVECTOR3 nor3);
	
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
		void SetTexcoord(D3DXVECTOR2 tex0, D3DXVECTOR2 tex1, D3DXVECTOR2 tex2, D3DXVECTOR2 tex3);

	protected:
		// テクスチャへのポインタ
		LPDIRECT3DTEXTURE9 m_pD3DTexture;
		// 頂点バッファインターフェースへのポインタ
		LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;
		// ワールドマトリックス
		D3DXMATRIX m_mtxWorld;

	private:
		// 現在の座標
		D3DXVECTOR3 m_pos;
		// 現在の向き
		D3DXVECTOR3 m_rot;
		// 大きさ
		D3DXVECTOR3 m_scale;
		// 色
		D3DXCOLOR m_color;
		// サイズ
		D3DXVECTOR3 m_size;
};
