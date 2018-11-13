#include "Engine.h"
#include "RenderSolutionBase.h"


CRenderSolutionBase::CRenderSolutionBase(void)
	: m_indexQueue(0)
{
	event_init(&m_eventExit, 0);
	event_init(&m_eventFinish, 1);
	event_init(&m_eventDispatch, 0);
	pthread_create(&m_thread, nullptr, WorkerThread, this);
}

CRenderSolutionBase::~CRenderSolutionBase(void)
{
	event_signal(&m_eventExit);
	event_signal(&m_eventDispatch);

	pthread_join(m_thread, nullptr);

	event_destroy(&m_eventExit);
	event_destroy(&m_eventFinish);
	event_destroy(&m_eventDispatch);
}

void* CRenderSolutionBase::WorkerThread(void *pParams)
{
	if (CRenderSolutionBase *pRenderSolution = (CRenderSolutionBase *)pParams) {
		while (true) {
			event_wait(&pRenderSolution->m_eventDispatch);
			{
				if (event_wait_timeout(&pRenderSolution->m_eventExit, 0) == NO_ERROR) {
					break;
				}

				pRenderSolution->RenderThread();
			}
			event_reset(&pRenderSolution->m_eventDispatch);
			event_signal(&pRenderSolution->m_eventFinish);
		}
	}

	return nullptr;
}
