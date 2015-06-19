/*-----------------------------------------------------------------------------
	�C���X�^���V���O(2D)
	author	HaradaYuto
	since	20150512
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/

#include "CInstancingObject.h"
#include "CManager.h"//�f�o�C�X�p
#include "Define.h"//�E�B���h�E�T�C�Y�擾�p

/*-----------------------------------------------------------------------------
	Init
-----------------------------------------------------------------------------*/

HRESULT CInstancingObject::Init( void )
{
	LPD3DXBUFFER code,err = NULL;

	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

	//Init�O�Ƀe�N�X�`�������[�h����Ă��Ȃ�������
	if( m_pTex == NULL )
	{
		MessageBox(NULL,"�e�N�X�`�������[�h����Ă��܂���","�G���[",MB_OK);
	}

	//�V�F�[�_�[����

	//�o�[�e�b�N�X�V�F�[�_�[
	D3DXCompileShaderFromFile( "InstancingShader2D.sh",NULL,NULL,"vsMain","vs_2_0",0,&code,&err,&m_pVsConstantTable);

	if( err != NULL )
	{
		MessageBox(NULL,(LPSTR)err->GetBufferPointer(),"�G���[",MB_OK);
		err->Release();
		err = NULL;
		return E_FAIL;
	}

	device->CreateVertexShader((DWORD*)code->GetBufferPointer(),&m_pVertexShader);
	code->Release();

	//�s�N�Z���V�F�[�_�[
	D3DXCompileShaderFromFile( "InstancingShader2D.sh",NULL,NULL,"psMain","ps_2_0",0,&code,NULL,&m_pPsConstantTable);

	if( err != NULL )
	{
		MessageBox(NULL,(LPSTR)err->GetBufferPointer(),"�G���[",MB_OK);
		err->Release();
		err = NULL;

		return E_FAIL;
	}

	device->CreatePixelShader((DWORD*)code->GetBufferPointer(),&m_pPixelShader);
	code->Release();

	//�f�[�^�\����`
	//���_�v�f�z������
	D3DVERTEXELEMENT9 elem[] ={
		{0,0,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION, 0},
		{0,8,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR,0},
		{0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
		{1,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1},
		{1,16,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2},
		{1,32,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,3},
		{1,48,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,4},
		{1,64,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,5},
		D3DDECL_END()
	};

	//�쐬
	if( FAILED(device->CreateVertexDeclaration( elem,&m_pDecl ) ) )
	{
		return E_FAIL;
	}

	//��{�}�`�쐬
	if( FAILED( device->CreateVertexBuffer(sizeof(Vertex2DIns)*4,0,0,D3DPOOL_MANAGED,&m_pBasePoly,NULL ) ) )
	{
		return E_FAIL;
	}

	//��{���
	Vertex2DIns *vtx = NULL;

	//���b�N
	m_pBasePoly->Lock(0,0,(void**)&vtx,0 );

	vtx[0].m_pos = D3DXVECTOR2( -0.5,0.5 );
	vtx[0].m_uv  = D3DXVECTOR2( 0,0 );
	vtx[0].m_col = D3DXCOLOR(1,1,1,1);

	vtx[1].m_pos = D3DXVECTOR2( 0.5,0.5 );
	vtx[1].m_col = D3DXCOLOR(1,1,1,1);
	vtx[1].m_uv  = D3DXVECTOR2( m_oneTexSize.x/m_texSize.x,0 );

	vtx[2].m_pos = D3DXVECTOR2( -0.5,-0.5 );
	vtx[2].m_col = D3DXCOLOR(1,1,1,1);
	vtx[2].m_uv  = D3DXVECTOR2( 0,m_oneTexSize.y/m_texSize.y );

	vtx[3].m_pos = D3DXVECTOR2( 0.5,-0.5 );
	vtx[3].m_col = D3DXCOLOR(1,1,1,1);
	vtx[3].m_uv  = D3DXVECTOR2( m_oneTexSize.x/m_texSize.x,m_oneTexSize.y/m_texSize.y );

	//�A�����b�N
	m_pBasePoly->Unlock();

	//�C���X�^���V���O�p�o�b�t�@�쐬
	if( FAILED( device->CreateVertexBuffer( sizeof( InsBuff ) * ARRAY_MAX, 0,0,D3DPOOL_MANAGED,&m_pInsBuff,NULL ) ) )
	{
		return E_FAIL;
	}

	//index�o�b�t�@
	if( FAILED( device->CreateIndexBuffer( sizeof(WORD)*4,
										 D3DUSAGE_WRITEONLY,
										 D3DFMT_INDEX16,
										 D3DPOOL_MANAGED,
										 &m_pIndexBuff,
										 NULL) ) )
	{
		return E_FAIL;
	}

	WORD *idx;

	m_pIndexBuff->Lock( 0,0,(void**)&idx,0 );

	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;
	idx[3] = 3;

	m_pIndexBuff->Unlock();

	return S_OK;
}

/*-----------------------------------------------------------------------------
	Uninit
-----------------------------------------------------------------------------*/

void CInstancingObject::Uninit()
{
	//�f�X�t���O�𗧂Ă�֐�
	Release();

	if( m_pVertexShader != NULL )
	{
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
	}

	if( m_pPixelShader != NULL )
	{
		m_pPixelShader->Release();
		m_pPixelShader = NULL;
	}

	if( m_pVsConstantTable != NULL )
	{
		m_pVsConstantTable->Release();
		m_pVsConstantTable = NULL;
	}

	if( m_pPsConstantTable != NULL )
	{
		m_pPsConstantTable->Release();
		m_pPsConstantTable = NULL;
	}

	if( m_pBasePoly != NULL )
	{
		m_pBasePoly->Release();
		m_pBasePoly = NULL;
	}

	if( m_pInsBuff != NULL )
	{
		m_pInsBuff->Release();
		m_pInsBuff = NULL;
	}

	if( m_pIndexBuff != NULL )
	{
		m_pIndexBuff->Release();
		m_pIndexBuff = NULL;
	}

	if( m_pDecl != NULL )
	{
		m_pDecl->Release();
		m_pDecl = NULL;
	}

	if( m_pTex != NULL )
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

/*-----------------------------------------------------------------------------
	Update
-----------------------------------------------------------------------------*/

void CInstancingObject::Update()
{

}

/*-----------------------------------------------------------------------------
	Draw
-----------------------------------------------------------------------------*/

void CInstancingObject::Draw()
{

	if( m_arrayCursol != 0)
	{
		LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

		//�C���X�^���V���O�p�o�b�t�@�ɏ�������
		ApplyBuffer();

		//�X�g���[���ւ̃f�[�^������H
		device->SetStreamSourceFreq( 0,D3DSTREAMSOURCE_INDEXEDDATA | m_arrayCursol );
		device->SetStreamSourceFreq( 1,D3DSTREAMSOURCE_INSTANCEDATA | 1 );
		device->SetVertexDeclaration( m_pDecl );

		//�X�g���[���ɗ���
		device->SetStreamSource( 0,m_pBasePoly,0,sizeof( Vertex2DIns ) );
		device->SetStreamSource( 1,m_pInsBuff,0,sizeof( InsBuff ) );

		//�V�F�[�_�[�Z�b�g

		HRESULT test;
		test = device->SetVertexShader( m_pVertexShader );
		test = device->SetPixelShader(  m_pPixelShader  );


		//�e�N�X�`���̃Z�b�g
		device->SetSamplerState( m_pPsConstantTable->GetSamplerIndex( "samp" ),D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		device->SetSamplerState( m_pPsConstantTable->GetSamplerIndex( "samp" ), D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
		device->SetSamplerState( m_pPsConstantTable->GetSamplerIndex( "samp" ), D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
		device->SetSamplerState( m_pPsConstantTable->GetSamplerIndex( "samp" ), D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�
	
		device->SetTexture( m_pPsConstantTable->GetSamplerIndex( "samp" ),m_pTex );



		D3DXMATRIX proj;

		proj = D3DXMATRIX((float)(2 / (float)SCREEN_WIDTH), 0.0f, 0.0f, 0.0f,
							 0.0f,				(float)(2 / (float)SCREEN_HEIGHT),	0.0f, 0.0f,
							 0.0f,				0.0f,				1.0, 0.0f,
							 0.0f,				0.0f,				0, 1.0f);
						 
		//�萔�Z�b�g
		//2D���ˉe�s��
		m_pVsConstantTable->SetMatrix( device,"mtx_proj",&proj );

		//�`��
		device->SetIndices( m_pIndexBuff );
		device->DrawIndexedPrimitive(  D3DPT_TRIANGLESTRIP,0,0,4,0,2 );

		//�V�F�[�_�[����
		device->SetVertexShader( NULL );
		device->SetPixelShader(  NULL );

		//�㏈��
		device->SetStreamSourceFreq( 0,1 );
		device->SetStreamSourceFreq( 1,1 );

		//�J�[�\���������i�܂�o�b�t�@��������
		m_arrayCursol = 0;
	}
}

/*-----------------------------------------------------------------------------
	�C���X�^���V���O�p�o�b�t�@�ɏ������݂���
-----------------------------------------------------------------------------*/

void CInstancingObject::ApplyBuffer()
{
	InsBuff *vtx = NULL;

	//���b�N
	m_pInsBuff->Lock( 0,0,(void**)&vtx,0 );

	//���݈˗�����Ă��镪�̃f�[�^���l�߂�
	for( unsigned int i = 0 ; i < m_arrayCursol ; i++ )
	{
		D3DXMATRIX mtxWorld,mtxScl,mtxRot,mtxTrans;

		//���[���h������
		D3DXMatrixIdentity( &mtxWorld );

		D3DXVECTOR2 scl = m_insBuffInfo[ i ].m_scl;
		D3DXVECTOR3 rot = m_insBuffInfo[ i ].m_rot;
		D3DXVECTOR2 pos = m_insBuffInfo[ i ].m_pos;
		//�X�P�[�����O
		D3DXMatrixScaling( &mtxScl,scl.x,scl.y,1 );

		//��]
		D3DXMatrixRotationYawPitchRoll( &mtxRot,rot.y,rot.x,rot.z );

		//�ړ�
		D3DXMatrixTranslation( &mtxTrans,pos.x,pos.y,0 );

		//�����i�|�����킹)
		D3DXMatrixMultiply( &mtxWorld,&mtxWorld,&mtxScl );
		D3DXMatrixMultiply( &mtxWorld,&mtxWorld,&mtxRot );
		D3DXMatrixMultiply( &mtxWorld,&mtxWorld,&mtxTrans );

		//�s����߂�
		vtx[i].m_worldMtx[0].x=  mtxWorld._11;
		vtx[i].m_worldMtx[0].y = mtxWorld._12;
		vtx[i].m_worldMtx[0].z = mtxWorld._13;
		vtx[i].m_worldMtx[0].w = mtxWorld._14;

		vtx[i].m_worldMtx[1].x = mtxWorld._21;
		vtx[i].m_worldMtx[1].y = mtxWorld._22;
		vtx[i].m_worldMtx[1].z=  mtxWorld._23;
		vtx[i].m_worldMtx[1].w = mtxWorld._24;

		vtx[i].m_worldMtx[2].x = mtxWorld._31;
		vtx[i].m_worldMtx[2].y = mtxWorld._32;
		vtx[i].m_worldMtx[2].z = mtxWorld._33;
		vtx[i].m_worldMtx[2].w = mtxWorld._34;

		vtx[i].m_worldMtx[3].x = mtxWorld._41;
		vtx[i].m_worldMtx[3].y = mtxWorld._42;
		vtx[i].m_worldMtx[3].z = mtxWorld._43;
		vtx[i].m_worldMtx[3].w = mtxWorld._44;


		//�e�N�X�`���̃|���S���̍���Uv���W��ۑ�
		vtx[i].m_texPos   = m_insBuffInfo[ i ].m_uv_pos;
	}

	//�A�����b�N
	m_pInsBuff->Unlock();
}

/*-----------------------------------------------------------------------------
	�C���X�X�^���V���O�p�f�[�^��ǉ�����
-----------------------------------------------------------------------------*/

bool CInstancingObject::AddData(const D3DXVECTOR2 &scl,const D3DXVECTOR3 &rot,const D3DXVECTOR2 &pos,
									const int tex_id_x,const int tex_id_y )
{
	//�o�b�t�@�̍ő傾�����ꍇ�͂Ȃɂ����܂���
	if( m_arrayCursol >= ARRAY_MAX )
	{
		return false;
	}

	//�f�[�^�l�߂�
	m_insBuffInfo[ m_arrayCursol ].m_scl = scl;
	m_insBuffInfo[ m_arrayCursol ].m_rot = rot;
	m_insBuffInfo[ m_arrayCursol ].m_pos = pos;

	
	//
	m_insBuffInfo[ m_arrayCursol ].m_uv_pos.x = (m_oneTexSize.x*tex_id_x) / m_texSize.x;
	m_insBuffInfo[ m_arrayCursol ].m_uv_pos.y = (m_oneTexSize.y*tex_id_y) / m_texSize.y;

	//�J�[�\�����Z���ꂢ�����ɒl����Ă��_��
	m_arrayCursol++;
	return true;
}

/*-----------------------------------------------------------------------------
	�C���X�X�^���V���O�p�f�[�^��ǉ�����
-----------------------------------------------------------------------------*/

bool CInstancingObject::AddDataScreen(const D3DXVECTOR2 &scl,const D3DXVECTOR3 &rot,const D3DXVECTOR2 &pos,
									const int tex_id_x,const int tex_id_y )
{
	//�o�b�t�@�̍ő傾�����ꍇ�͂Ȃɂ����܂���
	if( m_arrayCursol >= ARRAY_MAX )
	{
		return false;
	}

	D3DXVECTOR2 screen_pos;

	//�X�N���[�����W�n�ɃI�t�Z�b�g
	screen_pos.x = pos.x - (float)SCREEN_WIDTH/2;
	screen_pos.y = pos.y*-1 + (float)SCREEN_HEIGHT/2;

	//�f�[�^�l�߂�
	m_insBuffInfo[ m_arrayCursol ].m_scl = scl;
	m_insBuffInfo[ m_arrayCursol ].m_rot = rot;
	m_insBuffInfo[ m_arrayCursol ].m_pos = screen_pos;

	
	m_insBuffInfo[ m_arrayCursol ].m_uv_pos.x = ( m_oneTexSize.x*tex_id_x ) / m_texSize.x;
	m_insBuffInfo[ m_arrayCursol ].m_uv_pos.y = ( m_oneTexSize.y*tex_id_y ) / m_texSize.y;

	//�J�[�\�����Z���ꂢ�����ɒl����Ă��_��
	m_arrayCursol++;
	return true;
}

/*-----------------------------------------------------------------------------
	�e�N�X�`�������[�h����
-----------------------------------------------------------------------------*/

HRESULT CInstancingObject::LoadTexture( const char *file_path,const D3DXVECTOR2 &tex_size,const D3DXVECTOR2 &tex_one_size )
{
	//�ʂ�NULL�`�F�b�N�Ƃ��͂��܂���
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

	if( FAILED( D3DXCreateTextureFromFile(device,file_path,&m_pTex) ) )
	//�e�N�X�`���ǂݍ���
	{
		return E_FAIL;
	}
	m_texSize = tex_size;
	m_oneTexSize = tex_one_size;

	return S_OK;
}