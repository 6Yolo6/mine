#pragma once

#include "resource.h"
#include "framework.h"

//初始化
void game_init(HWND hWnd)
{
	static int i = 0;
	//默认开局低难度
	while (!i)
	{
		i++;
		low = true;
	}
	if (low)
	{
		row = 9;
		col = 9;
		mine = 10;
		flag_num = 10;
	}
	else if (mid)
	{
		row = 16;
		col = 16;
		mine = 40;
		flag_num = 40;
	}
	else if (high)
	{
		row = 16;
		col = 30;
		mine = 99;
		flag_num = 99;
	}
	flag00 = 0;
	hdc = GetDC(hWnd);
	hdcBmp = CreateCompatibleDC(hdc);
	hBmp = nullptr;
	area_2d.width = col;
	area_2d.height = row;
	area_2d.data = new int* [col + 2];
	left = new int* [col + 2];
	right = new int* [col + 2];
	for (int i = 0; i < col + 2; i++)
	{
		area_2d.data[i] = new int[row + 2];
		left[i] = new int[row + 2];
		right[i] = new int[row + 2];
		memset(area_2d.data[i], 0, (row + 2) * sizeof(int));
		memset(left[i], 0, (row + 2) * sizeof(int));
		memset(right[i], 0, (row + 2) * sizeof(int));
	}
	flag0 = true;
	Cnt = 0;
	set_mine();//随机布雷
	calc_mine_num();//计算周围雷数
	MoveWindow(hWnd, 100, 100, WIDTH - 50, HEIGHT - 50, true);
}

//随机布雷
void set_mine()
{
	srand((unsigned)time(NULL));
	int cnt = mine;
	int i, j;
	while (cnt)
	{
		i = rand() % col;
		j = rand() % row;
		if (0 == area_2d.data[i][j])
		{
			area_2d.data[i][j] = -1;
			cnt--;
		}
	}
}

//展开周围雷
void show_mine_area(HINSTANCE hInst)
{
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	SelectObject(hdcBmp, hBmp);
	int m, n;
	for (int i = 19; i < 19 + col * SIZE; i += SIZE)
	{
		for (int j = 70; j < 70 + row * SIZE; j += SIZE)
		{
			m = (i - 19) / SIZE;
			n = (j - 70) / SIZE;
			if (-1 == area_2d.data[m][n])
			{
				if (right[m][n])
					BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 64, SRCCOPY);
				else
					BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 80, SRCCOPY);
			}
		}
	}
}

//显示周围雷数
void show_mine_num(HINSTANCE hInst, int i, int j)
{
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	SelectObject(hdcBmp, hBmp);
	int x, y;
	x = (i - 19) / SIZE;
	y = (j - 70) / SIZE;
	BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240 - (area_2d.data[x][y] * 15), SRCCOPY);
}

