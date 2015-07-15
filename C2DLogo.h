/*-------------------------------------------------------------------------------------------------------------
	2Dロゴクラス
	author	HaradaYuto
	20150629
--------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------
	2Dポリゴンを表示するだけ
	微妙にアニメーションもおまけでつけてます
	実装アニメーション　拡大縮小
---------------------------------------------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/

#include "CScene.h"

/*-----------------------------------------------------------------------------
	2Dロゴクラス
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

		//とりあえずのセッターゲッター
		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x, m_pos.y, 0); }
		void SetPosition(D3DXVECTOR3 pos){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition(float x, float y, float z){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){ return m_rot; }
		void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
		void SetRotation(float x, float y, float z){ m_rot = D3DXVECTOR3(x, y, z); }

		//スケーリング
		void SetScling(const D3DXVECTOR2 scl){ m_scl = scl; }
		D3DXVECTOR2 GetScling(){ return m_scl; }
		
		/*----------------------------------------------------------------------------
			スケーリングアニメーション関数
			param ループ　開始スケール　変化量　速度
		-----------------------------------------------------------------------------*/
		void StartSclAnimation( const bool b_roop,const D3DXVECTOR2  &start_scl,const D3DXVECTOR2 &move_scl, float anim_spd );

		/*------------------------------------------------------------------------
			テクスチャをロードする
			param ファイルパス
		-------------------------------------------------------------------------*/
		void LoadTexture( const char *pfile_path);


	private:

		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;
		D3DXVECTOR2 m_scl;

		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		LPDIRECT3DTEXTURE9      m_pTex;

		//アニメーション用
		//アニメーションするか
		bool m_bAnim;
		//アニメーションidx
		int m_animIdx;
		//アニメーションループフラグ
		bool m_bAnimRoop;

		//アニメーション速度
		float m_animSpd;
		float m_animNow;

		//アニメーション情報
		D3DXVECTOR2 m_animMoveScl;
		D3DXVECTOR2 m_animStartScl;

};