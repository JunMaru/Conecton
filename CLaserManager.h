//=============================================================================
//
// CLaserManager 定義部分[ CLaserManager.h ]
// Author : Yutaka Ichikawa
//
//=============================================================================

// ２重インクルード防止
#pragma once

//=============================================================================
// インクルード
//=============================================================================
#include "Main.h"

//=============================================================================
// マクロ
//=============================================================================
#define MAX_LASER_NUM ( 10 )

//=============================================================================
// 前方定義
//=============================================================================
class CLaser;

//=============================================================================
// クラス定義
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

	// レーザー生成処理
	void CreateLaser(const D3DXVECTOR3& start_pos, const int& direction);

	static int GetCurrentLaser(void){ return m_nCurrentLaser; }
	static CLaser *GetLaser(int n_idx){ return m_pLaserArray[n_idx]; }

private:
	static CLaser *m_pLaserArray[MAX_LASER_NUM];

	static int m_nCurrentLaser;
};

// End of file