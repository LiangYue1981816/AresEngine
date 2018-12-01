#include <thread>
#include <jni.h>
#include "EngineHeader.h"


CScene *pMainScene[5][5] = { nullptr };

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_InitEngine(JNIEnv *env, jobject /* this */, int width, int height, jstring strAssetsPath)
{
    const char *szAssetsPath = env->GetStringUTFChars(strAssetsPath, JNI_FALSE);
    {
        InitAllocator();
        CGfxProfiler::SetEnableProfiler(true);

        CreateEngine(GFX_API_GLES3, RENDER_SOLUTION_DEFAULT, nullptr, width, height, gli::FORMAT_RGBA8_UNORM_PACK8);

        FileManager()->SetPack(szAssetsPath, ".xml");
        FileManager()->SetPack(szAssetsPath, ".png");
        FileManager()->SetPack(szAssetsPath, ".tga");
        FileManager()->SetPack(szAssetsPath, ".dds");
        FileManager()->SetPack(szAssetsPath, ".glsl");
        FileManager()->SetPack(szAssetsPath, ".vert");
        FileManager()->SetPack(szAssetsPath, ".frag");
        FileManager()->SetPack(szAssetsPath, ".comp");
        FileManager()->SetPack(szAssetsPath, ".mesh");
        FileManager()->SetPack(szAssetsPath, ".material");

        for (int y = -2; y <= 2; y++) {
            for (int x = -2; x <= 2; x++) {
                pMainScene[y + 2][x + 2] = SceneManager()->CreateScene((y + 2) * 5 + (x + 2));
                pMainScene[y + 2][x + 2]->LoadMesh("sponza.xml");
                pMainScene[y + 2][x + 2]->GetRootNode()->SetWorldScale(0.5f, 0.5f, 0.5f);
                pMainScene[y + 2][x + 2]->GetRootNode()->SetWorldPosition(x * 17.5f, 0.0f, y * 10.0f);
            }
        }
    }
    env->ReleaseStringUTFChars(strAssetsPath, szAssetsPath);
}

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_ExitEngine(JNIEnv *env, jobject /* this */)
{

}

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_Tick(JNIEnv *env, jobject /* this */)
{

}
