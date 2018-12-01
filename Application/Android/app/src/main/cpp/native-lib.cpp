#include <thread>
#include <jni.h>
#include "EngineHeader.h"


extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_InitEngine(JNIEnv *env, jobject /* this */)
{
    InitAllocator();
}

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_ExitEngine(JNIEnv *env, jobject /* this */)
{
    ExitAllocator();
}

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_Tick(JNIEnv *env, jobject /* this */)
{

}
