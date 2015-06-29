//=============================================================================
//
// �A���g���A�C�R������ [CAntonIconUI.cpp]
// Author : JUN MARUYAMA
// Since  : 2015/06/25
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAntonIconUI.h"
#include "CManager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CAntonIconUI::CAntonIconUI(int priority, OBJTYPE object_type) : CScene2D(priority,object_type)
{
	for (int nCnt = 0; nCnt < ICONTYPE_MAX; ++nCnt)
	{
		m_pIconTextures[nCnt] = nullptr;
	}

	m_iconType = ICONTYPE_NORMAL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAntonIconUI::~CAntonIconUI()
{
}

//=============================================================================
// �I������
//=============================================================================
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

//=============================================================================
// ��������
//=============================================================================
CAntonIconUI* CAntonIconUI::Create(D3DXVECTOR3 pos)
{
	CAntonIconUI *pAntonIconUI = new CAntonIconUI();
	
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	pAntonIconUI->Init(pos, rot, color);
	pAntonIconUI->SetScale(188.0f, 154.0f);
	pAntonIconUI->PrepareTextures();

	return pAntonIconUI;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CAntonIconUI::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pIconTextures[m_iconType]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �e�N�X�`����������
//=============================================================================
void CAntonIconUI::PrepareTextures(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data/texture/ui/normal.png", &m_pIconTextures[ICONTYPE_NORMAL]);
	D3DXCreateTextureFromFile(pDevice, "data/texture/ui/metal.png", &m_pIconTextures[ICONTYPE_METAL]);
	D3DXCreateTextureFromFile(pDevice, "data/texture/ui/minimum.png", &m_pIconTextures[ICONTYPE_MINIMUM]);
	D3DXCreateTextureFromFile(pDevice, "data/texture/ui/powerful.png", &m_pIconTextures[ICONTYPE_POWERFUL]);
}