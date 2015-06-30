//=============================================================================
//
// CLaserManager ��`����[ CLaserManager.h ]
// Author : Yutaka Ichikawa
//
//=============================================================================

// �Q�d�C���N���[�h�h�~
#pragma once

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "Main.h"

//=============================================================================
// �}�N��
//=============================================================================
#define MAX_LASER_NUM ( 10 )

//=============================================================================
// �O����`
//=============================================================================
class CLaser;

//=============================================================================
// �N���X��`
//=============================================================================
class CLaserManager
{
public:
	CLaserManager(){}
	~CLaserManager(){}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CLaserManager* Create(void);

	// ���[�U�[��������
	void CreateLaser(const D3DXVECTOR3& start_pos, int direction);

	static int GetCurrentLaser(void){ return m_nCurrentLaser; }
	static CLaser *GetLaser(int n_idx){ return m_pLaserArray[n_idx]; }

	bool GetLaserGoalFlag(void){ return m_bLaserGoalFlag; }

private:
	static CLaser *m_pLaserArray[MAX_LASER_NUM];

	bool m_bStopLaserFlag[MAX_LASER_NUM];

	static int m_nCurrentLaser;

	bool m_bLaserGoalFlag;
};

// End of file