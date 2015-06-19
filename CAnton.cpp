/*-----------------------------------------------------------------------------
	アントンクラス
	author	HaradaYuto
	since	20150519
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	warning対策
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CAnton.h"
#include "CManager.h"
#include "CScrollManager.h"
#include "Define.h"
#include <stdio.h>

static const float ANTON_SPEED_X = (0.01f);
static const float ANTON_MOVE_THRESHOLD = (3.0f);
													// 重さ		腕力	サイズ
static const CAnton::PARAMETER PERAMETER_TABLE[] = { { 20,		0,		D3DXVECTOR2(100.0f, 120.0f) },			// NORMAL
													{ 80,		0,		D3DXVECTOR2(100.0f, 120.0f) },			// METAL
													{ 20,		0,		D3DXVECTOR2(25.0f, 30.0f) },			// MINIMUM
													{ 20,		100,	D3DXVECTOR2(100.0f, 120.0f) }, };		// POWERFUL

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/

HRESULT CAnton::Init()
{
	this->TemporaryInit();
	return S_OK;
}

/*-----------------------------------------------------------------------------
	後処理
-----------------------------------------------------------------------------*/

void CAnton::Uninit()
{
	TemporaryUninit();
	Release();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/

void CAnton::Update()
{
	m_prevPos = m_pos;

	D3DXVECTOR2 diff;

	diff = m_tarPos - m_pos;
	//Hack とりあえず重力
	m_pos.x += diff.x * ANTON_SPEED_X;
	m_pos.y += m_parameter.nMass * 0.5f;//体重の0.1倍の落下

	if (diff.x > ANTON_MOVE_THRESHOLD)
	{
		CommandRightMove();
	}
	else if (diff.x < -ANTON_MOVE_THRESHOLD)
	{
		CommandLeftMove();
	}

	//とりあえずスクロール

	D3DXVECTOR2 sc;

	sc.x = -SCREEN_WIDTH/2+m_pos.x;//(m_pos.x - m_prevPos.x);

	sc.y = -SCREEN_HEIGHT/2+m_pos.y;;

	CScrollManager::SetScrollWorld( sc );
	
	TemporaryUpdate();
}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/

void CAnton::Draw()
{
	TemporaryDraw();
}

/*-----------------------------------------------------------------------------
	テクスチャをロード
-----------------------------------------------------------------------------*/

HRESULT CAnton::LoadTexture( const char *file_path )
{
	//別にNULLチェックとかはしません
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

	if( FAILED( D3DXCreateTextureFromFile(device,file_path,&m_pTex) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
	移動命令
-----------------------------------------------------------------------------*/


void CAnton::CommandRightMove()
{
	if( m_selectAnimIdx != 2 )
	{
		m_bDirectionRight = true;

		//m_tarPos.x += m_spd;

	
		m_selectAnimIdx = 0;
	}
}

void CAnton::CommandLeftMove()
{
	if( m_selectAnimIdx != 2 )
	{
		m_bDirectionRight = false;
		//m_tarPos.x -= m_spd;

		if(m_selectAnimIdx != 2 )
		m_selectAnimIdx = 0;
	}
}

/*-----------------------------------------------------------------------------
	変身命令
-----------------------------------------------------------------------------*/

void CAnton::CommandChangeNormal()
{
	m_mass = 20;

	m_selectAnimIdx = 1;
}

void CAnton::CommandChangeMetal()
{
	m_mass = 80;

	m_selectAnimIdx = 2;
}

/*-----------------------------------------------------------------------------
	テクスチャ情報読み込み
-----------------------------------------------------------------------------*/

bool CAnton::LoadTextureInfoFromText( const char *file_path )
{
	FILE *file;

	file = fopen( file_path,"rt" );

	if( file == NULL )
	{
		return false;
	}

	char buf[256];
	int dataSum = 0;
	int idx = 0;
	while(1)
	{
		//文字列ロード
		fscanf( file,"%s",buf );

		if( feof( file ) )
		{
			break;
		}

		if( strcmp( buf,"DATASUM" ) == 0 )
		{
			//総データ数ロード
			fscanf( file,"%d",&dataSum );

			//配列作成
			m_pTexInfoArray = new TexInfo[ dataSum ];
		}
		else if( strcmp( buf,"TEXPATH" ) == 0 )
		{
			//テクスチャロード
			fscanf( file,"%s",buf );
			LoadTexture( buf );
		}
		else if( strcmp( buf,"DATASTART" ) == 0 )//ループ指定など特殊データ指定を行う
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
						m_pTexInfoArray[ idx ].uv   = texPos;
						m_pTexInfoArray[ idx ].size = texSize;

						texPos.x += texSize.x;

						idx++;
					}
				}
				else if( strcmp( buf,"DATA" ) == 0 )
				{
					D3DXVECTOR2 texPos,texSize;
					fscanf( file,"%f,%f,%f,%f",&texPos.x,&texPos.y,&texSize.x,&texSize.y );

					m_pTexInfoArray[ idx ].uv   = texPos;
					m_pTexInfoArray[ idx ].size = texSize;

					idx++;
				}
				else if( strcmp( buf,"DATAEND" ) == 0 )
				{
					break;
				}
			}
		}
	}
	
	return true;
}

