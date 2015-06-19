/*-----------------------------------------------------------------------------
	2Dポリゴンクラス
	author	OkadaMoeto
	since	20140617
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CScene2D.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
// ファイル名
#define FILENAME_2DPOLYGON ("data/testsample/mikan.jpg")

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CScene2D::CScene2D(
					int nPriority,
					CScene::OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pD3DTexture = nullptr;
	m_pD3DVtxBuff = nullptr;
}

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CScene2D::~CScene2D()
{
}

/*-----------------------------------------------------------------------------
	生成(1)
-----------------------------------------------------------------------------*/
CScene2D* CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	CScene2D* pScene2D;

	// ２Ｄオブジェクトの生成
	pScene2D = new CScene2D();
	pScene2D->Init(pos, rot, col);

	// 自分自身を返す
	return pScene2D;
}

/*-----------------------------------------------------------------------------
	生成(2)
-----------------------------------------------------------------------------*/
CScene2D* CScene2D::Create(
							const char* file_name,
							D3DXVECTOR3 pos,
							D3DXVECTOR3 rot,
							float width,
							float height,
							D3DXCOLOR col)
{
	CScene2D* pScene2D;

	// 初期化処理
	pScene2D = new CScene2D();
	pScene2D->Init(file_name, pos, rot, col, width, height);

	pScene2D->SetDiffuse(col);

	// 自分自身を返す
	return pScene2D;
}

/*-----------------------------------------------------------------------------
	初期化(1)
-----------------------------------------------------------------------------*/
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	VERTEX_2D *pVtx = nullptr;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	SetPosition(pos);
	SetRotation(rot);

	// サイズを設定
	m_size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);

	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(
											sizeof(VERTEX_2D) * 4,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_2D,
											D3DPOOL_MANAGED,
											&m_pD3DVtxBuff,
											nullptr
	)))
	{
		return E_FAIL;
	}

	m_pD3DVtxBuff->Lock(0,0,(void**)&pVtx,0);

	pVtx[0].vtx = D3DXVECTOR3(
								m_pos.x - m_size.x * 0.5f,
								m_pos.y - m_size.y * 0.5f,
								0.0f);
	pVtx[1].vtx = D3DXVECTOR3(
								m_pos.x + m_size.x * 0.5f,
								m_pos.y - m_size.y * 0.5f,
								0.0f);
	pVtx[2].vtx = D3DXVECTOR3(
								m_pos.x - m_size.x * 0.5f,
								m_pos.y + m_size.y * 0.5f,
								0.0f);
	pVtx[3].vtx = D3DXVECTOR3(
								m_pos.x + m_size.x * 0.5f,
								m_pos.y + m_size.y * 0.5f,
								0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	m_pD3DVtxBuff->Unlock();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, FILENAME_2DPOLYGON, &m_pD3DTexture);

	// 半径のほうが扱いやすいので半分にしておく
	m_size *= 0.5f;

	// 対角線の長さを求める
	m_lengthDiagonal = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);

	// 対角線の角度を求める
	m_angleDiagonal = atan2f(m_size.x, m_size.x);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	初期化(2)
