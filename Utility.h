/*-----------------------------------------------------------------------------
	���[�e�B���e�B
	author	HaradaYuto
	since	20150625
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

namespace Utility
{
	/*-----------------------------------------------------------------------------
		Csv�̓��e��z��ɂ����(�f�[�^�͐��l����)
		param �񎟌��z��p�|�C���^�@�v�f���i�[�p�|�C���^
	------------------------------------------------------------------------------*/
	bool LoadCsv( const char *file_path,int **&array_2d,int *x_elem_num,int *y_elem_num );

	/*-----------------------------------------------------------------------------
		�񎟌��z��f���[�g�iint�̂�
		param �z��|�C���^ ���v�f��
	------------------------------------------------------------------------------*/
	void Delete2DArrayInt( int **&array_2d,int y_elem_num );
}