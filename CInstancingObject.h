/*-------------------------------------------------------------------------------------------
		CInstancingObject.h
		author HaradaYuto
		since 2015512
--------------------------------------------------------------------------------------------*/

//とりあえず2Dで作ってみます
//方針　今までは、配置用データをポインタで渡していましたが、今回は毎回登録処理を行い、バッファの検索速度を向上させます。
//テクスチャのロードはInitより前に行ってください。
//Init後のテクスチャロードのほうが都合がいいなら後で作ります

// ２重インクルード防止
#pragma once

/*-------------------------------------------------------------------------------------------
include
--------------------------------------------------------------------------------------------*/

#include "CScene.h"

/*--------------------------------------------------------------------------------------------
	インスタンシングオブジェクトクラス
--------------------------------------------------------------------------------------------*/

class CInstancingObject : public CScene
{
	public:

		//コンストラクタ　とりあえず何もしない
		CInstancingObject( const int Layer = 7 ) :	m_pVsConstantTable(NULL),m_pPsConstantTable(NULL),m_pVertexShader(NULL),
								m_pPixelShader(NULL),m_pDecl(NULL),m_pTex(NULL),m_pBasePoly(NULL),
								m_pInsBuff(NULL),m_pIndexBuff(NULL),
								m_arrayCursol(0),m_texSize(D3DXVECTOR2(0,0)),m_oneTexSize(D3DXVECTOR2(0,0)),CScene(Layer){}
		~CInstancingObject(){}

		/*-----------------------------------------------------------------------------
			テクスチャをロードする 必ずInit前に呼ぶこと
			param テクスチャサイズ
			param 一要素サイズ
			return 成否
		-----------------------------------------------------------------------------*/
		HRESULT LoadTexture( const char *file_path ,const D3DXVECTOR2 &tex_size,const D3DXVECTOR2 &tex_one_size);


		/*-----------------------------------------------------------------------------
			初期化
			return	HRESULT		初期化結果
		-----------------------------------------------------------------------------*/
		HRESULT Init(void);
		/*-----------------------------------------------------------------------------
			終了
		-----------------------------------------------------------------------------*/
		void Uninit(void);
		/*-----------------------------------------------------------------------------
			更新
		-----------------------------------------------------------------------------*/
		void Update(void);
		/*-----------------------------------------------------------------------------
			描画
		-----------------------------------------------------------------------------*/
		void Draw(void);

		/*-----------------------------------------------------------------------------
			インスタンシング用データ配列(m_insBufInfo[ ARRAY_MAX ])に配置データを送ります
			param scl rot pos 配置するオブジェクトのスケール 回転 位置 情報
			param tex_id テクスチャid x = 横 y = 縦 テクスチャを一枚あたりの大きさで切り分けたときのＩＤ
			----------------
			| 00 | 10 | 20 |
			----------------
			| 01 | 11 | 11 |
			----------------こんな感じ

			return 成否判定

			*AddDataScreenは配置を既存の2Dポリゴンと同じようにオフセットしています。
			左上00 ｙ↓+
		-----------------------------------------------------------------------------*/
		bool AddData( const D3DXVECTOR2 &scl,const D3DXVECTOR3 &rot,const D3DXVECTOR2 &pos,
						const int tex_id_x,const int tex_id_y );

		bool AddDataScreen( const D3DXVECTOR2 &scl,const D3DXVECTOR3 &rot,const D3DXVECTOR2 &pos,
							const int tex_id_x,const int tex_id_y );


		/*-----------------------------------------------------------------------------
			このクラスのポインタを取得する　外部のクラスはこのクラスポインタのAddDataのみ
			呼ぶこと
			return このクラスのポインタ
		-----------------------------------------------------------------------------*/
		CInstancingObject *GetThis(){ return this; }


		//謎純粋関数対策
		D3DXVECTOR3 GetPosition(){return D3DXVECTOR3(0,0,0);}
		void SetPosition( D3DXVECTOR3 pos ){} 
		void SetPosition( float x,float y,float z ){}
		D3DXVECTOR3 GetRotation(){return D3DXVECTOR3(0,0,0);}
		void SetRotation( D3DXVECTOR3 rot ){}
		void SetRotation( float x,float y,float z ){}

	private:

		/*-----------------------------------------------------------------------------
			インスタンシング用の情報をバッファに詰めます
		-----------------------------------------------------------------------------*/
		void ApplyBuffer(void);

		//定数バッファ
		LPD3DXCONSTANTTABLE m_pVsConstantTable;
		LPD3DXCONSTANTTABLE m_pPsConstantTable;

		//頂点シェーダー
		IDirect3DVertexShader9 *m_pVertexShader;

		//ピクセルシェーダー
		IDirect3DPixelShader9  *m_pPixelShader;

		//引数構成
		LPDIRECT3DVERTEXDECLARATION9 m_pDecl;

		//テクスチャ
		LPDIRECT3DTEXTURE9 m_pTex;

		//最大バッファ数
		static const int ARRAY_MAX = 12000;

		//バッファ
		typedef struct
		{
			D3DXVECTOR2 m_pos;
			D3DXVECTOR3 m_rot;
			D3DXVECTOR2 m_scl;
			D3DXVECTOR2 m_uv_pos;
		}InsInfo;

		//頂点情報
		typedef struct
		{
			D3DXVECTOR2 m_pos;
			D3DXCOLOR m_col;
			D3DXVECTOR2 m_uv;
		}Vertex2DIns;

		//インスタンシング情報
		typedef struct
		{
			D3DXVECTOR4  m_worldMtx[4];
			D3DXVECTOR2 m_texPos;
		}InsBuff;


		//インスタンシング用基本図形
		//1辺1の正方形
		LPDIRECT3DVERTEXBUFFER9 m_pBasePoly;

		//インスタンシング用配置情報バッファ
		LPDIRECT3DVERTEXBUFFER9 m_pInsBuff;

		//インデックスバッファ　インスタンシングがDrawIndexedPrimitiveじゃないと警告を出すっぽい
		LPDIRECT3DINDEXBUFFER9 m_pIndexBuff;

		//バッファ配列
		InsInfo m_insBuffInfo[ ARRAY_MAX ];

		unsigned int m_arrayCursol;//配列の指すidx

		//テクスチャの大きさ
		D3DXVECTOR2 m_texSize;
		//テクスチャ1枚の大きさ
		D3DXVECTOR2 m_oneTexSize;

};

//簡易ネーム
typedef CInstancingObject CInsObj;

