/*-----------------------------------------------------------------------------
 �v���C���[�N���X
 author JunMaruyama
 since 20150605
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 �C���N���[�h�K�[�h
-----------------------------------------------------------------------------*/
#pragma once

/*-----------------------------------------------------------------------------
 �C���N���[�h
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
 �O���N���X�錾
-----------------------------------------------------------------------------*/
class CAnton;
class CBeecon;
class CInputCommand;
class CAntonIconUI;
class CBeeconIconUI;

/*-----------------------------------------------------------------------------
 �v���C���[�N���X
-----------------------------------------------------------------------------*/
class CPlayer
{
public:
	CPlayer();
	virtual ~CPlayer();

	/*-----------------------------------------------------------------------------
		�v���C���[�̐���
		return	CPlayer*	CPlayer*�̃C���X�^���X
		param	pos			���S���W
		param	rot			����
		param	col			�F
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
