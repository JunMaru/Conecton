/*-----------------------------------------------------------------------------
	サウンドユーティリティ
	author	OkadaMoeto
	since	20140616
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CSoundXAudio2.h"
#include "Define.h"

/*-----------------------------------------------------------------------------
	静的メンバ変数の初期化
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
	コンストラクタ
-----------------------------------------------------------------------------*/
CSoundXAudio2::CSoundXAudio2()
{
	m_pXAudio2 = NULL;
	m_pMasteringVoice = NULL;
}

/*-----------------------------------------------------------------------------
	デストラクタ
-----------------------------------------------------------------------------*/
CSoundXAudio2::~CSoundXAudio2()
{
}

/*-----------------------------------------------------------------------------
	XAudio2の初期化
-----------------------------------------------------------------------------*/
HRESULT CSoundXAudio2::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(
					hWnd,
					"XAudio2オブジェクトの作成に失敗！",
					"WARNING(警告)",
					MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告", MB_ICONWARNING);

		if(m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < SL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
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
						"サウンドデータファイルの生成に失敗！(1)",
						"警告",
						MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// ファイルポインタを先頭に移動
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			MessageBox(
						hWnd,
						"サウンドデータファイルの生成に失敗！(2)",
						"警告",
						MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"WAVEファイルのチェックに失敗！(1)",
						"警告",
						MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"WAVEファイルのチェックに失敗！(2)",
						"警告",
						MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(
						hWnd,
						"WAVEファイルのチェックに失敗！(3)",
						"警告",
						MB_ICONWARNING);
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"フォーマットチェックに失敗！(1)",
						"警告",
						MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"フォーマットチェックに失敗！(2)",
						"警告",
						MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(
						hFile,
						'atad',
						&m_sizeAudio[nCntSound],
						&dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"オーディオデータ読み込みに失敗！(1)",
						"警告",
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
						"オーディオデータ読み込みに失敗！(2)",
						"警告",
						MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(
											&m_pSourceVoice[nCntSound],
											&(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(
						hWnd,
						"ソースボイスの生成に失敗！",
						"警告",
						MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_sizeAudio[nCntSound];
		buffer.pAudioData = m_pDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = 0;

		// オーディオバッファの登録
		m_pSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
	XAudio2の終了
-----------------------------------------------------------------------------*/
void CSoundXAudio2::Uninit(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SL_MAX; nCntSound++)
	{
		if(m_pSourceVoice[nCntSound])
		{
			// 一時停止
			m_pSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			m_pSourceVoice[nCntSound]->DestroyVoice();
			m_pSourceVoice[nCntSound] = NULL;
	
			// オーディオデータの開放
			free(m_pDataAudio[nCntSound]);
			m_pDataAudio[nCntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;
	
	if(m_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

/*-----------------------------------------------------------------------------
	セグメント再生
-----------------------------------------------------------------------------*/
HRESULT CSoundXAudio2::Play(SOUND_LABEL label)
{
#ifdef ON_SOUND
	// ステート
	XAUDIO2_VOICE_STATE xa2state;
	// バッファ
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_sizeAudio[label];
	buffer.pAudioData = m_pDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	// ループ再生
	if(m_param[label].bLoop == true)
	{
		buffer.LoopCount =  XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		// ループ再生なし
		buffer.LoopCount = 0;
	}

	// 状態取得
	m_pSourceVoice[label]->GetState(&xa2state);

	// 再生中
	if(xa2state.BuffersQueued != 0)
	{
		// 一時停止
		m_pSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_pSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_pSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_pSourceVoice[label]->Start(0);

#endif
	return S_OK;
}

/*-----------------------------------------------------------------------------
	セグメント停止
-----------------------------------------------------------------------------*/
void CSoundXAudio2::Stop(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_pSourceVoice[label]->GetState(&xa2state);

	// 再生中
	if(xa2state.BuffersQueued != 0)
	{
		// 一時停止
		m_pSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_pSourceVoice[label]->FlushSourceBuffers();
	}
}

/*-----------------------------------------------------------------------------
	セグメント全停止
-----------------------------------------------------------------------------*/
void CSoundXAudio2::AllStop(void)
{
	for(int nCntSound = 0; nCntSound < SL_MAX; nCntSound++)
	{
		if(m_pSourceVoice[nCntSound])
		{
			// 一時停止
			m_pSourceVoice[nCntSound]->Stop(0);
		}
	}
}

/*-----------------------------------------------------------------------------
	チャンクのチェック
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
	
	// ファイルポインタを先頭に移動
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		// チャンクの読み込み
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		// チャンクデータの読み込み
		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;

			// ファイルタイプの読み込み
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			// ファイルポインタをチャンクデータ分移動
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
	チャンクデータの読み込み
-----------------------------------------------------------------------------*/
HRESULT CSoundXAudio2::ReadChunkData(
										HANDLE hFile,
										void *pBuffer,
										DWORD dwBuffersize,
										DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	// ファイルポインタを指定位置まで移動
	if(SetFilePointer(
						hFile,
						dwBufferoffset,
						NULL,
						FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// データの読み込み
	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}
