/*-----------------------------------------------------------------------------
	Xファイルモデルクラス
	author	OkadaMoeto
	since	20140423
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"
#include "CScene.h"

/*-----------------------------------------------------------------------------
	Xファイルモデルクラス
-----------------------------------------------------------------------------*/
class CSceneX : public CScene
{
	public:
		CSceneX(
					int nPriority = 3,
					CScene::OBJTYPE objtype = CScene::OBJTYPE_X);
		virtual ~CSceneX();

		/*-----------------------------------------------------------------------------
			生成
			return	CSceneX*	CSceneXのインスタンス
			param	pos			座標
			param	rot			向き
			param	col			色
		-----------------------------------------------------------------------------*/
		static CSceneX* Create(
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

	protected:
		// テクスチャへのポインタ
		LPDIRECT3DTEXTURE9	m_pD3DTexture;
		// メッシュ情報へのポインタ
		LPD3DXMESH m_pD3DXMesh;
		// マテリアル情報へのポインタ
		LPD3DXBUFFER m_pD3DXBuffMat;
		// マテリアル情報の総数
		DWORD m_nNumMat;
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
};
