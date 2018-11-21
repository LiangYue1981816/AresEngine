#pragma once
#include "PreHeader.h"


class CALL_API CScene
{
	friend class CSceneNode;
	friend class CSceneManager;


private:
	CScene(uint32_t name, CSceneManager *pSceneManager);
	virtual ~CScene(void);


public:
	uint32_t GetName(void) const;
	CSceneNode* GetRootNode(void) const;
	CSceneNode* GetNode(uint32_t name) const;

private:
	bool AttachNode(CSceneNode *pNode);
	bool DetachNode(CSceneNode *pNode);

public:
	bool LoadMesh(const char *szFileName);
	void FreeNode(CSceneNode *pNode);
	void Free(void);

private:
	bool LoadMesh(TiXmlNode *pMeshNode);
	bool LoadNode(const CGfxMeshPtr &ptrMesh, TiXmlNode *pNode, CSceneNode *pParentSceneNode);
	bool LoadDraw(const CGfxMeshPtr &ptrMesh, TiXmlNode *pNode, CSceneNode *pCurrentSceneNode);


private:
	uint32_t m_name;

private:
	CSceneNode *m_pRootNode;
	eastl::unordered_map<uint32_t, CSceneNode*> m_pNodes;

private:
	CSceneManager *m_pSceneManager;
};
