//=============================================================================
//
// ���C�tUI���� [CAntonLifeUI.cpp]
// Author : JUN MARUYAMA
// Since  : 2015/06/25
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAntonLifeUI.h"
#include "CScene2D.h"
#include "Define.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
static const float HEART_X_DISTANCE = (60.0f);
static const float HEART_X_OFFSET = (110.0f);
static const float HEART_Y_OFFSET = (15.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CAntonLifeUI::CAntonLifeUI(int priority, OBJTYPE object_type) : CScene(priority, object_type)
{
	for (int nCnt = 0; nCnt < LIFE_MAX; ++nCnt)
	{
		m_pLifeHeartPolygon[nCnt] = nullptr;
	}

	m_pLifeBaseLinePolygon = nullptr;

	m_nLife = LIFE_MAX;
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAntonLifeUI::~CAntonLifeUI()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CAntonLifeUI::Init(void)
{
	m_pLifeBaseLinePolygon = CScene2D::Create("data/texture/ui/base_line.png", m_pos, m_rot, 310.0f, 45.0f);

	for (int nCnt = 0; nCnt < LIFE_MAX; ++nCnt)
	{
		float fXDistance = HEART_X_DISTANCE * nCnt - HEART_X_OFFSET;
		D3DXVECTOR3 heartPos = D3DXVECTOR3(m_pos.x + fXDistance, m_pos.y - HEART_Y_OFFSET, m_pos.z);
		m_pLifeHeartPolygon[nCnt] = CScene2D::Create("data/texture/ui/heart.png", heartPos, m_rot, 50.0f, 50.0f);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CAntonLifeUI::Uninit(void)
{
	m_pLifeBaseLinePolygon->Uninit();
	m_pLifeBaseLinePolygon = nullptr;

	for (int nCnt = 0; nCnt < LIFE_MAX; ++nCnt)
	{
		m_pLifeHeartPolygon[nCnt]->Uninit();
		m_pLifeHeartPolygon[nCnt] = nullptr;
	}
	
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CAntonLifeUI::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CAntonLifeUI::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CAntonLifeUI* CAntonLifeUI::Create(D3DXVECTOR3 pos)
{
	CAntonLifeUI *pLifeUI = new CAntonLifeUI();
	pLifeUI->SetPosition(pos);
	pLifeUI->Init();

	return pLifeUI;
}

//=============================================================================
// ���C�t���Z����
//=============================================================================
void CAntonLifeUI::AddLife(int nAdd)
{
	if (nAdd == 0)
	{
		return;
	}

	int checkLimNum = nAdd + m_nLife;
	bool bUpperLim = checkLimNum > LIFE_MAX;
	bool bLowerLim = checkLimNum < 0;

	// life�z��𒴂��Ă���or�}�C�i�X���Q�Ƃ���ꍇ
	if (bUpperLim || bLowerLim)
	{
		return;
	}

	int nWorkLifeArrayNo = m_nLife;
	int nCountMax = abs(nAdd);

	// �̗͂����炷��
	if (nAdd < 0)
	{
		for (int nCnt = 0; nCnt < nCountMax; ++nCnt)
		{
			nWorkLifeArrayNo += -1;
			m_pLifeHeartPolygon[nWorkLifeArrayNo]->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	// �̗͂𑝂₷��
	else
	{
		for (int nCnt = 0; nCnt < nCountMax; ++nCnt)
		{
			m_pLifeHeartPolygon[nWorkLifeArrayNo]->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			nWorkLifeArrayNo++;
		}
	}
	
	// �̗͂̔��f
	m_nLife += nAdd;
}