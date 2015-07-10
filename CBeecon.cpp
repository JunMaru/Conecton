/*-----------------------------------------------------------------------------
	ビーコンクラス
	author	HaradaYuto
	since	20150512
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	警告対策
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Define.h"
#include "CBeecon.h"
#include "CScrollManager.h"
#include < stdio.h >

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
static const int BEECON_ANIMATIONINFO_NUM = (3);
static const float BEECON_EIGHTMOVE_RADIUS = (1.5f);
static const float BEECON_EIGHTMOVE_SPEED = (0.1f);

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/

HRESULT CBeecon::Init()
{
	TemporaryInit();

	return S_OK;
}

/*-----------------------------------------------------------------------------
	後処理
-----------------------------------------------------------------------------*/
void CBeecon::Uninit()
{
	TemporaryUninit();

	Release();
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CBeecon::Update()
{
	UpdateEightMove();

	m_prevPos = m_pos;

	D3DXVECTOR2 diff;

	diff = m_tarPos - m_pos;

	m_fPosRadius += D3DX_PI * 0.03f;

	if (m_fPosRadius >= D3DX_PI)
	{
		m_fPosRadius = -D3DX_PI;
	}

	m_pos.x += diff.x * 0.1f;
	m_pos.y += diff.y * 0.1f;

	// 画面外判定
	const bool bLeftDispOut = (m_pos.x < 0.0f + m_scl.x / 2);
	const bool bRightDispOut = (m_pos.x > SCREEN_WIDTH - m_scl.x / 2);

	if (bLeftDispOut)
	{
		m_pos.x = 0.0f + m_scl.x / 2;
		m_tarPos.x = m_prevPos.x;
	}
	if (bRightDispOut)
	{
		m_pos.x = SCREEN_WIDTH - m_scl.x / 2;
		m_tarPos.x = m_prevPos.x;
	}

	//とりあえずスクロール

	D3DXVECTOR2 sc;
	sc.x = 0.0f;//-SCREEN_WIDTH/2+m_pos.x;//(m_pos.x - m_prevPos.x);
	sc.y = -SCREEN_HEIGHT / 2 + m_pos.y;

	CScrollManager::SetScrollWorld(sc);

	TemporaryUpdate();
}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/
void CBeecon::Draw()
{
	TemporaryDraw();
}

void CBeecon::CommandRightMove()
{
	m_bDirectionRight = true;
	m_tarPos.x += m_spd;
	m_bMoveOperated = true;
}

void CBeecon::CommandLeftMove()
{
	m_bDirectionRight = false;
	m_tarPos.x -= m_spd;
	m_bMoveOperated = true;
}

void CBeecon::CommandUpMove()
{
	m_tarPos.y -= m_spd;
	m_bMoveOperated = true;
}

void CBeecon::CommandDownMove()
{
	m_tarPos.y += m_spd;
	m_bMoveOperated = true;
}

void CBeecon::TemporaryInit()
{
	TexLoader::LoadTexSheetFromBin("data/texture_info/BeeconTexInfo.bin", m_pTexInfoArray, &m_pTex);
	m_action = ACTION_WALK;

	InitAnimaton();

	LPDIRECT3DDEVICE9 device = CManager::GetRenderer() -> GetDevice();

	device -> CreateVertexBuffer( sizeof( VERTEX_2D ) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL );

	VERTEX_2D *pVtx;

	m_pVtxBuff -> Lock( 0, 0, ( void** )&pVtx, 0 );

	//カラー　おそらく固定
	pVtx[ 0 ].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	pVtx[ 1 ].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	pVtx[ 2 ].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	pVtx[ 3 ].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );

	//これも固定
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
	m_eightMovePos = VEC2_ZERO;
	m_fEightMoveCount = -D3DX_PI;
	m_bEightMoveUpsideCircle = true;
	m_bMoveOperated = false;
	m_blockID = BLOCKID_NONE;
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

	for ( int nCnt = 0; nCnt < BEECON_ANIMATIONINFO_NUM; nCnt++ )
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

	if ((m_animCnt / m_animWait) >= m_animSum)
	{
		m_animCnt = 0;

		switch (m_action)
		{
			case CBeecon::ACTION_WALK:
				break;
			case CBeecon::ACTION_CONNECT:
				SetAction(ACTION_WALK);
				break;
			case CBeecon::ACTION_WARP:
				SetAction(ACTION_WALK);
				break;
			case CBeecon::ACTION_MAX:
				break;
			default:
				break;
		}
	}

	//とりあえず
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

	//初期化
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

/*-----------------------------------------------------------------------------
	アニメーション初期化処理
-----------------------------------------------------------------------------*/
void CBeecon::InitAnimaton(void)
{
	// アニメーション設定テーブル
	const int anTexIdArrayNumTable[] = { 8, 8, 8, };
	const bool abLoopTable[] = { true, false, false, };
	const int anWaitTimeTable[] = { 3, 3, 3, };
	int nTexIdCount = 0;

	// アニメーション情報の設定
	m_animSet = new AnimationInfo[BEECON_ANIMATIONINFO_NUM];

	for (int nAnimSetIdx = 0; nAnimSetIdx < BEECON_ANIMATIONINFO_NUM; ++nAnimSetIdx)
	{
		m_animSet[nAnimSetIdx].animSum = anTexIdArrayNumTable[nAnimSetIdx];
		m_animSet[nAnimSetIdx].animWait = anWaitTimeTable[nAnimSetIdx];
		m_animSet[nAnimSetIdx].bRoop = abLoopTable[nAnimSetIdx];
		m_animSet[nAnimSetIdx].texIdArray = new int[anTexIdArrayNumTable[nAnimSetIdx]];

		for (int nTexIdArrayIdx = 0; nTexIdArrayIdx < anTexIdArrayNumTable[nAnimSetIdx]; ++nTexIdArrayIdx)
		{
			m_animSet[nAnimSetIdx].texIdArray[nTexIdArrayIdx] = nTexIdCount;
			++nTexIdCount;
		}
	}
}

/*-----------------------------------------------------------------------------
	アニメーション参照値再設定処理
-----------------------------------------------------------------------------*/
void CBeecon::ResetSelectAnimetionIndex(void)
{
	m_selectAnimIdx = m_action;
	m_animCnt = 0;
}

/*-----------------------------------------------------------------------------
	8の字更新処理
-----------------------------------------------------------------------------*/
void CBeecon::UpdateEightMove(void)
{
	// ユーザーが移動を命令している場合
	if (m_bMoveOperated)
	{
		return;
	}

	m_fEightMoveCount += BEECON_EIGHTMOVE_SPEED;

	// 八の字の上側下側切替(floatなので%使うのが怖い為フラグで代用)
	if (m_fEightMoveCount > D3DX_PI)
	{
		m_fEightMoveCount = -D3DX_PI;
		m_bEightMoveUpsideCircle = !m_bEightMoveUpsideCircle;
	}

	// 上側時
	if (m_bEightMoveUpsideCircle)
	{
		m_eightMovePos.x = sinf(m_fEightMoveCount) * BEECON_EIGHTMOVE_RADIUS;
		m_eightMovePos.y = -cosf(m_fEightMoveCount) * BEECON_EIGHTMOVE_RADIUS;
		m_pos += m_eightMovePos;
		m_pos.y -= BEECON_EIGHTMOVE_RADIUS;

		return;
	}

	// 下側時
	m_eightMovePos.x = sinf(m_fEightMoveCount) * BEECON_EIGHTMOVE_RADIUS;
	m_eightMovePos.y = cosf(m_fEightMoveCount) * BEECON_EIGHTMOVE_RADIUS;
	m_pos += m_eightMovePos;
	m_pos.y += BEECON_EIGHTMOVE_RADIUS;
}

// End of file