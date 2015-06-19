//=============================================================================
//
// ���[�U�[���� [CLaser.cpp]
// Author : JUN MARUYAMA
// Since  : 2015/06/12
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Main.h"
#include "Define.h"
#include "CLaser.h"
#include "CScrollManager.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
static const float LASER_MOVE_SPEED = (10.0f);
static const float LASER_THICKNESS = (100.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLaser::CLaser(const int layer) : CScene(layer)
{
	m_startPosition = VEC3_ZERO;
	m_endPosition = VEC3_ZERO;
	m_worldStartPosition = VEC3_ZERO;
	m_worldEndPosition = VEC3_ZERO;
	m_rotation = VEC3_ZERO;
	m_direction = DIRECTION_MAX;
	m_fMoveSpeed = LASER_MOVE_SPEED;
	m_pVertexBuffer = nullptr;
	m_pTexture = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLaser::~CLaser()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLaser::Init(void)
{
	m_endPosition = m_startPosition;

	CalcRotation();
	CreateVertexBuffer();
	CreateTexture();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLaser::Uninit(void)
{
	if (m_pVertexBuffer != nullptr)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLaser::Update(void)
{
	// MoveStop�֐����Ă񂾏ꍇ�Am_fMoveSpeed��0.0f�ɂȂ��Đi�܂Ȃ�
	m_endPosition.x += m_fMoveSpeed;

	UpdateVertexBuffer();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLaser::Draw(void)
{
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

	device->SetFVF(FVF_VERTEX_2D);
	device->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VERTEX_2D));

	device->SetTexture(0, m_pTexture);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ��������
//=============================================================================
CLaser* CLaser::Create(const D3DXVECTOR3& start_position, const DIRECTION& direction)
{
	CLaser *pLaser = new CLaser();
	pLaser->SetStartPosition(start_position);
	pLaser->SetDirection(direction);
	pLaser->Init();

	return pLaser;
}

//=============================================================================
// ��]�v�Z����
//=============================================================================
void CLaser::CalcRotation(void)
{
	float pi = D3DX_PI / 2;
	m_rotation.z = pi * m_direction;
}

//=============================================================================
// ���_�o�b�t�@��������
//=============================================================================
void CLaser::CreateVertexBuffer(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	VERTEX_2D *pVertexInfo;

	m_pVertexBuffer->Lock(0, 0, (void**)&pVertexInfo, 0);

	D3DXVECTOR2 texWork[] = { D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0),
							D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 1), };

	for (int nCnt = 0; nCnt < 4; ++nCnt)
	{
		pVertexInfo[nCnt].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexInfo[nCnt].rhw = 1.0f;
		pVertexInfo[nCnt].tex = texWork[nCnt];
	}

	m_pVertexBuffer->Unlock();
}

//=============================================================================
// �e�N�X�`����������
//=============================================================================
void CLaser::CreateTexture(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(pDevice, "data/texture/laser/laser.png", &m_pTexture);
}

//=============================================================================
// ���_�o�b�t�@�X�V����
//=============================================================================
void CLaser::UpdateVertexBuffer(void)
{
	float fLength = m_endPosition.x - m_startPosition.x;

	// ���W�ϊ��O�̍��W���Z�o
	D3DXVECTOR3 workPos[4];
	workPos[0] = D3DXVECTOR3(0.0f, -LASER_THICKNESS / 2, 0.0f);
	workPos[1] = D3DXVECTOR3(fLength, -LASER_THICKNESS / 2, 0.0f);
	workPos[2] = D3DXVECTOR3(0.0f, LASER_THICKNESS / 2, 0.0f);
	workPos[3] = D3DXVECTOR3(fLength, LASER_THICKNESS / 2, 0.0f);

	// �X�N���[�����l��������]���S���W���Z�o
	D3DXVECTOR3 centerPos = m_startPosition;
	D3DXVECTOR3 scrollPos = CScrollManager::GetScrollWorld();
	centerPos -= scrollPos;
	centerPos.z = 0.0f;

	// ���W�ϊ��s��̍쐬
	D3DXMATRIX mtxTrans, mtxRot, mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxTrans);

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&mtxTrans, centerPos.x, centerPos.y, 0.0f);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���W�ϊ�
	for (int nCnt = 0; nCnt < 4; ++nCnt)
	{
		D3DXVec3TransformCoord(&workPos[nCnt], &workPos[nCnt], &mtxWorld);
	}

	// ���ۂ̎ˏo���W�ƏI�[���W���Z�o���邽�߂̃x�[�X���W
	D3DXVECTOR3 tmpStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 tmpEndPos = D3DXVECTOR3(fLength, 0.0f, 0.0f);

	// ���ۂ̍��W�n�̎ˏo���W/�I�[���W�̎Z�o
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxTrans);
	D3DXMatrixTranslation(&mtxTrans, m_startPosition.x, m_startPosition.y, m_startPosition.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	D3DXVec3TransformCoord(&m_worldStartPosition, &tmpStartPos, &mtxWorld);
	D3DXVec3TransformCoord(&m_worldEndPosition, &tmpEndPos, &mtxWorld);

	// ���W�ϊ��������̂𒸓_�o�b�t�@�ɏ�������
	VERTEX_2D *pVertexInfo;

	m_pVertexBuffer->Lock(0, 0, (void**)&pVertexInfo, 0);
	
	for (int nCnt = 0; nCnt < 4; ++nCnt)
	{
		pVertexInfo[nCnt].vtx = workPos[nCnt];
	}

	m_pVertexBuffer->Unlock();
}

//=============================================================================
// �i�s�ĊJ����
//=============================================================================
void CLaser::MoveRestart(void)
{
	m_fMoveSpeed = LASER_MOVE_SPEED;
}

//=============================================================================
// �i�s���f����
//=============================================================================
void CLaser::MoveStop(void)
{
	m_fMoveSpeed = 0.0f;
}