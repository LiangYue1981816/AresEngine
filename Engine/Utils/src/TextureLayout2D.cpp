#include "UtilsHeader.h"


CTextureLayout2D::CTextureLayout2D(int width, int height)
{
	m_pNodes.emplace_back(new Node(0, 0, width, height));
}

CTextureLayout2D::~CTextureLayout2D(void)
{
	for (uint32_t indexNode = 0; indexNode < m_pNodes.size(); indexNode++) {
		delete m_pNodes[indexNode];
	}
}


bool CTextureLayout2D::Add(int& x, int& y, int width, int height)
{
	int index = Add(0, width, height);

	if (index != -1) {
		m_pNodes[index]->used = true;

		x = m_pNodes[index]->x;
		y = m_pNodes[index]->y;

		return true;
	}
	else {
		return false;
	}
}

bool CTextureLayout2D::Remove(int x, int y, int width, int height)
{
	int indexRemoveNode = -1;

	for (uint32_t indexNode = 0; indexNode < m_pNodes.size(); indexNode++) {
		Node* pNode = m_pNodes[indexNode];

		if (pNode->x == x &&
			pNode->y == y &&
			pNode->width == width &&
			pNode->height == height) {
			indexRemoveNode = indexNode;
			break;
		}
	}

	if (indexRemoveNode != -1) {
		m_pNodes[indexRemoveNode]->used = false;

		int indexParentNode;
		int indexLastParentNode;

		indexParentNode = FindParentNode(indexRemoveNode);
		indexParentNode = IsNodeUsed(indexParentNode) ? -1 : indexParentNode;
		indexLastParentNode = indexParentNode;

		while (indexParentNode != -1 &&
			IsNodeUsed(m_pNodes[indexParentNode]->child0) == false &&
			IsNodeUsed(m_pNodes[indexParentNode]->child1) == false) {
			indexLastParentNode = indexParentNode;
			indexParentNode = FindParentNode(indexParentNode);
		}

		if (indexLastParentNode != -1) {
			RemoveChildren(indexLastParentNode);
		}

		return true;
	}
	else {
		return false;
	}
}

int CTextureLayout2D::Add(int index, int width, int height)
{
	int result = -1;
	Node* pNode = m_pNodes[index];

	if (result == -1 && pNode->child0 != -1) {
		result = Add(pNode->child0, width, height);
	}

	if (result == -1 && pNode->child1 != -1) {
		result = Add(pNode->child1, width, height);
	}

	if (result == -1 && pNode->child0 == -1 && pNode->child1 == -1) {
		if (pNode->used) {
			return -1;
		}

		if (pNode->width < width || pNode->height < height) {
			return -1;
		}

		if (pNode->width == width && pNode->height == height) {
			return index;
		}

		int excessWidth = pNode->width - width;
		int excessHeight = pNode->height - height;

		if (excessWidth > excessHeight) {
			pNode->child0 = m_pNodes.size();
			m_pNodes.emplace_back(new Node(pNode->x, pNode->y, width, pNode->height));

			pNode->child1 = m_pNodes.size();
			m_pNodes.emplace_back(new Node(pNode->x + width, pNode->y, pNode->width - width, pNode->height));
		}
		else {
			pNode->child0 = m_pNodes.size();
			m_pNodes.emplace_back(new Node(pNode->x, pNode->y, pNode->width, height));

			pNode->child1 = m_pNodes.size();
			m_pNodes.emplace_back(new Node(pNode->x, pNode->y + height, pNode->width, pNode->height - height));
		}

		return Add(pNode->child0, width, height);
	}

	return result;
}

int CTextureLayout2D::FindParentNode(int indexSearchNode)
{
	for (uint32_t indexNode = 0; indexNode < m_pNodes.size(); indexNode++) {
		if (m_pNodes[indexNode]->child0 == indexSearchNode ||
			m_pNodes[indexNode]->child1 == indexSearchNode) {
			return indexNode;
		}
	}

	return -1;
}

bool CTextureLayout2D::IsNodeUsed(int indexNode)
{
	if (indexNode == -1) {
		return false;
	}
	else {
		return m_pNodes[indexNode]->used || IsNodeUsed(m_pNodes[indexNode]->child0) || IsNodeUsed(m_pNodes[indexNode]->child1);
	}
}

void CTextureLayout2D::RemoveChildren(int indexNode)
{
	if (m_pNodes[indexNode]->child0 != -1) {
		RemoveChildren(m_pNodes[indexNode]->child0);
	}

	if (m_pNodes[indexNode]->child1 != -1) {
		RemoveChildren(m_pNodes[indexNode]->child1);
	}

	if (m_pNodes[indexNode]->child0 != -1) {
		int indexChild0 = m_pNodes[indexNode]->child0;

		delete m_pNodes[indexChild0];
		m_pNodes.erase(m_pNodes.begin() + indexChild0);

		for (uint32_t indexOtherNode = 0; indexOtherNode < m_pNodes.size(); indexOtherNode++) {
			if (m_pNodes[indexOtherNode]->child0 >= indexChild0) {
				m_pNodes[indexOtherNode]->child0--;
			}
			if (m_pNodes[indexOtherNode]->child1 >= indexChild0) {
				m_pNodes[indexOtherNode]->child1--;
			}
		}

		m_pNodes[indexNode]->child0 = -1;
	}

	if (m_pNodes[indexNode]->child1 != -1) {
		int indexChild1 = m_pNodes[indexNode]->child1;

		delete m_pNodes[indexChild1];
		m_pNodes.erase(m_pNodes.begin() + indexChild1);

		for (uint32_t indexOtherNode = 0; indexOtherNode < m_pNodes.size(); indexOtherNode++) {
			if (m_pNodes[indexOtherNode]->child0 >= indexChild1) {
				m_pNodes[indexOtherNode]->child0--;
			}
			if (m_pNodes[indexOtherNode]->child1 >= indexChild1) {
				m_pNodes[indexOtherNode]->child1--;
			}
		}

		m_pNodes[indexNode]->child1 = -1;
	}
}
