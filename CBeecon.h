/*-----------------------------------------------------------------------------
	ビーコン(主人公)クラス
	author	HaradaYuto
	since	20150519
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/

#include "CScene.h"
#include "TextureLoader.h"

/*-----------------------------------------------------------------------------
	class
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
	ビーコンクラス
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
			初期化
			return 成否
		------------------------------------------------------------------------------*/
		HRESULT Init();

		/*-----------------------------------------------------------------------------
			後処理
		------------------------------------------------------------------------------*/
		void Uninit();

		/*-----------------------------------------------------------------------------
			更新
		------------------------------------------------------------------------------*/
		void Update();

		/*-----------------------------------------------------------------------------
			描画
		------------------------------------------------------------------------------*/
		void Draw();

		/*------------------------------------------------------------------------------
			移動命令
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

		//とりあえず普通に描画
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		LPDIRECT3DTEXTURE9      m_pTex;

		//基本的な情報
		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;

		D3DXVECTOR2 m_prevPos;
		D3DXVECTOR2 m_tarPos;//目標座標
		float m_fPosRadius;

		float m_spd;//速度情報
		bool m_bDirectionRight;//向き

		//テクスチャのＵｖ値とサイズを保存しているデータ配列
		TexLoader::UvInfo *m_pTexInfoArray;

		//これらはテクスチャ空間での話
		//テクスチャ位置(0~1)
		D3DXVECTOR2 m_texPos;
		//テクスチャサイズ(0~1)
		D3DXVECTOR2 m_texSize;

		//アニメーションカウント
		float m_animCnt;

		//アニメーション速度
		int m_animWait;

		//アニメーション総数
		int m_animSum;

		//アニメーションidx
		int m_animIdx;

		//アニメーションオフセット(開始位置)
		int m_animOffset;

		int m_selectAnimIdx;

		typedef struct
		{
			int animWait;//待ち時間
			int animSum;
			int *texIdArray;
			bool bRoop;

		}AnimationInfo;

		AnimationInfo *m_animSet;

};
