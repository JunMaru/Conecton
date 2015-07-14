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

/*-----------------------------------------------------------------------------
�ÓI�����o�ϐ��̏�����
-----------------------------------------------------------------------------*/
CPlayer *CGame::m_pPlayer = nullptr;
CBlockManager *CGame::m_pBlockManager = nullptr;
CLaserManager *CGame::m_pLaserManager = nullptr;

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
	m_pPseudoLight = nullptr;
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
	m_pBlockManager = CBlockManager::Create( "data/stage_info/stage1.csv" );
	m_pPlayer = CPlayer::Create(VEC3_ZERO, VEC3_ZERO);
	InitGauge();
	m_pBackGround = CBackGround::Create("data/texture/game_bg/game_bg.jpg");
	m_pLifeUI = CAntonLifeUI::Create(D3DXVECTOR3(350.0f, 50.0f, 0.0f));

	m_pScrollManager = new CScrollManager();
	m_pScrollManager->Init();

	// ���[�U�[����e�X�g
	//CLaser::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), CLaser::DIRECTION_RIGHT);
	m_pLaserManager = CLaserManager::Create();

	m_pPseudoLight = CPseudoLight::Create("data/texture/pseudo_light/pseudo_light.png");

	m_pInputCommand = new CInputCommand(CManager::GetInputKeyboard(), CManager::GetInputJoypad());
	m_pInputCommand->Init();

	// 1�b�Ԃ̃t�F�[�h�C��
	CManager::GetPhaseFade()->Start(CFade::FADETYPE_IN, 60.0f, COL_WHITE);
}

/*-----------------------------------------------------------------------------
�I��
-----------------------------------------------------------------------------*/
void CGame::Uninit(void)
{
	// �`��ΏۃI�u�W�F�N�g�̉��
	CScene::ReleaseAll();

	m_pScrollManager->SetScrollWorld(VEC2_ZERO);
	delete m_pScrollManager;

	m_pPlayer->Uninit();
	delete m_pPlayer;

	m_pBlockManager->Uninit();
	delete m_pBlockManager;

	m_pGauge->Uninit();
	delete m_pGauge;

	m_pLaserManager->Uninit();
	delete m_pLaserManager;

	m_pInputCommand->Uninit();
	delete m_pInputCommand;
}

/*-----------------------------------------------------------------------------
�X�V
-----------------------------------------------------------------------------*/
void CGame::Update(void)
{
	// �L�[�{�[�h���͂��擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoyPad = CManager::GetInputJoypad();

	m_pInputCommand->Update();

	// �t�F�[�h���Ă��Ȃ���΍X�V
	if (CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
#ifdef _DEBUG
		if (pKeyboard->GetKeyTrigger(DIK_RETURN))
		{
			CManager::GetPhaseFade()->Start(
				CFade::FADETYPE_OUT,
				60.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
#endif
	}

	// �t�F�[�h�A�E�g������ɉ�ʑJ��
	if (CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		CManager::SetPhase(CManager::PHASE_RESULT);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CGame.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:�t�F�[�Y�J��\n");
#endif

	m_pPlayer->Update();

	m_pLaserManager->Update();

	CAnton *ant = m_pPlayer->GetAnton();

	if (ant->GetState() == CAnton::STATE_MINIMUM)
	{
		HitCheckMinimumAnton();
	}
	else if (ant->GetState() == CAnton::STATE_METAL)
	{
		HitCheckMetalAnton();
	}
	else
	{
		HitCheckAnton();
	}

	// �Q�[�W�e�X�g
	static float fTestGaugeVal = 50.0f;
	if (pKeyboard->GetKeyTrigger(DIK_8))
	{
		fTestGaugeVal -= 1.0f;
		m_pGauge->SetGaugeVal(fTestGaugeVal);
	}
	if (pKeyboard->GetKeyTrigger(DIK_9))
	{
		fTestGaugeVal += 1.0f;
		m_pGauge->SetGaugeVal(fTestGaugeVal);
	}

	// �u���b�N�ƃr�[�R���Ƃ̃R�l�N�g�`�F�b�N
	CheckConnectAction();
	CheckGimmickAction();
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
	m_pGauge->SetGaugeBaseVal(100.0f);
	m_pGauge->SetGaugeVal(50.0f);

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

	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x, beeconPos.y, 0.0f);
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
	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x, beeconPos.y, 0.0f);
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
	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x, beeconPos.y, 0.0f);
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
			// �r�[�R���ɃM�~�b�N�u���b�NID���Z�b�g���A�u���b�N�}�l�[�W���[�̃u���b�N�ɂ̓m�[�}���ɏ㏑��
			m_pPlayer->SetBeeconBlockID(static_cast<CPlayer::BEECON_BLOCKID>(blockIDFromBlockManager));
			m_pBlockManager->OverwriteGimmickBlock(CBlock::BLOCKID_SOIL, workPos);

			return true;
		}

		// �r�[�R�����Ȃ񂩂���̃u���b�N�������Ă����ꍇ
		else
		{
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
	CBeecon *pBeecon = m_pPlayer->GetBeecon();
	D3DXVECTOR2 beeconPos = pBeecon->GetPosition();
	D3DXVECTOR3 workPos = D3DXVECTOR3(beeconPos.x, beeconPos.y, 0.0f);
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

/*-----------------------------------------------------------------------------
 �M�~�b�N�A�N�V����
-----------------------------------------------------------------------------*/
void CGame::CheckGimmickAction(void)
{
	const bool bGimmickAction = m_pInputCommand->IsTrigger(CInputCommand::COMMAND_GIMMICKACTION);

	if (bGimmickAction == false)
	{
		return;
	}
}