/*-----------------------------------------------------------------------------
 バックグラウンドクラス
 author Jun Maruyama
 since 20150519
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 インクルードガード
-----------------------------------------------------------------------------*/
#pragma once

/*-----------------------------------------------------------------------------
 インクルード
-----------------------------------------------------------------------------*/
#include "CScene2D.h"

/*-----------------------------------------------------------------------------
 バックグラウンドクラス
-----------------------------------------------------------------------------*/
class CBackGround : public CScene2D
{
public:
	/*-----------------------------------------------------------------------------
	 コンストラクタ
	 param : priority(描画順番)
	 param : objtype(オブジェクトの種類)
	-----------------------------------------------------------------------------*/
	CBackGround(int priority, CScene::OBJTYPE objtype);

	/*-----------------------------------------------------------------------------
	 デストラクタ
	-----------------------------------------------------------------------------*/
	virtual ~CBackGround(void);

	/*-----------------------------------------------------------------------------
	 描画
	-----------------------------------------------------------------------------*/
	void Draw(void);
	
	/*-----------------------------------------------------------------------------
	 生成処理
	 param : file_name(テクスチャファイルパス)
	 param : priority(描画順番)
	-----------------------------------------------------------------------------*/
	static CBackGround* Create(const char *file_name, int priority = 0);

	/*-----------------------------------------------------------------------------
	 ゲッター/セッター
	-----------------------------------------------------------------------------*/
	void SetScrollSpeed(float speed){ m_scrollSpeed = speed; }
	float GetScroolSpeed(void){ return m_scrollSpeed; }

private:
	D3DXVECTOR2 m_scrollVol;
	float m_scrollSpeed;
};

