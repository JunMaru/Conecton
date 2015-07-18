/*-----------------------------------------------------------------------------
	�f�o�b�O�\���N���X
	author	OkadaMoeto
	since	20140722
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�x���}��
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS // �Z�L���A�֐��Ɋւ��郏�[�j���O������

/*-----------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
-----------------------------------------------------------------------------*/
#include "CDebugProcDX9.h"
#include <stdio.h>
#include <stdarg.h>
#include "Define.h"
#include "CManager.h"
#include "CRenderer.h"

/*-----------------------------------------------------------------------------
	�ÓI�����o�ϐ��̏�����
-----------------------------------------------------------------------------*/
LPD3DXFONT CDebugProcDX9::m_pD3DXFont = nullptr;
char CDebugProcDX9::m_aStr[LENGTH_STRING_BUFF] = {"\0"};
bool CDebugProcDX9::m_bDisp = true;
int CDebugProcDX9::m_countFrame = 0;

/*-----------------------------------------------------------------------------
	�R���X�g���N�^
-----------------------------------------------------------------------------*/
CDebugProcDX9::CDebugProcDX9()
{
	// �o�b�t�@�N���A
	memset(m_aStr, 0, sizeof m_aStr);
}


/*-----------------------------------------------------------------------------
	�f�X�g���N�^
-----------------------------------------------------------------------------*/
CDebugProcDX9::~CDebugProcDX9()
{
}

/*-----------------------------------------------------------------------------
	������
-----------------------------------------------------------------------------*/
void CDebugProcDX9::Init(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���\���p�t�H���g��ݒ�
	D3DXCreateFont(
					pDevice,				// �f�o�C�X
					FONT_HEIGHT,			// �����̍���(0:��������)
					FONT_WIDTH,				// �����̕�
					FW_NORMAL,				// �t�H���g�̑���
					0,						// �~�j�}�b�v���x����
					FALSE,					// �C�^���b�N��
					SHIFTJIS_CHARSET,		// �����Z�b�g(�V�t�gJIS)
					OUT_DEFAULT_PRECIS,		// �o�͐��x
					DEFAULT_QUALITY,		// �o�͕i��
					DEFAULT_PITCH,			// �t�H���g�s�b�`�ƃt�@�~��
					"Terminal",				// �t�H���g��
					&m_pD3DXFont);			// �t�H���g�|�C���^�ւ̃A�h���X

	m_bDisp = false;
	m_countFrame = 0;
}

/*-----------------------------------------------------------------------------
	�I��
-----------------------------------------------------------------------------*/
void CDebugProcDX9::Uninit(void)
{
	// ���\���p�t�H���g���
	if(m_pD3DXFont)
	{
		m_pD3DXFont->Release();
		m_pD3DXFont = nullptr;
	}
}

/*-----------------------------------------------------------------------------
	��������
	param	:*fmt	����������(printf�Ɠ��l)
	param	:...	�ό���
-----------------------------------------------------------------------------*/
void CDebugProcDX9::Print(char *fmt,...)
{
#ifdef _DEBUG
	va_list list;
	char *pCur;
	char aBuf[256]={"\0"};
	char aWk[32];
	
	char aConvertion[8] = {"\0"};
	char aPercent[8] = {"%."};
	char aFloating[8] = {"f"};
	char aDigit[8] = {"\0"};
	
	va_start(list, fmt);

	pCur = fmt;
	for( ; *pCur; ++pCur)
	{
		// '%'�̋L��(�����w��q)�܂ł�1�����o��
		if(*pCur != '%')
		{
			sprintf(aWk, "%c", *pCur);
		}
		else
		{
			// �����w��q�̊J�n'%'�̂��Ƃ̕ϊ��w��q�̈ʒu�ɂ��炷
			pCur++;

			switch(*pCur)
			{
				case 'd':
				{
					// �����^�ɌŒ�w��(va_arg)
					sprintf(aWk, "%d", va_arg(list, int));
					break;
				}

				case 'f':
				{
					// ��������(double�^�Ŏw��)
					sprintf(aWk, "%f", va_arg(list, double));
					break;
				}

				case 's':
				{	
					// ������
					sprintf(aWk, "%s", va_arg(list, char*));
					break;
				}

				case 'c':
				{
					// ����
					sprintf(aWk, "%c", va_arg(list, char));
					break;
				}

				default:
				{	
					sprintf(aWk, "%c", *pCur);
					break;
				}
			}
		}
		strcat(aBuf, aWk);
	}

	va_end(list);

	if((strlen(m_aStr) + strlen(aBuf)) < sizeof m_aStr - 1)
	{
		strcat(m_aStr, aBuf);
	}
#endif
}

/*-----------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------*/
void CDebugProcDX9::Draw(void)
{
#ifdef _DEBUG
	m_countFrame++;
	if(m_countFrame > GetFPS())
	{
		m_countFrame = 0;
	}

	if(m_bDisp)
	{
		RECT rect = {0, 0, (LONG)(SCREEN_WIDTH), (LONG)(SCREEN_HEIGHT)};

		m_pD3DXFont->DrawText(
								nullptr,
								m_aStr,
								-1,
								&rect,
								DT_LEFT,
								COL_BLACK);

		// �f�o�b�O���̃o�b�t�@�N���A
		memset(m_aStr, 0, sizeof m_aStr);
	}
#endif
}
