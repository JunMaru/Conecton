/*-----------------------------------------------------------------------------
	�J�����N���X
	author	OkadaMoeto
	since	20140508
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CCamera.h"
#include "CManager.h"
#include "CGame.h"
#include "CInput.h"
#include "CPlayer.h"

/*-----------------------------------------------------------------------------
	�}�N����`
-----------------------------------------------------------------------------*/
// �r���[�ϊ��p
static const D3DXVECTOR3 INIT_POS_EYE = D3DXVECTOR3(0.0f, 0.0f, -30.0f);
static const D3DXVECTOR3 INIT_POS_LOOK = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
static const D3DXVECTOR3 INIT_POS_VECUP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
static const D3DXVECTOR3 INIT_ROT_CAMERA = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

// �p�[�X�y�N�e�B�u�ˉe�s��p
static const float SET_FOVY = D3DX_PI / 4.0f;
static const float SET_ASPECT = 800.0f / 600.0f;
static const float SET_NEAR_Z = 1.0f;
static const float SET_FAR_Z = 15000.0f;

// �r���[�|�[�g�p
static const int VIEWPORT_X = 0;
static const int VIEWPORT_Y = 0;
static const int VIEWPORT_WIDTH = 800;
static const int VIEWPORT_HEIGHT = 600;
static const float VIEWPORT_MIN_Z = 0.0f;
static const float VIEWPORT_MAX_Z = 1.0f;

// �J�����̃L�[�ړ���
static const float CAMERA_KEY_MOVE = 0.1f;
static const float CAMERA_KEY_ROTATE = 0.01f;
static const float CAMERA_MOUSE_MOVE = 0.1f;
static const float CAMERA_MOUSE_ROTATE = 0.001f;
static const float CAMERA_MOUSE_DRAG = 0.1f;
static const float CAMERA_MOUSE_WHEEL = 0.01f;

