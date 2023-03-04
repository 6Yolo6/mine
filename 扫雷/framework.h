// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <windowsx.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>
#include <tchar.h>
#include <queue>
#include <iostream>

#define WIDTH (col * 16 + 98) //宽
#define HEIGHT row * 16 + 189 //高
#define ROWS 13 //行
#define COLS 13 //列
#define MINE_NUM 25//雷数
#define FLAG 25//旗数
#define SIZE 16//方块大小

struct MINE_AREA_2d
{
	int** data{ nullptr };
	int width{ 0 };
	int height{ 0 };
};

BOOL low = false, mid = false, high = false;
BOOL flag0;//重新开始
BOOL flag1, flag2; //鼠标按下移动消息
BOOL flag3 = false;//重置时间
int** left{ nullptr }; //判断用户左键是否按下 
int** right{ nullptr }; //判断用户右键是否按下 
int X[8] = { -1, 1, 0, 0, 1, -1, -1, 1 };//计算周围雷个数
int Y[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };
int Cnt;//左键未按下的格子
int row = ROWS;//行
int col = COLS;//列
int mine = MINE_NUM;//雷数
int size;//窗口
int flag00 = 0, flag_num = FLAG;//旗数
int width = WIDTH, height = HEIGHT;//宽度高度
	//计时变量
static int sec = 0;

PAINTSTRUCT ps;
HPEN hPen; //画笔
HBRUSH hBrush; //画刷
HDC hdc;
HDC hdcBmp;
HBITMAP hBmp; //位图 
POINT pt;//鼠标位置
POINT point1[7] = { 14,14, 166,14, 163,17, 17,17, 17,54, 14,56, 14,14 },
	  point2[7] = { 14,56, 17,54, 163,54, 163,17, 166,14, 166,56, 14,56 },
	  point3[7] = { 14,64, 166,64, 162,68, 18,68, 18,212, 14,216, 14,64 },
	  point4[7] = { 14,216, 18,213, 162,213, 162,68, 166,64, 166,216, 14,216 },

	  point5[7] = { 14,14, 278,14, 275,17, 17,17, 17,54, 14,56, 14,14 },
	  point6[7] = { 14,56, 17,54, 275,54, 275,17, 278,14, 278,56, 14,56 },
	  point7[7] = { 14,64, 278,64, 274,68, 18,68, 18,324, 14,328, 14,64 },
	  point8[7] = { 14,328, 18,325, 274,325, 274,68, 278,64, 278,328, 14,328 },

	  point9[7] = { 14,14, 502,14, 499,17, 17,17, 17,54, 14,56, 14,14 },
	  point10[7] = { 14,56, 17,54, 499,54, 499,17, 502,14, 502,56, 14,56 },
	  point11[7] = { 14,64, 502,64, 498,68, 18,68, 18,324, 14,328, 14,64 },
	  point12[7] = { 14,328, 18,325, 498,325, 498,68, 502,64, 502,328, 14,328 };

MINE_AREA_2d area_2d;//雷区
void game_paint(HWND hWnd, HINSTANCE hInst);//绘图
void destroy(HWND hWnd);//销毁
void mouse_move(HINSTANCE hInst, int pt_x, int pt_y);//移动笑脸按下
void mouse_left(HWND hWnd, HINSTANCE hInst, int pt_x, int pt_y);//鼠标左键按下
void mouse_right(HINSTANCE hInst, int pt_x, int pt_y);//鼠标右键按下
void game_init(HWND hWnd);//初始化
void set_mine();//随机布雷
void show_mine_area(HINSTANCE hInst);//展开周围雷
void show_mine_num(HINSTANCE hInst, int i, int j);//显示周围雷数
void show_blank_area(HINSTANCE hInst, int x, int y);//展开周围空白块
void calc_mine_num();//计算周围雷数
void win_lose(HWND hWnd, HINSTANCE hInst);//判断胜负
void game_over(HWND hWnd);//游戏结束
void game_start(HWND hWnd, HINSTANCE hInst);//重新开始
void check(int wmId, HMENU hMenu);//菜单栏的勾选
void time(HWND hWnd, HINSTANCE hInst);//时钟消息
void flag(HINSTANCE hInst);//插旗消息

