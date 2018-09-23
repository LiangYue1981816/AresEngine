for %%i in (.\source\*Blinn*.glsl) do (
	glslc -fshader-stage=vertex -DVERTEX_SHADER -I".\source\inc" %%i -o .\bin\%%~ni.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -I".\source\inc" %%i -o .\bin\%%~ni.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DNORMAL_MAP -I".\source\inc" %%i -o .\bin\%%~ni_N.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DNORMAL_MAP -I".\source\inc" %%i -o .\bin\%%~ni_N.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DSPECULAR_MAP -I".\source\inc" %%i -o .\bin\%%~ni_S.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DSPECULAR_MAP -I".\source\inc" %%i -o .\bin\%%~ni_S.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DNORMAL_MAP -DSPECULAR_MAP -I".\source\inc" %%i -o .\bin\%%~ni_NS.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DNORMAL_MAP -DSPECULAR_MAP -I".\source\inc" %%i -o .\bin\%%~ni_NS.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DAO_MAP -I".\source\inc" %%i -o .\bin\%%~ni_A.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DAO_MAP -I".\source\inc" %%i -o .\bin\%%~ni_A.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DAO_MAP -DNORMAL_MAP -I".\source\inc" %%i -o .\bin\%%~ni_AN.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DAO_MAP -DNORMAL_MAP -I".\source\inc" %%i -o .\bin\%%~ni_AN.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DAO_MAP -DSPECULAR_MAP -I".\source\inc" %%i -o .\bin\%%~ni_AS.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DAO_MAP -DSPECULAR_MAP -I".\source\inc" %%i -o .\bin\%%~ni_AS.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DAO_MAP -DNORMAL_MAP -DSPECULAR_MAP -I".\source\inc" %%i -o .\bin\%%~ni_ANS.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DAO_MAP -DNORMAL_MAP -DSPECULAR_MAP -I".\source\inc" %%i -o .\bin\%%~ni_ANS.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni.vert --output .\debug\%%~ni.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni.frag --output .\debug\%%~ni.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_N.vert --output .\debug\%%~ni_N.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_N.frag --output .\debug\%%~ni_N.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_S.vert --output .\debug\%%~ni_S.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_S.frag --output .\debug\%%~ni_S.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_NS.vert --output .\debug\%%~ni_NS.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_NS.frag --output .\debug\%%~ni_NS.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_A.vert --output .\debug\%%~ni_A.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_A.frag --output .\debug\%%~ni_A.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_AN.vert --output .\debug\%%~ni_AN.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_AN.frag --output .\debug\%%~ni_AN.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_AS.vert --output .\debug\%%~ni_AS.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_AS.frag --output .\debug\%%~ni_AS.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_ANS.vert --output .\debug\%%~ni_ANS.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_ANS.frag --output .\debug\%%~ni_ANS.frag
)

for %%i in (.\source\*Pbr*.glsl) do (
	glslc -fshader-stage=vertex -DVERTEX_SHADER -I".\source\inc" %%i -o .\bin\%%~ni.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -I".\source\inc" %%i -o .\bin\%%~ni.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DNORMAL_MAP -I".\source\inc" %%i -o .\bin\%%~ni_N.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DNORMAL_MAP -I".\source\inc" %%i -o .\bin\%%~ni_N.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DAO_MAP -I".\source\inc" %%i -o .\bin\%%~ni_A.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DAO_MAP -I".\source\inc" %%i -o .\bin\%%~ni_A.frag
	
	glslc -fshader-stage=vertex -DVERTEX_SHADER -DAO_MAP -DNORMAL_MAP -I".\source\inc" %%i -o .\bin\%%~ni_AN.vert
	glslc -fshader-stage=fragment -DFRAGMENT_SHADER -DAO_MAP -DNORMAL_MAP -I".\source\inc" %%i -o .\bin\%%~ni_AN.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni.vert --output .\debug\%%~ni.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni.frag --output .\debug\%%~ni.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_N.vert --output .\debug\%%~ni_N.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_N.frag --output .\debug\%%~ni_N.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_A.vert --output .\debug\%%~ni_A.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_A.frag --output .\debug\%%~ni_A.frag
	
	spirv-cross.exe --es --version 310 .\bin\%%~ni_AN.vert --output .\debug\%%~ni_AN.vert
	spirv-cross.exe --es --version 310 .\bin\%%~ni_AN.frag --output .\debug\%%~ni_AN.frag
)

glslc -fshader-stage=vertex -DVERTEX_SHADER -I".\source\inc" .\source\Default.glsl -o .\bin\Default.vert
glslc -fshader-stage=fragment -DFRAGMENT_SHADER -I".\source\inc" .\source\Default.glsl -o .\bin\Default.frag

glslc -fshader-stage=vertex -DVERTEX_SHADER -I".\source\inc" .\source\SkyBox.glsl -o .\bin\SkyBox.vert
glslc -fshader-stage=fragment -DFRAGMENT_SHADER -I".\source\inc" .\source\SkyBox.glsl -o .\bin\SkyBox.frag

copy /y .\bin\*.* ..\..\Bin\Data\Shader

pause