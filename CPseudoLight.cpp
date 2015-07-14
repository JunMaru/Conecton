/*-----------------------------------------------------------------------------
	擬似ライトクラス
	author Okada.Moeto
	since 20150714
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルード
-----------------------------------------------------------------------------*/
#include "CPseudoLight.h"
#include "Define.h"

/*-----------------------------------------------------------------------------
	コンストラクタ
-----------------------------------------------------------------------------*/
CPseudoLight::CPseudoLight(int priority, CScene::OBJTYPE objtype)
	: CScene2D(priority, objtype)
{
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CPseudoLight::~CPseudoLight(void)
{

}

/*-----------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------*/
void CPseudoLight::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

/*-----------------------------------------------------------------------------
	生成処理
-----------------------------------------------------------------------------*/
CPseudoLight* CPseudoLight::Create(const char *file_name, int priority)
{
	CPseudoLight* pObj = new CPseudoLight(priority, CScene::OBJTYPE_2D);
	
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);

	pObj->Init(
				file_name,
				pos,
				VEC3_ZERO,
				COL_WHITE,
				SCREEN_WIDTH,
				SCREEN_HEIGHT);

	return pObj;
}