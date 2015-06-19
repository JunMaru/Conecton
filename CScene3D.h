/*-----------------------------------------------------------------------------
	3Dポリゴンクラス
	author	OkadaMoeto
	since	20140422
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"
#include "CScene.h"

/*-----------------------------------------------------------------------------
	3Dポリゴンクラス
-----------------------------------------------------------------------------*/
class CScene3D : public CScene
{
	public:
		CScene3D(
					int nPriority = 4,
					CScene::OBJTYPE objtype = CScene::OBJTYPE_3D);
		virtual ~CScene3D();

		/*-----------------------------------------------------------------------------
			生成
			return	CScene3D*	CScene3D*のインスタンス
			param	pos			ポリゴン中心表示座標
			param	rot			向き
			param	col			色
		-----------------------------------------------------------------------------*/
		static CScene3D* Create(
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
			テクスチャへの設定
			param	pTexture		LPDIRECT3DTEXTURE9の設定値
		-----------------------------------------------------------------------------*/
		void SetTexture(LPDIRECT3DTEXTURE9 pTexture){ m_pD3DTexture = pTexture; }

		/*-----------------------------------------------------------------------------
			頂点色を設定
			param	diffuse		D3DXCOLORの設定値
		-----------------------------------------------------------------------------*/
		void SetDiffuse(D3DXCOLOR diffuse);

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
		// スケール
		D3DXVECTOR3 m_scale;
		// 色
		D3DXCOLOR m_color;
};
