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
#include "CScene2D.h"
#include "collisionDetection.h"

//�e�X�g
#include "CInstancingObject.h"
#include "CBlock.h"
#include "CAnton.h"
CInstancingObject *ins;

// temp(maru)
#include "CLaser.h"
#include "CBackGround.h"
#include "CScrollManager.h"
CScrollManager *pScrollManager;
CBackGround *pBackGround;

CBlock *Block[63];
CBlock *SpBlock[ 20 ];

/*-----------------------------------------------------------------------------
	�ÓI�����o�ϐ��̏�����
-----------------------------------------------------------------------------*/
CPlayer *CGame::m_pPlayer = nullptr;

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CGame::CGame()
{
	m_pPlayer = nullptr;
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
	m_pPlayer = CPlayer::Create(VEC3_ZERO,VEC3_ZERO);

	//test
	ins = new CInstancingObject(6);
	ins->LoadTexture( "data/texture/block/block.jpg",D3DXVECTOR2(300,100),D3DXVECTOR2(100,100) );
	ins->Init();
	
	for( int cnt = 0 ; cnt < 10 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( cnt * 100.0f,200.0f,0.0f );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 0,0 );

		Block[ cnt ] = b;
	}

	for( int cnt = 10 ; cnt < 20 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( cnt * 100.0f,700,0 );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 1,0 );

		Block[ cnt ] = b;
	}

	for( int cnt = 20 ; cnt < 40 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( 900,700  + 100 *(-20.0f +  cnt),0 );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 1,0 );

		Block[ cnt ] = b;
	}

	for( int cnt = 40 ; cnt < 60 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( 700.0f,700.0f + 100.0f *(-40 +  cnt),0.0f );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 1,0 );

		Block[ cnt ] = b;
	}
	
	for( int cnt = 60 ; cnt < 63 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( 700 + ( -60.0f +cnt) * 100,700 + 20 * 100,0 );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 1,0 );

		Block[ cnt ] = b;
	}
	
	for( int cnt = 0 ; cnt < 20 ; cnt++ )
	{
		CBlock *b = new CBlock;
		b->SetInsObj(ins);
		b->Init();
		b->SetPosition( 800,700.0f + 100.0f * cnt,0 );
		b->SetRotation(0,0,0);
		b->SetScaling( 100,100 );
		b->SetBlockTexID( 2,0 );

		SpBlock[ cnt ] = b;		
	}
	
	// temp(maru)
	pBackGround = CBackGround::Create("data/texture/anton/anton_t.png");
	
	pScrollManager = new CScrollManager();
	pScrollManager->Init();

	// ���[�U�[����e�X�g
	CLaser::Create(D3DXVECTOR3(0.0f,100.0f,0.0f), CLaser::DIRECTION_RIGHT);

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

	pScrollManager->SetScrollWorld(VEC2_ZERO);
	delete pScrollManager;

	m_pPlayer->Uninit();
	delete m_pPlayer;
}