//绘图
void game_paint(HWND hWnd, HINSTANCE hInst)
{
	int i;
	hdc = BeginPaint(hWnd, &ps);
	// Here your application is laid out.
	// For this introduction, we just print out "Hello, Windows desktop!"
	// in the top left corner.
	SetTextColor(hdc, RGB(50, 65, 65));
	hPen = CreatePen(PS_SOLID, 3, RGB(241, 241, 241));
	//大矩形
	hPen = CreatePen(PS_SOLID, 0, RGB(192, 192, 192));
	SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(RGB(192, 192, 192));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, 4, 4, 34 + col * SIZE, 82 + row * SIZE);
	if (mid)
	{
		mid = false;
		size = 2;
		//左上边框
		hPen = CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point5[0].x, point5[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point5[i].x, point5[i].y);
		hBrush = CreateSolidBrush(RGB(128, 128, 128));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point5, 7);
		//右下边框
		hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point6[0].x, point6[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point6[i].x, point6[i].y);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point6, 7);
		//左上边框
		hPen = CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point7[0].x, point7[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point7[i].x, point7[i].y);
		hBrush = CreateSolidBrush(RGB(128, 128, 128));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point7, 7);
		//右下边框
		hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point8[0].x, point8[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point8[i].x, point8[i].y);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point8, 7);
	}
	else if (low)
	{
		low = false;
		size = 1;
		//左上边框
		hPen = CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point1[0].x, point1[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point1[i].x, point1[i].y);
		hBrush = CreateSolidBrush(RGB(128, 128, 128));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point1, 7);
		//右下边框
		hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point2[0].x, point2[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point2[i].x, point2[i].y);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point2, 7);
		//左上边框
		hPen = CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point3[0].x, point3[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point3[i].x, point3[i].y);
		hBrush = CreateSolidBrush(RGB(128, 128, 128));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point3, 7);
		//右下边框
		hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point4[0].x, point4[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point4[i].x, point4[i].y);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point4, 7);
	}
	else if (high)
	{
		high = false;
		size = 3;
		//左上边框
		hPen = CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point9[0].x, point9[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point9[i].x, point9[i].y);
		hBrush = CreateSolidBrush(RGB(128, 128, 128));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point9, 7);
		//右下边框
		hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point10[0].x, point10[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point10[i].x, point10[i].y);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point10, 7);
		//左上边框
		hPen = CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point11[0].x, point11[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point11[i].x, point11[i].y);
		hBrush = CreateSolidBrush(RGB(128, 128, 128));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point11, 7);
		//右下边框
		hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, point12[0].x, point12[0].y, NULL);
		for (i = 1; i < 7; i++)
			LineTo(hdc, point12[i].x, point12[i].y);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		//填充颜色
		Polygon(hdc, point12, 7);
	}
	//贴图
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));//旗子数
	SelectObject(hdcBmp, hBmp);
	BitBlt(hdc, 24, 24, 23, 23, hdcBmp, 0, 253 - ((flag_num) / 100) * 23, SRCCOPY);
	BitBlt(hdc, 39, 24, 23, 23, hdcBmp, 0, 253 - ((flag_num) / 10 % 10) * 23, SRCCOPY);
	BitBlt(hdc, 54, 24, 23, 23, hdcBmp, 0, 253 - ((flag_num) % 10) * 23, SRCCOPY);
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));//笑脸
	SelectObject(hdcBmp, hBmp);
	BitBlt(hdc, (WIDTH / 2) - 42, 23, 24, 24, hdcBmp, 0, 95, SRCCOPY);
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));//计时器
	SelectObject(hdcBmp, hBmp);
	BitBlt(hdc, WIDTH - 128, 24, 23, 23, hdcBmp, 0, 253, SRCCOPY);
	BitBlt(hdc, WIDTH - 113, 24, 23, 23, hdcBmp, 0, 253, SRCCOPY);
	BitBlt(hdc, WIDTH - 98, 24, 23, 23, hdcBmp, 0, 253, SRCCOPY);
	//贴图
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	SelectObject(hdcBmp, hBmp);
	int x = 19;
	int y = 70;
	for (; x < 19 + SIZE * col; x += SIZE)
	{
		y = 70;
		for (; y < 70 + SIZE * row; y += SIZE)
		{
			BitBlt(hdc, x, y, SIZE, SIZE, hdcBmp, 0, 0, SRCCOPY);
		}
	}
}

