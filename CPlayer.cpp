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
#include "CInputCommand.h"
#include "CAntonIconUI.h"

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
	m_pInputCommand = nullptr;
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

	m_pBeecon = new CBeecon(7);
	m_pBeecon->Init();
	m_pBeecon->SetPosition(0, 0, 0);
	m_pBeecon->SetRotation(0, 0, 0);
	m_pBeecon->SetScaling(100, 130);
	m_pBeecon->SetTargetPosition(0, 0);
	m_pBeecon->SetSpd(8);

	m_pInputCommand = new CInputCommand(CManager::GetInputKeyboard(), CManager::GetInputJoypad());
	m_pInputCommand->Init();

	m_pAntonIconUI = CAntonIconUI::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f));

	return S_OK;
}

/*-----------------------------------------------------------------------------
	終了
-----------------------------------------------------------------------------*/
void CPlayer::Uninit(void)
{
	m_pInputCommand->Uninit();
	delete m_pInputCommand;

	// CSceneが勝手にdeleteまでしてくれるので、nullptrを代入するだけ
	m_pAnton = nullptr;
	m_pBeecon = nullptr;
	m_pAntonIconUI = nullptr;
}

/*-----------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------*/
void CPlayer::Update(void)
{
	m_pInputCommand->Update();

	if (m_pInputCommand->IsPress(CInputCommand::COMMAND_RIGHT))
	{
		m_pBeecon->CommandRightMove();
	}
	else if (m_pInputCommand->IsPress(CInputCommand::COMMAND_LEFT))
	{
		m_pBeecon->CommandLeftMove();
	}
	else if (m_pInputCommand->IsPress(CInputCommand::COMMAND_NORMAL))
	{
		m_pAnton->SetState(CAnton::STATE_NORMAL);
		m_pAntonIconUI->SetIconType(CAntonIconUI::ICONTYPE_NORMAL);
	}
	else if (m_pInputCommand->IsPress(CInputCommand::COMMAND_METTAL))
	{
		m_pAnton->SetState(CAnton::STATE_METAL);
		m_pAntonIconUI->SetIconType(CAntonIconUI::ICONTYPE_METAL);
	}
	else if (m_pInputCommand->IsPress(CInputCommand::COMMAND_MINIMAMU))
	{
		m_pAnton->SetState(CAnton::STATE_MINIMUM);
		m_pAntonIconUI->SetIconType(CAntonIconUI::ICONTYPE_MINIMUM);
	}
	else if (m_pInputCommand->IsPress(CInputCommand::COMMAND_CALLANTON))
	{
		D3DXVECTOR3 targetPos = m_pBeecon->GetPosition();

		m_pAnton->SetTargetPosition(targetPos.x, m_pAnton->GetPosition().y);
	}
}
