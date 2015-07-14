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
#include "CBeeconIconUI.h"
#include "Define.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
static const float BEECON_FOLLOW_ANTON_DOWN_SPEED_THRESHOLD = (30.0f);		// ビーコンがアントンに追従するy方向速度判定値

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
	m_pAntonIconUI = nullptr;
	m_pBeeconIconUI = nullptr;
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
	CreateAnton(pos);
	CreateBeecon(pos);

	m_pInputCommand = new CInputCommand(CManager::GetInputKeyboard(), CManager::GetInputJoypad());
	m_pInputCommand->Init();

	m_pAntonIconUI = CAntonIconUI::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	m_pBeeconIconUI = CBeeconIconUI::Create(D3DXVECTOR3(200.0f, 150.0f, 0.0f));

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

	CheckMove();
	CheckChangeNormalAnton();

	// アントンの自動移動
	D3DXVECTOR3 targetPos = m_pBeecon->GetPosition();
	m_pAnton->SetTargetPosition(targetPos.x, m_pAnton->GetPosition().y);

#if 1
	// test
	if (m_pInputCommand->IsTrigger(CInputCommand::COMMAND_NORMAL))
	{
		SetAntonState(ANTON_STATE_NORMAL);
		m_pBeeconIconUI->SetIconType(CBeeconIconUI::ICONTYPE_NONE);
		
		// testcode
		m_pBeecon->SetAction(CBeecon::ACTION_WALK);
	}
	else if (m_pInputCommand->IsTrigger(CInputCommand::COMMAND_METTAL))
	{
		SetAntonState(ANTON_STATE_METAL);

		// testcode
		m_pBeecon->SetAction(CBeecon::ACTION_CONNECT);
	}
	else if (m_pInputCommand->IsTrigger(CInputCommand::COMMAND_MINIMAMU))
	{
		SetAntonState(ANTON_STATE_MINIMUM);

		// testcode
		m_pBeecon->SetAction(CBeecon::ACTION_WARP);
	}
	else if (m_pInputCommand->IsTrigger(CInputCommand::COMMAND_POWERFULL))
	{
		SetAntonState(ANTON_STATE_POWERFUL);

		if (m_pAnton->GetAction() == CAnton::ACTION_PUSH)
		{
			m_pAnton->SetAction(CAnton::ACTION_WALK);
		}
		else
		{
			m_pAnton->SetAction(CAnton::ACTION_PUSH);
		}

		// testcode
		m_pBeecon->SetAction(CBeecon::ACTION_WARP);
	}

	if (m_pInputCommand->IsTrigger(CInputCommand::COMMAND_NORMAL))
	{
		m_pAnton->SetAction(CAnton::ACTION_FRONT);
	}
#endif

	// アントンが重力落下時
	float fAntonDownSpeed = m_pAnton->GetSpd();
	if (fAntonDownSpeed > BEECON_FOLLOW_ANTON_DOWN_SPEED_THRESHOLD)
	{
		D3DXVECTOR3 workPos = m_pBeecon->GetPosition();
		workPos.y += fAntonDownSpeed;
		m_pBeecon->SetPosition(workPos);
		m_pBeecon->SetTargetPosition(workPos.x, workPos.y);
	}
}

/*-----------------------------------------------------------------------------
	移動確認
-----------------------------------------------------------------------------*/
void CPlayer::CheckMove(void)
{
	const bool bRight = m_pInputCommand->IsPress(CInputCommand::COMMAND_RIGHT);
	const bool bLeft = m_pInputCommand->IsPress(CInputCommand::COMMAND_LEFT);
	const bool bUp = m_pInputCommand->IsPress(CInputCommand::COMMAND_UP);
	const bool bDown = m_pInputCommand->IsPress(CInputCommand::COMMAND_DOWN);

	// 左右判定
	if (bRight)
	{
		m_pBeecon->CommandRightMove();
	}
	else if (bLeft)
	{
		m_pBeecon->CommandLeftMove();
	}

	// 上下判定
	if (bUp)
	{
		m_pBeecon->CommandUpMove();
	}
	else if (bDown)
	{
		m_pBeecon->CommandDownMove();
	}

	// 何も入力していない
	if (!(bRight || bLeft || bUp || bDown))
	{
		m_pBeecon->CommandStopMove();
	}
}

/*-----------------------------------------------------------------------------
	アントン生成
-----------------------------------------------------------------------------*/
void CPlayer::CreateAnton(D3DXVECTOR3 pos)
{
	CheckFirstPosition(&pos, D3DXVECTOR2(100.0f, 130.0f));

	m_pAnton = new CAnton(4);
	m_pAnton->Init();
	m_pAnton->SetPosition(pos);
	m_pAnton->SetRotation(0, 0, 0);
	m_pAnton->SetScaling(100, 130);
	m_pAnton->SetTargetPosition(pos.x,pos.y);
	m_pAnton->SetSpd(8);
	m_pAnton->CommandChangeNormal();
}

/*-----------------------------------------------------------------------------
	ビーコン生成
-----------------------------------------------------------------------------*/
void CPlayer::CreateBeecon(D3DXVECTOR3 pos)
{
	CheckFirstPosition(&pos, D3DXVECTOR2(100.0f, 130.0f));

	m_pBeecon = new CBeecon(4);
	m_pBeecon->Init();
	m_pBeecon->SetPosition(pos);
	m_pBeecon->SetRotation(0, 0, 0);
	m_pBeecon->SetScaling(100, 130);
	m_pBeecon->SetTargetPosition(pos.x, pos.y);
	m_pBeecon->SetSpd(8);
}

/*-----------------------------------------------------------------------------
	初期座標検査処理
-----------------------------------------------------------------------------*/
void CPlayer::CheckFirstPosition(D3DXVECTOR3 *pos_pointer,const D3DXVECTOR2& scl)
{
	// 画面外判定
	const bool bLeftDispOut = (pos_pointer->x < 0.0f + scl.x / 2);
	const bool bRightDispOut = (pos_pointer->x > SCREEN_WIDTH - scl.x / 2);

	if (bLeftDispOut)
	{
		pos_pointer->x = 0.0f + scl.x / 2;
	}
	if (bRightDispOut)
	{
		pos_pointer->x = SCREEN_WIDTH - scl.x / 2;
	}
}

/*-----------------------------------------------------------------------------
	アントン状態セット処理
-----------------------------------------------------------------------------*/
void CPlayer::SetAntonState(ANTON_STATE state)
{
	m_pAnton->SetState(static_cast<CAnton::STATE>(state));
	m_pAntonIconUI->SetIconType(static_cast<CAntonIconUI::ICONTYPE>(state));
}

/*-----------------------------------------------------------------------------
	ビーコンブロック情報セット処理
-----------------------------------------------------------------------------*/
void CPlayer::SetBeeconBlockID(BEECON_BLOCKID block_id)
{
	m_pBeecon->SetBlockID(static_cast<CBeecon::BLOCKID>(block_id));

	// TODO::beeconのUI設定も変える
}

/*-----------------------------------------------------------------------------
	ノーマルアントンに戻る処理
-----------------------------------------------------------------------------*/
void CPlayer::CheckChangeNormalAnton(void)
{
	const bool bChangeNormalAnton = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CHANGENORMAL);

	if (bChangeNormalAnton == false)
	{
		return;
	}

	SetAntonState(ANTON_STATE_NORMAL);
}

void CPlayer::SetBeeconIconUIType(BEECON_ICONTYPE icon_type)
{
	m_pBeeconIconUI->SetIconType(static_cast<CBeeconIconUI::ICONTYPE>(icon_type));
}