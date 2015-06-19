/*-----------------------------------------------------------------------------
	3Dポリゴンクラス
	author	OkadaMoeto
	since	20140422
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CScene3D.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
// ファイル名
#define FILENAME_3DPOLYGON ("data/testsample/mikan.jpg")

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CScene3D::CScene3D(
					int nPriority,
					CScene::OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pD3DTexture = nullptr;
	m_pD3DVtxBuff = nullptr;
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CScene3D::~CScene3D()
{
}

/*-----------------------------------------------------------------------------
	生成
-----------------------------------------------------------------------------*/
CScene3D *CScene3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	CScene3D *pCScene3D;

	// ３Ｄポリゴン生成と初期化
	pCScene3D = new CScene3D();
	pCScene3D->Init(pos, rot, col);

	return pCScene3D;
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
HRESULT CScene3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	VERTEX_3D* pVtx = nullptr;
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// デバイス取得
	pDevice = CManager::GetRenderer()->GetDevice();

	SetPosition(pos);
	SetRotation(rot);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color = col;

	// 頂点情報の設定
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

	pVtx[0].vtx = D3DXVECTOR3(-30.0f, 15.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(30.0f, 15.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-30.0f, -15.0f, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(30.0f, -15.0f, 0.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].diffuse = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[1].diffuse = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[2].diffuse = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[3].diffuse = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pD3DVtxBuff->Unlock();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, FILENAME_3DPOLYGON, &m_pD3DTexture);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CScene3D::Uninit(void)
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
void CScene3D::Update(void)
{
}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/
void CScene3D::Draw(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// デバイス取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// 両面のポリゴンを描画
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl,m_scale.x,m_scale.y,m_scale.y);
	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_rot.y,m_rot.x,m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pD3DTexture);
	
	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	// 片面のみの描画に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

/*-----------------------------------------------------------------------------
	頂点色を設定
-----------------------------------------------------------------------------*/
void CScene3D::SetDiffuse(D3DXCOLOR diffuse)
{
	VERTEX_3D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].diffuse = diffuse;
	pVtx[1].diffuse = diffuse;
	pVtx[2].diffuse = diffuse;
	pVtx[3].diffuse = diffuse;

	m_pD3DVtxBuff->Unlock();
}
