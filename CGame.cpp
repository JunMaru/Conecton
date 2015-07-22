/*-----------------------------------------------------------------------------
�Q�[����ʃt�F�[�Y�N���X
author	OkadaMoeto
since	20140713
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CGame.h"
#include "Define.h"
#include "CManager.h"
#include "CInput.h"
#include "CPlayer.h"
#include "CFade.h"
#include "CScene.h"
#include "CScene2D.h"
#include "collisionDetection.h"
#include "CBlockManager.h"
#include "CGauge.h"
#include "CInstancingObject.h"
#include "CBlock.h"
#include "CAnton.h"
#include "CBeecon.h"
#include "CLaser.h"
#include "CBackGround.h"
#include "CAntonLifeUI.h"
#include "CScrollManager.h"
#include "CLaserManager.h"
#include "CInputCommand.h"
#include "CPseudoLight.h"
#include "C2DLogo.h"
#include "CConfigRecorder.h"
#include "CStageConfig.h"
#include "CPause.h"
#include "CSoundXAudio2.h"
#include "CLifeConfig.h"

/*-----------------------------------------------------------------------------
	�e�N�X�`���ǂݍ��ݐ�̃p�X�ݒ�
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_FONT_CLEAR = "data/texture/font/clear.png";
static const char* TEXTUREPATH_PSEUDOLIGHT = "data/texture/pseudo_light/pseudo_light.png";
static const char* TEXTUREPATH_BG_STAGE1 = "data/texture/game_bg/s1_bg.jpg";
static const char* TEXTUREPATH_BG_STAGE2 = "data/texture/game_bg/s2_bg.jpg";
static const char* TEXTUREPATH_BG_STAGE3 = "data/texture/game_bg/s3_bg.jpg";
static const char* TEXTUREPATH_BG_STAGE4 = "data/texture/game_bg/s4_bg.jpg";

/*-----------------------------------------------------------------------------
	�X�e�[�W�f�[�^�ǂݍ��ݐ�̃p�X�ݒ�
-----------------------------------------------------------------------------*/
static char* MAPDATAPATH_CSV_STAGE1 = "data/stage_info/stage1.csv";
static char* MAPDATAPATH_CSV_STAGE2 = "data/stage_info/stage2ex.csv";
static char* MAPDATAPATH_CSV_STAGE3 = "data/stage_info/stage3.csv";
static char* MAPDATAPATH_CSV_STAGE4 = "data/stage_info/stage4.csv";

/*-----------------------------------------------------------------------------
	�Q�[�WUI�̃x�[�X�l(���ꂪ�ő�l�ɂȂ�)
-----------------------------------------------------------------------------*/
static const float GAUGE_BASE_VALUE = (100.0f);

/*-----------------------------------------------------------------------------
	�Q�[���I�[�o�[�a�f�̐����ݒ�
-----------------------------------------------------------------------------*/
static const char* TEXTUREPATH_GAMEOVERBG = "data/texture/gameover_bg/gameover_bg.png";
static const D3DXVECTOR3 POS_GAMEOVERBG = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
static const float WIDTH_GAMEOVERBG = 1280.0f;
static const float HEIGHT_GAMEOVERBG = 720.0f;

/*-----------------------------------------------------------------------------
	�Q�[���I�[�o�[�̃^�C�g���ւ̑J��
-----------------------------------------------------------------------------*/
static const float TIME_AUTOCHANGE_GAMEOVER = 30.0f * 6.0f;

/*-----------------------------------------------------------------------------
�ÓI�����o�ϐ��̏�����
-----------------------------------------------------------------------------*/
CPlayer *CGame::m_pPlayer = nullptr;
CBlockManager *CGame::m_pBlockManager = nullptr;
CLaserManager *CGame::m_pLaserManager = nullptr;
CPause* CGame::m_pPause = nullptr;

/*-----------------------------------------------------------------------------
�O���[�o���ϐ�
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
�R���X�g���N�^
-----------------------------------------------------------------------------*/
CGame::CGame()
{
	m_pPlayer = nullptr;
	m_pLifeUI = nullptr;
	m_pGauge = nullptr;
	m_pScrollManager = nullptr;
	m_pBackGround = nullptr;
	m_pInputCommand = nullptr;
	m_fScore = 0.0f;
	m_pPseudoLight = nullptr;
	m_pEndLogo = nullptr;
	m_pPause = nullptr;
}

/*-----------------------------------------------------------------------------
�f�X�g���N�^
-----------------------------------------------------------------------------*/
CGame::~CGame()
{
}

