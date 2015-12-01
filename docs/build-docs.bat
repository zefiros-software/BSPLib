copy ..\readme.md .\docs\index.md
copy ..\bsp.pdf .\docs/bsp.pdf
cmd /c start http://127.0.0.1:8000
mkdocs serve