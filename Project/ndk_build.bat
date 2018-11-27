pushd .\jni\
python make_jni.py
%NDK_ROOT%\ndk-build.cmd -j8
popd

xcopy .\libs ..\Application\Android\app\libs /S /Y