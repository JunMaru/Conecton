/*-----------------------------------------------------------------------------
	Xファイルモデルクラス
	author	OkadaMoeto
	since	20140423
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CSceneX.h"
#include "CManager.h"
#include <assert.h>

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
// ファイル名
#define FILENAME_XFILEMODEL ("data/testsample/tank000.x")

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CSceneX::CSceneX(
					int nPriority,
					CScene::OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pD3DTexture = nullptr;
	m_pD3DXMesh = nullptr;
	m_pD3DXBuffMat = nullptr;
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CSceneX::~CSceneX()
{
}

/*-----------------------------------------------------------------------------
	生成
-----------------------------------------------------------------------------*/
CSceneX* CSceneX::Create(
							D3DXVECTOR3 pos,
							D3DXVECTOR3 rot,
							D3DXCOLOR col)
{
	CSceneX* pSceneX;

	pSceneX = new CSceneX();
	if(FAILED(pSceneX->Init(pos, rot, col)))
	{
		MessageBox(
					NULL,
					"CSceneX:D3DXLoadMeshFromX",
					"Xファイルモデル読込失敗",
					MB_OK);
	}

	return pSceneX;
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
HRESULT CSceneX::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// デバイス取得
	pDevice = CManager::GetRenderer()->GetDevice();

	m_pos	=	pos;
	m_rot	=	rot;
	m_scale	=	D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	 
	// Xファイルを読み込む
	if(FAILED(D3DXLoadMeshFromX(
								FILENAME_XFILEMODEL,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								nullptr,
								&m_pD3DXBuffMat,
								nullptr,
								&m_nNumMat,
								&m_pD3DXMesh
	)))
	{		
		return E_FAIL;
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CSceneX::Uninit(void)
{
	// テクスチャの解放
	if(m_pD3DTexture)
	{
		m_pD3DTexture->Release();
		m_pD3DTexture = nullptr;
	}

	// マテリアルの解放
	if(m_pD3DXBuffMat)
	{
		m_pD3DXBuffMat->Release();
		m_pD3DXBuffMat = nullptr;
	}

	// メッシュの解放
	if(m_pD3DXMesh)
	{
		m_pD3DXMesh->Release();
		m_pD3DXMesh = nullptr;
	}

	// オブジェクトを破棄
	Release();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CSceneX::Update(void)
{
}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// デバイス取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// マテリアルバッファ
	D3DXMATERIAL* pD3DXMat;
	// デフォルトのマテリアル
	D3DMATERIAL9 matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報のアドレスを取得
	pD3DXMat = (D3DXMATERIAL *)m_pD3DXBuffMat->GetBufferPointer();

	for(int i = 0; i < (int)m_nNumMat; i++)
	{
		// マテリアルを生成
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// テクスチャを設定
		pDevice->SetTexture(0, nullptr);
		// モデル描画
		m_pD3DXMesh->DrawSubset(i);
	}

	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}
