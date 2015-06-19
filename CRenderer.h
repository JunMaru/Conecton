/*-----------------------------------------------------------------------------
	DirectX�f�o�C�X�̐����ƕ`��N���X
	author	OkadaMoeto
	since	20140415
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
	�`��N���X
-----------------------------------------------------------------------------*/
class CRenderer
{
	public:
		CRenderer();
		~CRenderer();

		HRESULT Init(HWND hWnd, BOOL bWindow);
		void Uninit(void);
		void Update(void);
		void Draw(void);

		/*-----------------------------------------------------------------------------
			Direct3D�f�o�C�X�̎擾
			return	LPDIRECT3DDEVICE9		Direct3D�f�o�C�X
		-----------------------------------------------------------------------------*/
		LPDIRECT3DDEVICE9 GetDevice(void){ return m_pD3DDevice; }
		/*-----------------------------------------------------------------------------
			DirectX�p�����[�^�̎擾
			return	D3DPRESENT_PARAMETERS		DirectX�p�����[�^
		-----------------------------------------------------------------------------*/
		D3DPRESENT_PARAMETERS GetParameters(void){ return m_pParameters; }

		/*-----------------------------------------------------------------------------
			���C���[�t���[���t���O�̎擾
			return	bool		���C���[�t���[���t���O�̒l
		-----------------------------------------------------------------------------*/
		static bool GetFillModeWireFrame(void){ return m_bFillModeWireFrame; }
		/*-----------------------------------------------------------------------------
			���C���[�t���[���t���O�̐ݒ�
			param	flag		���C���[�t���[���t���O�̐ݒ�l
		-----------------------------------------------------------------------------*/
		static void SetFillModeWireFrame(bool flag){ m_bFillModeWireFrame = flag; }

	private:
		// Direct3D�I�u�W�F�N�g
		LPDIRECT3D9 m_pD3D;

		// Device�I�u�W�F�N�g(�`��ɕK�v)
		LPDIRECT3DDEVICE9 m_pD3DDevice;

		// �p�����[�^�ۑ��p
		D3DPRESENT_PARAMETERS m_pParameters;

		// �h��Ԃ����[�h�̃��C���[�t���[���\���̃t���O
		static bool m_bFillModeWireFrame;
};
