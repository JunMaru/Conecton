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
		BEECON_BLOCKID_NONE = 0,					// �Ȃ�
		BEECON_BLOCKID_LASER_CONTROL_DOWN = 16,		// ���[�U�[�i�s�����ύX�u���b�N_��
		BEECON_BLOCKID_LASER_CONTROL_UP,			// ���[�U�[�i�s�����ύX�u���b�N_��
		BEECON_BLOCKID_LASER_CONTROL_RIGHT,			// ���[�U�[�i�s�����ύX�u���b�N_�E
		BEECON_BLOCKID_LASER_CONTROL_LEFT,			// ���[�U�[�i�s�����ύX�u���b�N_��
		BEECON_BLOCKID_MAGNET,						// ����
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
