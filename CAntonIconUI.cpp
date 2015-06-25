#include "CAntonIconUI.h"
#include "CManager.h"

CAntonIconUI::CAntonIconUI(int priority, OBJTYPE object_type) : CScene2D(priority,object_type)
{
	for (int nCnt = 0; nCnt < ICONTYPE_MAX; ++nCnt)
	{
		m_pIconTextures[nCnt] = nullptr;
	}

	m_iconType = ICONTYPE_NORMAL;
}

CAntonIconUI::~CAntonIconUI()
{
}

void CAntonIconUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < ICONTYPE_MAX; ++nCnt)
	{
		if (m_pIconTextures[nCnt] == nullptr)
		{
			continue;
		}

		m_pIconTextures[nCnt]->Release();
		m_pIconTextures[nCnt] = nullptr;
	}

	CScene2D::Uninit();
}

CAntonIconUI* CAntonIconUI::Create(D3DXVECTOR3 pos)
{
	CAntonIconUI *pAntonIconUI = new CAntonIconUI();
	
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	pAntonIconUI->Init(pos, rot, color);
	pAntonIconUI->PrepareTextures();

	return pAntonIconUI;
}

void CAntonIconUI::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pIconTextures[m_iconType]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void CAntonIconUI::PrepareTextures(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	HRESULT hr = D3DXCreateTextureFromFile(pDevice, "data/texture/ui/normal.png", &m_pIconTextures[ICONTYPE_NORMAL]);
	D3DXCreateTextureFromFile(pDevice, "data/texture/ui/metal.png", &m_pIconTextures[ICONTYPE_METAL]);
	D3DXCreateTextureFromFile(pDevice, "data/texture/ui/minimum.png", &m_pIconTextures[ICONTYPE_MINIMUM]);
	D3DXCreateTextureFromFile(pDevice, "data/texture/ui/powerhul.png", &m_pIconTextures[ICONTYPE_POWERFUL]);
}