//消えるかもしれないのでコメントは最少で

void CAnton::TemporaryInit()
{
	SetState(STATE_NORMAL);

	//アニメーション管理テスト！！！
	m_animSet = new AnimationInfo[4];

	m_animSet[0].animSum = 8;
	m_animSet[0].animWait = 3;
	m_animSet[0].texIdArray = new int [8];
	m_animSet[0].bRoop = true;
	m_animSet[0].texIdArray[0] = 0;
	m_animSet[0].texIdArray[1] = 1;
	m_animSet[0].texIdArray[2] = 2;
	m_animSet[0].texIdArray[3] = 3;
	m_animSet[0].texIdArray[4] = 4;
	m_animSet[0].texIdArray[5] = 5;
	m_animSet[0].texIdArray[6] = 6;
	m_animSet[0].texIdArray[7] = 7;

	m_animSet[1].animSum = 1;
	m_animSet[1].animWait = 1;
	m_animSet[1].texIdArray = new int [1];
	m_animSet[1].bRoop = false;
	m_animSet[1].texIdArray[0] = 8;

	m_animSet[2].animSum = 1;
	m_animSet[2].animWait = 1;
	m_animSet[2].texIdArray = new int [1];
	m_animSet[2].bRoop = false;
	m_animSet[2].texIdArray[0] = 9;



	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

	//頂点作成
	device->CreateVertexBuffer( sizeof( VERTEX_2D ) * 4,D3DUSAGE_WRITEONLY,FVF_VERTEX_2D,D3DPOOL_MANAGED,&m_pVtxBuff,NULL );

	VERTEX_2D *vtx;

	m_pVtxBuff->Lock( 0,0,(void**)&vtx,0 );

	//カラー　おそらく固定
	vtx[0].diffuse = D3DCOLOR_ARGB(255,255,255,255 );
	vtx[1].diffuse = D3DCOLOR_ARGB(255,255,255,255 );
	vtx[2].diffuse = D3DCOLOR_ARGB(255,255,255,255 );
	vtx[3].diffuse = D3DCOLOR_ARGB(255,255,255,255 );

	//これも固定
	vtx[0].rhw = 1.0f;
	vtx[1].rhw = 1.0f;
	vtx[2].rhw = 1.0f;
	vtx[3].rhw = 1.0f;

	vtx[0].tex = D3DXVECTOR2( 0,0 );
	vtx[1].tex = D3DXVECTOR2( 1,0 );
	vtx[2].tex = D3DXVECTOR2( 0,1 );
	vtx[3].tex = D3DXVECTOR2( 1,1 );

	m_pVtxBuff->Unlock();

	m_animCnt = 0;
	m_animIdx = 0;
	m_animOffset = 0;
	m_animWait = 10;
	m_animSum = 8;
	m_selectAnimIdx = 0;
}

void CAnton::TemporaryUninit()
{
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if( m_pTex != NULL )
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	delete []m_pTexInfoArray;

	// 仮メモリーリーク対策
	for (int nCnt = 0; nCnt < 3; ++nCnt)
	{
		delete[] m_animSet[nCnt].texIdArray;
	}

	delete[] m_animSet;
}

	