/*-----------------------------------------------------------------------------
������
-----------------------------------------------------------------------------*/
void CGame::Init(void)
{
	m_pPseudoLight = CPseudoLight::Create(TEXTUREPATH_PSEUDOLIGHT);

	InitStage();

	m_pPlayer = CPlayer::Create(VEC3_ZERO, VEC3_ZERO);
	
	InitGauge();

	m_pLifeUI = CAntonLifeUI::Create(D3DXVECTOR3(350.0f, 50.0f, 0.0f));

	// ���݂̐ݒ�L�^����Ă��郉�C�t�̒l�ɏ���������
	int nowLife = CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE);
	nowLife = MAX_RETRYLIFE - nowLife;
	m_pLifeUI->AddLife(-nowLife);
	
	m_pScrollManager = new CScrollManager();
	m_pScrollManager->Init();

	m_pLaserManager = CLaserManager::Create();

	m_pInputCommand = new CInputCommand(CManager::GetInputKeyboard(), CManager::GetInputJoypad());
	m_pInputCommand->Init();

	m_pPause = new CPause();
	m_pPause->Init();

	// �|�[�Y�̑I���������������āA���[�v�����Ƃ��ɂ����Ȃ��ʑJ�ڂ݂����Ȃ̂�h�~
	CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_PAUSESLECT, 0);

	m_transitionID = TRANSITIONID_NONE;
	m_bTransition = false;

	InitGameOverBG();

	PlayBGM();

	// 1�b�Ԃ̃t�F�[�h�C��
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 30.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
�I��
-----------------------------------------------------------------------------*/
void CGame::Uninit(void)
{
	StopBGM();

	// �`��ΏۃI�u�W�F�N�g�̉��
	CScene::ReleaseAll();

	m_pScrollManager->SetScrollWorld(VEC2_ZERO);
	delete m_pScrollManager;

	m_pPlayer->Uninit();
	delete m_pPlayer;

	m_pBlockManager->Uninit();
	delete m_pBlockManager;

	m_pLaserManager->Uninit();
	delete m_pLaserManager;

	m_pInputCommand->Uninit();
	delete m_pInputCommand;

	m_pPause->Uninit();
	delete m_pPause;
}

/*-----------------------------------------------------------------------------
�X�V
-----------------------------------------------------------------------------*/
void CGame::Update(void)
{
	m_pInputCommand->Update();

	// �t�F�[�h���Ă��Ȃ���΍X�V
	if (CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
		PauseTo();

		CheckGameOver();

		// �����t�F�[�h�A�E�g�ɂ��Ȃ��悤�ɂ����Ŕ���������
		if(m_bTransition)
		{
			CManager::GetPhaseFade()->Start(
												CFade::FADETYPE_OUT,
												30.0f,
												COL_WHITE);
		}
	}

	CheckTransition();

	// �|�[�Y�̍X�V
	if(m_pPause->GetPause())
	{
		m_pPause->UpdateInputEvent(m_pInputCommand);

		m_pPause->Update();
	}
	else
	{
		m_pPlayer->Update();

		m_pLaserManager->Update();

		CheckConnectAction();
	}

	HitCheckItem();

	CAnton *ant = m_pPlayer->GetAnton();

	if(ant->GetState() == CAnton::STATE_MINIMUM)
	{
		HitCheckMinimumAnton();
	}
	else if(ant->GetState() == CAnton::STATE_METAL)
	{
		HitCheckMetalAnton();
	}
	else
	{
		HitCheckAnton();
	}

	CheckGameEnd();

	CheckPauseSelect();

#ifdef _DEBUG
	CDebugProcDX9::Print("[CGame.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:�t�F�[�Y�J��\n");
#endif
}

// �~�j�}���p�̂����蔻��B�������ʂ̂Ɠ����\�[�X�Ŏ����ł���
void CGame::HitCheckMinimumAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();
	CBlock *pBlock = nullptr;

	D3DXVECTOR3 antonPos = ant->GetPosition();
	D3DXVECTOR3 antonPosOld = ant->GetPositionOld();
	D3DXVECTOR3 antonPosTar = ant->GetTargetPosition();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(25.0f, 32.5f, 0.0f);
	D3DXVECTOR3 setAntonPos = antonPos;

	D3DXVECTOR3 blockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);

	int nBlockIdxX = (int)(antonPos.x / BLOCK_WIDTH);
	int nBlockIdxY = (int)(antonPos.y / BLOCK_HEIGHT);

	// ���S�ɂ��炷
	antonPos.x += antonSize.x * 2.0f;
	antonPos.y += antonSize.y * 3.0f;

	// �A���g����n�ʂ̏�ɏ悹�邽�߂̓����蔻��
	for (int nCntX = 0; nCntX < 2; nCntX++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + nCntX, nBlockIdxY + 2);

		if (pBlock != nullptr
		 && pBlock ->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
		{
			// ���W�𒆐S�ɕύX
			blockPos = pBlock->GetPosition() + blockSize;

			// X�����̓����蔻��
			if ((antonPos.x + antonSize.x) > (blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x) < (blockPos.x + blockSize.x))
			{
				// Y�����̓����蔻��
				if ((antonPos.y + antonSize.y) > (blockPos.y - blockSize.y)
					&& (antonPos.y - antonSize.y) < (blockPos.y + blockSize.y))
				{
					setAntonPos.y = ( blockPos.y ) - ( antonSize.y * 4.0f ) + ( 5.0f * 2.0f );
				}
			}
		}
	}

	// �ǂւ̂߂荞�݂�h�����[�v( ������ )
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + (nCnt * 2) - 1, nBlockIdxY + 1);

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
		{
			blockPos = pBlock->GetPosition() + blockSize;

			if ((antonPos.y + antonSize.y) - (blockPos.y - blockSize.y) >= BLOCK_HEIGHT
				&& (antonPos.x + antonSize.x - 20.0f) >(blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x + 20.0f) < (blockPos.x + blockSize.x))
			{
				// �i��ł�������ɕǂ�����ꍇ�͖ړI�̍��W�ƌ��݂̍��W���P�O�ɖ߂�
				if ((antonPosTar.x - antonPos.x) * (blockPos.x - antonPos.x) > 0)
				{
					setAntonPos = antonPosOld;

					antonPosTar = setAntonPos;

					ant->SetTargetPosition(antonPosTar.x, antonPosTar.y);
				}

			}
		}
	}

	ant->SetPosition(setAntonPos);
}

