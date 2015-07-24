/*-----------------------------------------------------------------------------
	�T�E���h���[�e�B���e�B
	author	OkadaMoeto
	since	20140616
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CSoundXAudio2.h"
#include "Define.h"

/*-----------------------------------------------------------------------------
	�ÓI�����o�ϐ��̏�����
-----------------------------------------------------------------------------*/
const PARAM CSoundXAudio2::m_param[SL_MAX] = 
{
	"data/testsample/bgm000.wav", true,
	"data/testsample/shot000.wav", false,
	"data/sound/bgm/BGM_Game.wav", true,
	"data/sound/bgm/BGM_Stage.wav", true,
	"data/sound/bgm/BGM_Title.wav", true,
	"data/sound/jingle/jingle_Clear.wav", false,
	"data/sound/jingle/jingle_Over.wav", false,
	"data/sound/se/Beecon_Connect.wav", false,
	"data/sound/se/Block_Brake.wav", false,
	"data/sound/se/Box_Slip.wav", false,
	"data/sound/se/Damage.wav", false,
	"data/sound/se/Food.wav", false,
	"data/sound/se/Kernel.wav", false,
	"data/sound/se/Laser_End.wav", false,
	"data/sound/se/Laser_Start.wav", false,
	"data/sound/se/Magnet.wav", false,
	"data/sound/se/Pi.wav", false,
	"data/sound/se/Picon.wav", false,
	"data/sound/se/Powerup_Metal.wav", false,
	"data/sound/se/Powerup_Minimum.wav", false,
	"data/sound/se/Powerup_Powerful.wav", false,
	"data/sound/se/Rot_Arrow.wav", false,
	"data/sound/se/Sandstorm.wav", false,
	"data/sound/se/UI_Enter.wav", false,
	"data/sound/se/Walk_Normal.wav", false,
	"data/sound/se/Walk_Metal.wav", false,
	"data/sound/se/Walk_Minimum.wav", false,
	"data/sound/se/Walk_Powerful.wav", false,
	"data/sound/se/Warp.wav", false,
};

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CSoundXAudio2::CSoundXAudio2()
{
	m_pXAudio2 = NULL;
	m_pMasteringVoice = NULL;
}

/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CSoundXAudio2::~CSoundXAudio2()
{
}

/*-----------------------------------------------------------------------------
	XAudio2�̏�����
-----------------------------------------------------------------------------*/
HRESULT CSoundXAudio2::Init(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(
					hWnd,
					"XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I",
					"WARNING(�x��)",
					MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();
		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x��", MB_ICONWARNING);

		if(m_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < SL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(
							m_param[nCntSound].pFilename,
							GENERIC_READ, FILE_SHARE_READ,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(
						hWnd,
						"�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)",
						"�x��",
						MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// �t�@�C���|�C���^��擪�Ɉړ�
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			MessageBox(
						hWnd,
						"�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)",
						"�x��",
						MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)",
						"�x��",
						MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)",
						"�x��",
						MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(
						hWnd,
						"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)",
						"�x��",
						MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)",
						"�x��",
						MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)",
						"�x��",
						MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(
						hFile,
						'atad',
						&m_sizeAudio[nCntSound],
						&dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)",
						"�x��",
						MB_ICONWARNING);
			return S_FALSE;
		}
		m_pDataAudio[nCntSound] = (BYTE*)malloc(m_sizeAudio[nCntSound]);
		hr = ReadChunkData(
							hFile,
							m_pDataAudio[nCntSound],
							m_sizeAudio[nCntSound],
							dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)",
						"�x��",
						MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = m_pXAudio2->CreateSourceVoice(
											&m_pSourceVoice[nCntSound],
											&(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"�\�[�X�{�C�X�̐����Ɏ��s�I",
						"�x��",
						MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_sizeAudio[nCntSound];
		buffer.pAudioData = m_pDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = 0;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_pSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
	XAudio2�̏I��
-----------------------------------------------------------------------------*/
void CSoundXAudio2::Uninit(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SL_MAX; nCntSound++)
	{
		if(m_pSourceVoice[nCntSound])
		{
			// �ꎞ��~
			m_pSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			m_pSourceVoice[nCntSound]->DestroyVoice();
			m_pSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(m_pDataAudio[nCntSound]);
			m_pDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;
	
	if(m_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

/*-----------------------------------------------------------------------------
	�Z�O�����g�Đ�
-----------------------------------------------------------------------------*/
HRESULT CSoundXAudio2::Play(SOUND_LABEL label)
{
#ifdef ON_SOUND
	// �X�e�[�g
	XAUDIO2_VOICE_STATE xa2state;
	// �o�b�t�@
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_sizeAudio[label];
	buffer.pAudioData = m_pDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	// ���[�v�Đ�
	if(m_param[label].bLoop == true)
	{
		buffer.LoopCount =  XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		// ���[�v�Đ��Ȃ�
		buffer.LoopCount = 0;
	}

	// ��Ԏ擾
	m_pSourceVoice[label]->GetState(&xa2state);

	// �Đ���
	if(xa2state.BuffersQueued != 0)
	{
		// �ꎞ��~
		m_pSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_pSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_pSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	m_pSourceVoice[label]->Start(0);

#endif
	return S_OK;
}

/*-----------------------------------------------------------------------------
	�Z�O�����g��~
-----------------------------------------------------------------------------*/
void CSoundXAudio2::Stop(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_pSourceVoice[label]->GetState(&xa2state);

	// �Đ���
	if(xa2state.BuffersQueued != 0)
	{
		// �ꎞ��~
		m_pSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_pSourceVoice[label]->FlushSourceBuffers();
	}
}

/*-----------------------------------------------------------------------------
	�Z�O�����g�S��~
-----------------------------------------------------------------------------*/
void CSoundXAudio2::AllStop(void)
{
	for(int nCntSound = 0; nCntSound < SL_MAX; nCntSound++)
	{
		if(m_pSourceVoice[nCntSound])
		{
			// �ꎞ��~
			m_pSourceVoice[nCntSound]->Stop(0);
		}
	}
}

/*-----------------------------------------------------------------------------
	�`�����N�̃`�F�b�N
-----------------------------------------------------------------------------*/
HRESULT CSoundXAudio2::CheckChunk(
									HANDLE hFile,
									DWORD format,
									DWORD *pChunkSize,
									DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	// �t�@�C���|�C���^��擪�Ɉړ�
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		// �`�����N�̓ǂݍ���
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		// �`�����N�f�[�^�̓ǂݍ���
		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;

			// �t�@�C���^�C�v�̓ǂݍ���
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
			if(SetFilePointer(
								hFile,
								dwChunkDataSize,
								NULL,
								FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

/*-----------------------------------------------------------------------------
	�`�����N�f�[�^�̓ǂݍ���
-----------------------------------------------------------------------------*/
HRESULT CSoundXAudio2::ReadChunkData(
										HANDLE hFile,
										void *pBuffer,
										DWORD dwBuffersize,
										DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	// �t�@�C���|�C���^���w��ʒu�܂ňړ�
	if(SetFilePointer(
						hFile,
						dwBufferoffset,
						NULL,
						FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// �f�[�^�̓ǂݍ���
	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}
