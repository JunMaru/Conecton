/*-----------------------------------------------------------------------------
 �o�b�N�O���E���h�N���X
 author Jun Maruyama 
 since 20150519
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 �C���N���[�h
-----------------------------------------------------------------------------*/
#include "CBackGround.h"
#include "Define.h"
#include "CScrollManager.h"

/*-----------------------------------------------------------------------------
 �R���X�g���N�^
-----------------------------------------------------------------------------*/
CBackGround::CBackGround(int priority, CScene::OBJTYPE objtype)
	: CScene2D(priority, objtype)
{
	m_scrollVol = VEC2_ZERO;
	m_scrollSpeed = 0.01f;
}

/*-----------------------------------------------------------------------------
 �f�X�g���N�^
-----------------------------------------------------------------------------*/
CBackGround::~CBackGround(void)
{

}

/*-----------------------------------------------------------------------------
 �`��
-----------------------------------------------------------------------------*/
void CBackGround::Draw(void)
{
	m_scrollVol = CScrollManager::GetScrollWorld();
	D3DXVECTOR2 scrollOffset = m_scrollVol * m_scrollSpeed;

	D3DXVECTOR2 texcoord[4] = { D3DXVECTOR2(0.0f + scrollOffset.x, 0.0f + scrollOffset.y),			// ����
								D3DXVECTOR2(1.0f + scrollOffset.x, 0.0f + scrollOffset.y),			// �E��
								D3DXVECTOR2(0.0f + scrollOffset.x, 1.0f + scrollOffset.y),			// ����
								D3DXVECTOR2(1.0f + scrollOffset.x, 1.0f + scrollOffset.y),			// �E��
								};

	SetTexcoord(texcoord[0], texcoord[1], texcoord[2], texcoord[3]);

	CScene2D::Draw();
}

/*-----------------------------------------------------------------------------
 ��������
-----------------------------------------------------------------------------*/
CBackGround* CBackGround::Create(const char *file_name, int priority)
{
	CBackGround* pBackGround = new CBackGround(priority, CScene::OBJTYPE_2D);
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);

	pBackGround->Init(file_name,pos,VEC3_ZERO,COL_WHITE,SCREEN_WIDTH,SCREEN_HEIGHT);

	return pBackGround;
}