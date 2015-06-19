/*-----------------------------------------------------------------------------
	�r�[�R���N���X
	author	HaradaYuto
	since	20150512
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/

#include "CBeecon.h"
#include "CScrollManager.h"
#include < stdio.h >

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/

HRESULT CBeecon::Init()
{
	TemporaryInit();

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�㏈��
-----------------------------------------------------------------------------*/
void CBeecon::Uninit()
{
	TemporaryUninit();

	Release();
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CBeecon::Update()
{
	m_prevPos = m_pos;

	D3DXVECTOR2 diff;

	diff = m_tarPos - m_pos;

	m_fPosRadius += D3DX_PI * 0.03f;

	if (m_fPosRadius >= D3DX_PI)
	{
		m_fPosRadius = -D3DX_PI;
	}

	m_pos.x += diff.x * 0.1f;

	TemporaryUpdate();
}

/*-----------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------*/
void CBeecon::Draw()
{
	TemporaryDraw();
}

/*-----------------------------------------------------------------------------
	�e�N�X�`�����[�h
-----------------------------------------------------------------------------*/
HRESULT CBeecon::LoadTexture(const char *file_path)
{
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer() -> GetDevice();

	if ( FAILED( D3DXCreateTextureFromFile( device, file_path, &m_pTex ) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�e�N�X�`�����ǂݍ���
-----------------------------------------------------------------------------*/
bool CBeecon::LoadTextureInfoFromText( const char *file_path )
{
	FILE *file;
	char buf[ 256 ];
	int dataSum = 0;
	int idx = 0;

	file = fopen( file_path, "rt" );

	if ( file == NULL )
	{
		return false;
	}

	while ( 1 )
	{
		// �����񃍁[�h
		fscanf( file, "%s", buf );

		if ( feof( file ) )
		{
			break;
		}

		// ���f�[�^�����[�h
		if ( strcmp( buf, "DATASUM" ) == 0 )
		{
			fscanf( file, "%d", &dataSum );

			// �w�肳�ꂽ���̔z��쐬
			m_pTexInfoArray = new TexInfo[ dataSum ];
		}

		// �e�N�X�`�����[�h
		else if ( strcmp( buf, "TEXPATH" ) == 0 )
		{
			fscanf( file, "%s", buf );
			LoadTexture( buf );
		}

		// �e�N�X�`���X�e�[�^�X���[�h
		else if ( strcmp( buf, "DATASTART" ) == 0 )
		{
			while ( 1 )
			{
				fscanf( file, "%s", buf );

				//���[�v�w��ȂǓ���f�[�^�w����s��
				if ( strcmp( buf, "FORX" ) == 0 )
				{
					int roopSum;
					fscanf( file, "%d", &roopSum );

					D3DXVECTOR2 texPos, texSize;
					fscanf( file, "%f,%f,%f,%f", &texPos.x, &texPos.y, &texSize.x, &texSize.y );

					for ( int i = 0; i < roopSum; i++ )
					{
						m_pTexInfoArray[ idx ].uv = texPos;
						m_pTexInfoArray[ idx ].size = texSize;

						texPos.x += texSize.x;

						idx++;
					}
				}
				else if ( strcmp( buf, "DATA" ) == 0 )
				{
					D3DXVECTOR2 texPos, texSize;
					fscanf( file, "%f,%f,%f,%f", &texPos.x, &texPos.y, &texSize.x, &texSize.y );

					m_pTexInfoArray[ idx ].uv = texPos;
					m_pTexInfoArray[ idx ].size = texSize;

					idx++;
				}
				else if ( strcmp(buf, "DATAEND" ) == 0 )
				{
					break;
				}
			}
		}
	}

	fclose( file );

	return true;
}

void CBeecon::CommandRightMove()
{
	m_bDirectionRight = true;
	m_tarPos.x += m_spd;
}

void CBeecon::CommandLeftMove()
{
	m_bDirectionRight = false;
	m_tarPos.x -= m_spd;
}

void CBeecon::TemporaryInit()
{
	// �A�j���[�V�����e�X�g
	m_animSet = new AnimationInfo;

	// ���s�A�j���[�V�����̊e��ݒ�
	m_animSet[ 0 ].animSum = 8;
	m_animSet[ 0 ].animWait = 2;
	m_animSet[ 0 ].texIdArray = new int[ 8 ];
	m_animSet[ 0 ].bRoop = true;
	m_animSet[ 0 ].texIdArray[ 0 ] = 0;
	m_animSet[ 0 ].texIdArray[ 1 ] = 1;
	m_animSet[ 0 ].texIdArray[ 2 ] = 2;
	m_animSet[ 0 ].texIdArray[ 3 ] = 3;
	m_animSet[ 0 ].texIdArray[ 4 ] = 4;
	m_animSet[ 0 ].texIdArray[ 5 ] = 5;
	m_animSet[ 0 ].texIdArray[ 6 ] = 6;
	m_animSet[ 0 ].texIdArray[ 7 ] = 7;

	LPDIRECT3DDEVICE9 device = CManager::GetRenderer() -> GetDevice();

	device -> CreateVertexBuffer( sizeof( VERTEX_2D ) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL );

	VERTEX_2D *pVtx;

	m_pVtxBuff -> Lock( 0, 0, ( void** )&pVtx, 0 );

	//�J���[�@�����炭�Œ�
	pVtx[ 0 ].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	pVtx[ 1 ].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	pVtx[ 2 ].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	pVtx[ 3 ].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );

	//������Œ�
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	pVtx[ 0 ].tex = D3DXVECTOR2( 0, 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1, 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0, 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1, 1 );

	m_pVtxBuff -> Unlock();

	m_animCnt = 0;
	m_animIdx = 0;
	m_animOffset = 0;
	m_animWait = 5;
	m_animSum = 8;
	m_selectAnimIdx = 0;
	m_fPosRadius = 0.0f;
}

void CBeecon::TemporaryUninit()
{
	if ( m_pVtxBuff != NULL )
	{
		m_pVtxBuff -> Release();
		m_pVtxBuff = NULL;
	}

	if ( m_pTex != NULL )
	{
		m_pTex -> Release();
		m_pTex = NULL;
	}

	delete[] m_pTexInfoArray;

	for ( int nCnt = 0; nCnt < 1; nCnt++ )
	{
		delete[] m_animSet[ nCnt ].texIdArray;
	}

	delete[] m_animSet;
}

void CBeecon::TemporaryUpdate()
{
	m_animSum = m_animSet[ m_selectAnimIdx ].animSum;
	m_animWait = m_animSet[ m_selectAnimIdx ].animWait;

	m_animIdx = ( int )( m_animCnt / m_animWait ) % m_animSum;

	m_texPos = m_pTexInfoArray[ m_animSet[ m_selectAnimIdx ].texIdArray[ m_animIdx ] ].uv;
	m_texSize = m_pTexInfoArray[ m_animSet[ m_selectAnimIdx ].texIdArray[ m_animIdx ] ].size;

	m_animCnt++;

	if ( ( m_animCnt / m_animSum ) >= m_animSum )
	{
		m_animCnt = 0;
	}

	//�Ƃ肠����
	VERTEX_2D *vtx;
	D3DXVECTOR2 sc_pos;
	D3DXVECTOR3 pos[ 4 ];
	D3DXMATRIX mtx_world, mtx_scl, mtx_rot, mtx_trans;

	sc_pos = m_pos - CScrollManager::GetScrollWorld();

	pos[ 0 ] = D3DXVECTOR3( -0.5, -0.5, 0 );
	pos[ 1 ] = D3DXVECTOR3(  0.5, -0.5, 0 );
	pos[ 2 ] = D3DXVECTOR3( -0.5,  0.5, 0 );
	pos[ 3 ] = D3DXVECTOR3(  0.5,  0.5, 0 );

	D3DXMatrixScaling( &mtx_scl, m_scl.x, m_scl.y, 0 );
	D3DXMatrixRotationYawPitchRoll( &mtx_rot, m_rot.y, m_rot.x, m_rot.z );
	D3DXMatrixTranslation( &mtx_trans, sc_pos.x, sc_pos.y, 0 );

	//������
	D3DXMatrixIdentity( &mtx_world );

	D3DXMatrixMultiply( &mtx_world, &mtx_world, &mtx_scl );
	D3DXMatrixMultiply( &mtx_world, &mtx_world, &mtx_rot );
	D3DXMatrixMultiply( &mtx_world, &mtx_world, &mtx_trans );

	for ( int i = 0; i < 4; i++ )
	{
		D3DXVec3TransformCoord( &pos[ i ], &pos[ i ], &mtx_world );
	}

	m_pVtxBuff -> Lock( 0, 0, ( void** )&vtx, 0 );
	vtx[ 0 ].vtx = pos[ 0 ];
	vtx[ 1 ].vtx = pos[ 1 ];
	vtx[ 2 ].vtx = pos[ 2 ];
	vtx[ 3 ].vtx = pos[ 3 ];
	if ( m_bDirectionRight )
	{
		vtx[ 0 ].tex = D3DXVECTOR2( m_texPos.x, m_texPos.y );
		vtx[ 1 ].tex = D3DXVECTOR2( m_texPos.x + m_texSize.x, m_texPos.y );
		vtx[ 2 ].tex = D3DXVECTOR2( m_texPos.x, m_texPos.y + m_texSize.y );
		vtx[ 3 ].tex = D3DXVECTOR2( m_texPos.x + m_texSize.x, m_texPos.y + m_texSize.y );
	}
	else
	{
		vtx[ 0 ].tex = D3DXVECTOR2( m_texPos.x + m_texSize.x, m_texPos.y );
		vtx[ 1 ].tex = D3DXVECTOR2( m_texPos.x, m_texPos.y );
		vtx[ 2 ].tex = D3DXVECTOR2( m_texPos.x + m_texSize.x, m_texPos.y + m_texSize.y );
		vtx[ 3 ].tex = D3DXVECTOR2( m_texPos.x, m_texPos.y + m_texSize.y );
	}

	m_pVtxBuff -> Unlock();
}

void CBeecon::TemporaryDraw()
{
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer() -> GetDevice();

	device -> SetFVF( FVF_VERTEX_2D );
	device -> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_2D ) );

	device -> SetTexture( 0, m_pTex );
	device -> DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

// End of file