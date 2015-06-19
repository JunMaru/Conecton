/*-----------------------------------------------------------------------------
	�I�u�W�F�N�g�N���X
	author	OkadaMoeto
	since	20140416
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CScene.h"
#include "CManager.h"

/*-----------------------------------------------------------------------------
	�ÓI�����o�ϐ��̏�����
-----------------------------------------------------------------------------*/
CScene* CScene::m_pTop[NUM_PRIORITY] = {};
CScene* CScene::m_pCur[NUM_PRIORITY] = {};
int CScene::m_countScene = 0;

/*-----------------------------------------------------------------------------
*
	���X�g�擪�擾
	return	:CScene	���X�g�̐擪��Ԃ�
	param	:priority	�`�惌�C���ԍ�
*
-----------------------------------------------------------------------------*/
CScene* CScene::GetListTop(int priority)
{
	return m_pTop[priority];
}

/*-----------------------------------------------------------------------------
*
	�R���X�g���N�^
*
-----------------------------------------------------------------------------*/
CScene::CScene(int priority, OBJTYPE objtype)
{
	// �z��O�̐��l�͊ۂ߂�
	if((priority < 0) || (priority >= NUM_PRIORITY))
	{
		priority = 3;
	}

	LinkList(priority);
	
	m_objtype = objtype;
	m_bDelete = false;
	m_bGlobal = false;
	m_bUpdate = true;
	m_bDraw = true;
	m_countScene++;
}

/*-----------------------------------------------------------------------------
*
	�f�X�g���N�^
*
-----------------------------------------------------------------------------*/
CScene::~CScene()
{
}

/*-----------------------------------------------------------------------------
*
	���X�g���̑S�X�V
*
-----------------------------------------------------------------------------*/
void CScene::UpdateAll(void)
{
	for(int i=0; i<NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			// �I�u�W�F�N�g�̍X�V����
			if(pScene->m_bUpdate)
			{
				pScene->Update();
			}

			pScene = pSceneNext;
		}
	}

	// �f�X�t���OON�Ȃ�΍X�V��ɃI�u�W�F�N�g���������
	for(int i = 0; i < NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			if(pScene->m_bDelete)
			{
				// ���X�g�����玩�����g���폜
				pScene->UnlinkList();

				delete pScene;
				pScene = nullptr;

				m_countScene--;
			}

			pScene = pSceneNext;
		}
	}
}

/*-----------------------------------------------------------------------------
*
	���X�g���̑S�`��
*
-----------------------------------------------------------------------------*/
void CScene::DrawAll(void)
{
	for(int i=0; i<NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			// �`�悵�Ȃ��ꍇ�͂Ƃ΂�
			if(pScene->m_bDraw)
			{
				// �I�u�W�F�N�g�̕`�揈��
				pScene->Draw();
			}

			pScene = pSceneNext;
		}
	}
}

/*-----------------------------------------------------------------------------
*
	���X�g���̃f�X�t���O��L���ɂ���
*
-----------------------------------------------------------------------------*/
void CScene::Release(void)
{
	// �f�X�t���OON
	m_bDelete = true;
}

/*-----------------------------------------------------------------------------
*
	���X�g���̑S���
*
-----------------------------------------------------------------------------*/
void CScene::ReleaseAll(void)
{
	for(int i=0; i<NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			// �I�u�W�F�N�g�𑶑������邽�߂ɍ폜���Ȃ��ꍇ�͏I���������Ƃ΂�
			if(!pScene->m_bGlobal)
			{
				// �I�u�W�F�N�g�̏I������
				pScene->Uninit();
			}

			pScene = pSceneNext;
		}
	}

	for(int i=0; i<NUM_PRIORITY; i++)
	{
		CScene* pScene = m_pTop[i];

		while(pScene)
		{
			CScene* pSceneNext = pScene->m_pNext;

			if(pScene->m_bDelete)
			{
				// ���X�g����폜
				pScene->UnlinkList();

				// �I�u�W�F�N�g�̉��
				delete pScene;
				pScene = nullptr;
			}

			pScene = pSceneNext;
		}
	}
}

