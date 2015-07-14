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
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pD3DTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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