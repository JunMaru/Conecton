/*-----------------------------------------------------------------------------
	�G���g���|�C���g(�E�B���h�E����)����
	author	OkadaMoeto
	since	20140414
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	���������[�N�`�F�b�N�Ď��p
-----------------------------------------------------------------------------*/
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "Main.h"
#include <string>
#include "Define.h"
#include "Resource.h"

/*-----------------------------------------------------------------------------
	�}�N����`
-----------------------------------------------------------------------------*/
using std::string;

// �E�C���h�E�̃N���X��
static const string CLASS_NAME = "AppClass";
// �E�C���h�E�̃L���v�V������
static const string WINDOW_NAME = "���˂��Ƃ�";
// �t���[�����[�g
static const float FRAME_RATE = 1000.0f / 30.0f;

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
// FPS�J�E���^
static int g_countFPS = 0;

/*-----------------------------------------------------------------------------
	FPS�擾
-----------------------------------------------------------------------------*/
int GetFPS()
{
	return g_countFPS;
}

/*-----------------------------------------------------------------------------
	�G���g���|�C���g
-----------------------------------------------------------------------------*/
int APIENTRY WinMain(
						HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPSTR lpCmdLine,
						int nCmdShow
)
{
	// �A�v���P�[�V�����̏I�����Ƀ��������[�N�`�F�b�N(�o�͂̃f�o�b�O���Q��)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �����Ă��ǂ����ǁA�x�����o��
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	// �V�X�e���^�C�}�[�̕���\���P�����ɐݒ�
	timeBeginPeriod(1);

	// �E�B���h�E�\���̂̓��e�ݒ�
	WNDCLASSEX	wcex = {
						sizeof(WNDCLASSEX),									// WNDCLASSEX�̃������T�C�Y���w��
						CS_CLASSDC,											// �\������E�C���h�E�̃X�^�C����ݒ�
						WndProc,											// �E�B���h�E�v���V�[�W���̃A�h���X���w��
						0,													// �ʏ�͎g�p���Ȃ��̂�"0"���w��
						0,													// �ʏ�͎g�p���Ȃ��̂�"0"���w��
						hInstance,											// WinMain�̃p�����[�^�̃C���X�^���X�n���h����ݒ�
						LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1)),			// �g�p����A�C�R�����w��(Windows�������Ă���A�C�R�����g���Ȃ�NULL)
						LoadCursor(NULL, IDC_ARROW),						// �}�E�X�J�[�\�����w��
						(HBRUSH)(COLOR_WINDOW + 1),							// �E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
						NULL,												// Window�ɂ��郁�j���[��ݒ�
						CLASS_NAME.c_str(),									// �E�C���h�E�N���X�̖��O
						LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),	// �������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	};
	
	// �E�B���h�E�̃n���h��(���ʎq)
	HWND hWnd;
	// ���b�Z�[�W
	MSG msg;
	// �N���C�A���g�̈�
	RECT rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	// �E�B���h�E�T�C�Y�Œ�, �E�B���h�E�̍ő剻�g�p�s��
	DWORD style = WS_OVERLAPPEDWINDOW &~ WS_THICKFRAME &~ WS_MAXIMIZEBOX;
	// �E�B���h�E�̕�
	int windowWidth;
	// �E�B���h�E�̍���
	int windowHeight;
	// �E�B���h�E��X���W
	int windowX;
	// �E�B���h�E��Y���W
	int windowY;

	// �`��̈���w�肷��
	AdjustWindowRect(&rect, style, FALSE);

	// �E�B���h�E�̍��W�𒆐S�ɂ���ʒu���Z�o����
	windowWidth = rect.right - rect.left;
	windowHeight = rect.bottom - rect.top;
	windowX = ((GetSystemMetrics(SM_CXFULLSCREEN) - (windowWidth)) / 2);
	windowY = ((GetSystemMetrics(SM_CYFULLSCREEN) - (windowHeight)) / 2);

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(
							0,						// �g���E�B���h�E�X�^�C��
							CLASS_NAME.c_str(),		// �E�B���h�E�N���X�̖��O
							WINDOW_NAME.c_str(),	// �E�B���h�E�̖��O
							style,					// �E�B���h�E�X�^�C��
							windowX,				// �E�B���h�E�̍���w���W
							windowY,				// �E�B���h�E�̍���x���W
							windowWidth,			// �E�B���h�E�̕�
							windowHeight,			// �E�B���h�E�̍���
							NULL,					// �e�E�B���h�E�̃n���h��
							NULL,					// ���j���[�n���h���܂��͎q�E�B���h�EID
							hInstance,				// �C���X�^���X�n���h��
							NULL					// �E�B���h�E�쐬�f�[�^
	);

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// �Q�[���Ǘ��҂𐶐�
	CManager* pManager = NULL;
	pManager = new CManager();
	pManager->Init(hInstance, hWnd, TRUE);

	// �V�X�e���������~���b�P�ʂŎ擾
	dwExecLastTime	= 
	dwFPSLastTime	= timeGetTime();
	// �t���[���J�E���g������
	dwCurrentTime	= 
	dwFrameCount	= 0;
	
	// ���b�Z�[�W���[�v
	while(1)
	{
		// ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windows�̏���
			if(msg.message == WM_QUIT)
			{
				// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);	// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);	// �E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{
			// ���݂̃V�X�e���������擾(�^�C�}�[�l)
			dwCurrentTime = timeGetTime();
			// 0.5�b���ƂɎ��s���Čv��
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// FPS�𑪒�
				g_countFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
		
				// �^�C�}�[�l���X�V
				dwFPSLastTime = dwCurrentTime;
				// �t���[���J�E���^�����Z�b�g
				dwFrameCount = 0;
			}
			// 1/60�b���ƂɎ��s(60FPS)
			if((dwCurrentTime - dwExecLastTime) >= FRAME_RATE)
			{
#ifdef _DEBUG
				// FPS�o��
				CDebugProcDX9::Print("�f�o�b�O���\n");
				CDebugProcDX9::Print("FPS:%d\n", g_countFPS);
#endif
				// �^�C�}�[�l���X�V
				dwExecLastTime = dwCurrentTime;

				pManager->Update();
				
				pManager->Draw();

				// �t���[���J�E���g���J�E���g�A�b�v
				dwFrameCount++;
			}
		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME.c_str(), wcex.hInstance);

	// �V�X�e���^�C�}�[�̕���\�����ɖ߂�
 	timeEndPeriod(1);

	// �Q�[���Ǘ��҂̉��
	if(pManager)
	{
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	return (int)msg.wParam;
}

/*-----------------------------------------------------------------------------
	�E�B���h�E�v���V�[�W��
-----------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		// �E�B���h�E������ꂽ��
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		case WM_KEYDOWN:
		{
			switch(wParam)
			{
				// ESC�L�[�������ꂽ��E�B���h�E���폜
				case VK_ESCAPE:
				{
					DestroyWindow(hWnd);
					break;
				}
			}
			break;
		}

		default:
		{
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
