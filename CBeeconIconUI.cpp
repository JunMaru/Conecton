#include "CBeeconIconUI.h"
#include "CScene2D.h"
#include "Define.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CBeeconIconUI::CBeeconIconUI()
{
	m_iconType = ICONTYPE_NONE;
	m_pTVFrame = nullptr;
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

HRESULT CBeeconIconUI::Init(void)
{
	D3DXVECTOR2 workUV[] = { D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), };
	m_pTVFrame = CScene2D::Create("data/texture/ui/beecon_icon_ui.png", m_pos, m_rot, 100.0f, 100.0f);
	m_pTVFrame->SetTexcoord(workUV[0], workUV[1], workUV[2], workUV[3]);

	for (int nCnt = 0; nCnt < 4; ++nCnt)
	{
		workUV[nCnt].x += m_iconType + TEXTURE_OFFSET;
	}
	
	m_pBeeconIcon = CScene2D::Create("data/texture/ui/beecon_icon_ui.png", m_pos, m_rot, 100.0f, 100.0f);


	return S_OK;
}

void CBeeconIconUI::Uninit(void)
{

}

void CBeeconIconUI::Update(void)
{

}
void CBeeconIconUI::Draw(void)
{

}

CBeeconIconUI* CBeeconIconUI::Create(D3DXVECTOR3 pos)
{
	CBeeconIconUI *pBeeconIcon = new CBeeconIconUI();
	pBeeconIcon->SetPosition(pos);
	pBeeconIcon->Init();

	return pBeeconIcon;
}
