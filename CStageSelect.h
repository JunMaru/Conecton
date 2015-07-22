/*-----------------------------------------------------------------------------
	ステージセレクト画面クラス
	author	OkadaMoeto
	since	20150720
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "CPhase.h"
#include "CStageConfig.h"

/*-----------------------------------------------------------------------------
	Contentクラス
-----------------------------------------------------------------------------*/
class CScene2D;
class CInputCommand;
class CStageSelect : public CPhase
{
	public:
		CStageSelect(){}
		virtual ~CStageSelect(){}

		/*-----------------------------------------------------------------------------
			初期化
		-----------------------------------------------------------------------------*/
		void Init(void);

		/*-----------------------------------------------------------------------------
			終了
		-----------------------------------------------------------------------------*/
		void Uninit(void);

		/*-----------------------------------------------------------------------------
			更新
		-----------------------------------------------------------------------------*/
		void Update(void);

	private:
		void CrateStageInfoUI(void);
		void CreateObjectSS(void);
		void CreateSelectUI(void);
		void CreateObjectCharacter(void);
		void InitSelect(void);
		void InitAnimationCharacter(void);
		bool IsSelected(void);
		void UpdateInputEvent(void);
		void CommandSelect(void);
		void CommandLeft(void);
		void CommandRight(void);
		void UpdateStageNumUI(void);
		void UpdateSS(void);
		void UpdateAnimationBeecon(void);
		void UpdateAnimationAnton(void);
		void MoveCharacter(void);
		void BlinkSelect(float flashTime);
		void PlayBGM(void);
		void StopBGM(void);

		CInputCommand *m_pInputCommand;

		CScene2D *m_pStageSelectBg;
		CScene2D *m_pInfoPanel;
		CScene2D *m_pFrameSS;
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

		enum AnimListBeecon
		{
			ALB_WAIT = 0,
			ALB_MAX
		};
		AnimListBeecon m_selectAnimBeecon;

		enum AnimListAnton
		{
			ALA_WAIT = 0,
			ALA_MAX
		};
		AnimListAnton m_selectAnimAnton;

		struct AnimInfoSimple
		{
			int wait;
			D3DXVECTOR2 uv[4];
		};

		AnimInfoSimple m_beeconAnimWait[8];
		bool m_bRoopStopBeecon;
		float m_countAnimBeecon;
		int m_idxAnimBeecon;

		AnimInfoSimple m_antonAnimWait[8];
		bool m_bRoopStopAnton;
		float m_countAnimAnton;
		int m_idxAnimAnton;

		// 番号（0～3）でステージ１～４を識別
		int m_selectStage;
		bool m_bSelected;

		float m_countBlink;

		bool m_bChange;
		float m_countChange;
};