/*-----------------------------------------------------------------------------
*
	���X�g�ɓo�^
	param	:nPriorty	�`�惌�C���ԍ�
*
-----------------------------------------------------------------------------*/
void CScene::LinkList(int priority)
{
	// �v���C�I���e�B�ԍ����擾
	m_drawLayerPriority = priority;

	// ���X�g�̐擪(Top)�����݂��Ȃ��ꍇ
	if(!m_pTop[m_drawLayerPriority])
	{
		m_pTop[m_drawLayerPriority] = this;
	}

	// ���݂̃I�u�W�F�N�g(Cur)�����݂��Ȃ��ꍇ
	if(!m_pCur[m_drawLayerPriority])
	{
		m_pCur[m_drawLayerPriority] = this;
	}

	// ���݂̃I�u�W�F�N�g(Cur)�̎��̃I�u�W�F�N�g������(this)�ɂ���
	m_pCur[m_drawLayerPriority]->m_pNext = this;

	// ���݂̃I�u�W�F�N�g(Cur)������(this)�̏ꍇ
	if(m_pCur[m_drawLayerPriority] == this)
	{
		// ����(this)�̑O�̃I�u�W�F��NULL����
		m_pPrev = nullptr;
	}
	else
	{
		// ����(this)�̑O�̃I�u�W�F�N�g�����݂̃I�u�W�F�N�g(Cur)�ɂ���
		m_pPrev = m_pCur[m_drawLayerPriority];
	}

	// ���݂̃I�u�W�F�N�g(Cur)������(this)�ɂ���
	m_pCur[m_drawLayerPriority] = this;

	// ����(this)�̎��̃I�u�W�F��NULL�ɂ���
	m_pNext = nullptr;
}

/*-----------------------------------------------------------------------------
*
	���X�g����폜
*
-----------------------------------------------------------------------------*/
void CScene::UnlinkList(void)
{
	CScene* pScene,*pSceneNext,*pScenePrev;

	// ���X�g�̐擪(Top)���猻�݈ʒu(Cur)�Ɍ������ĒH���Ă���
	pScene = m_pTop[m_drawLayerPriority];

	while(pScene)
	{
		pSceneNext = pScene->m_pNext;

		// �Ώۂ̃I�u�W�F�N�g������(this)�̏ꍇ
		if(pScene == this)
		{
			// �Ώۂ̃I�u�W�F�N�g�̑O�ɃI�u�W�F�N�g�����݂���ꍇ
			if(pScene->m_pPrev)
			{	
				// ����(this)�̑O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g������(this)�̎��̃I�u�W�F�N�g�ɂ���
				pScene->m_pPrev->m_pNext = pScene->m_pNext;
			}

			// �Ώۂ̃I�u�W�F�N�g�̎��ɃI�u�W�F�N�g�����݂���ꍇ
			if(pScene->m_pNext)
			{
				// ����(this)�̎��̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g������(this)�̑O�̃I�u�W�F�N�g�ɂ���
				pScene->m_pNext->m_pPrev = pScene->m_pPrev;
			}

			// �Ώۂ̃I�u�W�F�N�g�����X�g�̐擪(Top)�̏ꍇ
			if(pScene == m_pTop[m_drawLayerPriority])
			{
				m_pTop[m_drawLayerPriority] = pSceneNext;
			}
		}

		pScene = pSceneNext;
	}

	// ���X�g�̌��݈ʒu(Cur)����擪(Top)�Ɍ������ĒH���Ă���
	pScene = m_pCur[m_drawLayerPriority];

	while(pScene)
	{
		pScenePrev = pScene->m_pPrev;

		// �Ώۂ̃I�u�W�F�N�g������(this)�̏ꍇ
		if(pScene == this)
		{
			// �Ώۂ̃I�u�W�F�N�g�̑O�ɃI�u�W�F�N�g�����݂���ꍇ
			if(pScene->m_pPrev)
			{
				// ����(this)�̑O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g������(this)�̎��̃I�u�W�F�N�g�ɂ���
				pScene->m_pPrev->m_pNext = pScene->m_pNext;
			}

			// �Ώۂ̃I�u�W�F�N�g�̎��ɃI�u�W�F�N�g�����݂���ꍇ
			if(pScene->m_pNext)
			{
				// ����(this)�̎��̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g������(this)�̑O�̃I�u�W�F�N�g�̂���
				pScene->m_pNext->m_pPrev = pScene->m_pPrev;
			}

			// �Ώۂ̃I�u�W�F�N�g�����݂̃I�u�W�F�N�g(Cur)�̏ꍇ
			if(pScene == m_pCur[m_drawLayerPriority])
			{
				m_pCur[m_drawLayerPriority] = pScenePrev;
			}
		}

		pScene = pScenePrev;
	}
}
