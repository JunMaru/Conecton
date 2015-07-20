/*-----------------------------------------------------------------------------
	�X�e�[�W�Z���N�g��ʃN���X
	author	OkadaMoeto
	since	20150720
-----------------------------------------------------------------------------*/

// �Q�d�C���N���[�h�h�~
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "main.h"
#include "CPhase.h"
#include "CStageConfig.h"

/*-----------------------------------------------------------------------------
	Content�N���X
-----------------------------------------------------------------------------*/
class CScene2D;
class CInputCommand;
class CStageSelect : public CPhase
{
	public:
		CStageSelect(){}
		virtual ~CStageSelect(){}

		/*-----------------------------------------------------------------------------
			������
		-----------------------------------------------------------------------------*/
		void Init(void);

		/*-----------------------------------------------------------------------------
			�I��
		-----------------------------------------------------------------------------*/
		void Uninit(void);

		/*-----------------------------------------------------------------------------
			�X�V
		-----------------------------------------------------------------------------*/
		void Update(void);

	private:
		void CrateStageInfoUI(void);
		void CreateObjectSS(void);
		void CreateSelectUI(void);
		void CreateObjectCharacter(void);
		void InitSelect(void);
		bool IsSelected(void);
		void UpdateInputEvent(void);
		void CommandSelect(void);
		void CommandLeft(void);
		void CommandRight(void);
		void UpdateStageNumUI(void);
		void UpdateSS(void);
		void MoveCharacter(void);

		CInputCommand *m_pInputCommand;

		CScene2D *m_pStageSelectBg;
		CScene2D *m_pInfoPanel;
		CScene2D *m_pStage1SS;
		CScene2D *m_pStage2SS;
		CScene2D *m_pStage3SS;
		CScene2D *m_pStage4SS;
		CScene2D *m_pStageText;
		CScene2D *m_pStageNumText;
		CScene2D *m_pSelectScaffold;
		CScene2D *m_pSelectButton;
		CScene2D *m_pSelectText;
		CScene2D *m_pBeecon;
		CScene2D *m_pAnton;

		// �ԍ��i0�`3�j�ŃX�e�[�W�P�`�S������
		int m_selectStage;
		bool m_bSelected;
};
