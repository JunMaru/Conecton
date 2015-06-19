/*-----------------------------------------------------------------------------
	カメラクラス
	author	OkadaMoeto
	since	20140508
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CCamera.h"
#include "CManager.h"
#include "CGame.h"
#include "CInput.h"
#include "CPlayer.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/
// ビュー変換用
static const D3DXVECTOR3 INIT_POS_EYE = D3DXVECTOR3(0.0f, 0.0f, -30.0f);
static const D3DXVECTOR3 INIT_POS_LOOK = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
static const D3DXVECTOR3 INIT_POS_VECUP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
static const D3DXVECTOR3 INIT_ROT_CAMERA = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

// パースペクティブ射影行列用
static const float SET_FOVY = D3DX_PI / 4.0f;
static const float SET_ASPECT = 800.0f / 600.0f;
static const float SET_NEAR_Z = 1.0f;
static const float SET_FAR_Z = 15000.0f;

// ビューポート用
static const int VIEWPORT_X = 0;
static const int VIEWPORT_Y = 0;
static const int VIEWPORT_WIDTH = 800;
static const int VIEWPORT_HEIGHT = 600;
static const float VIEWPORT_MIN_Z = 0.0f;
static const float VIEWPORT_MAX_Z = 1.0f;

// カメラのキー移動量
static const float CAMERA_KEY_MOVE = 0.1f;
static const float CAMERA_KEY_ROTATE = 0.01f;
static const float CAMERA_MOUSE_MOVE = 0.1f;
static const float CAMERA_MOUSE_ROTATE = 0.001f;
static const float CAMERA_MOUSE_DRAG = 0.1f;
static const float CAMERA_MOUSE_WHEEL = 0.01f;

// 移動速度
static const float CAMERA_MOVE = 1.0f;
// 移動慣性係数
static const float CAMERA_RATE_MOVE = 0.10f;
// 回転速度
static const float CAMERA_ROTATE = D3DX_PI * 0.01f;
// 回転慣性係数
static const float CAMERA_RATE_ROTATE = 0.10f;

/*-----------------------------------------------------------------------------
	コンストラクタ
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
	デストラクタ
-----------------------------------------------------------------------------*/
CCamera::~CCamera()
{
}

/*-----------------------------------------------------------------------------
	カメラの初期化
-----------------------------------------------------------------------------*/
HRESULT CCamera::Init(void)
{
	// 視点位置設定
	m_posEye = INIT_POS_EYE;
	// 注視点位置設定
	m_posLook = INIT_POS_LOOK;
	// 上方向ベクトル設定
	m_vecup = INIT_POS_VECUP;
	// 視角度の設定
	m_rot = INIT_ROT_CAMERA;

	// 目標位置を設定
	m_posDestEye = m_posEye;
	m_rotDest = m_rot;

	// ビューポートの設定
	m_viewport.X = VIEWPORT_X;
	m_viewport.Y = VIEWPORT_Y;
	m_viewport.Width = VIEWPORT_WIDTH;
	m_viewport.Height = VIEWPORT_HEIGHT;
	m_viewport.MinZ = VIEWPORT_MIN_Z;
	m_viewport.MaxZ = VIEWPORT_MAX_Z;

	// 視点と注視点の二点間の距離を算出
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
	カメラの終了
-----------------------------------------------------------------------------*/
void CCamera::Uninit(void)
{
}

/*-----------------------------------------------------------------------------
	カメラの更新
-----------------------------------------------------------------------------*/
void CCamera::Update(void)
{
	OperateCameraMouse();
}

/*-----------------------------------------------------------------------------
	カメラの描画設定
-----------------------------------------------------------------------------*/
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// デバイス取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// ビュー行列の初期化
	D3DXMatrixIdentity(&m_mtxView);
	// ビュー行列の作成( カメラの位置と方向を設定 )
	D3DXMatrixLookAtLH(&m_mtxView, &m_posEye, &m_posLook, &m_vecup);
	// ビュー行列の設定
	pDevice->SetTransform(D3DTS_VIEW,&m_mtxView);

	// 射影行列の初期化
	D3DXMatrixIdentity(&m_mtxProjection);
	// 射影行列の作成( 画角と描画範囲の設定 )
	D3DXMatrixPerspectiveFovLH(
								&m_mtxProjection,
								SET_FOVY,
								SET_ASPECT,
								SET_NEAR_Z,
								SET_FAR_Z);

	// 射影行列の設定
	pDevice->SetTransform(D3DTS_PROJECTION,&m_mtxProjection);
}

/*-----------------------------------------------------------------------------
	チートカメラ操作(スクショなど撮る際)
-----------------------------------------------------------------------------*/
void CCamera::OperateCameraMouse(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	float fValueX, fValueY, fValueZ;
	fValueX = (float)pInputMouse->GetAxisX();
	fValueY = (float)pInputMouse->GetAxisY();
	fValueZ = (float)pInputMouse->GetAxisZ();

	// マウス左押下ドラッグにより視点の移動
	if(pInputMouse->GetLeftPress())
	{
		// マウス右押下時ドラッグにより視点の旋回
		if(pInputMouse->GetRightPress())
		{
			// 左旋回
			if(fValueX < 0)
			{
				m_rot.y += D3DX_PI * fValueX * CAMERA_MOUSE_ROTATE;
			
				// PI以上または-PI以上ならば角度を正規化
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
			// 右旋回
			else if(fValueX > 0)
			{
				m_rot.y += D3DX_PI * fValueX * CAMERA_MOUSE_ROTATE;
			
				// PI以上または-PI以上ならば角度を正規化
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
			// 上旋回
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
			// 下旋回
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
	// マウス右押下時ドラッグにより注視点旋回
	else if(pInputMouse->GetRightPress())
	{
		// 左旋回
		if(fValueX < 0)
		{
			m_rot.y += D3DX_PI * fValueX * CAMERA_MOUSE_ROTATE;
			
			// PI以上または-PI以上ならば角度を正規化
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
		// 右旋回
		else if(fValueX > 0)
		{
			m_rot.y += D3DX_PI * fValueX * CAMERA_MOUSE_ROTATE;
			
			// PI以上または-PI以上ならば角度を正規化
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
	カメラ位置を初期値に戻す
-----------------------------------------------------------------------------*/
void CCamera::ResetCamera(void)
{
	m_posEye = INIT_POS_EYE;
	m_posLook = INIT_POS_LOOK;
	m_vecup = INIT_POS_VECUP;
	m_rot = INIT_ROT_CAMERA;

	m_posDestEye = m_posEye;
	m_rotDest = m_rot;

	// 視点と注視点の二点間の距離を算出
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