void CGame::HitCheckMetalAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();
	CBlock *pBlock = nullptr;

	D3DXVECTOR3 antonPos = ant->GetPosition();
	D3DXVECTOR3 antonPosOld = ant->GetPositionOld();
	D3DXVECTOR3 antonPosTar = ant->GetTargetPosition();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(50.0f, 65.0f, 0.0f);
	D3DXVECTOR3 setAntonPos = antonPos;

	int nBlockIdxX = (int)(antonPos.x / BLOCK_WIDTH);
	int nBlockIdxY = (int)(antonPos.y / BLOCK_HEIGHT);

	D3DXVECTOR3 blockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);

	// ���S�ɂ��炷
	antonPos += antonSize;

	// �A���g����n�ʂ̏�ɏ悹�邽�߂̓����蔻��
	for (int nCntX = 0; nCntX < 2; nCntX++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + nCntX, nBlockIdxY + 2);

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
		{
			// ���W�𒆐S�ɕύX
			blockPos = pBlock->GetPosition() + blockSize;

			// X�����̓����蔻��
			if ((antonPos.x + antonSize.x) >(blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x) < (blockPos.x + blockSize.x))
			{
				// Y�����̓����蔻��
				if ( (antonPos.y + antonSize.y) > (blockPos.y - blockSize.y)
				  && (antonPos.y - antonSize.y) < (blockPos.y + blockSize.y) )
				{
					if ( pBlock -> GetBlockId() == CBlock::BLOCKID_SOIL_CRACK
					  || pBlock -> GetBlockId() == CBlock::BLOCKID_GRASS_CRACK )
					{
						pBlock -> Uninit();
						pBlock = NULL;
						CBlockManager::SetBlock( nBlockIdxX + nCntX, nBlockIdxY + 2, nullptr );
					}
					else
					{
						// 30.0f�́A�n�ʂɂ߂荞�܂Ȃ����߂̕␳�l
						setAntonPos.y = ( blockPos.y ) - ( ( antonSize.y - 5.0f ) * 2 ) - 30.0f;
					}
				}
			}
		}
	}

	// �ǂւ̂߂荞�݂�h�����[�v( ������ )
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + (nCnt * 3) - 1, nBlockIdxY + nCnt);

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
		{
			blockPos = pBlock->GetPosition() + blockSize;

			if ((antonPos.y + antonSize.y) - (blockPos.y - blockSize.y) >= BLOCK_HEIGHT
				&& (antonPos.x + antonSize.x - 20.0f) >(blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x + 20.0f) < (blockPos.x + blockSize.x))
			{
				// �i��ł�������ɕǂ�����ꍇ�͖ړI�̍��W�ƌ��݂̍��W���P�O�ɖ߂�
				if ((antonPosTar.x - antonPos.x) * (blockPos.x - antonPos.x) > 0)
				{
					setAntonPos.x = antonPosOld.x;

					antonPosTar = setAntonPos;

					ant->SetTargetPosition(antonPosTar.x, antonPosTar.y);
				}

			}
		}
	}

	ant->SetPosition(setAntonPos);
}

