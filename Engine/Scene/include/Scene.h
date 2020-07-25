#pragma once
#include "PreHeader.h"


class CScene
{
	friend class CSceneNode;
	friend class CSceneManager;


private:
	CScene(uint32_t name, CSceneManager* pSceneManager);
	virtual ~CScene(void);


public:
	uint32_t GetName(void) const;
	CSceneNode* GetRootNode(void) const;
	CSceneManager* GetSceneManager(void) const;


private:
	uint32_t m_name;

private:
	CSceneNode* m_pRootNode;
	CSceneManager* m_pSceneManager;
};
