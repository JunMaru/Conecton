/*-----------------------------------------------------------------------------
	ブロッククラス
	author	HaradaYuto
	since	20150519
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/

#include "CScene.h"

/*-----------------------------------------------------------------------------
	マクロ
-----------------------------------------------------------------------------*/
#define BLOCK_WIDTH ( 50.0f )
#define BLOCK_HEIGHT ( 50.0f )

/*-----------------------------------------------------------------------------
	class
-----------------------------------------------------------------------------*/

class CInstancingObject;

/*-----------------------------------------------------------------------------
	ブロッククラス
------------------------------------------------------------------------------*/

class CBlock : public CScene
{
	public:

		typedef enum
		{
			BLOCKID_NONE = 0,				// なし
			BLOCKID_SOIL,					// 土ブロック
			BLOCKID_GRASS,				// 草ブロック
			BLOCKID_SOIL_CRACK,			// ひび割れ土ブロック
			BLOCKID_GRASS_CRACK,			// ひび割れ草ブロック
			BLOCKID_METAL,				// メタル変身ブロック
			BLOCKID_MINIMUM,				// ミニマム変身ブロック
			BLOCKID_POWERFUL,				// パワフル変身ブロック
			BLOCKID_LASER_GOAL_TOP,		// レーザーのゴール_上
			BLOCKID_LASER_GOAL_BOTTOM,	// レーザーのゴール_下
			BLOCKID_LASER_GOAL_LEFT,		// レーザーのゴール_左
			BLOCKID_LASER_GOAL_RIGHT,		// レーザーのゴール_右
			BLOCKID_LASER_START_TOP,		// レーザーのスタート_上
			BLOCKID_LASER_START_BOTTOM,	// レーザーのスタート_下
			BLOCKID_LASER_START_LEFT,		// レーザーのスタート_左
			BLOCKID_LASER_START_RIGHT,	// レーザーのスタート_右
			BLOCKID_LASER_CONTROL_DOWN,	// レーザー進行方向変更ブロック_下
			BLOCKID_LASER_CONTROL_UP,		// レーザー進行方向変更ブロック_上
			BLOCKID_LASER_CONTROL_RIGHT,	// レーザー進行方向変更ブロック_右
			BLOCKID_LASER_CONTROL_LEFT,	// レーザー進行方向変更ブロック_左
			BLOCKID_MAGNET,				// 磁石
			BLOCKID_WOODBOX,				// 木箱
			BLOCKID_SWITCH,				// スイッチ
			BLOCKID_WARP,					// ワープゾーン
			BLOCKID_ALL,
		}BLOCKID;

		CBlock( const int Layer = 0 ) : m_scl(D3DXVECTOR2(0,0) ),m_rot( D3DXVECTOR3(0,0,0) ),m_pos( D3DXVECTOR2(0,0) ),
										m_blockIdX(0),m_blockIdY(0),CScene( Layer ){}
		virtual ~CBlock(){}

		HRESULT Init();
		void Uninit();
		void Update();
		void Draw();

		/*-----------------------------------------------------------------------------
			インスタンシングオブジェクトをセット
			param インスタンシングオブジェクトへのポインタ
		------------------------------------------------------------------------------*/
		void SetInsObj( CInstancingObject *ins_obj ){ m_pInsObj = ins_obj; }

		//とりあえずのセッターゲッター
		D3DXVECTOR3 GetPosition(){ return D3DXVECTOR3(m_pos.x,m_pos.y,0); }
		void SetPosition( D3DXVECTOR3 pos ){ m_pos.x = pos.x; m_pos.y = pos.y; }
		void SetPosition( float x,float y,float z ){ m_pos.x = x; m_pos.y = y; }
		D3DXVECTOR3 GetRotation(){return m_rot;}
		void SetRotation( D3DXVECTOR3 rot ){ m_rot = rot; }
		void SetRotation( float x,float y,float z ){ m_rot = D3DXVECTOR3(x,y,z); }

		void SetScaling( const float x,const float y ){ m_scl.x = x; m_scl.y = y; }

		void SetBlockId(BLOCKID blockType){ m_blockType = blockType; }
		BLOCKID GetBlockId(void){ return m_blockType; }

		/*------------------------------------------------------------------------------
			ブロックのテクスチャIDをセットする
			param テクスチャへのID
		-------------------------------------------------------------------------------*/
		void SetBlockTexID( const int id_x,const int id_y ){ m_blockIdX = id_x; m_blockIdY = id_y; }

		/*------------------------------------------------------------------------------
			２枚目のテクスチャIDのセッター
			param テクスチャへのID
		------------------------------------------------------------------------------*/
		void SetSecondTexID( const int id_x, const int id_y ){ m_secondTexIdX = id_x; m_secondTexIdY = id_y; }

	private:



		//関数はAddData(Screen)しか読んではいけない
		CInstancingObject *m_pInsObj;

		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;

		//ブロックのテクスチャID
		int m_blockIdX;
		int m_blockIdY;

		int m_secondTexIdX;
		int m_secondTexIdY;

		// 読み込んだテクスチャID保存場所
		BLOCKID m_blockType;
};