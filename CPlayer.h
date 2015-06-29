/*-----------------------------------------------------------------------------
 プレイヤークラス
 author JunMaruyama
 since 20150605
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 インクルードガード
-----------------------------------------------------------------------------*/
#pragma once

/*-----------------------------------------------------------------------------
 インクルード
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
 前方クラス宣言
-----------------------------------------------------------------------------*/
class CAnton;
class CBeecon;
class CInputCommand;
class CAntonIconUI;
class CBeeconIconUI;

/*-----------------------------------------------------------------------------
 プレイヤークラス
-----------------------------------------------------------------------------*/
class CPlayer
{
public:
	CPlayer();
	virtual ~CPlayer();

	/*-----------------------------------------------------------------------------
		プレイヤーの生成
		return	CPlayer*	CPlayer*のインスタンス
		param	pos			中心座標
		param	rot			向き
		param	col			色
	-----------------------------------------------------------------------------*/
	static CPlayer* Create(
							D3DXVECTOR3 pos,
							D3DXVECTOR3 rot,
							D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	void Uninit(void);
	void Update(void);

	CAnton* GetAnton(void){ return m_pAnton; }
	CBeecon* GetBeecon(void){ return m_pBeecon; }
	
private:
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);
	void MoveCheck(void);
	CAnton *m_pAnton;
	CBeecon *m_pBeecon;
	CInputCommand *m_pInputCommand;
	CAntonIconUI *m_pAntonIconUI;
	CBeeconIconUI *m_pBeeconIconUI;
};
