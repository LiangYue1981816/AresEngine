#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


class CApplication
{
public:
	CApplication(void);
	virtual ~CApplication(void);


public:
	virtual bool Create(void* hInstance, void* hWnd, void* hDC, int width, int height) = 0;
	virtual void Destroy(void) = 0;

private:
	virtual void TickInternal(void) = 0;

public:
	void Tick(void);
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


protected:
	void* m_hDC;
	int m_width;
	int m_height;
};


class CApplicationOpenGL : public CApplication
{
public:
	CApplicationOpenGL(void);
	virtual ~CApplicationOpenGL(void);


public:
	virtual bool Create(void* hInstance, void* hWnd, void* hDC, int width, int height);
	virtual void Destroy(void);

private:
	virtual void TickInternal(void);
};


class CApplicationVulkan : public CApplication
{
public:
	CApplicationVulkan(void);
	virtual ~CApplicationVulkan(void);


public:
	virtual bool Create(void* hInstance, void* hWnd, void* hDC, int width, int height);
	virtual void Destroy(void);

private:
	virtual void TickInternal(void);
};
