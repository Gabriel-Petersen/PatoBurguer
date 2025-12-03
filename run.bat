@echo off
title Launcher do Jogo

echo ===========================================
echo        SELECIONE A BUILD PARA RODAR
echo ===========================================
echo 1 - normal_wFile.exe
echo 2 - dev_wFile.exe
echo 3 - normal_noFile.exe
echo 4 - dev_noFile.exe
echo ===========================================
set /p escolha="Digite o numero: "

if "%escolha%"=="1" set EXE=normal_wFile.exe
if "%escolha%"=="2" set EXE=dev_wFile.exe
if "%escolha%"=="3" set EXE=normal_noFile.exe
if "%escolha%"=="4" set EXE=dev_noFile.exe

if not exist "%EXE%" (
    echo ERRO: O arquivo %EXE% nao existe na pasta atual.
    echo Mova o .exe desejado para esta pasta e tente novamente.
    pause
    exit /b
)

REM ============================
REM Tenta abrir no Windows Terminal
REM ============================
where wt >nul 2>nul
if %errorlevel%==0 (
    wt "%~dp0%EXE%"
    exit /b
)

REM ============================
REM Cai para PowerShell se WT não existir
REM ============================
where powershell >nul 2>nul
if %errorlevel%==0 (
    powershell -Command "%~dp0%EXE%"
    exit /b
)

REM ============================
REM Último recurso → CMD mesmo
REM ============================
"%~dp0%EXE%"
