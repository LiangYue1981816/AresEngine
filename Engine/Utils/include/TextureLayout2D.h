#pragma once
#include "PreHeader.h"


class CTextureLayout2D
{
public:
	CTextureLayout2D(int width, int height);
	virtual ~CTextureLayout2D(void);


public:
	bool Add(int& x, int& y, int width, int height);
	bool Remove(int x, int y, int width, int height);

private:
	int Add(int index, int width, int height);
	int FindParentNode(int indexSearchNode);
	bool IsNodeUsed(int indexNode);
	void RemoveChildren(int indexNode);


private:
	struct Node {
		Node()
		{

		}

		Node(int _x, int _y, int _width, int _height)
			: used(false)
			, child0(-1)
			, child1(-1)
			, x(_x)
			, y(_y)
			, width(_width)
			, height(_height)
		{

		}

		bool used;
		int child0;
		int child1;
		int x;
		int y;
		int width;
		int height;
	};

	std::vector<Node*> m_pNodes;
};