/*-----------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------*/
void CGame::Update(void)
{
	// �L�[�{�[�h���͂��擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoyPad = CManager::GetInputJoypad();
	
	// �t�F�[�h���Ă��Ȃ���΍X�V
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_NONE)
	{
#ifdef _DEBUG
		if(pKeyboard->GetKeyTrigger(DIK_RETURN))
		{
			CManager::GetPhaseFade()->Start(
												CFade::FADETYPE_OUT,
												60.0f,
												D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
#endif
	}
	
	// �t�F�[�h�A�E�g������ɉ�ʑJ��
	if(CManager::GetPhaseFade()->GetFadetype() == CFade::FADETYPE_UNOUT)
	{
		CManager::SetPhase(CManager::PHASE_RESULT);
	}

#ifdef _DEBUG
	CDebugProcDX9::Print("[CGame.cpp]\n");
	CDebugProcDX9::Print("[ENTER]:�t�F�[�Y�J��\n");
#endif

	m_pPlayer->Update();

	CAnton *ant = m_pPlayer->GetAnton();

	if (ant->GetState() == CAnton::STATE_MINIMUM)
	{
		HitCheckMinimumAnton();
	}
	else
	{
		HitCheckAnton();
	}
}

// �~�j�}���p�̂����蔻��B�������ʂ̂Ɠ����\�[�X�Ŏ����ł���
void CGame::HitCheckMinimumAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();
	D3DXVECTOR3 workAntonSize;
	workAntonSize.x = 10.0f;
	workAntonSize.y = ant->GetParameter().size.y;
	workAntonSize.z = 0.0f;


	//Hack �������G�Ȃ����蔻��
	for (int cnt = 0; cnt < 63; cnt++)
	{
		//�A���g��
		D3DXVECTOR3 left_ant = ant->GetPosition();

		left_ant.x -= workAntonSize.x;
		left_ant.y -= workAntonSize.y / 2;

		//�u���b�N
		D3DXVECTOR3 left_b = Block[cnt]->GetPosition();
		left_b.x -= 50;
		left_b.y -= 50;

		if (CheckCollisionRectVsRect(left_ant, workAntonSize, left_b, D3DXVECTOR3(100, 100, 0)) && left_b.y - left_ant.y >= 0)
		{
			D3DXVECTOR3 set_pos = Block[cnt]->GetPosition();
			float fAntonSize = workAntonSize.y * 0.75f;			// 0.5f���Ɣ����߂荞�ނ̂ł���ɂ������������グ��
			set_pos.x = ant->GetPosition().x;
			set_pos.y -= fAntonSize + 50;
			ant->SetPosition(set_pos);
		}
	}

	for (int cnt = 0; cnt < 20; cnt++)
	{
		if (SpBlock[cnt] == NULL)
		{
			continue;
		}
		//�A���g��
		D3DXVECTOR3 left_ant = ant->GetPosition();

		left_ant.x -= workAntonSize.x;
		left_ant.y -= workAntonSize.y / 2;

		//�u���b�N
		D3DXVECTOR3 left_b = SpBlock[cnt]->GetPosition();
		left_b.x -= 50;
		left_b.y -= 50;

		if (CheckCollisionRectVsRect(left_ant, workAntonSize, left_b, D3DXVECTOR3(100, 100, 0)) && left_b.y - left_ant.y >= 0)
		{
				D3DXVECTOR3 set_pos = SpBlock[cnt]->GetPosition();
				float fAntonSize = workAntonSize.y * 0.75f;			// 0.5f���Ɣ����߂荞�ނ̂ł���ɂ������������グ��
				set_pos.x = ant->GetPosition().x;
				set_pos.y -= fAntonSize + 50;
				ant->SetPosition(set_pos);
		}
	}

}
void CGame::HitCheckAnton(void)
{
	CAnton *ant = m_pPlayer->GetAnton();

	//Hack �������G�Ȃ����蔻��
	for (int cnt = 0; cnt < 63; cnt++)
	{
		//�A���g��
		D3DXVECTOR3 left_ant = ant->GetPosition();

		left_ant.x -= 10;
		left_ant.y -= 60;

		//�u���b�N
		D3DXVECTOR3 left_b = Block[cnt]->GetPosition();
		left_b.x -= 50;
		left_b.y -= 50;

		if (CheckCollisionRectVsRect(left_ant, D3DXVECTOR3(10, 120, 0), left_b, D3DXVECTOR3(100, 100, 0)) && left_b.y - left_ant.y >= 0)
		{
			D3DXVECTOR3 set_pos = Block[cnt]->GetPosition();
			set_pos.x = ant->GetPosition().x;
			set_pos.y -= 60 + 50;
			ant->SetPosition(set_pos);
		}
	}

	for (int cnt = 0; cnt < 20; cnt++)
	{
		if (SpBlock[cnt] == NULL)
		{
			continue;
		}
		//�A���g��
		D3DXVECTOR3 left_ant = ant->GetPosition();

		left_ant.x -= 10;
		left_ant.y -= 60;

		//�u���b�N
		D3DXVECTOR3 left_b = SpBlock[cnt]->GetPosition();
		left_b.x -= 50;
		left_b.y -= 50;

		if (CheckCollisionRectVsRect(left_ant, D3DXVECTOR3(10, 120, 0), left_b, D3DXVECTOR3(100, 100, 0)) && left_b.y - left_ant.y >= 0)
		{
			if (ant->GetMass() >= 50)
			{
				SpBlock[cnt]->Uninit();
				SpBlock[cnt] = NULL;
			}
			else
			{
				D3DXVECTOR3 set_pos = SpBlock[cnt]->GetPosition();
				set_pos.x = ant->GetPosition().x;
				set_pos.y -= 60 + 50;
				ant->SetPosition(set_pos);
			}
		}
	}

}
