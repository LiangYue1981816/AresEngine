#include <thread>
#include <jni.h>
#include "EngineHeader.h"


static const uint32_t loop = 10000;
static const uint32_t count = 50000;
static uint32_t size[count] = { 0 };
static void *pointer[count] = { nullptr };
static bool allocate[count] = { false };

void AllocatorThread(uint32_t minSize, uint32_t maxSize, uint32_t indexBegin, uint32_t indexEnd)
{
    uint32_t begin = tick();
    {
        for (uint32_t indexLoop = 0; indexLoop < loop; indexLoop++) {
            for (uint32_t index = indexBegin; index < indexEnd; index++) {
                if (allocate[index]) {
                    allocate[index] = false;
                    pointer[index] = AllocMemory(size[index]);
                }
                else {
                    allocate[index] = true;
                    if (pointer[index]) {
                        FreeMemory(pointer[index]);
                        pointer[index] = nullptr;
                    }
                }
            }
        }
    }
    uint32_t end = tick();

    for (uint32_t index = indexBegin; index < indexEnd; index++) {
        if (pointer[index]) {
            FreeMemory(pointer[index]);
            pointer[index] = nullptr;
        }
    }

    LogOutput("MemTest Allocator", "[%d, %d] = %f\n", minSize, maxSize, (end - begin) / 1000000.0f);
}

void SystemOSThread(uint32_t minSize, uint32_t maxSize, uint32_t indexBegin, uint32_t indexEnd)
{
    uint32_t begin = tick();
    {
        for (uint32_t indexLoop = 0; indexLoop < loop; indexLoop++) {
            for (uint32_t index = indexBegin; index < indexEnd; index++) {
                if (allocate[index]) {
                    allocate[index] = false;
                    pointer[index] = malloc(size[index]);
                }
                else {
                    allocate[index] = true;
                    if (pointer[index]) {
                        free(pointer[index]);
                        pointer[index] = nullptr;
                    }
                }
            }
        }
    }
    uint32_t end = tick();

    for (uint32_t index = indexBegin; index < indexEnd; index++) {
        if (pointer[index]) {
            free(pointer[index]);
            pointer[index] = nullptr;
        }
    }

    LogOutput("MemTest SystemOS", "[%d, %d] = %f\n", minSize, maxSize, (end - begin) / 1000000.0f);
}

void TestAllocator(uint32_t threadCount, uint32_t minSize, uint32_t maxSize)
{
    srand(tick());
    for (uint32_t index = 0; index < count; index++) {
        size[index] = (rand() % (maxSize - minSize)) + minSize;
        allocate[index] = (rand() % 2) == 0;
    }

    uint32_t step = count / threadCount;
    {
        std::thread threads[64];

        for (uint32_t indexThread = 0; indexThread < threadCount; indexThread++) {
            threads[indexThread] = std::thread(AllocatorThread, minSize, maxSize, indexThread * step, (indexThread + 1) * step);
        }

        for (uint32_t indexThread = 0; indexThread < threadCount; indexThread++) {
            threads[indexThread].join();
        }
    }
    {
        std::thread threads[64];

        for (uint32_t indexThread = 0; indexThread < threadCount; indexThread++) {
            threads[indexThread] = std::thread(SystemOSThread, minSize, maxSize, indexThread * step, (indexThread + 1) * step);
        }

        for (uint32_t indexThread = 0; indexThread < threadCount; indexThread++) {
            threads[indexThread].join();
        }
    }
}

void TestAllocator()
{
    TestAllocator(4, 4, 16);
    TestAllocator(4, 16, 64);
    TestAllocator(4, 64, 128);
}


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
    TestAllocator();
}
