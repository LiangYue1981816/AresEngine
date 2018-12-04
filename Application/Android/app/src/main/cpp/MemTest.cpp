#include "EngineHeader.h"


static const uint32_t loop = 10000;
static const uint32_t count = 30000;
static uint32_t size[count] = { 0 };
static void *pointer[count] = { nullptr };
static bool allocate[count] = { false };

class CMemTestTask : public CTask
{
public:
    CMemTestTask(void)
    {

    }
    virtual ~CMemTestTask(void)
    {

    }

public:
    void SetParams(uint32_t minSize, uint32_t maxSize, uint32_t indexBegin, uint32_t indexEnd)
    {
        m_minSize = minSize;
        m_maxSize = maxSize;
        m_indexBegin = indexBegin;
        m_indexEnd = indexEnd;
    }

    virtual void TaskFunc(void *pParams)
    {
        if (pParams) {
            AllocatorThread(m_minSize, m_maxSize, m_indexBegin, m_indexEnd);
        }
        else {
            SystemOSThread(m_minSize, m_maxSize, m_indexBegin, m_indexEnd);
        }
    }

private:
    void AllocatorThread(uint32_t minSize, uint32_t maxSize, uint32_t indexBegin, uint32_t indexEnd)
    {
        uint32_t begin = Tick();
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
        uint32_t end = Tick();

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
        uint32_t begin = Tick();
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
        uint32_t end = Tick();

        for (uint32_t index = indexBegin; index < indexEnd; index++) {
            if (pointer[index]) {
                free(pointer[index]);
                pointer[index] = nullptr;
            }
        }

        LogOutput("MemTest SystemOS", "[%d, %d] = %f\n", minSize, maxSize, (end - begin) / 1000000.0f);
    }

private:
    uint32_t m_minSize;
    uint32_t m_maxSize;
    uint32_t m_indexBegin;
    uint32_t m_indexEnd;
};

void TestAllocator(uint32_t minSize, uint32_t maxSize)
{
    srand(Tick());
    for (uint32_t index = 0; index < count; index++) {
        size[index] = (rand() % (maxSize - minSize)) + minSize;
        allocate[index] = (rand() % 2) == 0;
    }

    CTaskGraph taskGraph("");
    CMemTestTask tasks[THREAD_COUNT];
    uint32_t step = count / THREAD_COUNT;

    for (uint32_t indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
        tasks[indexThread].SetParams(minSize, maxSize, indexThread * step, (indexThread + 1) * step);
    }

    {
        for (uint32_t indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
            taskGraph.Task(&tasks[indexThread], nullptr, nullptr, nullptr);
        }

        taskGraph.Dispatch();
        taskGraph.Wait();
    }
    {
        for (uint32_t indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
            taskGraph.Task(&tasks[indexThread], &indexThread, nullptr, nullptr);
        }

        taskGraph.Dispatch();
        taskGraph.Wait();
    }
}

void TestAllocator()
{
    TestAllocator(4, 16);
    TestAllocator(16, 64);
    TestAllocator(64, 128);
}
