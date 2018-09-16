for %%i in (.\source\*.glsl) do (
	glslc -fshader-stage=vertex -DVERTEX_SHADER -I".\source\inc" %%i -o .\bin\%%~ni.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -I".\source\inc" %%i -o .\bin\%%~ni.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni.vert --output .\debug\%%~ni.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni.frag --output .\debug\%%~ni.frag
)

copy /y .\bin\*.* ..\..\Bin\Shader

pause