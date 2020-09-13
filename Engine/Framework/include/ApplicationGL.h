#pragma once
#include "Application.h"
#include "GLES3Renderer.h"


class CApplicationGL : public CApplication
{
public:
	CApplicationGL(void);
	virtual ~CApplicationGL(void);


public:
	virtual bool Create(void* hInstance, void* hWnd, void* hDC, int width, int height);
	virtual void Destroy(void);


private:
	virtual void UpdateInternal(float deltaTime);


private:
	void* m_hDC;
	int m_width;
	int m_height;
};