void CGame::HitCheckAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();
	CBlock *pBlock = nullptr;

	D3DXVECTOR3 antonPos = ant->GetPosition();
	D3DXVECTOR3 antonPosOld = ant->GetPositionOld();
	D3DXVECTOR3 antonPosTar = ant->GetTargetPosition();
	D3DXVECTOR3 antonSize = D3DXVECTOR3(50.0f, 65.0f, 0.0f);
	D3DXVECTOR3 setAntonPos = antonPos;

	int nBlockIdxX = (int)(antonPos.x / BLOCK_WIDTH);
	int nBlockIdxY = (int)(antonPos.y / BLOCK_HEIGHT);

	D3DXVECTOR3 blockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 blockSize = D3DXVECTOR3(BLOCK_WIDTH / 2.0f, BLOCK_HEIGHT / 2.0f, 0.0f);

	// ���S�ɂ��炷
	antonPos += antonSize;

	// �A���g����n�ʂ̏�ɏ悹�邽�߂̓����蔻��
	for (int nCntX = 0; nCntX < 2; nCntX++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + nCntX, nBlockIdxY + 2);

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
		{
			// ���W�𒆐S�ɕύX
			blockPos = pBlock->GetPosition() + blockSize;

			// X�����̓����蔻��
			if ((antonPos.x + antonSize.x) > (blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x) < (blockPos.x + blockSize.x))
			{
				// Y�����̓����蔻��
				if ((antonPos.y + antonSize.y) > (blockPos.y - blockSize.y)
					&& (antonPos.y - antonSize.y) < (blockPos.y + blockSize.y))
				{
					setAntonPos.y = (blockPos.y) - ((antonSize.y - 5.0f) * 2);
				}
			}
		}
	}

	// �ǂւ̂߂荞�݂�h�����[�v( ������ )
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pBlock = CBlockManager::GetBlock(nBlockIdxX + (nCnt * 3) - 1, nBlockIdxY + nCnt);

		if (pBlock != nullptr
		 && pBlock->GetBlockId() < CBlock::BLOCKID_WARP_BLUE)
		{
			blockPos = pBlock->GetPosition() + blockSize;

			if ((antonPos.y + antonSize.y) - (blockPos.y - blockSize.y) >= BLOCK_HEIGHT
				&& (antonPos.x + antonSize.x - 20.0f) > (blockPos.x - blockSize.x)
				&& (antonPos.x - antonSize.x + 20.0f) < (blockPos.x + blockSize.x))
			{
				// �i��ł�������ɕǂ�����ꍇ�͖ړI�̍��W�ƌ��݂̍��W���P�O�ɖ߂�
				if ((antonPosTar.x - antonPos.x) * (blockPos.x - antonPos.x) > 0)
				{
					setAntonPos = antonPosOld;

					antonPosTar = setAntonPos;

					ant->SetTargetPosition(antonPosTar.x, antonPosTar.y);
				}
				
			}
		}
	}

	ant->SetPosition(setAntonPos);
}

//=============================================================================
// �Q�[�W����������
//=============================================================================
void CGame::InitGauge(void)
{
	m_pGauge = new CGauge(6);
	m_pGauge->Init();
	m_pGauge->SetPosition(D3DXVECTOR3(1200.0f, 100.0f, 0.0f));
	m_pGauge->SetScling(D3DXVECTOR2(116.0f, 116.0f));
	m_pGauge->SetGaugeBaseVal(GAUGE_BASE_VALUE);
	m_pGauge->SetGaugeVal(0.0f);

	const char *pFilePathTable[] = { "data/texture/ui/gauge_base.png", "data/texture/ui/gauge.png", "data/texture/ui/gauge_frame.png", };
	const int nLoadFileNum = 3;
	for (int nCnt = 0; nCnt < nLoadFileNum; ++nCnt)
	{
		m_pGauge->LoadTexture(nCnt, pFilePathTable[nCnt]);
	}
}

//=============================================================================
// �R�l�N�g�A�N�V��������
//=============================================================================
void CGame::CheckConnectAction(void)
{
	const bool bConnect = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CONNECT);

	if (bConnect == false)
	{
		return;
	}

	const float fOffsetX = 25.0f;
	const float fOffsetY = 25.0f;

	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x + fOffsetX, beeconPos.y + fOffsetY, 0.0f);
	CBlock::BLOCKID blockIDFromBlockManager = CBlock::BLOCKID_NONE;

	// �����Ńu���b�N�}�l�[�W���[����u���b�N�����擾
	blockIDFromBlockManager = m_pBlockManager->GetBlockID(workPos);

	// �R�l�N�g�悪�����Ȃ������ꍇ
	if (blockIDFromBlockManager == CBlock::BLOCKID_NONE)
	{
		// �R�l�N�g�Ȃ�ĂȂ�����
		return;
	}

	// �r�[�R���͍��R�l�N�g���[�V�������H
	if (pBeecon->GetAction() == CBeecon::ACTION_CONNECT)
	{
		// �R�l�N�g���̃R�l�N�g�B�_���[�b�^�C
		return;
	}

	// ����Ă����͕̂ϐg�u���b�N���H
	if (ConnectChangeAntonBlock())
	{
		// �I�΂ꂽ�͕̂ϐg�u���b�N�ł���
		return;
	}

	// ����Ă����̂̓M�~�b�N�u���b�N���H
	if (ConnectGimmickBlock())
	{
		// �I�΂ꂽ�̂̓M�~�b�N�u���b�N�ł���
		return;
	}

	// ����Ă����̂̓m�[�}���u���b�N���H
	if (ConnectNormalBlock())
	{
		// �I�΂ꂽ�̂̓m�[�}���u���b�N�ł���
		return;
	}

	// �ϐg�ł��M�~�b�N�ł��m�[�}���u���b�N�ł��Ȃ��ł�
	return;
}

