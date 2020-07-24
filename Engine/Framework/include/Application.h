#pragma once
#include "PreHeader.h"


class CALL_API CApplication
{
public:
	CApplication(void);
	virtual ~CApplication(void);


public:
	virtual bool Create(void* hInstance, void* hWnd, void* hDC, int width, int height) = 0;
	virtual void Destroy(void) = 0;
	virtual void Update(void);

private:
	virtual void UpdateInternal(float deltaTime) = 0;

public:
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


protected:
	void* m_hDC;
	int m_width;
	int m_height;
};


class CALL_API CApplicationOpenGL : public CApplication
{
public:
	CApplicationOpenGL(void);
	virtual ~CApplicationOpenGL(void);


public:
	virtual bool Create(void* hInstance, void* hWnd, void* hDC, int width, int height);
	virtual void Destroy(void);

private:
	virtual void UpdateInternal(float deltaTime);
};


class CALL_API CApplicationVulkan : public CApplication
{
public:
	CApplicationVulkan(void);
	virtual ~CApplicationVulkan(void);


public:
	virtual bool Create(void* hInstance, void* hWnd, void* hDC, int width, int height);
	virtual void Destroy(void);

private:
	virtual void UpdateInternal(float deltaTime);
};
