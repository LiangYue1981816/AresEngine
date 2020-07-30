#pragma once
#include "imgui.h"

class CImGUI_Console
{
public:
	CImGUI_Console(void);
	virtual ~CImGUI_Console(void);


public:
	void Draw();

private:
	void ClearLog();
	void AddLog(const char* fmt, ...) IM_FMTARGS(2);
	void ExecCommand(const char* command_line);
	int TextEditCallback(ImGuiInputTextCallbackData* data);

private:
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);


private:
	char                  InputBuf[256];
	ImVector<char*>       Items;
	ImVector<const char*> Commands;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	bool                  AutoScroll;
	bool                  ScrollToBottom;
};