//=============================================================================
// �R�l�N�g�A�N�V����(�ϐg�u���b�N)����
//=============================================================================
bool CGame::ConnectChangeAntonBlock(void)
{
	const float fOffsetX = 25.0f;
	const float fOffsetY = 25.0f;

	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x + fOffsetX, beeconPos.y + fOffsetY, 0.0f);
	CBlock::BLOCKID blockIDFromBlockManager = m_pBlockManager->GetBlockID(workPos);

	const CBlock::BLOCKID aChangeAntonBlockTable[] = { CBlock::BLOCKID_METAL, CBlock::BLOCKID_MINIMUM, CBlock::BLOCKID_POWERFUL, };
	const CPlayer::ANTON_STATE aAntonStateTable[] = { CPlayer::ANTON_STATE_METAL, CPlayer::ANTON_STATE_MINIMUM, CPlayer::ANTON_STATE_POWERFUL, };

	// �擾���Ă����u���b�N��񂪃A���g����ϐg��������̂��H
	for (int nCnt = 0; nCnt < sizeof(aChangeAntonBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		// �e�[�u��������
		if (blockIDFromBlockManager != aChangeAntonBlockTable[nCnt])
		{
			continue;
		}

		// �ϐg�u���b�N�������ꍇ
		// �A���g����ϐg�����A�u���b�N���m�[�}���ɂ���
		m_pPlayer->SetAntonState(aAntonStateTable[nCnt]);

		// �u���b�N�}�l�[�W���[�̃u���b�N���m�[�}���ɏ㏑��
		m_pBlockManager->OverwriteGimmickBlock(CBlock::BLOCKID_SOIL, workPos);

		pBeecon->SetAction(CBeecon::ACTION_CONNECT);
		return true;
	}

	return false;
}

//=============================================================================
// �R�l�N�g�A�N�V����(�M�~�b�N�u���b�N)����
//=============================================================================
bool CGame::ConnectGimmickBlock(void)
{
	const float fOffsetX = 25.0f;
	const float fOffsetY = 25.0f;

	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x + fOffsetX, beeconPos.y + fOffsetY, 0.0f);
	CBlock::BLOCKID blockIDFromBlockManager = m_pBlockManager->GetBlockID(workPos);

	const CBlock::BLOCKID aGimmickBlockTable[] = { CBlock::BLOCKID_LASER_CONTROL_DOWN, CBlock::BLOCKID_LASER_CONTROL_LEFT,
													CBlock::BLOCKID_LASER_CONTROL_RIGHT, CBlock::BLOCKID_LASER_CONTROL_UP,
													CBlock::BLOCKID_MAGNET, };
	const int nGimmickBlock2BeeconIconUIOffset = CBlock::BLOCKID_LASER_CONTROL_DOWN - 1;

	CBlock::BLOCKID blockIDFromBeecon = static_cast<CBlock::BLOCKID>(pBeecon->GetBlockID());

	// �擾���Ă����u���b�N���̓M�~�b�N�Ȃ̂��H
	for (int nCnt = 0; nCnt < sizeof(aGimmickBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		// �e�[�u��������
		if (blockIDFromBlockManager != aGimmickBlockTable[nCnt])
		{
			continue;
		}

		// �r�[�R���ɑ΂���A�N�V�����ݒ�
		pBeecon->SetAction(CBeecon::ACTION_CONNECT);
		m_pPlayer->SetBeeconIconUIType(static_cast<CPlayer::BEECON_ICONTYPE>(aGimmickBlockTable[nCnt] - nGimmickBlock2BeeconIconUIOffset));

		// �M�~�b�N�u���b�N�������ꍇ
		// �r�[�R�����M�~�b�N�u���b�N�̃X�g�b�N�������Ă��Ȃ��ꍇ
		if (blockIDFromBeecon == CBlock::BLOCKID_NONE)
		{
			int nLaserNo = m_pBlockManager->GetBlockHitLaserNo(workPos);
			m_pLaserManager->ReplaceLaser(nLaserNo);

			// �r�[�R���ɃM�~�b�N�u���b�NID���Z�b�g���A�u���b�N�}�l�[�W���[�̃u���b�N�ɂ̓m�[�}���ɏ㏑��
			m_pPlayer->SetBeeconBlockID(static_cast<CPlayer::BEECON_BLOCKID>(blockIDFromBlockManager));
			m_pBlockManager->OverwriteGimmickBlock(CBlock::BLOCKID_SOIL, workPos);

			return true;
		}

		// �r�[�R�����Ȃ񂩂���̃u���b�N�������Ă����ꍇ
		else
		{
			int nLaserNo = m_pBlockManager->GetBlockHitLaserNo(workPos);
			m_pLaserManager->ReplaceLaser(nLaserNo);

			// �r�[�R���ɃM�~�b�N�u���b�NID���Z�b�g���A�u���b�N�}�l�[�W���[�̃u���b�N�ɂ̓r�[�R���������Ă����̂��㏑��
			m_pPlayer->SetBeeconBlockID(static_cast<CPlayer::BEECON_BLOCKID>(blockIDFromBlockManager));
			m_pBlockManager->OverwriteGimmickBlock(blockIDFromBeecon, workPos);

			return true;
		}
	}

	return false;
}

