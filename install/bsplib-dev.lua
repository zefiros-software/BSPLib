--[[ @cond ___LICENSE___
-- Copyright (c) 2016 Koen Visscher, Paul Visscher and individual contributors.
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.
--
-- @endcond
--]]

dofile( "assets/Zefiros-Software/BSPLib/Zefiros-Software/Anaconda/anaconda.lua" )

local anaBin = os.get() == "windows" and os.getenv("UserProfile") .. "/zpm-anaconda/Scripts/" or "~/zpm-anaconda/bin/"

if os.get() == "windows" then
    os.executef( "%spip install mkdocs -U", anaBin )
    os.executef( "%spip install mkdocs-bootswatch -U", anaBin )
    os.executef( "%spip install pymdown-extensions -U", anaBin )
    os.executef( "%spip install markdown-checklist -U", anaBin )
else
    os.executef( "%s/python3 %spip install mkdocs -U", anaBin, anaBin )
    os.executef( "%s/python3 %spip install mkdocs-bootswatch -U", anaBin, anaBin )
    os.executef( "%s/python3 %spip install pymdown-extensions -U", anaBin, anaBin )
    os.executef( "%s/python3 %spip install markdown-checklist -U", anaBin, anaBin )
end