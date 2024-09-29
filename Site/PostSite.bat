:: Doxygen ../Doxyfile

@echo off
:: Define source and destination directories
set "source=ART_Docs\html"
set "destination=ART_Tutorials\static\ART_Docs"

:: Check if source directory exists
if not exist "%source%" (
    echo Source directory does not exist: %source%
    exit /b 1
)

:: Create destination directory if it does not exist
if not exist "%destination%" (
    echo Creating destination directory...
    md "%destination%" >nul 2>&1
)

:: Clear destination folder
echo Deleting destination folder contents...
rd /s /q "%destination%" >nul 2>&1
md "%destination%" >nul 2>&1

:: Copy files from source to destination
echo Copying files...
xcopy "%source%\*" "%destination%\" /s /e /y >nul 2>&1

:: Notify completion
echo Files copied from %source% to %destination%

set "GIT_USER=Jath-Alison"
cd ./ART_Tutorials
npm run deploy