void CAnton::TemporaryUpdate()
{

	m_animSum = m_animSet[ m_selectAnimIdx ].animSum;
	m_animWait = m_animSet[ m_selectAnimIdx ].animWait;

	m_animIdx = (int)( m_animCnt/m_animWait ) % m_animSum;

	m_texPos = m_pTexInfoArray[ m_animSet[ m_selectAnimIdx ].texIdArray[ m_animIdx ] ].uv;
	m_texSize= m_pTexInfoArray[ m_animSet[ m_selectAnimIdx ].texIdArray[ m_animIdx ] ].size;

	m_animCnt++;


	if( (m_animCnt/m_animWait) >=m_animSum )
	{
		m_animCnt = 0;
	}

	if( (abs( m_prevPos.x - m_pos.x )< 1) && m_selectAnimIdx != 2)
	{
		m_selectAnimIdx = 1;
	}


	VERTEX_2D *vtx;

	//とりあえず
	D3DXVECTOR2 sc_pos;

	sc_pos = m_pos - CScrollManager::GetScrollWorld();

	D3DXVECTOR3 pos[4];

	pos[ 0 ] = D3DXVECTOR3( -0.5, -0.5,0 );
	pos[ 1 ] = D3DXVECTOR3(  0.5, -0.5,0 );
	pos[ 2 ] = D3DXVECTOR3( -0.5,  0.5,0 );
	pos[ 3 ] = D3DXVECTOR3(  0.5,  0.5,0 );

	D3DXMATRIX mtx_world,mtx_scl,mtx_rot,mtx_trans;

	//D3DXMatrixScaling( &mtx_scl,m_scl.x,m_scl.y,0 );
	D3DXMatrixScaling(&mtx_scl, m_parameter.size.x, m_parameter.size.y, 0);
	D3DXMatrixRotationYawPitchRoll( &mtx_rot,m_rot.y,m_rot.x,m_rot.z );
	D3DXMatrixTranslation( &mtx_trans,sc_pos.x,sc_pos.y,0 );

	//初期化
	D3DXMatrixIdentity( &mtx_world );

	D3DXMatrixMultiply( &mtx_world,&mtx_world,&mtx_scl );
	D3DXMatrixMultiply( &mtx_world,&mtx_world,&mtx_rot );
	D3DXMatrixMultiply( &mtx_world,&mtx_world,&mtx_trans );

	for( int i = 0 ; i < 4 ; i++ )
	{
		D3DXVec3TransformCoord( &pos[i],&pos[i],&mtx_world );
	}

	m_pVtxBuff->Lock( 0,0,(void**)&vtx,0 );

	vtx[0].vtx = pos[0];
	vtx[1].vtx = pos[1];
	vtx[2].vtx = pos[2];
	vtx[3].vtx = pos[3];

	if( m_bDirectionRight )
	{
		vtx[0].tex = D3DXVECTOR2( m_texPos.x ,m_texPos.y );
		vtx[1].tex = D3DXVECTOR2( m_texPos.x + m_texSize.x,m_texPos.y );
		vtx[2].tex = D3DXVECTOR2( m_texPos.x ,m_texPos.y + m_texSize.y );
		vtx[3].tex = D3DXVECTOR2( m_texPos.x + m_texSize.x ,m_texPos.y + m_texSize.y );
	}
	else
	{
		vtx[0].tex = D3DXVECTOR2( m_texPos.x + m_texSize.x ,m_texPos.y );
		vtx[1].tex = D3DXVECTOR2( m_texPos.x,m_texPos.y );
		vtx[2].tex = D3DXVECTOR2( m_texPos.x + m_texSize.x ,m_texPos.y + m_texSize.y );
		vtx[3].tex = D3DXVECTOR2( m_texPos.x,m_texPos.y + m_texSize.y );
	}


	m_pVtxBuff->Unlock();
}

void CAnton::TemporaryDraw()
{
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

	device->SetFVF( FVF_VERTEX_2D );
	device->SetStreamSource( 0,m_pVtxBuff,0,sizeof( VERTEX_2D ) );

	//
	device->SetTexture( 0,m_pTex );
	device->DrawPrimitive( D3DPT_TRIANGLESTRIP,0,2 );

}

void CAnton::AnimationUpdate()
{

}

void CAnton::SetState(const CAnton::STATE state)
{
	m_state = state;
	m_parameter = PERAMETER_TABLE[m_state];

	// TODO:テクスチャが用意されたら外す
	if (m_state < 2)
	{
		m_selectAnimIdx = m_state + 1;
	}
}