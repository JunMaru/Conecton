/*-----------------------------------------------------------------------------
	3D�|���S���N���X
	author	OkadaMoeto
	since	20140422
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CScene3D.h"

/*-----------------------------------------------------------------------------
	�}�N����`
-----------------------------------------------------------------------------*/
// �t�@�C����
#define FILENAME_3DPOLYGON ("data/testsample/mikan.jpg")

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CScene3D::CScene3D(
					int nPriority,
					CScene::OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pD3DTexture = nullptr;
	m_pD3DVtxBuff = nullptr;
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CScene3D::~CScene3D()
{
}

/*-----------------------------------------------------------------------------
	����
-----------------------------------------------------------------------------*/
CScene3D *CScene3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	CScene3D *pCScene3D;

	// �R�c�|���S�������Ə�����
	pCScene3D = new CScene3D();
	pCScene3D->Init(pos, rot, col);

	return pCScene3D;
}

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
HRESULT CScene3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	VERTEX_3D* pVtx = nullptr;
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// �f�o�C�X�擾
	pDevice = CManager::GetRenderer()->GetDevice();

	SetPosition(pos);
	SetRotation(rot);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color = col;

	// ���_���̐ݒ�
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

	// ���_�o�b�t�@�̐���
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

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, FILENAME_3DPOLYGON, &m_pD3DTexture);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CScene3D::Uninit(void)
{
	// �e�N�X�`���̉�� 
	if(m_pD3DTexture)
	{
		m_pD3DTexture->Release();
		m_pD3DTexture = nullptr;
	}

	// ���_�o�b�t�@�̉��
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = nullptr;
	}

	// �I�u�W�F�N�g��j��
	Release();
}
/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CScene3D::Update(void)
{
}

/*-----------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------*/
void CScene3D::Draw(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// �f�o�C�X�擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���ʂ̃|���S����`��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl,m_scale.x,m_scale.y,m_scale.y);
	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_rot.y,m_rot.x,m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pD3DTexture);
	
	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̐ݒ�
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	// �Жʂ݂̂̕`��ɖ߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

/*-----------------------------------------------------------------------------
	���_�F��ݒ�
-----------------------------------------------------------------------------*/
void CScene3D::SetDiffuse(D3DXCOLOR diffuse)
{
	VERTEX_3D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].diffuse = diffuse;
	pVtx[1].diffuse = diffuse;
	pVtx[2].diffuse = diffuse;
	pVtx[3].diffuse = diffuse;

	m_pD3DVtxBuff->Unlock();
}