-----------------------------------------------------------------------------*/
HRESULT CScene2D::Init(const char* file_name, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float width, float height)
{
	VERTEX_2D *pVtx = nullptr;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	SetPosition(pos);
	SetRotation(rot);

	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(
											sizeof(VERTEX_2D) * 4,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_2D,
											D3DPOOL_MANAGED,
											&m_pD3DVtxBuff,
											nullptr
	)))
	{
		return E_FAIL;
	}

	m_pD3DVtxBuff ->Lock(0,0,(void**)&pVtx,0);

	// 中心点から幅と高さで矩形の各頂点を指定
	pVtx[0].vtx = D3DXVECTOR3(pos.x - width * 0.5f, pos.y - height * 0.5f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(pos.x + width * 0.5f, pos.y - height * 0.5f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(pos.x - width * 0.5f, pos.y + height * 0.5f, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(pos.x + width * 0.5f, pos.y + height * 0.5f, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	m_pD3DVtxBuff->Unlock();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, file_name, &m_pD3DTexture);

	// 幅と高さを取得
	m_size.x = width * 0.5f;
	m_size.y = height * 0.5f;

	// 対角線の長さを求める
	m_lengthDiagonal = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);

	// 対角線の角度を求める
	m_angleDiagonal = atan2f(m_size.y, m_size.x);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CScene2D::Uninit(void)
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
void CScene2D::Update(void)
{
}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/
void CScene2D::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

/*-----------------------------------------------------------------------------
	頂点座標を設定
-----------------------------------------------------------------------------*/
void CScene2D::SetVertex(void)
{
	VERTEX_2D *pVtx;

	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx.x = (m_pos.x - sinf(m_rot.z  + m_angleDiagonal)
					* m_lengthDiagonal);
	pVtx[1].vtx.x = (m_pos.x + sinf(-m_rot.z + m_angleDiagonal)
					* m_lengthDiagonal);
	pVtx[2].vtx.x = (m_pos.x - sinf(-m_rot.z + m_angleDiagonal)
					* m_lengthDiagonal);
	pVtx[3].vtx.x = (m_pos.x + sinf(m_rot.z  + m_angleDiagonal)
					* m_lengthDiagonal);

	pVtx[0].vtx.y = (m_pos.y - cosf(-m_rot.z - m_angleDiagonal)
					* m_lengthDiagonal);
	pVtx[1].vtx.y = (m_pos.y - cosf(m_rot.z  - m_angleDiagonal)
					* m_lengthDiagonal);
	pVtx[2].vtx.y = (m_pos.y + cosf(m_rot.z  - m_angleDiagonal)
					* m_lengthDiagonal);
	pVtx[3].vtx.y = (m_pos.y + cosf(-m_rot.z - m_angleDiagonal)
					* m_lengthDiagonal);

	m_pD3DVtxBuff->Unlock();
}

/*-----------------------------------------------------------------------------
	頂点反射色を設定
-----------------------------------------------------------------------------*/
void CScene2D::SetDiffuse(D3DXCOLOR diffuse)
{
	VERTEX_2D *pVtx;

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
void CScene2D::SetTexcoord(
							D3DXVECTOR2 tex0,
							D3DXVECTOR2 tex1,
							D3DXVECTOR2 tex2,
							D3DXVECTOR2 tex3)
{
	VERTEX_2D *pVtx;

	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = tex0;
	pVtx[1].tex = tex1;
	pVtx[2].tex = tex2;
	pVtx[3].tex = tex3;

	m_pD3DVtxBuff->Unlock();
}

/*-----------------------------------------------------------------------------
	ポリゴンのサイズ設定
-----------------------------------------------------------------------------*/
void CScene2D::SetScale(float width, float height)
{
	// 幅と高さを取得
	m_size.x = width * 0.5f;
	m_size.y = height * 0.5f;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	VERTEX_2D *pVtx = nullptr;
	m_pD3DVtxBuff->Lock(0,0,(void**)&pVtx,0);

	pVtx[0].vtx = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	m_pD3DVtxBuff->Unlock();

	// 対角線の長さを求める
	m_lengthDiagonal = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);

	// 対角線の角度を求める
	m_angleDiagonal = atan2f(m_size.y, m_size.x);
}

void CScene2D::SetScale(D3DXVECTOR2 size)
{
	// 幅と高さを取得
	m_size.x = size.x * 0.5f;
	m_size.y = size.y * 0.5f;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	VERTEX_2D *pVtx = nullptr;
	m_pD3DVtxBuff->Lock(0,0,(void**)&pVtx,0);

	pVtx[0].vtx = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	m_pD3DVtxBuff->Unlock();

	// 対角線の長さを求める
	m_lengthDiagonal = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);

	// 対角線の角度を求める
	m_angleDiagonal = atan2f(m_size.y, m_size.x);
}
