/*-----------------------------------------------------------------------------
	DirectX�f�o�C�X�̐����ƕ`��N���X
	author	OkadaMoeto
	since	20140415
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CRenderer.h"
#include "Define.h"
#include "CManager.h"
#include "CCamera.h"
#include "CScene.h"

/*-----------------------------------------------------------------------------
	�ÓI�����o�ϐ��̏�����
-----------------------------------------------------------------------------*/
bool CRenderer::m_bFillModeWireFrame = false;

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CRenderer::~CRenderer()
{
}

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == nullptr)
	{
		// DirectX User �����^�C���擾���悤�B
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}


	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			= (UINT)SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= (UINT)SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			// �o�b�N�o�b�t�@�̃t�H�[�}�b�g�͌��ݐݒ肳��Ă�����̂��g��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬

	// D16�ȉ��ł��_���Ȃ�΃G���[���b�Z�[�W
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if(bWindow)
	{
		// �E�B���h�E���[�h
		d3dpp.BackBufferFormat				= D3DFMT_UNKNOWN;					// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz	= 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{
		// �t���X�N���[�����[�h
		d3dpp.BackBufferFormat				= D3DFMT_R5G6B5;					// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz	= 0;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(m_pD3D->CreateDevice(
									D3DADAPTER_DEFAULT,						// �f�B�X�v���C�A�_�v�^
									D3DDEVTYPE_HAL,							// �f�B�X�v���C�^�C�v
									hWnd,									// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
									D3DCREATE_HARDWARE_VERTEXPROCESSING,	// �f�o�C�X�쐬����̑g�ݍ��킹
									&d3dpp,									// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
									&m_pD3DDevice)))						// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if(FAILED(
					m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
					D3DDEVTYPE_HAL, 
					hWnd, 
					D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
					&d3dpp,
					&m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if(FAILED(
						m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
						D3DDEVTYPE_REF,
						hWnd, 
						D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
						&d3dpp,
						&m_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}

			// �n�[�h�E�F�A�Ő����o���Ȃ��������̏ꍇ�́A���������Ȃǐ���ɏo�͂���Ȃ����b�Z�[�W�o��
			MessageBox(hWnd, "���������ȂǂŐ���ɉ�ʂ��o�͂���Ȃ������ꂪ����܂�", "WARNING", MB_ICONWARNING);
		}
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̕ۑ�
	m_pParameters = d3dpp;

	// ���ʃ|���S��������
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Z�o�b�t�@�L��
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// �A���t�@�u�����h�L��
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// �ʏ�u�����h��ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// UV���W�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���̊g�k���̃��j�A��Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// �e�N�X�`���̍������킹�̓���
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �w�i��������
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CRenderer::Uninit(void)
{
	// Dirext3D�I�u�W�F�N�g�̉��
	if(m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	// �f�o�C�X�I�u�W�F�N�g�̉��
	if(m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CRenderer::Update(void)
{
	CScene::UpdateAll();
}

/*-----------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------*/
void CRenderer::Draw(void)
{
	CCamera* pCamera = CManager::GetCamera();

	if(m_bFillModeWireFrame)
	{
		// �`����@�����C���[�t���[���ɕύX����
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		// �`����@�����ɖ߂�
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	// ��ʂ̃N���A
	m_pD3DDevice->Clear(
						0,
						nullptr,
						(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
						D3DCOLOR_RGBA(0, 0, 0, 0),
						1.0f,
						0);

	// �`��J�n
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		pCamera->SetCamera();

		CScene::DrawAll();

#ifdef _DEBUG
		// �f�o�b�O����
		CDebugProcDX9::Draw();
#endif

		// �`��I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
