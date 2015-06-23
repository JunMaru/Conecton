/*-----------------------------------------------------------------------------
	�e�N�X�`���V�[�g�̃��[�h�n�֐�
	author	HaradaYuto
	since	20150616
-----------------------------------------------------------------------------*/
#pragma once
#include <d3dx9.h>

namespace TexLoader
{
	typedef struct
	{
		D3DXVECTOR2 uv;
		D3DXVECTOR2 size;
	}UvInfo;

	/*------------------------------------------------------------------------------
		�e�N�X�`����񃍁[�h
		param		�e�N�X�`���t�@�C���p�X�i�e�L�X�g�ւ́j
					�e�N�X�`��uv�T�C�Y�|�C���^�i������new�����)
					�e�N�X�`���|�C���^
		return		�������ꂽ�z��v�f��
	-------------------------------------------------------------------------------*/
	int LoadTexSheetFromText( const char *file_path, UvInfo *& uv_info, LPDIRECT3DTEXTURE9 *tex = NULL );
	int LoadTexSheetFromBin ( const char *file_path, UvInfo *& uv_info, LPDIRECT3DTEXTURE9 *tex = NULL );

}
