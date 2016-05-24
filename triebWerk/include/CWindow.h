#pragma once
#include <Windows.h>
#include <queue>

namespace triebWerk
{
	class CWindow
	{
	private:
		static const DWORD WindowStyleFullscreen = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
		static const DWORD WindowStyleWindowed = WS_OVERLAPPEDWINDOW;

		HWND m_WindowHandle;
		std::queue<MSG> m_MessageQueue;

		unsigned short m_Width;
		unsigned short m_Height;

		unsigned short m_DefaultWidth;
		unsigned short m_DefaultHeight;

		bool m_IsFullscreen;
		bool m_ShowCursor;
		bool m_IsSizing;

	public:
		CWindow();
		~CWindow();

	public:
		//Initialize the window and displays it in focus
		bool Initialize(const bool a_Fullscreen, const unsigned short a_ScreenWidth, const unsigned short a_ScreenHeight, const char* a_Name);

		//Get the first window event in queue
		const MSG GetWindowEvent();
        size_t GetWindowEventCount() const;

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		//Resizes the window to the given paramenter
		void ChangeWindowSettings(const bool a_IsFullscreen, const unsigned short a_ScreenWidth, const unsigned short a_ScreenHeight);
		
		HWND& GetWindowHandle();

		static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		inline unsigned short GetScreenWidth() const;
		inline unsigned short GetScreenHeight() const;
		
		bool IsWindowFullscreen();
		void UpdateWindow();

		static int GetMaximalDisplayWidth();
		static int GetMaximalDisplayHeight();
	};
}