@echo off
cd ./bin
doxygen ../reference.doxy 
hhc.exe ../out/html/index.hhp 
cd "../out/html/"
move "PlotLib Reference.*" "../../"