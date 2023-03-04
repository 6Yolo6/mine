// 扫雷.cpp : 定义应用程序的入口点。
//

#include "扫雷.h"

#define MAX_LOADSTRING 100




// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


//void MoveWindow(int x, int y, int nWidth, int nHight, BOOL BRePaint);//改变窗口位置和尺寸

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	//wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));//换图标
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
	wcex.lpszClassName = szWindowClass;
	//wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MY));//图标

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中
	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX/*禁用窗口最大化*/,
		CW_USEDEFAULT, 0,
		WIDTH, HEIGHT, //宽度高度
		nullptr, nullptr,
		hInstance, nullptr);

	if (!hWnd)
	{
		MessageBox(NULL, _T("窗口创建失败"), NULL, NULL);
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
//自定义消息
#define WM_TIME (WM_USER + 100) 
#define WM_FLAG (WM_USER + 101)

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//菜单
	static HMENU hMenu = GetMenu(hWnd);
	switch (message)
	{
	case WM_CREATE://窗口消息
	{
		game_init(hWnd);//初始化数据
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT: //帮助
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT://退出
			DestroyWindow(hWnd);
			break;
		case ID_32773://中级
			sec = 0;
			mid = true;
			game_start(hWnd, hInst);
			check(wmId, hMenu);
			break;
		case ID_32772://初级
			sec = 0;
			low = true;
			game_start(hWnd, hInst);
			check(wmId, hMenu);
			break;
		case ID_32774://高级
			sec = 0;
			high = true;
			game_start(hWnd, hInst);
			check(wmId, hMenu);
			break;
		case ID_32771://开局
			sec = 0;
			if (1 == size)
				low = true;
			else if (2 == size)
				mid = true;
			else if (3 == size)
				high = true;
			game_start(hWnd, hInst);
			check(wmId, hMenu);
			break;
		case ID_32775://自定义
			check(wmId, hMenu);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_MOUSEMOVE: //鼠标移动
	{
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		mouse_move(hInst, pt.x, pt.y);
	}
	break;
	case WM_LBUTTONDOWN: //鼠标左键按下
	{
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		mouse_left(hWnd, hInst, pt.x, pt.y);
		win_lose(hWnd, hInst);
	}
	break;
	case WM_LBUTTONUP: //鼠标左键释放
	{
		flag1 = false;
		flag2 = false;
		hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));//笑脸
		SelectObject(hdcBmp, hBmp);
		BitBlt(hdc, (WIDTH / 2) - 42, 23, 24, 24, hdcBmp, 0, 95, SRCCOPY);
	}
	break;
	case WM_RBUTTONDOWN://鼠标右键按下
	{
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		if (flag0)
		{
			mouse_right(hInst, pt.x, pt.y);
			if (pt.x > 19 && pt.x < 19 + SIZE * col && pt.y > 70 && pt.y < 70 + SIZE * row)
				SendMessage(hWnd, WM_FLAG, wParam, lParam);
		}
	}
	break;
	case WM_CHAR: //键盘响应
	{
		if (LOWORD(wParam) == VK_ESCAPE) //ESC键
		{
			MessageBox(hWnd, TEXT("程序即将退出"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
			exit(0);
		}
	}
	break;
	case WM_TIMER://时钟消息
	{
		sec++;
		//发送自定义消息
		SendMessage(hWnd, WM_TIME, wParam, lParam);
	}
	break;
	case WM_TIME://时间消息
	{
		time(hWnd, hInst);
	}
	break;
	case WM_FLAG://插旗消息
	{
		flag(hInst);
	}
	break;
	case WM_PAINT:
	{
		game_paint(hWnd, hInst);//绘制游戏区域
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		destroy(hWnd);//销毁
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



