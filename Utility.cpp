/*-----------------------------------------------------------------------------
	���[�e�B���e�B
	author	HaradaYuto
	since	20150625
-----------------------------------------------------------------------------*/

#include "Utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace Utility
{
	bool LoadCsv( const char *pfile_path,int **&array_2d,int *x_elem_num,int *y_elem_num )
	{
		FILE *file;
		int cnt = 0;
		file = fopen( pfile_path,"rt" );

		char buf[1024];
		char *ptr;

		int x,y;

		x = 0;
		y = 0;

		fscanf(file,"%s",buf );

		//�Ō�J���}�Ȃ��̂�+1�ŏC��
		//1,1,1 strlen 5 /2 ->2 �ʒu����Ȃ�
		//�Ȃ̂ōŌ���J���}�����邱�Ƃɂ���+1
		x = strlen( buf ) +1;
		x = x/2;

		fseek(file,0,SEEK_SET );

		while(1)
		{
			//��s�ǂݍ���
			fscanf( file,"%s",buf );

			if(feof(file) )
			{
				break;
			}

			y++;
		}

		//�z�񐶐�
		int **array_buf;
		array_buf = new int*[ y ];

		for( int cnt = 0 ; cnt < y ; cnt++ )
		{
			array_buf[ cnt ] = new int[ x ];
		}
		
		//�v�f����������
		*x_elem_num = x;
		*y_elem_num = y;

		fseek( file,0,SEEK_SET );

		x = 0;
		y = 0;

		while(1)
		{
			fscanf( file,"%s",buf );

			if(feof(file) )
			{
				break;
			}

			ptr = strtok( buf,"," );
			int num;
			num = atoi( ptr );

			array_buf[y][x] = num;

			x++;

			while( ptr != NULL )
			{
				ptr = strtok( NULL,"," );

				if( ptr )
				{
					num = atoi( ptr );
					array_buf[y][x] = num;
					x++;
				}
			}
			x = 0;
			y++;
		}

		array_2d = array_buf;

		fclose(file);

		return true;

	}

	void Delete2DArrayInt( int **&array_2d,int y_elem_num )
	{
		for( int cnt = 0; cnt < y_elem_num; cnt++ )
		{
			delete []array_2d[cnt];
		}

		delete []array_2d;
	}
}