//=============================================================================
// �R�l�N�g�A�N�V����(�m�[�}���u���b�N)����
//=============================================================================
bool CGame::ConnectNormalBlock(void)
{
	const float fOffsetX = 25.0f;
	const float fOffsetY = 25.0f;

	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x + fOffsetX, beeconPos.y + fOffsetY, 0.0f);
	CBlock::BLOCKID blockIDFromBlockManager = m_pBlockManager->GetBlockID(workPos);

	const CBlock::BLOCKID aGimmickBlockTable[] = { CBlock::BLOCKID_LASER_CONTROL_DOWN, CBlock::BLOCKID_LASER_CONTROL_LEFT,
													CBlock::BLOCKID_LASER_CONTROL_RIGHT, CBlock::BLOCKID_LASER_CONTROL_UP,
													CBlock::BLOCKID_MAGNET, };

	CBlock::BLOCKID blockIDFromBeecon = static_cast<CBlock::BLOCKID>(pBeecon->GetBlockID());
	bool bGimmick = false;

	// �r�[�R���͉�������̃M�~�b�N�u���b�N�������Ă��邩�H
	for (int nCnt = 0; nCnt < sizeof(aGimmickBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		// �e�[�u��������
		if (blockIDFromBeecon != aGimmickBlockTable[nCnt])
		{
			continue;
		}

		bGimmick = true;
		break;
	}

	// �r�[�R�����M�~�b�N�������Ă��Ȃ����m�[�}���u���b�N�ɒu���Ȃ��̂ł��悤�Ȃ�
	if (bGimmick == false)
	{
		return false;
	}

	// �M�~�b�N�u���b�N����e�[�u��
	const CBlock::BLOCKID aNormalBlockTable[] = { CBlock::BLOCKID_GRASS, CBlock::BLOCKID_SOIL, };

	// �擾���Ă����u���b�N���̓m�[�}���Ȃ̂��H
	for (int nCnt = 0; nCnt < sizeof(aNormalBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		// �e�[�u��������
		if (blockIDFromBlockManager != aNormalBlockTable[nCnt])
		{
			continue;
		}

		int nLaserNo = m_pBlockManager->GetBlockHitLaserNo(workPos);
		m_pLaserManager->ReplaceLaser(nLaserNo);

		// �m�[�}���������ꍇ
		// �r�[�R���ɓ����Ă����M�~�b�N���A�u���b�N�}�l�[�W���[�̃u���b�N�ɏ㏑���B�r�[�R���̂͋�
		m_pPlayer->SetBeeconBlockID(CPlayer::BEECON_BLOCKID_NONE);
		m_pBlockManager->OverwriteGimmickBlock(blockIDFromBeecon, workPos);

		pBeecon->SetAction(CBeecon::ACTION_CONNECT);
		m_pPlayer->SetBeeconIconUIType(CPlayer::BEECON_ICONTYPE_NONE);
		return true;
	}

	return false;
}

//=============================================================================
// �A�C�e���Ƃ̓����蔻��
//=============================================================================
void CGame::HitCheckItem(void)
{
	const float fAntonOffsetX = 50.0f;
	const float fAntonOffsetY = 50.0f;

	D3DXVECTOR3 antonPos = m_pPlayer->GetAnton()->GetPosition();
	antonPos.x += fAntonOffsetX;
	antonPos.y += fAntonOffsetY;

	CBlock::BLOCKID blockIDFromBlockManager = m_pBlockManager->GetBlockID(antonPos);

	// ��������̃u���b�N���H
	if (blockIDFromBlockManager == CBlock::BLOCKID_NONE)
	{
		return;
	}

	const CBlock::BLOCKID aItemBlockTable[] = { CBlock::BLOCKID_FOOD_ACORN, CBlock::BLOCKID_FOOD_APPLE, CBlock::BLOCKID_FOOD_MUSHROOM, };
	const int nFoodNum = m_pBlockManager->GetFoodNum();
	const float fFoodScore = GAUGE_BASE_VALUE / nFoodNum;

	// �A�C�e���`�F�b�N
	for (int nCnt = 0; nCnt < sizeof(aItemBlockTable) / sizeof(CBlock::BLOCKID); ++nCnt)
	{
		if (blockIDFromBlockManager != aItemBlockTable[nCnt])
		{
			continue;
		}

		m_fScore += fFoodScore;
		m_pGauge->SetGaugeVal(m_fScore);

		m_pBlockManager->OverwriteGimmickBlock(CBlock::BLOCKID_NONE, antonPos);
	}
}

//=============================================================================
// �Q�[���I���`�F�b�N����
//=============================================================================
void CGame::CheckGameEnd(void)
{
	const bool bFadeNone = (CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE);

	if (bFadeNone == false)
	{
		return;
	}

	const bool bEnd = m_pLaserManager->GetLaserGoalFlag();

	if (bEnd == false)
	{
		return;
	}

	const bool bMoveHand = (m_pPlayer->GetAntonAction() == CPlayer::ANTON_ACTION_FRONT) && (m_pPlayer->GetAntonState() == CPlayer::ANTON_STATE_NORMAL);

	if (bMoveHand == false)
	{
		m_pPlayer->SetAntonState(CPlayer::ANTON_STATE_NORMAL);
		m_pPlayer->SetAntonAction(CPlayer::ANTON_ACTION_FRONT);
		m_pPlayer->SetStopMove(true);
	}

	if (m_pEndLogo == nullptr)
	{
		D3DXVECTOR2 texSize = D3DXVECTOR2(1076.0f, 237.0f);
		const float fLogoOffsetY = -200.0f;
		m_pEndLogo = new C2DLogo();
		m_pEndLogo->LoadTexture(TEXTUREPATH_FONT_CLEAR);
		m_pEndLogo->Init();
		m_pEndLogo->SetPosition(D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y + fLogoOffsetY,0.0f));
		m_pEndLogo->SetScling(texSize / 2);
		m_pEndLogo->StartSclAnimation(true, texSize / 4, texSize / 2, 0.01f);

		PlayJingleClear();
	}

	const bool bEnter = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_ENTER);

	if (bEnter == true)
	{
		m_bTransition = true;
		SetTransitionID(TRANSITIONID_STAGESELECT);

		// �N���A���T�Ń��C�t���ő�l�܂ŏグ��
		CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_RETRYLIFE, MAX_RETRYLIFE);
	}
}