//移动笑脸按下
void mouse_move(HINSTANCE hInst, int pt_x, int pt_y)
{
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));//笑脸按下
	SelectObject(hdcBmp, hBmp);
	if (flag1) //笑脸
	{
		if (pt_x >= (WIDTH / 2) - 41 && pt_x <= (WIDTH / 2) - 41 + 24 && pt_y >= 23 && pt_y <= 47)
		{
			BitBlt(hdc, (WIDTH / 2) - 42, 23, 24, 24, hdcBmp, 0, 0, SRCCOPY);
		}
		else
		{
			BitBlt(hdc, (WIDTH / 2) - 42, 23, 24, 24, hdcBmp, 0, 95, SRCCOPY);
		}
	}
}

//鼠标左键按下
void mouse_left(HWND hWnd, HINSTANCE hInst, int pt_x, int pt_y)
{
	int x, y, i, j;
	if (pt.x >= (WIDTH / 2) - 42 && pt.x <= (WIDTH / 2) - 42 + 24 && pt.y >= 23 && pt.y <= 47)//笑脸按下
	{
		flag1 = true;
		hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		SelectObject(hdcBmp, hBmp);
		BitBlt(hdc, (WIDTH / 2) - 42, 23, 24, 24, hdcBmp, 0, 0, SRCCOPY);
		sec = 0;
		if (1 == size)
			low = true;
		else if (2 == size)
			mid = true;
		else if (3 == size)
			high = true;
		game_start(hWnd, hInst);//重新开始

	}
	else if (flag0)//翻开格子
	{
		if (pt.x > 19 && pt.x < 19 + SIZE * col && pt.y > 70 && pt.y < 70 + SIZE * row)
		{
			if (!flag3)
			{
				flag3 = true;
				SetTimer(hWnd, 1, 1000, NULL);
			}
			flag2 = true;
			hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));//笑脸O
			SelectObject(hdcBmp, hBmp);
			BitBlt(hdc, (WIDTH / 2) - 42, 24, 24, 24, hdcBmp, 0, 72, SRCCOPY);
			hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			SelectObject(hdcBmp, hBmp);
			x = (pt.x - 19) / 16;
			y = (pt.y - 70) / 16;
			i = (x * 16) + 19;
			j = (y * 16) + 70;
			left[x][y] = 1;
			if (!right[x][y])
			{
				if (-1 == area_2d.data[x][y])
				{
					show_mine_area(hInst);//周围雷
					BitBlt(hdc, i, j, 16, 16, hdcBmp, 0, 48, SRCCOPY);//红雷
					hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));//失败笑脸
					SelectObject(hdcBmp, hBmp);
					BitBlt(hdc, (WIDTH / 2) - 42, 23, 24, 24, hdcBmp, 0, 48, SRCCOPY);
					hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));//白云
					SelectObject(hdcBmp, hBmp);
					BitBlt(hdc, WIDTH - 66, 20, 150, 240, hdcBmp, 0, 0, SRCCOPY);
					game_over(hWnd);
				}
				else if (0 == area_2d.data[x][y])
				{
					area_2d.data[x][y] = -10;
					BitBlt(hdc, i, j, 16, 16, hdcBmp, 0, 240, SRCCOPY);//空白块
					show_blank_area(hInst, x, y);//周围空白块
				}
				else if (area_2d.data[x][y])
				{
					show_mine_num(hInst, i, j);//周围雷数
					//area_2d.data[x][y] = -10;
					//BitBlt(hdc, i, j, 16, 16, hdcBmp, 0, 240, SRCCOPY);//空白块

				}
			}
		}
		else
		{
			hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));//笑脸O
			SelectObject(hdcBmp, hBmp);
			BitBlt(hdc, (WIDTH / 2) - 42, 24, 24, 24, hdcBmp, 0, 72, SRCCOPY);
		}
	}
}

