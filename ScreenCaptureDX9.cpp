/*-----------------------------------------------------------------------------
	DirectX9�X�N���[���V���b�g�o�b�N�o�b�t�@�o�͏���
	author	OkadaMoeto
	since	20140617
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�x���}��
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS // �Z�L���A�֐��Ɋւ��郏�[�j���O������

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "ScreenCaptureDX9.h"
#include "Define.h"
#include <stdio.h>
#include <tchar.h>
#include <ctime>
#include "CDebugProcDX9.h"

/*-----------------------------------------------------------------------------
	�}�N����`
-----------------------------------------------------------------------------*/
// �ۑ��t�@�C���p�X�i�N_��_��_��_��_�t���[���j
#define FILENAME_OUTPUT_DIR_BMP ("data/screenshot/%d_%02d_%02d_%02d_%02d_%02d_%02d.bmp")
#define FILENAME_OUTPUT_DIR_JPG ("data/screenshot/%d_%02d_%02d_%02d_%02d_%02d_%02d.jpg")

/*-----------------------------------------------------------------------------
	�o�b�N�o�b�t�@�o��(�X�N���[���L���v�`��)
-----------------------------------------------------------------------------*/
bool CaptureImageFromBackBuffer(D3DXIMAGE_FILEFORMAT file_format)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DPRESENT_PARAMETERS pParam = CManager::GetRenderer()->GetParameters();
	// �T�[�t�F�C�X
	LPDIRECT3DSURFACE9 pSurface = NULL;
	// �o�b�N�o�b�t�@
	LPDIRECT3DSURFACE9 pBackBuff = NULL;
	// �t�@�C���p�X��
	char fileName[256];
	// �^�C���X�^���v
	time_t timer;
	struct tm *local;
		
	// �^�C���X�^���v�쐬
	timer = time(NULL);
	// �n�����ɕϊ�
	local = localtime(&timer);

	// BMP, JPG�`���̃t�H�[�}�b�g���T�|�[�g
	if(file_format != D3DXIFF_BMP && file_format != D3DXIFF_JPG)
	{
		return false;
	}

	//�o�͂���t�H�[�}�b�g�ɂ���ăt�@�C���̊g���q��ύX����B
	switch(file_format)
	{
		case D3DXIFF_BMP:
			// tm_year + 1900 (�N - 1900)����Ă���̂�1900����
			// tm_mon + 1 �O�n�܂�Ȃ̂Ł{�P�����ق���������Ղ�
			_stprintf(
						fileName,
						FILENAME_OUTPUT_DIR_BMP,
						local->tm_year + 1900,
						local->tm_mon + 1,
						local->tm_mday,
						local->tm_hour,
						local->tm_min,
						local->tm_sec,
						CDebugProcDX9::GetCountFrame()
			);
			break;

		case D3DXIFF_JPG:
			_stprintf(
						fileName,
						FILENAME_OUTPUT_DIR_JPG,
						local->tm_year + 1900,
						local->tm_mon + 1,
						local->tm_mday,
						local->tm_hour, 
						local->tm_min,
						local->tm_sec,
						CDebugProcDX9::GetCountFrame()
			);
			break;
	}

	// �L���v�`���p�̃T�[�t�F�C�X���쐬
	if(FAILED(pDevice->CreateOffscreenPlainSurface(
													(UINT)SCREEN_WIDTH,
													(UINT)SCREEN_HEIGHT, 
													pParam.BackBufferFormat,
													D3DPOOL_SYSTEMMEM,
													&pSurface,
													NULL
	)))
	{
		return false;
	}

	// �o�b�N�o�b�t�@�̎擾
	pDevice->GetRenderTarget(0, &pBackBuff);

	// �o�b�N�o�b�t�@�f�[�^�̓]��
	pDevice->GetRenderTargetData(pBackBuff, pSurface);

	// �p�ӂ������͉̂��
	pBackBuff->Release();
	pBackBuff = nullptr;

	// �t�@�C���o��
	D3DXSaveSurfaceToFile(fileName, file_format, pSurface, NULL, NULL);
	
	// �p�ӂ������͉̂��
	pSurface->Release();
	pSurface = nullptr;
	
	return true;
}
