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

	enum ANTON_STATE
	{
		ANTON_STATE_NORMAL = 0,
		ANTON_STATE_METAL,
		ANTON_STATE_MINIMUM,
		ANTON_STATE_POWERFUL,
		ANTON_STATE_MAX,
	};

	enum BEECON_BLOCKID
	{
		BEECON_BLOCKID_NONE = 0,					// なし
		BEECON_BLOCKID_LASER_CONTROL_DOWN = 16,		// レーザー進行方向変更ブロック_下
		BEECON_BLOCKID_LASER_CONTROL_UP,			// レーザー進行方向変更ブロック_上
		BEECON_BLOCKID_LASER_CONTROL_RIGHT,			// レーザー進行方向変更ブロック_右
		BEECON_BLOCKID_LASER_CONTROL_LEFT,			// レーザー進行方向変更ブロック_左
		BEECON_BLOCKID_MAGNET,						// 磁石
		BEECON_BLOCKID_MAX = 6,
	};

	enum BEECON_ICONTYPE
	{
		BEECON_ICONTYPE_NONE = 0,
		BEECON_ICONTYPE_DOWNARROW,
		BEECON_ICONTYPE_UPEERARROW,
		BEECON_ICONTYPE_RIGHTARROW,
		BEECON_ICONTYPE_LEFTARROW,
		BEECON_ICONTYPE_MAGNET,
		BEECON_ICONTYPE_MAX,
	};


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

	void SetAntonState(ANTON_STATE state);
	void SetBeeconBlockID(BEECON_BLOCKID block_id);
	void SetBeeconIconUIType(BEECON_ICONTYPE icon_type);

	CAnton* GetAnton(void){ return m_pAnton; }
	CBeecon* GetBeecon(void){ return m_pBeecon; }
	
private:
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);
	void CreateAnton(D3DXVECTOR3 pos);
	void CreateBeecon(D3DXVECTOR3 pos);
	void CheckMove(void);
	void CheckChangeNormalAnton(void);
	void CheckFirstPosition(D3DXVECTOR3 *pos, const D3DXVECTOR2& scl);

	CAnton *m_pAnton;
	CBeecon *m_pBeecon;
	CInputCommand *m_pInputCommand;
	CAntonIconUI *m_pAntonIconUI;
	CBeeconIconUI *m_pBeeconIconUI;
};