// �ړ����x
static const float CAMERA_MOVE = 1.0f;
// �ړ������W��
static const float CAMERA_RATE_MOVE = 0.10f;
// ��]���x
static const float CAMERA_ROTATE = D3DX_PI * 0.01f;
// ��]�����W��
static const float CAMERA_RATE_ROTATE = 0.10f;

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CCamera::CCamera()
{
	m_posEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecup = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDestEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDistanceEyeLookatAxisXYZ = 0.0f;
	m_fDistanceEyeLookatAxisXZ = 0.0f;
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CCamera::~CCamera()
{
}

/*-----------------------------------------------------------------------------
	�J�����̏�����
-----------------------------------------------------------------------------*/
HRESULT CCamera::Init(void)
{
	// ���_�ʒu�ݒ�
	m_posEye = INIT_POS_EYE;
	// �����_�ʒu�ݒ�
	m_posLook = INIT_POS_LOOK;
	// ������x�N�g���ݒ�
	m_vecup = INIT_POS_VECUP;
	// ���p�x�̐ݒ�
	m_rot = INIT_ROT_CAMERA;

	// �ڕW�ʒu��ݒ�
	m_posDestEye = m_posEye;
	m_rotDest = m_rot;

	// �r���[�|�[�g�̐ݒ�
	m_viewport.X = VIEWPORT_X;
	m_viewport.Y = VIEWPORT_Y;
	m_viewport.Width = VIEWPORT_WIDTH;
	m_viewport.Height = VIEWPORT_HEIGHT;
	m_viewport.MinZ = VIEWPORT_MIN_Z;
	m_viewport.MaxZ = VIEWPORT_MAX_Z;

	// ���_�ƒ����_�̓�_�Ԃ̋������Z�o
	float fDistanceXZ;
	
	m_fDistanceEyeLookatAxisXYZ = sqrtf((m_posLook.x - m_posEye.x) * (m_posLook.x - m_posEye.x)
										+ (m_posLook.y - m_posEye.y) * (m_posLook.y - m_posEye.y)
										+ (m_posLook.z - m_posEye.z) * (m_posLook.z - m_posEye.z));
	
	fDistanceXZ = sqrtf((m_posLook.x - m_posEye.x) * (m_posLook.x - m_posEye.x)
						+ (m_posLook.z - m_posEye.z) * (m_posLook.z - m_posEye.z));
	
	m_rot.y = atan2f((m_posLook.x - m_posEye.x), (m_posLook.z - m_posEye.z));
	m_rot.x = atan2f((m_posLook.y - m_posEye.y), fDistanceXZ);
	
	m_fDistanceEyeLookatAxisXZ = cosf(m_rot.x) * m_fDistanceEyeLookatAxisXYZ;
	
	if(fDistanceXZ == 0.0f)
	{
		m_vecup = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�J�����̏I��
-----------------------------------------------------------------------------*/
void CCamera::Uninit(void)
{
}

/*-----------------------------------------------------------------------------
	�J�����̍X�V
-----------------------------------------------------------------------------*/
void CCamera::Update(void)
{
	OperateCameraMouse();
}

/*-----------------------------------------------------------------------------
	�J�����̕`��ݒ�
-----------------------------------------------------------------------------*/
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// �f�o�C�X�擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �r���[�s��̏�����
	D3DXMatrixIdentity(&m_mtxView);
	// �r���[�s��̍쐬( �J�����̈ʒu�ƕ�����ݒ� )
	D3DXMatrixLookAtLH(&m_mtxView, &m_posEye, &m_posLook, &m_vecup);
	// �r���[�s��̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW,&m_mtxView);

	// �ˉe�s��̏�����
	D3DXMatrixIdentity(&m_mtxProjection);
	// �ˉe�s��̍쐬( ��p�ƕ`��͈͂̐ݒ� )
	D3DXMatrixPerspectiveFovLH(
								&m_mtxProjection,
								SET_FOVY,
								SET_ASPECT,
								SET_NEAR_Z,
								SET_FAR_Z);

	// �ˉe�s��̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION,&m_mtxProjection);
}

/*-----------------------------------------------------------------------------
	�`�[�g�J��������(�X�N�V���ȂǎB���)
-----------------------------------------------------------------------------*/
void CCamera::OperateCameraMouse(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// �}�E�X�擾
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	float fValueX, fValueY, fValueZ;
	fValueX = (float)pInputMouse->GetAxisX();
	fValueY = (float)pInputMouse->GetAxisY();
	fValueZ = (float)pInputMouse->GetAxisZ();

	// �}�E�X�������h���b�O�ɂ�莋�_�̈ړ�
	if(pInputMouse->GetLeftPress())
	{
		// �}�E�X�E�������h���b�O�ɂ�莋�_�̐���
		if(pInputMouse->GetRightPress())
		{
			// ������
			if(fValueX < 0)
			{
				m_rot.y += D3DX_PI * fValueX * CAMERA_MOUSE_ROTATE;
			
				// PI�ȏ�܂���-PI�ȏ�Ȃ�Ίp�x�𐳋K��
				if(m_rot.y < -D3DX_PI)
				{
					m_rot.y += D3DX_PI * 2.0f;
				}
				else if(m_rot.y > D3DX_PI)
				{
					m_rot.y -= D3DX_PI * 2.0f;
				}

				m_posEye.x = m_posLook.x - sinf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
				m_posEye.z = m_posLook.z - cosf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
			}
			// �E����
			else if(fValueX > 0)
			{
				m_rot.y += D3DX_PI * fValueX * CAMERA_MOUSE_ROTATE;
			
				// PI�ȏ�܂���-PI�ȏ�Ȃ�Ίp�x�𐳋K��
				if(m_rot.y < -D3DX_PI)
				{
					m_rot.y += D3DX_PI * 2.0f;
				}
				else if(m_rot.y > D3DX_PI)
				{
					m_rot.y -= D3DX_PI * 2.0f;
				}

				m_posEye.x = m_posLook.x - sinf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
				m_posEye.z = m_posLook.z - cosf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
			}
			// �����
			if(fValueY < 0)
			{
				m_rot.x -= D3DX_PI * fValueY * CAMERA_MOUSE_ROTATE;
			
				if(m_rot.x > (D3DX_PI/2.0f - D3DX_PI*0.02f))
				{
					m_rot.x = (D3DX_PI/2.0f - D3DX_PI*0.02f);
				}

				m_posEye.y = m_posLook.y - sinf(m_rot.x)
							* m_fDistanceEyeLookatAxisXYZ;

				m_fDistanceEyeLookatAxisXZ = cosf(m_rot.x)
											* m_fDistanceEyeLookatAxisXYZ;
				m_posEye.x = m_posLook.x - sinf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
				m_posEye.z = m_posLook.z - cosf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
			}
			// ������
			else if(fValueY > 0)
			{
				m_rot.x -= D3DX_PI * fValueY * CAMERA_MOUSE_ROTATE;
			
				if(m_rot.x < (-D3DX_PI/2.0f + D3DX_PI*0.02f))
				{
					m_rot.x = (-D3DX_PI/2.0f + D3DX_PI*0.02f);
				}

				m_posEye.y = m_posLook.y - sinf(m_rot.x)
							* m_fDistanceEyeLookatAxisXYZ;

				m_fDistanceEyeLookatAxisXZ = cosf(m_rot.x)
											* m_fDistanceEyeLookatAxisXYZ;
				m_posEye.x = m_posLook.x - sinf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
				m_posEye.z = m_posLook.z - cosf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
			}
		}
		else
		{
			if(fValueX != 0 || fValueY != 0)
			{
				float angle = atan2f(fValueX, fValueY);
				float length = sqrtf(fValueX * fValueX + fValueY * fValueY)
								* CAMERA_MOUSE_DRAG;

				angle -= m_rot.y;
				if(angle < -D3DX_PI) angle += D3DX_PI * 2.0f;

				m_posLook.x -= sinf(angle) * length;
				m_posLook.z += cosf(angle) * length;

				m_posEye.x = m_posLook.x - sinf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
				m_posEye.z = m_posLook.z - cosf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
			}
		}
	}
	// �}�E�X�E�������h���b�O�ɂ�蒍���_����
	else if(pInputMouse->GetRightPress())
	{
		// ������
		if(fValueX < 0)
		{
			m_rot.y += D3DX_PI * fValueX * CAMERA_MOUSE_ROTATE;
			
			// PI�ȏ�܂���-PI�ȏ�Ȃ�Ίp�x�𐳋K��
			if(m_rot.y < -D3DX_PI)
			{
				m_rot.y += D3DX_PI * 2.0f;
			}
			else if(m_rot.y > D3DX_PI)
			{
				m_rot.y -= D3DX_PI * 2.0f;
			}

			m_posLook.x = m_posEye.x + sinf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
			m_posLook.z = m_posEye.z + cosf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
		}
		// �E����
		else if(fValueX > 0)
		{
			m_rot.y += D3DX_PI * fValueX * CAMERA_MOUSE_ROTATE;
			
			// PI�ȏ�܂���-PI�ȏ�Ȃ�Ίp�x�𐳋K��
			if(m_rot.y < -D3DX_PI)
			{
				m_rot.y += D3DX_PI * 2.0f;
			}
			else if(m_rot.y > D3DX_PI)
			{
				m_rot.y -= D3DX_PI * 2.0f;
			}

			m_posLook.x = m_posEye.x + sinf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
			m_posLook.z = m_posEye.z + cosf(m_rot.y)
							* m_fDistanceEyeLookatAxisXZ;
		}
	}
}

/*-----------------------------------------------------------------------------
	�J�����ʒu�������l�ɖ߂�
-----------------------------------------------------------------------------*/
void CCamera::ResetCamera(void)
{
	m_posEye = INIT_POS_EYE;
	m_posLook = INIT_POS_LOOK;
	m_vecup = INIT_POS_VECUP;
	m_rot = INIT_ROT_CAMERA;

	m_posDestEye = m_posEye;
	m_rotDest = m_rot;

	// ���_�ƒ����_�̓�_�Ԃ̋������Z�o
	float fDistanceXZ = 0.0f;
	
	m_fDistanceEyeLookatAxisXYZ = sqrtf((m_posLook.x - m_posEye.x) * (m_posLook.x - m_posEye.x)
										+ (m_posLook.y - m_posEye.y) * (m_posLook.y - m_posEye.y)
										+ (m_posLook.z - m_posEye.z) * (m_posLook.z - m_posEye.z));
	
	fDistanceXZ = sqrtf((m_posLook.x - m_posEye.x) * (m_posLook.x - m_posEye.x)
						+ (m_posLook.z - m_posEye.z) * (m_posLook.z - m_posEye.z));
	
	m_rot.y = atan2f((m_posLook.x - m_posEye.x), (m_posLook.z - m_posEye.z));
	m_rot.x = atan2f((m_posLook.y - m_posEye.y), fDistanceXZ);
	
	m_fDistanceEyeLookatAxisXZ = cosf(m_rot.x) * m_fDistanceEyeLookatAxisXYZ;
	
	if(fDistanceXZ == 0.0f)
	{
		m_vecup = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
}
