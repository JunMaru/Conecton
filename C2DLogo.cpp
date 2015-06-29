/*-------------------------------------------------------------------------------------------------------------
2Dロゴクラス
author	HaradaYuto
20150629
--------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------
include
--------------------------------------------------------------------------------------------------------------*/

#include"C2DLogo.h"
#include "CManager.h"
#include "Define.h"

/*--------------------------------------------------------------------------------------------------------------
	Init
---------------------------------------------------------------------------------------------------------------*/

HRESULT C2DLogo::Init()
{	
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

	//頂点作成
	device->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *vtx;

	m_pVtxBuff->Lock(0, 0, (void**)&vtx, 0);

	//カラー　おそらく固定
	vtx[0].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);
	vtx[1].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);
	vtx[2].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);
	vtx[3].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);


	//これも固定
	vtx[0].rhw = 1.0f;
	vtx[1].rhw = 1.0f;
	vtx[2].rhw = 1.0f;
	vtx[3].rhw = 1.0f;


	vtx[0].tex = D3DXVECTOR2(0, 0);
	vtx[1].tex = D3DXVECTOR2(1, 0);
	vtx[2].tex = D3DXVECTOR2(0, 1);
	vtx[3].tex = D3DXVECTOR2(1, 1);

	m_pVtxBuff->Unlock();

	m_pos = D3DXVECTOR2(0, 0);
	m_rot = D3DXVECTOR3(0, 0,0);
	m_scl = D3DXVECTOR2(0, 0);

	return S_OK;
}

/*--------------------------------------------------------------------------------------------------------------
	Uninit
---------------------------------------------------------------------------------------------------------------*/

void C2DLogo::Uninit()
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

/*--------------------------------------------------------------------------------------------------------------
	Update
---------------------------------------------------------------------------------------------------------------*/

void C2DLogo::Update()
{
	//	アニメーション部
	if (m_bAnim)
	{
		switch ( m_animIdx )
		{
			//スケーリングアニメーション
			case 0:

				m_scl = m_animScl * m_animNow;

				m_animNow += m_animSpd;

				if (m_animNow > 1 || m_animNow < 0 )
				{
					if (m_bAnimRoop)
					{
						m_animSpd *= -1;
					}
					else
					{
						m_bAnim = false;
					}
				}

				break;

			default:
				break;
		}
	}
}

/*--------------------------------------------------------------------------------------------------------------
	Draw
---------------------------------------------------------------------------------------------------------------*/

void C2DLogo::Draw()
{
	
	D3DXMATRIX mtx_world, mtx_scl, mtx_rot, mtx_trans;
	VERTEX_2D *vtx;
	D3DXVECTOR3 pos[4];
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();


	//下地描画
	pos[0] = D3DXVECTOR3(-0.5, -0.5, 0);
	pos[1] = D3DXVECTOR3(0.5, -0.5, 0);
	pos[2] = D3DXVECTOR3(-0.5, 0.5, 0);
	pos[3] = D3DXVECTOR3(0.5, 0.5, 0);

	D3DXMatrixIdentity(&mtx_world);

	D3DXMatrixScaling(&mtx_scl, m_scl.x, m_scl.y, 0);
	D3DXMatrixTranslation(&mtx_trans, m_pos.x, m_pos.y, 0);

	D3DXMatrixMultiply(&mtx_world, &mtx_world, &mtx_scl);
	D3DXMatrixMultiply(&mtx_world, &mtx_world, &mtx_trans);

	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mtx_world);
	}

	m_pVtxBuff->Lock(0, 0, (void**)&vtx, 0);

	vtx[0].vtx = pos[0];
	vtx[1].vtx = pos[1];
	vtx[2].vtx = pos[2];
	vtx[3].vtx = pos[3];

	m_pVtxBuff->Unlock();

	device->SetFVF(FVF_VERTEX_2D);
	device->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	device->SetTexture(0, m_pTex);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
}

/*--------------------------------------------------------------------------------------------------------------
	スケーリングアニメーションスタート
---------------------------------------------------------------------------------------------------------------*/

void C2DLogo::StartSclAnimation(const bool b_roop, const D3DXVECTOR2 tar_scl, float anim_spd)
{
	m_bAnim = true;
	m_bAnimRoop = b_roop;
	m_animScl = tar_scl;
	m_animSpd = anim_spd;
	m_animNow = 0;
	m_animIdx = 0;
}

/*--------------------------------------------------------------------------------------------------------------
	ロードテクスチャ
---------------------------------------------------------------------------------------------------------------*/

void C2DLogo::LoadTexture(const char *pfile_path)
{
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(device, pfile_path, &m_pTex);
}