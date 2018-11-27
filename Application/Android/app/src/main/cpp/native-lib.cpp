#include <jni.h>
#include <string>
#include "EngineHeader.h"

extern "C" JNIEXPORT jstring JNICALL Java_com_engine_application_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */)
{
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_InitEngine(JNIEnv *env, jobject /* this */)
{
    InitAllocator();
}

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_ExitEngine(JNIEnv *env, jobject /* this */)
{

}

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_Tick(JNIEnv *env, jobject /* this */)
{

}
