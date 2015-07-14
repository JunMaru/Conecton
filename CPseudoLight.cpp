/*-----------------------------------------------------------------------------
	�[�����C�g�N���X
	author Okada.Moeto
	since 20150714
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h
-----------------------------------------------------------------------------*/
#include "CPseudoLight.h"
#include "Define.h"

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CPseudoLight::CPseudoLight(int priority, CScene::OBJTYPE objtype)
	: CScene2D(priority, objtype)
{
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CPseudoLight::~CPseudoLight(void)
{

}

/*-----------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------*/
void CPseudoLight::Draw(void)
{
	CScene2D::Draw();
}

/*-----------------------------------------------------------------------------
	��������
-----------------------------------------------------------------------------*/
CPseudoLight* CPseudoLight::Create(const char *file_name, int priority)
{
	CPseudoLight* pObj = new CPseudoLight(priority, CScene::OBJTYPE_2D);
	
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);

	pObj->Init(
				file_name,
				pos,
				VEC3_ZERO,
				COL_WHITE,
				SCREEN_WIDTH,
				SCREEN_HEIGHT);

	return pObj;
}