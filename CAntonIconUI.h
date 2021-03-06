//=============================================================================
//
// アントンアイコン処理 [CAntonIconUI.h]
// Author : JUN MARUYAMA
// Since  : 2015/06/25
//
//=============================================================================
//*****************************************************************************
// インクルードガード
//*****************************************************************************
#pragma once

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CScene2D.h"

//*****************************************************************************
// アントンアイコンクラス
//*****************************************************************************
class CAntonIconUI : public CScene2D
{
public:

	// アイコンタイプ
	enum ICONTYPE
	{
		ICONTYPE_NORMAL = 0,
		ICONTYPE_METAL,
		ICONTYPE_MINIMUM,
		ICONTYPE_POWERFUL,
		ICONTYPE_MAX,
	};

	//=============================================================================
	// コンストラクタ
	// arg1 : const int priority(描画順番)
	// arg2 : const int object_type(オブジェクトの種類)
	//=============================================================================
	CAntonIconUI(int priority = 6,OBJTYPE object_type = OBJTYPE_2D);

	//=============================================================================
	// デストラクタ
	//=============================================================================
	~CAntonIconUI();

	//=============================================================================
	// 終了処理
	//=============================================================================
	void Uninit(void);

	//=============================================================================
	// 描画処理
	//=============================================================================
	void Draw(void);

	//=============================================================================
	// 生成処理
	//=============================================================================
	static CAntonIconUI* Create(D3DXVECTOR3 pos);

	//=============================================================================
	// アイコンタイプセッター
	//=============================================================================
	void SetIconType(ICONTYPE icontype){ m_iconType = icontype; }

private:

	//=============================================================================
	// テクスチャ準備処理
	//=============================================================================
	void PrepareTextures(void);

	ICONTYPE m_iconType;										// アイコンタイプ
	LPDIRECT3DTEXTURE9 m_pIconTextures[ICONTYPE_MAX];			// アイコンテクスチャ格納変数
};

