/*-----------------------------------------------------------------------------
	プレイヤークラス
	author	OkadaMoeto
	since	20140527
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CPlayer.h"
#include "CAnton.h"
#include "CBeecon.h"
#include "CInput.h"

/*-----------------------------------------------------------------------------
	マクロ定義
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
*
	コンストラクタ
*
-----------------------------------------------------------------------------*/
CPlayer::CPlayer()
{
	m_pAnton = nullptr;
	m_pBeecon = nullptr;
}

/*-----------------------------------------------------------------------------
*
	デストラクタ
*
-----------------------------------------------------------------------------*/
CPlayer::~CPlayer()
{
}

/*-----------------------------------------------------------------------------
	生成
-----------------------------------------------------------------------------*/
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	CPlayer *pPlayer = new CPlayer();
	pPlayer->Init(pos, rot, col);

	return pPlayer;
}

/*-----------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------*/
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	m_pAnton = new CAnton(7);
	m_pAnton->Init();
	m_pAnton->SetPosition(0, 0, 0);
	m_pAnton->SetRotation(0, 0, 0);
	m_pAnton->SetScaling(100, 120);
	m_pAnton->SetTargetPosition(0, 0);
	m_pAnton->SetSpd(8);
	m_pAnton->CommandChangeNormal();
	m_pAnton->LoadTextureInfoFromText("data/texture_info/AntonTexInfo.txt");

	m_pBeecon = new CBeecon(7);
	m_pBeecon->Init();
	m_pBeecon->SetPosition(0, 0, 0);
	m_pBeecon->SetRotation(0, 0, 0);
	m_pBeecon->SetScaling(100, 130);
	m_pBeecon->SetTargetPosition(0, 0);
	m_pBeecon->SetSpd(8);
	m_pBeecon->LoadTextureInfoFromText("data/texture_info/BeeconTexInfo.txt");

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CPlayer::Uninit(void)
{
	// CSceneが勝手にdeleteまでしてくれるので、nullptrを代入するだけ
	m_pAnton = nullptr;
	m_pBeecon = nullptr;
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CPlayer::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	
	if (pJoypad != nullptr)
	{
		if (pJoypad->GetKeyPress(CInputJoypad::GAMEPAD_LSTICK_LEFT))
		{
			m_pBeecon->CommandLeftMove();
		}
		else if (pJoypad->GetKeyPress(CInputJoypad::GAMEPAD_LSTICK_RIGHT))
		{
			m_pBeecon->CommandRightMove();
		}

		if (pJoypad->GetKeyTrigger(CInputJoypad::GAMEPAD_2))
		{
			m_pAnton->SetState(CAnton::STATE_METAL);
		}
		else if (pJoypad->GetKeyTrigger(CInputJoypad::GAMEPAD_1))
		{
		m_pAnton->SetState(CAnton::STATE_NORMAL);
		}
		else if (pJoypad->GetKeyTrigger(CInputJoypad::GAMEPAD_3))
		{
			D3DXVECTOR3 targetPos = m_pBeecon->GetPosition();

			m_pAnton->SetTargetPosition(targetPos.x, m_pAnton->GetPosition().y);
		}
	}

	if (pKeyboard->GetKeyPress(DIK_D))
	{
		m_pBeecon->CommandRightMove();
	}
	else if (pKeyboard->GetKeyPress(DIK_A))
	{
		m_pBeecon->CommandLeftMove();
	}
	else if (pKeyboard->GetKeyTrigger(DIK_1))
	{
		m_pAnton->SetState(CAnton::STATE_NORMAL);
	}
	else if (pKeyboard->GetKeyTrigger(DIK_2))
	{
		m_pAnton->SetState(CAnton::STATE_METAL);
	}
	else if (pKeyboard->GetKeyTrigger(DIK_3))
	{
		m_pAnton->SetState(CAnton::STATE_MINIMUM);
	}
	
		else if (pKeyboard->GetKeyTrigger(DIK_4))
	{
		D3DXVECTOR3 targetPos = m_pBeecon->GetPosition();

		m_pAnton->SetTargetPosition(targetPos.x, m_pAnton->GetPosition().y);
	}
}