//展开周围空白块
void show_blank_area(HINSTANCE hInst, int x, int y)
{
	std::queue<POINT> q;
	POINT point = { x, y };
	q.push(point);
	while (!q.empty()) {
		hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		SelectObject(hdcBmp, hBmp);
		int mine_x = q.front().x;
		int mine_y = q.front().y;
		if (mine_x + 1 < area_2d.width && !right[mine_x + 1][mine_y]) {
			if (0 == area_2d.data[mine_x + 1][mine_y]) {
				point = { mine_x + 1, mine_y };
				q.push(point);
				area_2d.data[mine_x + 1][mine_y] = -10;
				left[mine_x + 1][mine_y] = 1;
				int i = ((mine_x + 1) * SIZE) + 19;
				int j = (mine_y * SIZE) + 70;
				BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
			}
			show_mine_num(hInst, ((mine_x + 1) * SIZE) + 19, (mine_y * SIZE) + 70);
		}
		if (mine_x - 1 >= 0 && !right[mine_x - 1][mine_y]) {
			if (0 == area_2d.data[mine_x - 1][mine_y]) {
				point = { mine_x - 1, mine_y };
				q.push(point);
				area_2d.data[mine_x - 1][mine_y] = -10;
				left[mine_x - 1][mine_y] = 1;
				int i = ((mine_x - 1) * SIZE) + 19;
				int j = (mine_y * SIZE) + 70;
				BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
			}
			show_mine_num(hInst, ((mine_x - 1) * SIZE) + 19, (mine_y * SIZE) + 70);
		}
		if (mine_y + 1 < area_2d.height && !right[mine_x][mine_y + 1]) {
			if (0 == area_2d.data[mine_x][mine_y + 1]) {
				point = { mine_x, mine_y + 1 };
				q.push(point);
				area_2d.data[mine_x][mine_y + 1] = -10;
				left[mine_x][mine_y + 1] = 1;
				int i = (mine_x * SIZE) + 19;
				int j = ((mine_y + 1) * SIZE) + 70;
				BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
			}
			show_mine_num(hInst, (mine_x * SIZE) + 19, ((mine_y + 1) * SIZE) + 70);
		}
		if (mine_y - 1 >= 0 && !right[mine_x][mine_y - 1]) {
			if (0 == area_2d.data[mine_x][mine_y - 1]) {
				point = { mine_x, mine_y - 1 };
				q.push(point);
				area_2d.data[mine_x][mine_y - 1] = -10;
				left[mine_x][mine_y - 1] = 1;
				int i = (mine_x * SIZE) + 19;
				int j = ((mine_y - 1) * SIZE) + 70;
				BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
			}
			show_mine_num(hInst, (mine_x * SIZE) + 19, ((mine_y - 1) * SIZE) + 70);
		}
		if (mine_x + 1 < area_2d.width && mine_y + 1 < area_2d.height && !right[mine_x + 1][mine_y + 1]) {
			if (0 == area_2d.data[mine_x + 1][mine_y + 1]) {
				point = { mine_x + 1, mine_y + 1 };
				q.push(point);
				area_2d.data[mine_x + 1][mine_y + 1] = -10;
				left[mine_x + 1][mine_y + 1] = 1;
				int i = ((mine_x + 1) * SIZE) + 19;
				int j = ((mine_y + 1) * SIZE) + 70;
				BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
			}
			show_mine_num(hInst, ((mine_x + 1) * SIZE) + 19, ((mine_y + 1) * SIZE) + 70);
		}
		if (mine_x + 1 < area_2d.width && mine_y - 1 >= 0 && !right[mine_x + 1][mine_y - 1]) {
			if (0 == area_2d.data[mine_x + 1][mine_y - 1]) {
				point = { mine_x + 1, mine_y - 1 };
				q.push(point);
				area_2d.data[mine_x + 1][mine_y - 1] = -10;
				left[mine_x + 1][mine_y - 1] = 1;
				int i = ((mine_x + 1) * SIZE) + 19;
				int j = ((mine_y - 1) * SIZE) + 70;
				BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
			}
			show_mine_num(hInst, ((mine_x + 1) * SIZE) + 19, ((mine_y - 1) * SIZE) + 70);
		}
		if (mine_x - 1 >= 0 && mine_y - 1 >= 0 && !right[mine_x - 1][mine_y - 1]) {
			if (0 == area_2d.data[mine_x - 1][mine_y - 1]) {
				point = { mine_x - 1, mine_y - 1 };
				q.push(point);
				area_2d.data[mine_x - 1][mine_y - 1] = -10;
				left[mine_x - 1][mine_y - 1] = 1;
				int i = ((mine_x - 1) * SIZE) + 19;
				int j = ((mine_y - 1) * SIZE) + 70;
				BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
			}
			show_mine_num(hInst, ((mine_x - 1) * SIZE) + 19, ((mine_y - 1) * SIZE) + 70);
		}
		if (mine_x - 1 >= 0 && mine_y + 1 < area_2d.height && !right[mine_x - 1][mine_y + 1]) {
			if (0 == area_2d.data[mine_x - 1][mine_y + 1]) {
				point = { mine_x - 1, mine_y + 1 };
				q.push(point);
				area_2d.data[mine_x - 1][mine_y + 1] = -10;
				left[mine_x - 1][mine_y + 1] = 1;
				int i = ((mine_x - 1) * SIZE) + 19;
				int j = ((mine_y + 1) * SIZE) + 70;
				BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
			}
			show_mine_num(hInst, ((mine_x - 1) * SIZE) + 19, ((mine_y + 1) * SIZE) + 70);
		}
		q.pop();

		//for (int k = 0; k < 8; k++)
		//{
		//	mine_x = x + X[k];
		//	mine_y = y + Y[k];
		//	if (mine_x >= 0 && mine_x < area_2d.width && mine_y >= 0 && mine_y < area_2d.height)
		//	{
		//		if (0 == area_2d.data[mine_x][mine_y])
		//		{
		//			int i = (mine_x * SIZE) + 19;
		//			int j = (mine_y * SIZE) + 70;
		//			if (right[mine_x][mine_y])
		//				continue;
		//			BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
		//			//show_blank_area(hInst, mine_x, mine_y);
		//		}
		//		else
		//		{
		//			show_mine_num(hInst, (mine_x * SIZE) + 19, (mine_y * SIZE) + 70);
		//		}
		//		left[mine_x][mine_y] = 1;
		//	}
		//}
		//q.pop();
	}
	//int i, j, mine_x, mine_y;
	//hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	//SelectObject(hdcBmp, hBmp);
	//for (int k = 0; k < 8; k++)
	//{
	//	mine_x = x + X[k];
	//	mine_y = y + Y[k];
	//	if (mine_x >= 0 && mine_x < area_2d.width && mine_y >= 0 && mine_y < area_2d.height)
	//	{
	//		if (0 == area_2d.data[mine_x][mine_y])
	//		{
	//			area_2d.data[mine_x][mine_y] = -10;
	//			i = (mine_x * SIZE) + 19;
	//			j = (mine_y * SIZE) + 70;
	//			if (right[mine_x][mine_y])
	//				continue;
	//			BitBlt(hdc, i, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
	//			//BitBlt(hdc, i, j + 1, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
	//			//BitBlt(hdc, i, j - 1, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
	//			//BitBlt(hdc, i - 1, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
	//			//BitBlt(hdc, i + 1, j, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
	//			//BitBlt(hdc, i + 1, j - 1, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
	//			//BitBlt(hdc, i + 1, j + 1, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
	//			//BitBlt(hdc, i - 1, j - 1, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
	//			//BitBlt(hdc, i - 1, j + 1, SIZE, SIZE, hdcBmp, 0, 240, SRCCOPY);
	//			show_blank_area(hInst, mine_x, mine_y);
	//		}
	//		else
	//		{
	//			//show_blank_area(hInst, (mine_x * SIZE) + 19, (mine_y * SIZE) + 70);
	//			show_mine_num(hInst, (mine_x * SIZE) + 19, (mine_y* SIZE) + 70);
	//		}
	//		left[mine_x][mine_y] = 1;
	//	}
	//}
}
//计算周围雷数
void calc_mine_num()
{
	for (int i = 0; i < area_2d.width; i++)
	{
		for (int j = 0; j < area_2d.height; j++)
		{
			if (area_2d.data[i][j] == 0)
			{
				int num = 0;
				int mine_x, mine_y;
				for (int k = 0; k < 8; k++)
				{
					mine_x = i + X[k];
					mine_y = j + Y[k];
					num += (mine_x >= 0 && mine_x < area_2d.width && mine_y >= 0 && mine_y < area_2d.height && area_2d.data[mine_x][mine_y] == -1);
				}
				area_2d.data[i][j] = num;
			}
		}
	}
}

