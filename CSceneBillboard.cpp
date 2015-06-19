/*-----------------------------------------------------------------------------
	ビルボードクラス
	author	OkadaMoeto
	since	20140617
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CSceneBillboard.h"
#include "CManager.h"
#include "CCamera.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
// ファイル名
#define FILENAME_BILLBOARD ("data/texture/bullet/bullet000.png")

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CSceneBillboard::CSceneBillboard(
									int nPriority,
									CScene::OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pD3DTexture = nullptr;
	m_pD3DVtxBuff = nullptr;
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CSceneBillboard::~CSceneBillboard()
{
}

/*-----------------------------------------------------------------------------
	生成
-----------------------------------------------------------------------------*/
CSceneBillboard* CSceneBillboard::Create(
											D3DXVECTOR3 pos,
											D3DXVECTOR3 rot,
											D3DXCOLOR col)
{
	CSceneBillboard* pBillboard;

	// ビルボードオブジェクトの生成
	pBillboard = new CSceneBillboard();
	pBillboard->Init(pos, rot, col);

	return pBillboard;
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
HRESULT CSceneBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	VERTEX_3D* pVtx = nullptr;
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// デバイス取得
	pDevice = CManager::GetRenderer()->GetDevice();

	SetPosition(pos);
	SetRotation(rot);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color = col;
	SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(
											sizeof(VERTEX_3D) * 4,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&m_pD3DVtxBuff,
											nullptr
	)))
	{
		return E_FAIL;
	}

	// 頂点バッファの生成
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(
								m_pos.x - m_size.x,
								m_pos.y + m_size.y,
								m_pos.z + m_size.z);
	pVtx[1].vtx = D3DXVECTOR3(
								m_pos.x + m_size.x,
								m_pos.y + m_size.y,
								m_pos.z + m_size.z);
	pVtx[2].vtx = D3DXVECTOR3(
								m_pos.x - m_size.x,
								m_pos.y - m_size.y,
								m_pos.z + m_size.z);
	pVtx[3].vtx = D3DXVECTOR3(
								m_pos.x + m_size.x,
								m_pos.y - m_size.y,
								m_pos.z + m_size.z);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pD3DVtxBuff->Unlock();

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CSceneBillboard::Uninit(void)
{
	// テクスチャの解放 
	if(m_pD3DTexture)
	{
		m_pD3DTexture->Release();
		m_pD3DTexture = nullptr;
	}

	// 頂点バッファの解放
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = nullptr;
	}

	// オブジェクトを破棄
	Release();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CSceneBillboard::Update(void)
{
}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/
void CSceneBillboard::Draw(void)
{
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	CCamera* pCamera = nullptr;

	// デバイス取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// カメラ取得
	pCamera = CManager::GetCamera();
	// カメラの法線を取得
	mtxView = pCamera->GetMtxView();

	// 行列の初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// 逆行列を求める
	D3DXMatrixInverse(&m_mtxWorld, nullptr, &mtxView);

	// 行列を綺麗にしておく
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// ライティングの設定を切る
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_rot.y, m_rot.x,m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x,m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点情報をバインド
	pDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	// テクスチャの設定
	pDevice->SetTexture(0, m_pD3DTexture);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 ,2);

	// ライティングの設定を元に戻す
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
}

/*-----------------------------------------------------------------------------
	頂点座標を設定
-----------------------------------------------------------------------------*/
void CSceneBillboard::SetVertex(
									D3DXVECTOR3 vtx0,
									D3DXVECTOR3 vtx1,
									D3DXVECTOR3 vtx2,
									D3DXVECTOR3 vtx3)
{
	VERTEX_3D* pVtx;

	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = vtx0;
	pVtx[1].vtx = vtx1;
	pVtx[2].vtx = vtx2;
	pVtx[3].vtx = vtx3;

	m_pD3DVtxBuff->Unlock();
}

/*-----------------------------------------------------------------------------
	頂点法線を設定
-----------------------------------------------------------------------------*/
void CSceneBillboard::SetNormal(
									D3DXVECTOR3 nor0,
									D3DXVECTOR3 nor1,
									D3DXVECTOR3 nor2,
									D3DXVECTOR3 nor3)
{
	VERTEX_3D* pVtx;

	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].nor = nor0;
	pVtx[1].nor = nor1;
	pVtx[2].nor = nor2;
	pVtx[3].nor = nor3;

	m_pD3DVtxBuff->Unlock();
}

/*-----------------------------------------------------------------------------
	頂点反射色を設定
-----------------------------------------------------------------------------*/
void CSceneBillboard::SetDiffuse(D3DXCOLOR diffuse)
{
	VERTEX_3D* pVtx;

	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].diffuse = diffuse;
	pVtx[1].diffuse = diffuse;
	pVtx[2].diffuse = diffuse;
	pVtx[3].diffuse = diffuse;

	m_pD3DVtxBuff->Unlock();
}

/*-----------------------------------------------------------------------------
	頂点テクスチャ座標を設定
-----------------------------------------------------------------------------*/
void CSceneBillboard::SetTexcoord(
									D3DXVECTOR2 tex0,
									D3DXVECTOR2 tex1,
									D3DXVECTOR2 tex2,
									D3DXVECTOR2 tex3)
{
	VERTEX_3D* pVtx;

	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = tex0;
	pVtx[1].tex = tex1;
	pVtx[2].tex = tex2;
	pVtx[3].tex = tex3;

	m_pD3DVtxBuff->Unlock();
}
