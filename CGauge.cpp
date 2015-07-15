
#include "CGauge.h"
#include "CManager.h"

/*--------------------------------------------------------------------------------------
	Init
----------------------------------------------------------------------------------------*/

HRESULT CGauge::Init()
{
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

	m_pos = D3DXVECTOR2( 0,0 );
	m_rot = D3DXVECTOR3(0,0,0);
	m_scl = D3DXVECTOR2( 0,0 );

	for( int i = 0 ; i < MAX_TEX ; i++ )
	{
		m_pTex[i] = NULL;
	}

	return S_OK;
}

/*--------------------------------------------------------------------------------------
	Uninit
----------------------------------------------------------------------------------------*/

void CGauge::Uninit()
{
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	for( int i = 0 ; i < MAX_TEX ; i++ )
	{
		if( m_pTex[i] != NULL )
		{
			m_pTex[i]->Release();
			m_pTex[i] = NULL;
		}
	}

	Release();
}

/*--------------------------------------------------------------------------------------
	Update
----------------------------------------------------------------------------------------*/

void CGauge::Update()
{
	//ゲージ割合計算
	float k = ( m_gaugeVal / m_gaugeBaseVal );

	if( k > 1 )
	{
		k = 1;
	}

	m_gaugeSclY = m_scl.y * k;

}

/*--------------------------------------------------------------------------------------
	Draw
----------------------------------------------------------------------------------------*/

