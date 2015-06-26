#pragma once
#include "CScene.h"
class CScene2D;

class CAntonLifeUI : public CScene
{
public:

	static const int LIFE_MAX = 5;

	CAntonLifeUI();
	~CAntonLifeUI();

	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAntonLifeUI* Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPosition(void){ return m_pos; }
	void SetPosition(D3DXVECTOR3 pos){ m_pos = pos; }
	void SetPosition(float x, float y, float z){ m_pos = D3DXVECTOR3(x, y, z); }
	D3DXVECTOR3 GetRotation(void){ return m_rot; }
	void SetRotation(D3DXVECTOR3 rot){ m_rot = rot; }
	void SetRotation(float x, float y, float z){ m_rot = D3DXVECTOR3(x, y, z); }

	void AddLife(int nAdd);

private:
	HRESULT Init(void);

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;

	int m_nLife;
	CScene2D *m_pLifeHeartPolygon[LIFE_MAX];
	CScene2D *m_pLifeBaseLinePolygon;
};