//鼠标右键按下
void mouse_right(HINSTANCE hInst, int pt_x, int pt_y)
{
	int x, y, i, j;
	if (pt_x > 19 && pt_x < 19 + SIZE * col && pt_y > 70 && pt_y < 70 + SIZE * row)
	{
		x = (pt_x - 19) / SIZE;
		y = (pt_y - 70) / SIZE;
		i = (x * SIZE) + 19;
		j = (y * SIZE) + 70;
		hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		SelectObject(hdcBmp, hBmp);
		if (right[x][y])
		{
			right[x][y] = 0;
			BitBlt(hdc, i, j, 16, 16, hdcBmp, 0, 0, SRCCOPY);//空白块
			flag00--;
		}
		else if (!right[x][y] && !left[x][y])
		{
			right[x][y] = 1;
			BitBlt(hdc, i, j, 16, 16, hdcBmp, 0, 16, SRCCOPY);//旗
			flag00++;
		}
	}
}

//时钟消息
void time(HWND hWnd, HINSTANCE hInst)
{
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));//计时器
	SelectObject(hdcBmp, hBmp);
	BitBlt(hdc, WIDTH - 128, 24, 13, 23, hdcBmp, 0, 253 - (sec / 100) * 23, SRCCOPY);//百位
	BitBlt(hdc, WIDTH - 113, 24, 13, 23, hdcBmp, 0, 253 - ((sec % 100) / 10) * 23, SRCCOPY);//十位
	BitBlt(hdc, WIDTH - 98, 24, 13, 23, hdcBmp, 0, 253 - (sec % 10) * 23, SRCCOPY);//个位
	if (999 == (sec / 100) + ((sec % 100) / 10) + (sec % 10))
	{
		KillTimer(hWnd, 1);
		sec = 0;
		SetTimer(hWnd, 1, 1000, NULL);
	}
}

//插旗消息
void flag(HINSTANCE hInst)
{
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));//旗子数
	SelectObject(hdcBmp, hBmp);
	if (flag_num - flag00 >= 0)
	{
		BitBlt(hdc, 24, 24, 23, 23, hdcBmp, 0, 253 - ((flag_num - flag00) / 100) * 23, SRCCOPY);//百位
		BitBlt(hdc, 39, 24, 23, 23, hdcBmp, 0, 253 - ((flag_num - flag00) / 10 % 10) * 23, SRCCOPY);//十位
		BitBlt(hdc, 54, 24, 23, 23, hdcBmp, 0, 253 - ((flag_num - flag00) % 10) * 23, SRCCOPY);//个位
	}
	else
	{
		BitBlt(hdc, 24, 24, 23, 23, hdcBmp, 0, 0, SRCCOPY);//负号
		BitBlt(hdc, 39, 24, 23, 23, hdcBmp, 0, 253 - (((flag_num - flag00) * (-1)) / 10 % 10) * 23, SRCCOPY);//十位
		BitBlt(hdc, 54, 24, 23, 23, hdcBmp, 0, 253 - (((flag_num - flag00) * (-1)) % 10) * 23, SRCCOPY);//个位
	}
}

//销毁 
void destroy(HWND hWnd) 
{
	//// End application-specific layout section.
	for (int i = 0; i < col + 2; i++)
	{
		delete[] area_2d.data[i];
		delete[] left[i];
		delete[] right[i];
		area_2d.data[i] = nullptr;
		left[i] = nullptr;
		right[i] = nullptr;
	}
	delete[] area_2d.data;
	area_2d.data = nullptr;
	delete[] left;
	left = nullptr;
	delete[] right;
	right = nullptr;
	EndPaint(hWnd, &ps);
	//释放
	ReleaseDC(hWnd, hdc);
	ReleaseDC(hWnd, hdcBmp);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	DeleteObject(hBmp);
	KillTimer(hWnd, 1);
}



