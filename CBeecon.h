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
		typedef enum
		{
			BLOCKID_NONE = 0,				// なし
			BLOCKID_SOIL,					// 土ブロック
			BLOCKID_GRASS,					// 草ブロック
			BLOCKID_SOIL_CRACK,				// ひび割れ土ブロック
			BLOCKID_GRASS_CRACK,			// ひび割れ草ブロック
			BLOCKID_METAL,					// メタル変身ブロック
			BLOCKID_MINIMUM,				// ミニマム変身ブロック
			BLOCKID_POWERFUL,				// パワフル変身ブロック
			BLOCKID_LASER_GOAL_TOP,			// レーザーのゴール_上
			BLOCKID_LASER_GOAL_BOTTOM,		// レーザーのゴール_下
			BLOCKID_LASER_GOAL_LEFT,		// レーザーのゴール_左
			BLOCKID_LASER_GOAL_RIGHT,		// レーザーのゴール_右
			BLOCKID_LASER_START_TOP,		// レーザーのスタート_上
			BLOCKID_LASER_START_BOTTOM,		// レーザーのスタート_下
			BLOCKID_LASER_START_LEFT,		// レーザーのスタート_左
			BLOCKID_LASER_START_RIGHT,		// レーザーのスタート_右
			BLOCKID_LASER_CONTROL_DOWN,		// レーザー進行方向変更ブロック_下
			BLOCKID_LASER_CONTROL_UP,		// レーザー進行方向変更ブロック_上
			BLOCKID_LASER_CONTROL_RIGHT,	// レーザー進行方向変更ブロック_右
			BLOCKID_LASER_CONTROL_LEFT,		// レーザー進行方向変更ブロック_左
			BLOCKID_MAGNET,					// 磁石
			BLOCKID_WOODBOX,				// 木箱
			BLOCKID_SWITCH,					// スイッチ
			BLOCKID_WARP,					// ワープゾーン
			BLOCKID_ALL,
		}BLOCKID;

		enum ACTION
		{
			ACTION_WALK = 0,
			ACTION_CONNECT,
			ACTION_WARP,
			ACTION_MAX,
		};

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
		void CommandUpMove();
		void CommandDownMove();
		void CommandStopMove(){ m_bMoveOperated = false; }


		float GetSpd(){ return m_spd; }
		void  SetSpd(const float spd){ m_spd = spd; }

		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x, m_pos.y, 0); }
		void SetPosition(D3DXVECTOR3 pos){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition(float x, float y, float z){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){ return m_rot; }
		void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
		void SetRotation(float x, float y, float z){ m_rot = D3DXVECTOR3(x, y, z); }

		D3DXVECTOR3 GetPositionOld(){ return D3DXVECTOR3(m_prevPos.x, m_prevPos.y, 0); }
		void SetPositionOld(D3DXVECTOR3 pos){ m_prevPos.x = pos.x; m_prevPos.y = pos.y; }
		void SetPositionOld(float x, float y, float z){ m_prevPos.x = x; m_prevPos.y = y; }

		void SetScaling(const float x, const float y){ m_scl.x = x; m_scl.y = y; }
		D3DXVECTOR3 GetTargetPosition(){ return D3DXVECTOR3(m_tarPos.x, m_tarPos.y, 0); }
		void SetTargetPosition(const float x, const float y){ m_tarPos.x = x; m_tarPos.y = y; }

		D3DXVECTOR2 GetPreviewPosition(void){ return m_prevPos; }

		ACTION GetAction(void){ return m_action; }
		void SetAction(ACTION action){ m_action = action; ResetSelectAnimetionIndex(); }

		BLOCKID GetBlockID(void){ return m_blockID; }
		void SetBlockID(BLOCKID block_id){ m_blockID = block_id; }

	private:

		typedef struct
		{
			int animWait;//待ち時間
			int animSum;
			int *texIdArray;
			bool bRoop;

		}AnimationInfo;

		void TemporaryInit();
		void TemporaryUninit();
		void TemporaryUpdate();
		void TemporaryDraw();

		void InitAnimaton(void);
		void ResetSelectAnimetionIndex(void);
		void UpdateEightMove(void);

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

		AnimationInfo *m_animSet;
		ACTION m_action;

		D3DXVECTOR2 m_eightMovePos;
		float m_fEightMoveCount;
		bool m_bEightMoveUpsideCircle;
		bool m_bMoveOperated;

		BLOCKID m_blockID;
};
