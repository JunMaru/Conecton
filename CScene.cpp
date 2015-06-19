/*-----------------------------------------------------------------------------
	オブジェクトクラス
	author	OkadaMoeto
	since	20140416
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CScene.h"
#include "CManager.h"

/*-----------------------------------------------------------------------------
	静的メンバ変数の初期化
-----------------------------------------------------------------------------*/
CScene* CScene::m_pTop[NUM_PRIORITY] = {};
CScene* CScene::m_pCur[NUM_PRIORITY] = {};
int CScene::m_countScene = 0;

/*-----------------------------------------------------------------------------
*
	リスト先頭取得
	return	:CScene	リストの先頭を返す
	param	:priority	描画レイヤ番号
*
-----------------------------------------------------------------------------*/
CScene* CScene::GetListTop(int priority)
{
	return m_pTop[priority];
}

/*-----------------------------------------------------------------------------
*
	コンストラクタ
*
-----------------------------------------------------------------------------*/
CScene::CScene(int priority, OBJTYPE objtype)
{
	// 想定外の数値は丸める
	if((priority < 0) || (priority >= NUM_PRIORITY))
	{
		priority = 3;
	}

	LinkList(priority);
	
	m_objtype = objtype;
	m_bDelete = false;
	m_bGlobal = false;
	m_bUpdate = true;
	m_bDraw = true;
	m_countScene++;
}

/*-----------------------------------------------------------------------------
*
	デストラクタ
*
-----------------------------------------------------------------------------*/
CScene::~CScene()
{
}

/*-----------------------------------------------------------------------------
*
	リスト内の全更新
*
-----------------------------------------------------------------------------*/
void CScene::UpdateAll(void)
{
	for(int i=0; i<NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			// オブジェクトの更新処理
			if(pScene->m_bUpdate)
			{
				pScene->Update();
			}

			pScene = pSceneNext;
		}
	}

	// デスフラグONならば更新後にオブジェクトを解放する
	for(int i = 0; i < NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			if(pScene->m_bDelete)
			{
				// リスト内から自分自身を削除
				pScene->UnlinkList();

				delete pScene;
				pScene = nullptr;

				m_countScene--;
			}

			pScene = pSceneNext;
		}
	}
}

/*-----------------------------------------------------------------------------
*
	リスト内の全描画
*
-----------------------------------------------------------------------------*/
void CScene::DrawAll(void)
{
	for(int i=0; i<NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			// 描画しない場合はとばす
			if(pScene->m_bDraw)
			{
				// オブジェクトの描画処理
				pScene->Draw();
			}

			pScene = pSceneNext;
		}
	}
}

/*-----------------------------------------------------------------------------
*
	リスト内のデスフラグを有効にする
*
-----------------------------------------------------------------------------*/
void CScene::Release(void)
{
	// デスフラグON
	m_bDelete = true;
}

/*-----------------------------------------------------------------------------
*
	リスト内の全解放
*
-----------------------------------------------------------------------------*/
void CScene::ReleaseAll(void)
{
	for(int i=0; i<NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			// オブジェクトを存続させるために削除しない場合は終了処理をとばす
			if(!pScene->m_bGlobal)
			{
				// オブジェクトの終了処理
				pScene->Uninit();
			}

			pScene = pSceneNext;
		}
	}

	for(int i=0; i<NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			if(pScene->m_bDelete)
			{
				// リストから削除
				pScene->UnlinkList();

				// オブジェクトの解放
				delete pScene;
				pScene = nullptr;
			}

			pScene = pSceneNext;
		}
	}
}

/*-----------------------------------------------------------------------------
*
	リストに登録
	param	:nPriorty	描画レイヤ番号
*
-----------------------------------------------------------------------------*/
void CScene::LinkList(int priority)
{
	// プライオリティ番号を取得
	m_drawLayerPriority = priority;

	// リストの先頭(Top)が存在しない場合
	if(!m_pTop[m_drawLayerPriority])
	{
		m_pTop[m_drawLayerPriority] = this;
	}

	// 現在のオブジェクト(Cur)が存在しない場合
	if(!m_pCur[m_drawLayerPriority])
	{
		m_pCur[m_drawLayerPriority] = this;
	}

	// 現在のオブジェクト(Cur)の次のオブジェクトを自分(this)にする
	m_pCur[m_drawLayerPriority]->m_pNext = this;

	// 現在のオブジェクト(Cur)が自分(this)の場合
	if(m_pCur[m_drawLayerPriority] == this)
	{
		// 自分(this)の前のオブジェをNULLする
		m_pPrev = nullptr;
	}
	else
	{
		// 自分(this)の前のオブジェクトを現在のオブジェクト(Cur)にする
		m_pPrev = m_pCur[m_drawLayerPriority];
	}

	// 現在のオブジェクト(Cur)を自分(this)にする
	m_pCur[m_drawLayerPriority] = this;

	// 自分(this)の次のオブジェをNULLにする
	m_pNext = nullptr;
}

/*-----------------------------------------------------------------------------
*
	リストから削除
*
-----------------------------------------------------------------------------*/
void CScene::UnlinkList(void)
{
	CScene* pScene,*pSceneNext,*pScenePrev;

	// リストの先頭(Top)から現在位置(Cur)に向かって辿っていく
	pScene = m_pTop[m_drawLayerPriority];

	while(pScene)
	{
		pSceneNext = pScene->m_pNext;

		// 対象のオブジェクトが自分(this)の場合
		if(pScene == this)
		{
			// 対象のオブジェクトの前にオブジェクトが存在する場合
			if(pScene->m_pPrev)
			{	
				// 自分(this)の前のオブジェクトの次のオブジェクトを自分(this)の次のオブジェクトにする
				pScene->m_pPrev->m_pNext = pScene->m_pNext;
			}

			// 対象のオブジェクトの次にオブジェクトが存在する場合
			if(pScene->m_pNext)
			{
				// 自分(this)の次のオブジェクトの前のオブジェクトを自分(this)の前のオブジェクトにする
				pScene->m_pNext->m_pPrev = pScene->m_pPrev;
			}

			// 対象のオブジェクトがリストの先頭(Top)の場合
			if(pScene == m_pTop[m_drawLayerPriority])
			{
				m_pTop[m_drawLayerPriority] = pSceneNext;
			}
		}

		pScene = pSceneNext;
	}

	// リストの現在位置(Cur)から先頭(Top)に向かって辿っていく
	pScene = m_pCur[m_drawLayerPriority];

	while(pScene)
	{
		pScenePrev = pScene->m_pPrev;

		// 対象のオブジェクトが自分(this)の場合
		if(pScene == this)
		{
			// 対象のオブジェクトの前にオブジェクトが存在する場合
			if(pScene->m_pPrev)
			{
				// 自分(this)の前のオブジェクトの次のオブジェクトを自分(this)の次のオブジェクトにする
				pScene->m_pPrev->m_pNext = pScene->m_pNext;
			}

			// 対象のオブジェクトの次にオブジェクトが存在する場合
			if(pScene->m_pNext)
			{
				// 自分(this)の次のオブジェクトの前のオブジェクトを自分(this)の前のオブジェクトのする
				pScene->m_pNext->m_pPrev = pScene->m_pPrev;
			}

			// 対象のオブジェクトが現在のオブジェクト(Cur)の場合
			if(pScene == m_pCur[m_drawLayerPriority])
			{
				m_pCur[m_drawLayerPriority] = pScenePrev;
			}
		}

		pScene = pScenePrev;
	}
}
