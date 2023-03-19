@echo off

set CommonCompilerFlags=-MT -nologo -GR- -Gm- -EHa- -Od -Oi -WX -wd4456 -wd4201 -wd4100 -wd4189 -FC -Z7  
set CommonLinkerFlags= -opt:ref user32.lib gdi32.lib winmm.lib
REM TODO - can we just build both with one exe?

IF NOT EXIST ..\..\builddecoder mkdir ..\..\builddecoder
pushd ..\..\builddecoder

cl %CommonCompilerFlags% ..\Decoder_8086\code\win32_decoder.cpp /link %CommonLinkerFlags% 
popd
