/*-----------------------------------------------------------------------------
	X�t�@�C�����f���N���X
	author	OkadaMoeto
	since	20140423
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CSceneX.h"
#include "CManager.h"
#include <assert.h>

/*-----------------------------------------------------------------------------
	�}�N����`
-----------------------------------------------------------------------------*/
// �t�@�C����
#define FILENAME_XFILEMODEL ("data/testsample/tank000.x")

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CSceneX::CSceneX(
					int nPriority,
					CScene::OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pD3DTexture = nullptr;
	m_pD3DXMesh = nullptr;
	m_pD3DXBuffMat = nullptr;
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CSceneX::~CSceneX()
{
}

/*-----------------------------------------------------------------------------
	����
-----------------------------------------------------------------------------*/
CSceneX* CSceneX::Create(
							D3DXVECTOR3 pos,
							D3DXVECTOR3 rot,
							D3DXCOLOR col)
{
	CSceneX* pSceneX;

	pSceneX = new CSceneX();
	if(FAILED(pSceneX->Init(pos, rot, col)))
	{
		MessageBox(
					NULL,
					"CSceneX:D3DXLoadMeshFromX",
					"X�t�@�C�����f���Ǎ����s",
					MB_OK);
	}

	return pSceneX;
}

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
HRESULT CSceneX::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// �f�o�C�X�擾
	pDevice = CManager::GetRenderer()->GetDevice();

	m_pos	=	pos;
	m_rot	=	rot;
	m_scale	=	D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	 
	// X�t�@�C����ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(
								FILENAME_XFILEMODEL,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								nullptr,
								&m_pD3DXBuffMat,
								nullptr,
								&m_nNumMat,
								&m_pD3DXMesh
	)))
	{		
		return E_FAIL;
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CSceneX::Uninit(void)
{
	// �e�N�X�`���̉��
	if(m_pD3DTexture)
	{
		m_pD3DTexture->Release();
		m_pD3DTexture = nullptr;
	}

	// �}�e���A���̉��
	if(m_pD3DXBuffMat)
	{
		m_pD3DXBuffMat->Release();
		m_pD3DXBuffMat = nullptr;
	}

	// ���b�V���̉��
	if(m_pD3DXMesh)
	{
		m_pD3DXMesh->Release();
		m_pD3DXMesh = nullptr;
	}

	// �I�u�W�F�N�g��j��
	Release();
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CSceneX::Update(void)
{
}

/*-----------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------*/
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// �f�o�C�X�擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �}�e���A���o�b�t�@
	D3DXMATERIAL* pD3DXMat;
	// �f�t�H���g�̃}�e���A��
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����̃A�h���X���擾
	pD3DXMat = (D3DXMATERIAL *)m_pD3DXBuffMat->GetBufferPointer();

	for(int i = 0; i < (int)m_nNumMat; i++)
	{
		// �}�e���A���𐶐�
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// �e�N�X�`����ݒ�
		pDevice->SetTexture(0, nullptr);
		// ���f���`��
		m_pD3DXMesh->DrawSubset(i);
	}

	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}
