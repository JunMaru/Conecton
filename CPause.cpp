/*-----------------------------------------------------------------------------
	�|�[�Y�N���X
	author	OkadaMoeto
	since	20140716
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CPause.h"
#include "define.h"
#include "CScene2D.h"
#include "CInput.h"
#include "CGame.h"
#include "CPauseMenu.h"
#include "CManager.h"
#include "CFade.h"
#include "CConfigRecorder.h"
#include "CInputCommand.h"

/*-----------------------------------------------------------------------------
	�}�N����`
-----------------------------------------------------------------------------*/
static const D3DXVECTOR3 STARTPOS_TOP_CURSOR	= D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 250.0f, 150.0f, 0.0f);
static const D3DXVECTOR3 STARTPOS_UNDER_CURSOR	= D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - 250.0f, 325.0f, 0.0f);

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CPause::CPause()
{
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CPause::~CPause()
{
}

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
HRESULT CPause::Init(void)
{
	// �|�[�Y���j���[�̐���
	m_pauseMenu = new CPauseMenu();
	m_pauseMenu->Init();

	// �|�[�Y�̍X�V�������Ȃ����߂�OFF
	m_bPause = false;

	// ������Ԃŕ\�������Ȃ��悤�ɂ���
	m_pauseMenu->HideAll();

	m_curve = 0.0f;

	m_bSelsect = false;
	m_bUp = false;
	m_bDown = false;

	return S_OK;
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CPause::Uninit(void)
{
	// �|�[�Y���j���[�̉��
	if(m_pauseMenu)
	{
		delete m_pauseMenu;
		m_pauseMenu = nullptr;
	}
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CPause::Update(void)
{
	// �g�p���Ă��Ȃ�������X�V���Ȃ�
	if(!m_bPause){ return; }

	m_pauseMenu->Update();

	MoveCursor();

	SelectMenu();
}

/*-----------------------------------------------------------------------------
	�|�[�Y���j���[�\��
-----------------------------------------------------------------------------*/
void CPause::Enable(void)
{
	m_bPause = true;

	// �|�[�Y���j���[�����ʒu���Q�[���ɖ߂�ɐݒ�
	m_pauseMenu->SetPositionCursor(STARTPOS_TOP_CURSOR);
	m_menuCursorPos = CPauseMenu::PAUSEMENU_RETRY;

	m_pauseMenu->DispAll();
}

/*-----------------------------------------------------------------------------
	�|�[�Y���j���[��\��
-----------------------------------------------------------------------------*/
void CPause::Disable(void)
{
	m_bPause = false;

	m_pauseMenu->HideAll();
}

/*-----------------------------------------------------------------------------
	�J�[�\���ʒu���㏸
-----------------------------------------------------------------------------*/
void CPause::UpCoursorPos(void)
{
	// �I�����Ă��郁�j���[�����̃��j���[��I�����Ă���悤�ɂ��炷
	m_menuCursorPos--;

	if(m_menuCursorPos < CPauseMenu::PAUSEMENU_RETRY)
	{
		// ���j���[�ʒu����ԏ�̏�Ԃł���ɓ��l�̓��͂������Ƃ��ɉ��Ƀ��[�v������
		m_pauseMenu->SetPositionCursor(STARTPOS_UNDER_CURSOR);
		m_menuCursorPos = CPauseMenu::PAUSEMENU_EXIT;
	}
	else
	{
		// ���j���[�ʒu�̏グ��
		D3DXVECTOR3 cursorPos = m_pauseMenu->GetPositionCursor();
		float offsetY = 300.0f;
		cursorPos.y -= offsetY;
		m_pauseMenu->SetPositionCursor(cursorPos);
	}
}

/*-----------------------------------------------------------------------------
	���J�[�\���ʒu�����~
-----------------------------------------------------------------------------*/
void CPause::DownCoursorPos(void)
{
	// �I�����Ă��郁�j���[���牺�̃��j���[��I�����Ă���悤�ɂ��炷
	m_menuCursorPos++;

	if(m_menuCursorPos >= CPauseMenu::PAUSEMENU_MAX)
	{
		// ���j���[�ʒu����ԉ��̏�Ԃł���ɓ��l�̓��͂������Ƃ��ɏ�Ƀ��[�v������
		m_pauseMenu->SetPositionCursor(STARTPOS_TOP_CURSOR);
		m_menuCursorPos = CPauseMenu::PAUSEMENU_RETRY;
	}
	else
	{
		// ���j���[�ʒu��������
		D3DXVECTOR3 cursorPos = m_pauseMenu->GetPositionCursor();
		float offsetY = 300.0f;
		cursorPos.y += offsetY;
		m_pauseMenu->SetPositionCursor(cursorPos);
	}
}

/*-----------------------------------------------------------------------------
	���͂ɍ��킹���J�[�\���̈ړ�
-----------------------------------------------------------------------------*/
void CPause::MoveCursor(void)
{
	if(m_bUp)
	{
		UpCoursorPos();
	}

	if(m_bDown)
	{
		DownCoursorPos();
	}
}

/*-----------------------------------------------------------------------------
	�I�����Ă���|�[�Y���j���[�̌��菈���ƑΉ������ԑJ��
-----------------------------------------------------------------------------*/
void CPause::SelectMenu(void)
{
	// �|�[�Y���j���[�̒��_�J���[
	D3DXCOLOR selectCol = COL_WHITE;
	D3DXCOLOR non_col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.75f);

	switch(m_menuCursorPos)
	{
		case CPauseMenu::PAUSEMENU_RETRY:
		{
			m_pauseMenu->SetColorRetry(selectCol);
			m_pauseMenu->SetColorExit(non_col);

			if(m_bSelsect)
			{
				m_pauseMenu->HideAll();
				Disable();
				CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_PAUSESLECT, CGame::PAUSEID_RETRY);
			}
			break;
		}

		case CPauseMenu::PAUSEMENU_EXIT:
		{
			m_pauseMenu->SetColorRetry(non_col);
			m_pauseMenu->SetColorExit(selectCol);

			if(m_bSelsect)
			{
				m_pauseMenu->HideAll();
				Disable();
				CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_PAUSESLECT, CGame::PAUSEID_EXIT);
			}
			break;
		}
	}
}

void CPause::UpdateInputEvent(CInputCommand* pInputCmd)
{
	if(pInputCmd == nullptr)
	{
		return;
	}

	bool bSelect = pInputCmd->IsTrigger(CInputCommand::COMMAND_CONNECT);
	if(bSelect)
	{
		CommandSelect();
	}
	else
	{
		m_bSelsect = false;
	}

	bool bUp = pInputCmd->IsTrigger(CInputCommand::COMMAND_UP);
	if(bUp)
	{
		CommandUp();
	}
	else
	{
		m_bUp = false;
	}

	bool bDown = pInputCmd->IsTrigger(CInputCommand::COMMAND_DOWN);
	if(bDown)
	{
		CommandDown();
	}
	else
	{
		m_bDown = false;
	}
}

void CPause::CommandSelect(void)
{
	m_bSelsect = true;
}

void CPause::CommandUp(void)
{
	m_bUp = true;
}

void CPause::CommandDown(void)
{
	m_bDown = true;
}

