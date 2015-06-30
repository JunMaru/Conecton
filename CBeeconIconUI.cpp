#include "CBeeconIconUI.h"
#include "CScene2D.h"
#include "Define.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CBeeconIconUI::CBeeconIconUI()
{
	m_iconType = ICONTYPE_NONE;
	m_pBeeconIcon = nullptr;
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBeeconIconUI::~CBeeconIconUI()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBeeconIconUI::Init(void)
{
	m_pBeeconIcon = CScene2D::Create("data/texture/ui/beecon_icon_ui.png", m_pos, m_rot, 100.0f, 100.0f);
	ChangeTexture();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBeeconIconUI::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBeeconIconUI::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CBeeconIconUI::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CBeeconIconUI* CBeeconIconUI::Create(D3DXVECTOR3 pos)
{
	CBeeconIconUI *pBeeconIcon = new CBeeconIconUI();
	pBeeconIcon->SetPosition(pos);
	pBeeconIcon->Init();

	return pBeeconIcon;
}

//=============================================================================
// テクスチャ切替処理
//=============================================================================
void CBeeconIconUI::ChangeTexture(void)
{
	D3DXVECTOR2 workUV[] = { D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.25f, 0.0f), D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(0.25f, 1.0f), };
	for (int nCnt = 0; nCnt < 4; ++nCnt)
	{
		workUV[nCnt].x += (m_iconType) * 0.25f;
	}

	m_pBeeconIcon->SetTexcoord(workUV[0], workUV[1], workUV[2], workUV[3]);
}
