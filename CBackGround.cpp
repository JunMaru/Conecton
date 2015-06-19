/*-----------------------------------------------------------------------------
 バックグラウンドクラス
 author Jun Maruyama 
 since 20150519
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 インクルード
-----------------------------------------------------------------------------*/
#include "CBackGround.h"
#include "Define.h"
#include "CScrollManager.h"

/*-----------------------------------------------------------------------------
 コンストラクタ
-----------------------------------------------------------------------------*/
CBackGround::CBackGround(int priority, CScene::OBJTYPE objtype)
	: CScene2D(priority, objtype)
{
	m_scrollVol = VEC2_ZERO;
	m_scrollSpeed = 0.01f;
}

/*-----------------------------------------------------------------------------
 デストラクタ
-----------------------------------------------------------------------------*/
CBackGround::~CBackGround(void)
{

}

/*-----------------------------------------------------------------------------
 描画
-----------------------------------------------------------------------------*/
void CBackGround::Draw(void)
{
	m_scrollVol = CScrollManager::GetScrollWorld();
	D3DXVECTOR2 scrollOffset = m_scrollVol * m_scrollSpeed;

	D3DXVECTOR2 texcoord[4] = { D3DXVECTOR2(0.0f + scrollOffset.x, 0.0f + scrollOffset.y),			// 左上
								D3DXVECTOR2(1.0f + scrollOffset.x, 0.0f + scrollOffset.y),			// 右上
								D3DXVECTOR2(0.0f + scrollOffset.x, 1.0f + scrollOffset.y),			// 左下
								D3DXVECTOR2(1.0f + scrollOffset.x, 1.0f + scrollOffset.y),			// 右下
								};

	SetTexcoord(texcoord[0], texcoord[1], texcoord[2], texcoord[3]);

	CScene2D::Draw();
}

/*-----------------------------------------------------------------------------
 生成処理
-----------------------------------------------------------------------------*/
CBackGround* CBackGround::Create(const char *file_name, int priority)
{
	CBackGround* pBackGround = new CBackGround(priority, CScene::OBJTYPE_2D);
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);

	pBackGround->Init(file_name,pos,VEC3_ZERO,COL_WHITE,SCREEN_WIDTH,SCREEN_HEIGHT);

	return pBackGround;
}