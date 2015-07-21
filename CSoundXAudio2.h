/*-----------------------------------------------------------------------------
	�T�E���h���[�e�B���e�B
	author	OkadaMoeto
	since	20140616
-----------------------------------------------------------------------------*/

// �C���N���[�h�K�[�h( ��d�C���N���[�h�h�~ )
#pragma once

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
	�p�����[�^�\����
-----------------------------------------------------------------------------*/
typedef struct
{
	char *pFilename;	// �T�E���h�t�@�C����
	bool bLoop;			// �Đ����̃��[�v�t���O
}PARAM;

/*-----------------------------------------------------------------------------
	�T�E���h(XAudio2)�N���X
-----------------------------------------------------------------------------*/
class CSoundXAudio2
{
	public:
		// �T�E���h�t�@�C�����x�����im_param�ɓ����Ƀt�@�C���p�X���L�q�j
		typedef enum
		{
			SL_BGM_TEST,
			SL_SE_SHOT_TEST,
			SL_BGM_GAME,
			SL_BGM_STAGESELECT,
			SL_BGM_TITLE,
			SL_JINGLE_CLEAR,
			SL_JINGLE_OVER,
			SL_SE_BEECON_CONNECT,
			SL_SE_BLOCK_BRAKE,
			SL_SE_BOX_SLIP,
			SL_SE_DAMAGE,
			SL_SE_KERNEL,
			SL_SE_LASER_END,
			SL_SE_LASER_START,
			SL_SE_MAGNET,
			SL_SE_PICON,
			SL_SE_POWERUP_METAL,
			SL_SE_POWERUP_MINI,
			SL_SE_POWERUP_POWERFUL,
			SL_SE_ROT_ARROW,
			SL_SE_SANDSTORM,
			SL_SE_ENTER,
			SL_SE_WALK_NORMAL,
			SL_SE_WALK_METAL,
			SL_SE_WALK_MINI,
			SL_SE_WALK_POWERFUL,
			SL_SE_WARP,
			SL_MAX,
		}SOUND_LABEL;

		CSoundXAudio2();
		~CSoundXAudio2();

		/*-----------------------------------------------------------------------------
			XAudio2�̏�����
			param	hWnd	�E�B���h�E�̃n���h��
		-----------------------------------------------------------------------------*/
		HRESULT Init(HWND hWnd);
		void Uninit(void);

		/*-----------------------------------------------------------------------------
			�T�E���h�Đ�
			param	label	�T�E���hID(SOUNDLABLE_xx)
		-----------------------------------------------------------------------------*/
		HRESULT Play(SOUND_LABEL label);

		/*-----------------------------------------------------------------------------
			�T�E���h��~
			param	label	�T�E���hID(SOUNDLABLE_xx)
		-----------------------------------------------------------------------------*/
		void Stop(SOUND_LABEL label);

		// �T�E���h�S��~
		void AllStop(void);

	private:
		/*-----------------------------------------------------------------------------
			�`�����N�̃`�F�b�N
			return	HRESULT				�`�F�b�N����
			param	hFile				�t�@�C���n���h��
			param	format				�t�H�[�}�b�g
			param	pChunkSize			�`�����N�̃T�C�Y
			param	pChunkDataPosition	�`�����N�̃f�[�^�̃f�B���N�g��
		-----------------------------------------------------------------------------*/
		HRESULT CheckChunk(
							HANDLE hFile,
							DWORD format,
							DWORD *pChunkSize,
							DWORD *pChunkDataPosition);

		/*-----------------------------------------------------------------------------
			�`�����N�f�[�^�̓ǂݍ���
			return	HRESULT			�Ǎ��݌���
			param	hFile			�t�@�C���n���h��
			param	pBuffer			�o�b�t�@�̃|�C���^
			param	dwBuffersize	�o�b�t�@�̃T�C�Y
			param	dwBufferoffset	�o�b�t�@�̃|�W�V����
		-----------------------------------------------------------------------------*/
		HRESULT ReadChunkData(
								HANDLE hFile,
								void *pBuffer,
								DWORD dwBuffersize,
								DWORD dwBufferoffset);

		// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
		IXAudio2 *m_pXAudio2;
		// �}�X�^�[�{�C�X
		IXAudio2MasteringVoice *m_pMasteringVoice;
		// �\�[�X�{�C�X
		IXAudio2SourceVoice *m_pSourceVoice[SL_MAX];
		// �I�[�f�B�I�f�[�^
		BYTE *m_pDataAudio[SL_MAX];
		// �I�[�f�B�I�f�[�^�T�C�Y
		DWORD m_sizeAudio[SL_MAX];

		// ���f�B�A�̃p�����[�^( PARAM�\���̂̃t�H�[�}�b�g )
		static const PARAM m_param[SL_MAX];
};