void CGauge::Draw()
{
	D3DXMATRIX mtx_world,mtx_scl,mtx_rot,mtx_trans;
	VERTEX_2D *vtx;
	D3DXVECTOR3 pos[4];
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

	
	//下地描画
	pos[ 0 ] = D3DXVECTOR3( -0.5, -0.5,0 );
	pos[ 1 ] = D3DXVECTOR3(  0.5, -0.5,0 );
	pos[ 2 ] = D3DXVECTOR3( -0.5,  0.5,0 );
	pos[ 3 ] = D3DXVECTOR3(  0.5,  0.5,0 );

	D3DXMatrixIdentity( &mtx_world );

	D3DXMatrixScaling( &mtx_scl,m_scl.x,m_scl.y,0 );
	D3DXMatrixTranslation( &mtx_trans,m_pos.x,m_pos.y,0 );

	D3DXMatrixMultiply( &mtx_world,&mtx_world,&mtx_scl );
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

	m_pVtxBuff->Unlock();

	device->SetFVF( FVF_VERTEX_2D );
	device->SetStreamSource( 0,m_pVtxBuff,0,sizeof( VERTEX_2D ) );
	device->SetTexture( 0,m_pTex[0] );
	device->DrawPrimitive( D3DPT_TRIANGLESTRIP,0,2 );
	

	//マスク
	pos[ 0 ] = D3DXVECTOR3( -0.5, -0.5,0 );
	pos[ 1 ] = D3DXVECTOR3(  0.5, -0.5,0 );
	pos[ 2 ] = D3DXVECTOR3( -0.5,  0.5,0 );
	pos[ 3 ] = D3DXVECTOR3(  0.5,  0.5,0 );
	
	D3DXMatrixScaling( &mtx_scl,m_scl.x,m_scl.y,0 );
	D3DXMatrixTranslation( &mtx_trans,m_pos.x,m_pos.y,0 );

	//初期化
	D3DXMatrixIdentity( &mtx_world );

	D3DXMatrixMultiply( &mtx_world,&mtx_world,&mtx_scl );
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

	m_pVtxBuff->Unlock();

	device->SetFVF( FVF_VERTEX_2D );
	device->SetStreamSource( 0,m_pVtxBuff,0,sizeof( VERTEX_2D ) );

	//ステンシルバッファ有効
	device->SetRenderState( D3DRS_STENCILENABLE,true );
	//Ｚバッファへの書き込み無効
	device->SetRenderState( D3DRS_ZWRITEENABLE,false );
	//カラーバッファ無効
	device->SetRenderState( D3DRS_COLORWRITEENABLE,0x0);
	//ステンシルバッファ　常に描画
	device->SetRenderState( D3DRS_STENCILFUNC,D3DCMP_ALWAYS );
	//描画できるなら+1する
	device->SetRenderState( D3DRS_STENCILPASS,D3DSTENCILOP_INCR );

	device->SetRenderState( D3DRS_ALPHATESTENABLE,true );
	device->SetRenderState( D3DRS_ALPHAREF, 0 );
	device->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );

	device->SetTexture( 0,m_pTex[0] );
	device->DrawPrimitive( D3DPT_TRIANGLESTRIP,0,2 );

	device->SetRenderState( D3DRS_ALPHATESTENABLE,false );

	//参照値を１にする
	device->SetRenderState( D3DRS_STENCILREF,0x01 );
	//ステンシルバッファの値が　= 1で比較
	device->SetRenderState( D3DRS_STENCILFUNC,D3DCMP_EQUAL );

	//カラーバッファ有効
	device->SetRenderState( D3DRS_COLORWRITEENABLE,0xf );

	//Zバッファ有効化
	device->SetRenderState( D3DRS_ZWRITEENABLE,true );
	//ゲージ描画
	//ここだけ行列計算しない
	pos[0] = D3DXVECTOR3( m_pos.x - m_scl.x/2,m_pos.y - m_gaugeSclY + m_scl.y/2,0 );
	pos[1] = D3DXVECTOR3( m_pos.x + m_scl.x/2,m_pos.y - m_gaugeSclY + m_scl.y/2,0 );
	pos[2] = D3DXVECTOR3( m_pos.x - m_scl.x/2,m_pos.y + m_scl.y/2,0 );
	pos[3] = D3DXVECTOR3( m_pos.x + m_scl.x/2,m_pos.y + m_scl.y/2,0 );

	m_pVtxBuff->Lock( 0,0,(void**)&vtx,0 );

	vtx[0].vtx = pos[0];
	vtx[1].vtx = pos[1];
	vtx[2].vtx = pos[2];
	vtx[3].vtx = pos[3];
	

	m_pVtxBuff->Unlock();

	device->SetFVF( FVF_VERTEX_2D );
	device->SetStreamSource( 0,m_pVtxBuff,0,sizeof( VERTEX_2D ) );
	device->SetTexture( 0,m_pTex[1] );
	device->DrawPrimitive( D3DPT_TRIANGLESTRIP,0,2 );

	//ステンシルバッファ無効化
	device->SetRenderState( D3DRS_STENCILENABLE,false );



	//枠描画
	pos[ 0 ] = D3DXVECTOR3( -0.5, -0.5,0 );
	pos[ 1 ] = D3DXVECTOR3(  0.5, -0.5,0 );
	pos[ 2 ] = D3DXVECTOR3( -0.5,  0.5,0 );
	pos[ 3 ] = D3DXVECTOR3(  0.5,  0.5,0 );

	D3DXMatrixIdentity( &mtx_world );

	D3DXMatrixScaling( &mtx_scl,m_scl.x,m_scl.y,0 );
	D3DXMatrixTranslation( &mtx_trans,m_pos.x,m_pos.y,0 );

	D3DXMatrixMultiply( &mtx_world,&mtx_world,&mtx_scl );
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

	m_pVtxBuff->Unlock();

	device->SetFVF( FVF_VERTEX_2D );
	device->SetStreamSource( 0,m_pVtxBuff,0,sizeof( VERTEX_2D ) );
	device->SetTexture( 0,m_pTex[2] );
	device->DrawPrimitive( D3DPT_TRIANGLESTRIP,0,2 );
}

/*----------------------------------------------------------------------------------
	テクスチャロード
-----------------------------------------------------------------------------------*/

void CGauge::LoadTexture( const int idx ,const char *pfile_path )
{
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

	if( idx >= 0 && idx < MAX_TEX )
	{
		D3DXCreateTextureFromFile(device,pfile_path,&m_pTex[idx]);
	}
}