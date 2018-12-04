#include <thread>
#include <jni.h>
#include "EngineHeader.h"


uint32_t frameTime = 0;
uint32_t frameCount = 0;
uint32_t lastProfilerTick = 0;
CScene *pMainScene[5][5] = { nullptr };

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_InitEngine(JNIEnv *env, jobject /* this */, int width, int height, jstring strAssetsPath)
{
    const char *szAssetsPath = env->GetStringUTFChars(strAssetsPath, JNI_FALSE);
    {
        InitAllocator();
        CGfxProfiler::SetEnableProfiler(true);

        CreateEngine(GFX_API_GLES3, RENDER_SOLUTION_DEFAULT, nullptr, width, height, gli::FORMAT_RGBA8_UNORM_PACK8);

        MainCamera()->SetScissor(0.0f, 0.0f, 1.0f * width, 1.0f * height);
        MainCamera()->SetViewport(0.0f, 0.0f, 1.0f * width, 1.0f * height);
        MainCamera()->SetPerspective(60.0f, 1.0f * width / height, 0.1f, 100.0f);

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

extern "C" JNIEXPORT void JNICALL Java_com_engine_application_MainActivity_Update(JNIEnv *env, jobject /* this */)
{
    MainCamera()->SetLookat(-0.0412254706f, 54.6759644f, 32.0880432f, -0.0412254222f, 53.8372955f, 31.5434036f, 0.0f, 1.0f, 0.0f);

    uint32_t begin = Tick();
    {
        Engine()->Update();
        Engine()->Present();
    }
    uint32_t end = Tick();

    frameTime += end - begin;
    frameCount += 1;

    uint32_t currTick = Tick();
    float deltaProfilerTick = (currTick - lastProfilerTick) / 1000000.0f;

    if (deltaProfilerTick > 1.0f) {
        CGfxProfiler::LogGfxMemory();
        CGfxProfiler::LogProfiler(frameCount);
        CGfxProfiler::ResetSamples();

        LogOutput("Frame", "Time = %fms\n", frameTime / frameCount / 1000.0f);

        frameTime = 0;
        frameCount = 0;
        lastProfilerTick = currTick;
    }
}