//菜单栏的勾选
void check(int wmId, HMENU hMenu)
{
	WORD ID[5] = { ID_32771, ID_32772, ID_32773, ID_32774, ID_32775 };
	for (int i = 0; i < 5; i++)
	{
		if (wmId == ID[i])
		{
			CheckMenuItem(hMenu, wmId, MF_CHECKED);
		}
		else
		{
			CheckMenuItem(hMenu, ID[i], MF_UNCHECKED);
		}
	}
}

//判断胜负
void win_lose(HWND hWnd, HINSTANCE hInst)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (0 == left[i][j] || -1 == area_2d.data[i][j])
			{
				Cnt++;
			} 
		}
	}
	if (mine == Cnt)
	{
		KillTimer(hWnd, 1);
		flag0 = false;
		hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));//胜利笑脸
		SelectObject(hdcBmp, hBmp);
		BitBlt(hdc, (WIDTH / 2) - 42, 24, 24, 24, hdcBmp, 0, 24, SRCCOPY);
		hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));//vv
		SelectObject(hdcBmp, hBmp);
		BitBlt(hdc, WIDTH - 66, 25, 250, 250, hdcBmp, 65, 20, SRCCOPY);
		MessageBox(hWnd, TEXT("You win !"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		Cnt = 0;
	}
	Cnt = 0;
}

//游戏结束
void game_over(HWND hWnd)
{
	KillTimer(hWnd, 1);
	flag0 = false;
	MessageBox(hWnd, TEXT("Game over !"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
}


//重新开始
void game_start(HWND hWnd, HINSTANCE hInst)
{
	flag3 = false;
	KillTimer(hWnd, 1);
	game_init(hWnd);
	InvalidateRect(hWnd, NULL, TRUE);
	game_paint(hWnd, hInst);
}