/*-----------------------------------------------------------------------------
	2D�|���S���N���X
	author	OkadaMoeto
	since	20140617
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CScene2D.h"

/*-----------------------------------------------------------------------------
	�}�N����`
-----------------------------------------------------------------------------*/
// �t�@�C����
#define FILENAME_2DPOLYGON ("data/testsample/mikan.jpg")

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CScene2D::CScene2D(
					int nPriority,
					CScene::OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pD3DTexture = nullptr;
	m_pD3DVtxBuff = nullptr;
}

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CScene2D::~CScene2D()
{
}

/*-----------------------------------------------------------------------------
	����(1)
-----------------------------------------------------------------------------*/
CScene2D* CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	CScene2D* pScene2D;

	// �Q�c�I�u�W�F�N�g�̐���
	pScene2D = new CScene2D();
	pScene2D->Init(pos, rot, col);

	// �������g��Ԃ�
	return pScene2D;
}

/*-----------------------------------------------------------------------------
	����(2)
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

	// ����������
	pScene2D = new CScene2D();
	pScene2D->Init(file_name, pos, rot, col, width, height);

	pScene2D->SetDiffuse(col);

	// �������g��Ԃ�
	return pScene2D;
}

/*-----------------------------------------------------------------------------
	������(1)
-----------------------------------------------------------------------------*/
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	VERTEX_2D *pVtx = nullptr;

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	SetPosition(pos);
	SetRotation(rot);

	// �T�C�Y��ݒ�
	m_size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
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

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, FILENAME_2DPOLYGON, &m_pD3DTexture);

	// ���a�̂ق��������₷���̂Ŕ����ɂ��Ă���
	m_size *= 0.5f;

	// �Ίp���̒��������߂�
	m_lengthDiagonal = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);

	// �Ίp���̊p�x�����߂�
	m_angleDiagonal = atan2f(m_size.x, m_size.x);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	������(2)
-----------------------------------------------------------------------------*/
HRESULT CScene2D::Init(const char* file_name, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float width, float height)
{
	VERTEX_2D *pVtx = nullptr;

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	SetPosition(pos);
	SetRotation(rot);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
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

	// ���S�_���畝�ƍ����ŋ�`�̊e���_���w��
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

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, file_name, &m_pD3DTexture);

	// ���ƍ������擾
	m_size.x = width * 0.5f;
	m_size.y = height * 0.5f;

	// �Ίp���̒��������߂�
	m_lengthDiagonal = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);

	// �Ίp���̊p�x�����߂�
	m_angleDiagonal = atan2f(m_size.y, m_size.x);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CScene2D::Uninit(void)
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
void CScene2D::Update(void)
{
}

/*-----------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------*/
void CScene2D::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pD3DTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

/*-----------------------------------------------------------------------------
	���_���W��ݒ�
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
	���_���ːF��ݒ�
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
	���_�e�N�X�`�����W��ݒ�
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
	�|���S���̃T�C�Y�ݒ�
-----------------------------------------------------------------------------*/
void CScene2D::SetScale(float width, float height)
{
	// ���ƍ������擾
	m_size.x = width * 0.5f;
	m_size.y = height * 0.5f;

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	VERTEX_2D *pVtx = nullptr;
	m_pD3DVtxBuff->Lock(0,0,(void**)&pVtx,0);

	pVtx[0].vtx = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	m_pD3DVtxBuff->Unlock();

	// �Ίp���̒��������߂�
	m_lengthDiagonal = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);

	// �Ίp���̊p�x�����߂�
	m_angleDiagonal = atan2f(m_size.y, m_size.x);
}

void CScene2D::SetScale(D3DXVECTOR2 size)
{
	// ���ƍ������擾
	m_size.x = size.x * 0.5f;
	m_size.y = size.y * 0.5f;

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	VERTEX_2D *pVtx = nullptr;
	m_pD3DVtxBuff->Lock(0,0,(void**)&pVtx,0);

	pVtx[0].vtx = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	m_pD3DVtxBuff->Unlock();

	// �Ίp���̒��������߂�
	m_lengthDiagonal = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);

	// �Ίp���̊p�x�����߂�
	m_angleDiagonal = atan2f(m_size.y, m_size.x);
}
