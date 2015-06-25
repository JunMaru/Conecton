#pragma once
#include "CScene2D.h"
class CAntonIconUI : public CScene2D
{
public:
	enum ICONTYPE
	{
		ICONTYPE_NORMAL = 0,
		ICONTYPE_METAL,
		ICONTYPE_MINIMUM,
		ICONTYPE_POWERFUL,
		ICONTYPE_MAX,
	};

	CAntonIconUI(int priority = 6,OBJTYPE object_type = OBJTYPE_2D);
	~CAntonIconUI();

	void Uninit(void);
	void Draw(void);

	static CAntonIconUI* Create(D3DXVECTOR3 pos);
	void SetIconType(ICONTYPE icontype){ m_iconType = icontype; }

private:
	void PrepareTextures(void);
	ICONTYPE m_iconType;
	LPDIRECT3DTEXTURE9 m_pIconTextures[ICONTYPE_MAX];
};