/*-----------------------------------------------------------------------------
	�ݒ�L�^����I������Ă���X�e�[�W���擾���A
	�V�т����X�e�[�W�̃u���b�N�Ɣw�i�𐶐� / ����������
-----------------------------------------------------------------------------*/
void CGame::InitStage(void)
{
	int selectStage = CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_STAGESELECT);

	switch(selectStage)
	{
		case STAGEID_1:
			m_pBlockManager = CBlockManager::Create(MAPDATAPATH_CSV_STAGE1);
			m_pBackGround = CBackGround::Create(TEXTUREPATH_BG_STAGE1);
			break;

		case STAGEID_2:
			m_pBlockManager = CBlockManager::Create(MAPDATAPATH_CSV_STAGE2);
			m_pBackGround = CBackGround::Create(TEXTUREPATH_BG_STAGE2);
			break;

		case STAGEID_3:
			m_pBlockManager = CBlockManager::Create(MAPDATAPATH_CSV_STAGE3);
			m_pBackGround = CBackGround::Create(TEXTUREPATH_BG_STAGE3);
			break;

		case STAGEID_4:
			m_pBlockManager = CBlockManager::Create(MAPDATAPATH_CSV_STAGE4);
			m_pBackGround = CBackGround::Create(TEXTUREPATH_BG_STAGE4);
			break;

		default:
			break;
	}
}

/*-----------------------------------------------------------------------------
	�|�[�Y�̓��͏������`�F�b�N���؂�ւ���
-----------------------------------------------------------------------------*/
void CGame::PauseTo(void)
{
	const bool bPause = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_PAUSE);

	if (bPause == false)
	{
		return;
	}

	if(!m_pPause->GetPause())
	{
		m_pPause->Enable();
	}
	else
	{
		m_pPause->Disable();
	}
}

/*-----------------------------------------------------------------------------
	��ʑJ�ڂ̃C�x���g�X�V
-----------------------------------------------------------------------------*/
void CGame::CheckPauseSelect(void)
{
	int selectPauseMenu = CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_PAUSESLECT);

	if(selectPauseMenu == 0)
	{
		return;
	}

	// �P�x�����ʂ�悤�Ƀt���O�n�m�Ȃ�ΕԂ�
	if(m_bTransition)
	{
		return;
	}

	if(m_bGameOver)
	{
		return;
	}

	switch(selectPauseMenu)
	{
		case PAUSEID_RETRY:
			Retry();
			break;

		case PAUSEID_EXIT:
			ReturnToStageSelect();
			break;

		default:
			break;
	}
}

