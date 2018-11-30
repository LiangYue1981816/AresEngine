pushd .\jni\
call ndk_build.bat
popd

xcopy .\libs ..\Application\Android\app\libs /S /Y