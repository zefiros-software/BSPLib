@echo off
start /wait powershell -NoProfile -ExecutionPolicy Bypass -Command "iex ((new-object net.webclient).DownloadString('https://chocolatey.org/install.ps1'))" && SET PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin

start /wait cmd /c choco install python -y
start /wait cmd /c python ./get-pip.py
start /wait cmd /c pip install mkdocs
./build-docs.bat