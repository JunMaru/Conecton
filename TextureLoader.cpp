/*-----------------------------------------------------------------------------
	�e�N�X�`���V�[�g�̃��[�h�n�֐�
	author	HaradaYuto
	since	20150616
-----------------------------------------------------------------------------*/

#include "TextureLoader.h"
#include <stdio.h>

//�����̊֐��Q�Ƀ}�l�[�W���ˑ�������̂����ꂩ�Ǝv��������
//���X�Ȃ̂ŋ��d�����i�ǂ��������܂ō���Ȃ�)
#include "CManager.h"

namespace TexLoader
{
	int LoadTexSheetFromText( const char *file_path, UvInfo *& uv_info, LPDIRECT3DTEXTURE9 *tex )
	{
		FILE *file;

		file = fopen( file_path,"rt" );

		if( file == NULL )
		{
			fclose( file );
			return -1;
		}

		//������
		char buf[256];
		int dataSum = 0;
		int idx = 0;

		while(1)
		{
			//�����񃍁[�h
			fscanf( file,"%s",buf );

			if( feof( file ) )
			{
				break;
			}

			if( strcmp( buf,"DATASUM" ) == 0 )
			{
				//���f�[�^�����[�h
				fscanf( file,"%d",&dataSum );

				//�z��쐬
				uv_info = new UvInfo[ dataSum ];
			}
			else if( strcmp( buf,"TEXPATH" ) == 0 && tex != NULL )
			{
				//�e�N�X�`�����[�h
				fscanf( file,"%s",buf );


				LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

				if( FAILED( D3DXCreateTextureFromFile(device,buf,tex) ) )
				{
					//�G���[
					fclose( file );
					return -1;
				}

			}
			else if( strcmp( buf,"DATASTART" ) == 0 )//���[�v�w��ȂǓ���f�[�^�w����s��
			{
				while( 1 )
				{
					fscanf( file,"%s",buf );

					if( strcmp( buf,"FORX" ) == 0 )
					{
						int roopSum;
						fscanf( file,"%d",&roopSum );

						D3DXVECTOR2 texPos,texSize;
						fscanf( file,"%f,%f,%f,%f",&texPos.x,&texPos.y,&texSize.x,&texSize.y );

						for( int i = 0 ; i < roopSum ; i++ )
						{
							uv_info[ idx ].uv   = texPos;
							uv_info[ idx ].size = texSize;

							texPos.x += texSize.x;

							idx++;
						}
					}
					else if( strcmp( buf,"DATA" ) == 0 )
					{
						D3DXVECTOR2 texPos,texSize;
						fscanf( file,"%f,%f,%f,%f",&texPos.x,&texPos.y,&texSize.x,&texSize.y );

						uv_info[ idx ].uv   = texPos;
						uv_info[ idx ].size = texSize;

						idx++;
					}
					else if( strcmp( buf,"DATAEND" ) == 0 )
					{
						break;
					}
				}
			}
		}
	
			fclose( file );
			return dataSum;
	}

	int LoadTexSheetFromBin ( const char *file_path, UvInfo *& uv_info, LPDIRECT3DTEXTURE9 *tex  )
	{
		FILE *file = NULL;

		file = fopen( file_path,"rb" );

		if( file == NULL )
		{
			fclose( file );
			return -1;
		}

		int  len;
		char *texPath;

		fread( &len,sizeof( int ),1,file );

		if( len >0)
		{
			texPath = new char[ len ];
			fread( texPath,sizeof(char),len,file );
			LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

			if( FAILED( D3DXCreateTextureFromFile(device,texPath,tex) ) )
			{
				//�G���[
				fclose( file );
				return -1;
			}
			
		}

		int dataSum = 0;
		fread( &dataSum,sizeof( int ),1,file );

		uv_info = new UvInfo[ dataSum ];

		for( int i = 0 ; i < dataSum ; i++ )
		{
			fread( &uv_info[i].uv.x,sizeof(float),1,file );
			fread( &uv_info[i].uv.y,sizeof(float),1,file );
			fread( &uv_info[i].size.x,sizeof(float),1,file );
			fread( &uv_info[i].size.y,sizeof(float),1,file );
		}

		fclose( file );
		return dataSum;
	}
}
