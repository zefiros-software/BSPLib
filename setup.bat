start /wait powershell -NoProfile -ExecutionPolicy Bypass -Command "wget https://3230d63b5fc54e62148e-c95ac804525aac4b6dba79b00b39d1d3.ssl.cf1.rackcdn.com/Anaconda3-2.5.0-Windows-x86_64.exe -OutFile bin/Anaconda3-2.5.0-Windows-x86_64.exe "
start /wait bin/Anaconda3-2.5.0-Windows-x86_64.exe 
start /wait pip install seaborn
