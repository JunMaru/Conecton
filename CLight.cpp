/*-----------------------------------------------------------------------------
	ライトクラス
	author	OkadaMoeto
	since	20140508
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CLight.h"

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CLight::CLight()
{
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CLight::~CLight()
{
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
HRESULT CLight::Init(void)
{
	SetLight();

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CLight::Uninit(void)
{
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CLight::Update(void)
{
}

/*-----------------------------------------------------------------------------
	ライト設定
-----------------------------------------------------------------------------*/
void CLight::SetLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	D3DXVECTOR3 vecDir;	

	// デバイス取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// 自然光の設定 左奥から右手前へを照らす光源
	ZeroMemory(&m_Light[0],sizeof(D3DLIGHT9));
	m_Light[0].Type = D3DLIGHT_DIRECTIONAL;
	// ライト色の設定
	m_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_Light[0].Ambient = D3DXCOLOR(0.80f, 0.80f, 0.80f, 0.0f);
	// ライト方向の設定
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[0].Direction,&vecDir);
	pDevice->SetLight(0,&m_Light[0]);
	pDevice->LightEnable(0,TRUE);

	// 逆光の設定 一番強い光のほぼ反対側から6割ほどの強さの光
	ZeroMemory(&m_Light[1],sizeof(D3DLIGHT9));
	m_Light[1].Type = D3DLIGHT_DIRECTIONAL;
	// ライト色の設定
	m_Light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 0.0f);
	m_Light[1].Ambient = D3DXCOLOR(0.20f, 0.20f, 0.20f, 0.0f);
	// ライト方向の設定
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[1].Direction,&vecDir);
	pDevice->SetLight(1,&m_Light[1]);
	pDevice->LightEnable(1,TRUE);

	// 環境光の設定 後ろから照らされる一番弱めの光
	ZeroMemory(&m_Light[2],sizeof(D3DLIGHT9));
	m_Light[2].Type = D3DLIGHT_DIRECTIONAL;
	// ライト色の設定
	m_Light[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 0.0f);
	m_Light[2].Ambient = D3DXCOLOR(0.10f, 0.10f, 0.10f, 0.0f);
	// ライト方向の設定
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[2].Direction,&vecDir);
	pDevice->SetLight(2,&m_Light[2]);
	pDevice->LightEnable(2,TRUE);

	// ライティングモード
	pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, TRUE);
}
