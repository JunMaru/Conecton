/*-----------------------------------------------------------------------------
	�v���C���[�N���X
	author	OkadaMoeto
	since	20140527
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
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
	�萔��`
-----------------------------------------------------------------------------*/
static const float BEECON_FOLLOW_ANTON_DOWN_SPEED_THRESHOLD = (30.0f);		// �r�[�R�����A���g���ɒǏ]����y�������x����l

/*-----------------------------------------------------------------------------
*
	�R���X�g���N�^
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
	�f�X�g���N�^
*
-----------------------------------------------------------------------------*/
CPlayer::~CPlayer()
{
}

/*-----------------------------------------------------------------------------
	����
-----------------------------------------------------------------------------*/
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	CPlayer *pPlayer = new CPlayer();
	pPlayer->Init(pos, rot, col);

	return pPlayer;
}

/*-----------------------------------------------------------------------------
	������
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
	�I��
-----------------------------------------------------------------------------*/
void CPlayer::Uninit(void)
{
	m_pInputCommand->Uninit();
	delete m_pInputCommand;

	// CScene�������delete�܂ł��Ă����̂ŁAnullptr�������邾��
	m_pAnton = nullptr;
	m_pBeecon = nullptr;
	m_pAntonIconUI = nullptr;
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CPlayer::Update(void)
{
	m_pInputCommand->Update();

	CheckMove();

	// �A���g���̎����ړ�
	D3DXVECTOR3 targetPos = m_pBeecon->GetPosition();
	m_pAnton->SetTargetPosition(targetPos.x, m_pAnton->GetPosition().y);

	if (m_pInputCommand->IsTrigger(CInputCommand::COMMAND_NORMAL))
	{
		m_pAnton->SetState(CAnton::STATE_NORMAL);
		m_pAntonIconUI->SetIconType(CAntonIconUI::ICONTYPE_NORMAL);
		m_pBeeconIconUI->SetIconType(CBeeconIconUI::ICONTYPE_NONE);
		
		// testcode
		m_pBeecon->SetAction(CBeecon::ACTION_WALK);
	}
	else if (m_pInputCommand->IsTrigger(CInputCommand::COMMAND_METTAL))
	{
		m_pAnton->SetState(CAnton::STATE_METAL);
		m_pAntonIconUI->SetIconType(CAntonIconUI::ICONTYPE_METAL);
		m_pBeeconIconUI->SetIconType(CBeeconIconUI::ICONTYPE_METAL);

		// testcode
		m_pBeecon->SetAction(CBeecon::ACTION_CONNECT);
	}
	else if (m_pInputCommand->IsTrigger(CInputCommand::COMMAND_MINIMAMU))
	{
		m_pAnton->SetState(CAnton::STATE_MINIMUM);
		m_pAntonIconUI->SetIconType(CAntonIconUI::ICONTYPE_MINIMUM);
		m_pBeeconIconUI->SetIconType(CBeeconIconUI::ICONTYPE_MINIMUM);

		// testcode
		m_pBeecon->SetAction(CBeecon::ACTION_WARP);
	}
	else if (m_pInputCommand->IsTrigger(CInputCommand::COMMAND_POWERFULL))
	{
		m_pAnton->SetState(CAnton::STATE_POWERFUL);
		m_pAntonIconUI->SetIconType(CAntonIconUI::ICONTYPE_POWERFUL);
		m_pBeeconIconUI->SetIconType(CBeeconIconUI::ICONTYPE_POWERFUL);

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


	// �A���g�����d�͗�����
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
	�ړ��m�F
-----------------------------------------------------------------------------*/
void CPlayer::CheckMove(void)
{
	if (m_pInputCommand->IsPress(CInputCommand::COMMAND_RIGHT))
	{
		m_pBeecon->CommandRightMove();
	}
	else if (m_pInputCommand->IsPress(CInputCommand::COMMAND_LEFT))
	{
		m_pBeecon->CommandLeftMove();
	}
	if (m_pInputCommand->IsPress(CInputCommand::COMMAND_UP))
	{
		m_pBeecon->CommandUpMove();
	}
	else if (m_pInputCommand->IsPress(CInputCommand::COMMAND_DOWN))
	{
		m_pBeecon->CommandDownMove();
	}
}

/*-----------------------------------------------------------------------------
	�A���g������
-----------------------------------------------------------------------------*/
void CPlayer::CreateAnton(D3DXVECTOR3 pos)
{
	CheckFirstPosition(&pos, D3DXVECTOR2(100.0f, 130.0f));

	m_pAnton = new CAnton(6);
	m_pAnton->Init();
	m_pAnton->SetPosition(pos);
	m_pAnton->SetRotation(0, 0, 0);
	m_pAnton->SetScaling(100, 130);
	m_pAnton->SetTargetPosition(pos.x,pos.y);
	m_pAnton->SetSpd(8);
	m_pAnton->CommandChangeNormal();
}

/*-----------------------------------------------------------------------------
	�r�[�R������
-----------------------------------------------------------------------------*/
void CPlayer::CreateBeecon(D3DXVECTOR3 pos)
{
	CheckFirstPosition(&pos, D3DXVECTOR2(100.0f, 130.0f));

	m_pBeecon = new CBeecon(6);
	m_pBeecon->Init();
	m_pBeecon->SetPosition(100, 0, 0);
	m_pBeecon->SetRotation(0, 0, 0);
	m_pBeecon->SetScaling(100, 130);
	m_pBeecon->SetTargetPosition(0, 0);
	m_pBeecon->SetSpd(8);
}

/*-----------------------------------------------------------------------------
	�������W��������
-----------------------------------------------------------------------------*/
void CPlayer::CheckFirstPosition(D3DXVECTOR3 *pos_pointer,const D3DXVECTOR2& scl)
{
	// ��ʊO����
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