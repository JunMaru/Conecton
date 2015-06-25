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
			BLOCKTYPE_SOIL = 0,		// 土ブロック
			BLOCKTYPE_GRASS_SOIL,	// 草の生えた土ブロック
			BLOCKTYPE_CRACK,		// ひび割れブロック
			BLOCKTYPE_ALL,
		}BLOCKTYPE;

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

		/*------------------------------------------------------------------------------
			ブロックのテクスチャIDをセットする
			param テクスチャへのID
		-------------------------------------------------------------------------------*/
		void SetBlockTexID( const int id_x,const int id_y ){ m_blockIdX = id_x; m_blockIdY = id_y; }

	private:



		//関数はAddData(Screen)しか読んではいけない
		CInstancingObject *m_pInsObj;

		D3DXVECTOR2 m_scl;
		D3DXVECTOR2 m_pos;
		D3DXVECTOR3 m_rot;

		//ブロックのテクスチャID
		int m_blockIdX;
		int m_blockIdY;







};