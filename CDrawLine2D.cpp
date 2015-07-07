/*-----------------------------------------------------------------------------
	2D描画ライン
	author	HaradaYuto
	since	20150703
-----------------------------------------------------------------------------*/

//複数の2Dポリゴンを一括描画します。

/*----------------------------------------------------------------------------
	インクルード
-----------------------------------------------------------------------------*/

#include "CDrawLine2D.h"
#include "CManager.h"

/*----------------------------------------------------------------------------
	Init
-----------------------------------------------------------------------------*/

HRESULT CDrawLine2D::Init()
{

	return S_OK;
}

HRESULT CDrawLine2D::Init(const char *file_path, const int polygon_max)
{
	LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();
	//テクスチャとuv値情報ロード
	TexLoader::LoadTexSheetFromBin(file_path, m_pTexUvInfo, &m_pTex );

	int vertex = polygon_max * 4;
	int index  = polygon_max * 6;

	//配列生成
	m_polygonArray = new PolygonInfo[ polygon_max ];

	//頂点バッファとインデックスバッファを生成
	if( FAILED(device->CreateVertexBuffer(sizeof(VERTEX_2D)*vertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff,NULL)))	
	{
		return E_FAIL;
	}

	if (FAILED(device->CreateIndexBuffer(sizeof(WORD) * index, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

/*----------------------------------------------------------------------------	
	Uninit
-----------------------------------------------------------------------------*/

void CDrawLine2D::Uninit()
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	delete[]m_pTexUvInfo;
	delete[]m_polygonArray;
}

/*----------------------------------------------------------------------------
	Update
-----------------------------------------------------------------------------*/

void CDrawLine2D::Update()
{

}

/*----------------------------------------------------------------------------
	Draw
-----------------------------------------------------------------------------*/

void CDrawLine2D::Draw()
{
	//描画対象無の場合スルー
	if ( m_drawPolygonSum > 0 )
	{
		ApplyData();

		LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

		//バッファセット
		device->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
		device->SetIndices(m_pIdxBuff);

		//FVFセット
		device->SetFVF(FVF_VERTEX_2D);

		//テクスチャセット
		device->SetTexture(0, m_pTex);

		//ドロー
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_drawPolygonSum, 0, m_drawPolygonSum * 2);

		//描画後リセット
		m_drawPolygonSum = 0;
		
	}

}

/*----------------------------------------------------------------------------
	ApplyData
-----------------------------------------------------------------------------*/

void CDrawLine2D::ApplyData()
{
	D3DXMATRIX mtxIdentity;
	D3DXMatrixIdentity(&mtxIdentity);

	VERTEX_2D *vtx;

	m_pVtxBuff->Lock(0, 0, (void**)&vtx, 0);

	for (int i = 0; i < m_drawPolygonSum; i++)
	{
		D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTrans;

		D3DXVECTOR3 vtxBuff[4];

		vtxBuff[0] = D3DXVECTOR3(-0.5, -0.5, 0);
		vtxBuff[1] = D3DXVECTOR3( 0.5, -0.5, 0);
		vtxBuff[2] = D3DXVECTOR3(-0.5,  0.5, 0);
		vtxBuff[3] = D3DXVECTOR3( 0.5,  0.5, 0);

		//初期化
		mtxWorld = mtxIdentity;

		//行列設定
		D3DXMatrixScaling(&mtxScl, m_polygonArray[i].scl.x, m_polygonArray[i].scl.y,0);
		D3DXMatrixRotationZ(&mtxRot, m_polygonArray[i].rot);
		D3DXMatrixTranslation(&mtxTrans, m_polygonArray[i].pos.x, m_polygonArray[i].pos.y, 0);

		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		//頂点に適用
		for (int j = 0; j < 4; j++)
		{
			D3DXVec3TransformCoord(&vtxBuff[j], &vtxBuff[j], &mtxWorld);
		}

		//書き込み開始
		vtx[0].vtx = vtxBuff[0];
		vtx[1].vtx = vtxBuff[1];
		vtx[2].vtx = vtxBuff[2];
		vtx[3].vtx = vtxBuff[3];

		vtx[0].rhw = 1.0f;
		vtx[1].rhw = 1.0f;
		vtx[2].rhw = 1.0f;
		vtx[3].rhw = 1.0f;

		vtx[0].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);
		vtx[1].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);
		vtx[2].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);
		vtx[3].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);

		int texIdx = m_polygonArray[i].texIdx;

		vtx[0].tex = D3DXVECTOR2(m_pTexUvInfo[texIdx].uv.x,
			                     m_pTexUvInfo[texIdx].uv.y);
		vtx[1].tex = D3DXVECTOR2(m_pTexUvInfo[texIdx].uv.x+m_pTexUvInfo[texIdx].size.x,
			                     m_pTexUvInfo[texIdx].uv.y);
		vtx[2].tex = D3DXVECTOR2(m_pTexUvInfo[texIdx].uv.x,
			                     m_pTexUvInfo[texIdx].uv.y + m_pTexUvInfo[texIdx].size.y);
		vtx[3].tex = D3DXVECTOR2(m_pTexUvInfo[texIdx].uv.x + m_pTexUvInfo[texIdx].size.x,
								 m_pTexUvInfo[texIdx].uv.y + m_pTexUvInfo[texIdx].size.y);

		vtx += 4;
	}

	m_pVtxBuff->Unlock();

	//インデックスバッファ
	WORD *index;

	int idxNum = 0;
	int bufNum = 0;

	m_pIdxBuff->Lock(0, 0, (void**)&index, 0);

	for (int i = 0; i < m_drawPolygonSum; i++)
	{
		index[idxNum] = bufNum;
		idxNum++;
		index[idxNum] = bufNum+1;
		idxNum++;
		index[idxNum] = bufNum+2;
		idxNum++;
		index[idxNum] = bufNum+2;
		idxNum++;
		index[idxNum] = bufNum+1;
		idxNum++;
		index[idxNum] = bufNum+3;
		idxNum++;

		bufNum += 4;
	}

	m_pIdxBuff->Unlock();
}

/*----------------------------------------------------------------------------
	AddPolygon
-----------------------------------------------------------------------------*/

void CDrawLine2D::AddPolygon(D3DXVECTOR2 scl, float rot, D3DXVECTOR2 pos, const int tex_idx)
{
	m_polygonArray[m_drawPolygonSum].scl = scl;
	m_polygonArray[m_drawPolygonSum].rot = rot;
	m_polygonArray[m_drawPolygonSum].pos = D3DXVECTOR3(pos.x,pos.y,0);
	m_polygonArray[m_drawPolygonSum].texIdx = tex_idx;

	m_drawPolygonSum++;
}


