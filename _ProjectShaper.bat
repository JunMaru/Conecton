@echo off
rem �F�X����
title ProjectShaper
rem ��������
set /p pushKey="VSProject���V�F�C�v�A�b�v���܂��B��낵���ł����H(y/n)"
if "%pushkey%" == "y" goto KILL
if "%pushkey%" == "Y" goto KILL
if "%pushkey%" == "yes" goto KILL
if "%pushkey%" == "YES" goto KILL

rem NO -> EXT

goto EXT

rem Delete for Folder and object
:KILL
rem Debug or debug
for /r %%A in ( debug ) do ( if exist "%%A" ( rmdir /s /q "%%A"))
for /r %%B in ( release ) do ( if exist "%%B" ( rmdir /s /q "%%B"))
for /r %%E in ( *.suo ) do ( if exist "%%E" ( del /s /q "%%E"))
for /r %%F in ( *.user ) do ( if exist "%%F" ( del /s /q "%%F"))
for /r %%G in ( *.sdf ) do ( if exist "%%G" ( del /s /q "%%G"))
for /r %%M in ( *.opensdf ) do ( if exist "%%M" ( del /s /q "%%M"))
for /r %%N in ( Thumbs.db ) do ( if exist "%%N" ( del /s /q "%%N"))


goto EXT


:EXT