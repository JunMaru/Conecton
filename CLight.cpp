/*-----------------------------------------------------------------------------
	���C�g�N���X
	author	OkadaMoeto
	since	20140508
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CLight.h"

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CLight::CLight()
{
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CLight::~CLight()
{
}

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
HRESULT CLight::Init(void)
{
	SetLight();

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CLight::Uninit(void)
{
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CLight::Update(void)
{
}

/*-----------------------------------------------------------------------------
	���C�g�ݒ�
-----------------------------------------------------------------------------*/
void CLight::SetLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	D3DXVECTOR3 vecDir;	

	// �f�o�C�X�擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���R���̐ݒ� ��������E��O�ւ��Ƃ炷����
	ZeroMemory(&m_Light[0],sizeof(D3DLIGHT9));
	m_Light[0].Type = D3DLIGHT_DIRECTIONAL;
	// ���C�g�F�̐ݒ�
	m_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_Light[0].Ambient = D3DXCOLOR(0.80f, 0.80f, 0.80f, 0.0f);
	// ���C�g�����̐ݒ�
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[0].Direction,&vecDir);
	pDevice->SetLight(0,&m_Light[0]);
	pDevice->LightEnable(0,TRUE);

	// �t���̐ݒ� ��ԋ������̂قڔ��Α�����6���قǂ̋����̌�
	ZeroMemory(&m_Light[1],sizeof(D3DLIGHT9));
	m_Light[1].Type = D3DLIGHT_DIRECTIONAL;
	// ���C�g�F�̐ݒ�
	m_Light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 0.0f);
	m_Light[1].Ambient = D3DXCOLOR(0.20f, 0.20f, 0.20f, 0.0f);
	// ���C�g�����̐ݒ�
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[1].Direction,&vecDir);
	pDevice->SetLight(1,&m_Light[1]);
	pDevice->LightEnable(1,TRUE);

	// �����̐ݒ� ��납��Ƃ炳����Ԏ�߂̌�
	ZeroMemory(&m_Light[2],sizeof(D3DLIGHT9));
	m_Light[2].Type = D3DLIGHT_DIRECTIONAL;
	// ���C�g�F�̐ݒ�
	m_Light[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 0.0f);
	m_Light[2].Ambient = D3DXCOLOR(0.10f, 0.10f, 0.10f, 0.0f);
	// ���C�g�����̐ݒ�
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[2].Direction,&vecDir);
	pDevice->SetLight(2,&m_Light[2]);
	pDevice->LightEnable(2,TRUE);

	// ���C�e�B���O���[�h
	pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, TRUE);
}
