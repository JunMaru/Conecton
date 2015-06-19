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
			SOUND_LABEL_BGM_000,
			SOUND_LABEL_SE_SHOT_000,
			SOUND_LABEL_MAX,
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
		IXAudio2SourceVoice *m_pSourceVoice[SOUND_LABEL_MAX];
		// �I�[�f�B�I�f�[�^
		BYTE *m_pDataAudio[SOUND_LABEL_MAX];
		// �I�[�f�B�I�f�[�^�T�C�Y
		DWORD m_sizeAudio[SOUND_LABEL_MAX];

		// ���f�B�A�̃p�����[�^( PARAM�\���̂̃t�H�[�}�b�g )
		static const PARAM m_param[SOUND_LABEL_MAX];
};