/*-----------------------------------------------------------------------------
	��ʑJ�ڂ̃C�x���g�X�V
-----------------------------------------------------------------------------*/
void CGame::CheckTransition(void)
{
	if(m_transitionID == TRANSITIONID_NONE)
	{
		return;
	}

	// �t�F�[�h�A�E�g������ɉ�ʑJ��
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		switch(m_transitionID)
		{
			case TRANSITIONID_GAME_RETRY:
				CManager::SetPhase(CManager::PHASE_GAME_RETRY);
				break;

			case TRANSITIONID_STAGESELECT:
				CManager::SetPhase(CManager::PHASE_STAGESELECT);
				break;

			case TRANSITIONID_TITLE:
				CManager::SetPhase(CManager::PHASE_TITLE);
				break;

			default:
				break;
		}
	}
}

/*-----------------------------------------------------------------------------
	�Q�[���I�[�o�[�a�f�̏�����
-----------------------------------------------------------------------------*/
void CGame::InitGameOverBG(void)
{
	m_pGameOverBG = CScene2D::Create(
										TEXTUREPATH_GAMEOVERBG,
										POS_GAMEOVERBG,
										VEC3_ZERO,
										WIDTH_GAMEOVERBG,
										HEIGHT_GAMEOVERBG);

	m_pGameOverBG->SetDraw(false);

	m_bGameOver = false;

	m_autoChange = 0.0f;
}

/*-----------------------------------------------------------------------------
	�|�[�Y�ɂĂq�d�s�q�x�I�����̏���
-----------------------------------------------------------------------------*/
void CGame::Retry(void)
{
	// ���C�t�\���ȂǂɑΉ������邽�߂ɁA���g���C���C�t�l���P����
	int nowLife = CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE);
	CManager::GetConfigRecorder()->Set(CConfigRecorder::CI_RETRYLIFE, nowLife - 1);

	m_bTransition = true;
	SetTransitionID(TRANSITIONID_GAME_RETRY);
}

/*-----------------------------------------------------------------------------
	�|�[�Y�ɂĂd�w�h�s�I�����̏���
-----------------------------------------------------------------------------*/
void CGame::ReturnToStageSelect(void)
{
	if(CManager::GetConfigRecorder()->Get(CConfigRecorder::CI_RETRYLIFE) == 0)
	{
		m_pGameOverBG->SetDraw(true);
		m_bGameOver = true;

		PlayJingleOver();
	}
	else
	{
		m_bTransition = true;
		SetTransitionID(TRANSITIONID_STAGESELECT);
	}
}

/*-----------------------------------------------------------------------------
	�Q�[���I�[�o�[�\��
-----------------------------------------------------------------------------*/
void CGame::CheckGameOver(void)
{
	if(m_bGameOver == false)
	{
		return;
	}

	bool bSkip = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_CONNECT);
	if(bSkip)
	{
		m_autoChange = TIME_AUTOCHANGE_GAMEOVER;
	}

	m_autoChange++;

	bool bAutoChange = m_autoChange > TIME_AUTOCHANGE_GAMEOVER;
	if(bAutoChange)
	{
		m_bTransition = true;
		SetTransitionID(TRANSITIONID_TITLE);
	}
}


/*-----------------------------------------------------------------------------
	�Q�[��BGM�Đ�
-----------------------------------------------------------------------------*/
void CGame::PlayBGM(void)
{
	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_BGM_GAME);
}

/*-----------------------------------------------------------------------------
	�Q�[��BGM��~
-----------------------------------------------------------------------------*/
void CGame::StopBGM(void)
{
	CManager::GetSoundXAudio2()->Stop(CSoundXAudio2::SL_BGM_GAME);
}

/*-----------------------------------------------------------------------------
	�Q�[���N���A�W���O���Đ�
-----------------------------------------------------------------------------*/
void CGame::PlayJingleClear(void)
{
	// �X�e�[�W�N���A�W���O���Đ��̑O��BGM�͎~�߂Ă���
	CManager::GetSoundXAudio2()->Stop(CSoundXAudio2::SL_BGM_GAME);

	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_JINGLE_CLEAR);
}

/*-----------------------------------------------------------------------------
	�Q�[���I�[�o�[�W���O���Đ�
-----------------------------------------------------------------------------*/
void CGame::PlayJingleOver(void)
{
	// �Q�[���I�[�o�[�W���O���Đ��̑O��BGM�͎~�߂Ă���
	CManager::GetSoundXAudio2()->Stop(CSoundXAudio2::SL_BGM_GAME);

	CManager::GetSoundXAudio2()->Play(CSoundXAudio2::SL_JINGLE_OVER);
}

