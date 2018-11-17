#pragma once
#include "PreHeader.h"
#include "SceneDefinition.h"


class CALL_API CScene
{
	friend class CSceneNode;
	friend class CSceneManager;


private:
	CScene(uint32_t name);
	virtual ~CScene(void);


public:
	uint32_t GetName(void) const;
	CSceneNode* GetRootNode(void) const;

public:
	bool AttachNode(CSceneNode *pNode);
	bool DetachNode(CSceneNode *pNode);
	CSceneNode* GetNode(uint32_t name) const;

public:
	bool Load(const char *szFileName);
	void Free(void);

private:
	bool LoadScene(TiXmlNode *pSceneNode);
	bool LoadNode(TiXmlNode *pNode, CSceneNode *pParentNode);
	bool LoadMesh(TiXmlNode *pNode, CSceneNode *pCurrentNode);


private:
	uint32_t m_name;

private:
	CSceneNode *m_pRootNode;
	eastl::unordered_map<uint32_t, CSceneNode*> m_pNodes;
};
