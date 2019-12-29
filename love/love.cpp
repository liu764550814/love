
#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<math.h>
#include<mmsystem.h>//播放音乐，多媒体设备
#pragma comment(lib, "winmm.lib")//播放音乐

#if 0

#define NUM 13//在整个屏幕内，最多允许13个烟花弹


struct jet
{
	int x, y;//烟花弹的坐标
	int hx, hy;//烟花弹的最高坐标
	int height;//高度
	bool isshoot;//是否发射

	DWORD t1, t2, dt;//控制速度
	IMAGE img[2];//存放图片的数组
	byte n : 1;//图片的下标
}Jet[NUM];

struct fire
{
	int r;
	int max_r;
	int x, y;
	int cen_x, cen_y;
	int width, height;
	int xy[240][240];

	bool isshow;
	bool isdraw;

	DWORD t1, t2, dt;
}fire[NUM];


void welcome()
{
	initgraph(800, 600);//设置窗口
	srand((unsigned)time(NULL));//随机数

	mciSendString(L"open ./fire/bk1.mp3 alias music", 0, 0, 0); //mci Send String 发送字符串的指令给底层的声卡驱动
	mciSendString(L"play music", 0, 0, 0);//播放音乐


	////实现颜色的改变
	//while (!kbhit())
	//{
	//	setbkcolor(RGB(rand() % 255, rand() % 255, rand() % 255));//随机生成颜色的rgb值；
	//	cleardevice();//刷新屏幕
	//	Sleep(100);//休眠
	//}
	settextcolor(YELLOW);
	for (int i = 0; i < 50;++i)
	{
		settextstyle(i, 0, L"华文彩云");

		//在一个圆上取60个点
		int x = 300 + int(120 * sin(2 * 3.1415926*i / 60));
		int y = 200 + int(120 * cos(2 * 3.1415926*i / 60));
		
		outtextxy(x, y, L"情人节快乐");
		//outtextxy(50, 100, L"情人节快乐");
		Sleep(25);
	}


	getchar();
	cleardevice();


	settextstyle(25, 0, L"楷体");
	outtextxy(400, 200, L"天天开心");
	outtextxy(440, 240, L"——刘超明");


	getchar();

}

void load()
{


	IMAGE flimg, gimg;
	loadimage(&gimg,L"./fire/flower.jpg",3120,240);

	for (int i = 0; i < NUM; i++)
	{
		SetWorkingImage(&flimg);
		getimage(&gimg, i * 240, 0, 240, 240);
		SetWorkingImage(&gimg);

		for (int a = 0; a < 240; a++)
		{
			for (int b = 0; b < 240;b++)
			{
				fire[i].xy[a][b] = getpixel(a, b);
			}
		}
	}


	IMAGE jetimg;
	loadimage(&jetimg, L"./fire/shoot.jpg", 200, 50);

	for (int  i = 0; i < NUM; i++)//最多同时存在13个烟花
	{
		SetWorkingImage(&jetimg);//设置当前的设备为jetimg
		int n = rand() % 5;
		getimage(&Jet[i].img[0], n * 20, 0, 20, 50);//从当前设备获取图片
		getimage(&Jet[i].img[1], (n+5) * 20, 0, 20, 50);//从当前设备获取图片
	}


	SetWorkingImage(NULL);//设置当前的设备为窗口


	//putimage(200,200,&Jet[3].img[1]);
}


void init(int i)
{

	int r[13] = { 120, 120, 150, 155, 123, 147, 130, 140, 132, 137, 133, 127, 110 };
	int x[13] = { 100, 110, 109, 112, 120, 115, 105, 114, 125, 108, 130, 122, 90};
	int y[13] = { 107, 100, 109, 112, 110, 115, 121, 114, 117, 108, 130, 127, 95 };

	fire[i].x = 0;
	fire[i].y = 0;
	fire[i].width = 240;
	fire[i].height = 240;
	fire[i].max_r = r[i];
	fire[i].cen_x = x[i];
	fire[i].cen_y = y[i];
	fire[i].isshow = false;
	fire[i].dt = 5;
	fire[i].t1 = timeGetTime();
	fire[i].r = 0;


	Jet[i].x = 0;
	Jet[i].y = 0;
	Jet[i].hx = 0;
	Jet[i].hy = 0;
	Jet[i].height = 0;
	Jet[i].t1 = GetTickCount();
	Jet[i].dt = rand() % 10;
	Jet[i].n = 0;
	Jet[i].isshoot = false;
}

void chose(DWORD &t1)//选择烟花弹发射
{
	DWORD t2 = timeGetTime();

	if (t2-t1>100)//产生一个烟花弹
	{
		int n = rand() % 20;

		if (n < 13 && Jet[n].isshoot == false)
		{
			//重置烟花弹的数据
			Jet[n].x = rand() % 800;
			Jet[n].y = rand() % 100 + 500;
			Jet[n].hy = rand() % 400;
			Jet[n].hx = Jet[n].x;
			Jet[n].height = Jet[n].y - Jet[n].hy;
			Jet[n].isshoot = true;

			putimage(Jet[n].x, Jet[n].y, &Jet[n].img[Jet[n].n]);

		}
		t1 = t2;
	}
}

void shoot()
{
	for (int i = 0; i < 13; i++)
	{
		Jet[i].t2 = timeGetTime();
		if (Jet[i].t2 - Jet[i].t1>Jet[i].dt&&Jet[i].isshoot==true)
		{
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			if (Jet[i].y >= Jet[i].height)
			{
				Jet[i].n++;
				Jet[i].y -= 5;
			}
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			if ((Jet[i].y - Jet[i].hy) * 4 < Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			if (Jet[i].y <= Jet[i].hy)
			{
				// 播放爆炸声
				/*char c1[50], c2[30], c3[30];
				sprintf(c1, "open ./fire/bomb.wav alias n%d", i);
				sprintf(c2, "play n%d", i);
				sprintf(c3, "close s%d", i);

				mciSendString(c3, 0, 0, 0);
				mciSendString(c1, 0, 0, 0);
				mciSendString(c2, 0, 0, 0);*/

				putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// 擦掉烟花弹
				fire[i].x = Jet[i].hx + 10;											// 在烟花弹中间爆炸
				fire[i].y = Jet[i].hy;												// 在最高点绽放
				fire[i].isshow = true;					// 开始绽放
				Jet[i].isshoot = false;					// 停止发射

			}

			Jet[i].t1 = Jet[i].t2;
		}

	}
}

void style(DWORD &st1)
{
	DWORD st2 = timeGetTime();

	if (st2-st1>20000)
	{
		int x[13] = { 60, 75, 91, 100, 95, 75, 60, 45, 25, 15, 25, 41, 60 };
		int y[13] = { 65, 53, 40, 22, 5, 4, 20, 4, 5, 22, 40, 53, 65 };

		for (int i = 0; i < NUM; i++)
		{
			Jet[i].x = x[i] * 10;
			Jet[i].y = (y[i] + 75) * 10;
			Jet[i].hx = Jet[i].x;
			Jet[i].hy = y[i] * 10;
			Jet[i].height = Jet[i].y - Jet[i].hy;
			Jet[i].isshoot = true;
			Jet[i].dt = 7;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// 显示烟花弹

			/**** 设置烟花参数 ***/
			fire[i].x = Jet[i].x + 10;
			fire[i].y = Jet[i].hy;
			fire[i].isshow = false;
			fire[i].r = 0;

			/**** 播放发射声音 ***/
			/*char c1[50], c2[30], c3[30];
			sprintf(c1, "open ./fire/shoot.mp3 alias s%d", i);
			sprintf(c2, "play s%d", i);
			sprintf(c3, "close n%d", i);

			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);*/
		}
		st1 - st2;
	}
}

void show(DWORD *pMem)
{
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };

	for (int i = 0; i < NUM; i++)
	{
		fire[i].t2 = timeGetTime();

		if (fire[i].t2 - fire[i].t1 > fire[i].dt && fire[i].isshow == true)
		{
			if (fire[i].r < fire[i].max_r)
			{
				fire[i].r++;
				fire[i].dt = drt[fire[i].r / 10];
				fire[i].isdraw = true;
			}

			if (fire[i].r >= fire[i].max_r - 1)
			{
				fire[i].isdraw = false;
				init(i);
			}
			fire[i].t1 = fire[i].t2;
		}

		if (fire[i].isdraw)
		{
			for (double a = 0; a <= 6.28; a += 0.01)
			{
				int x1 = (int)(fire[i].cen_x + fire[i].r * cos(a));				// 相对于图片左上角的坐标
				int y1 = (int)(fire[i].cen_y - fire[i].r * sin(a));

				if (x1 > 0 && x1 < fire[i].width && y1 > 0 && y1 < fire[i].height)	// 只输出图片内的像素点
				{
					int b = fire[i].xy[x1][y1] & 0xff;
					int g = (fire[i].xy[x1][y1] >> 8) & 0xff;
					int r = (fire[i].xy[x1][y1] >> 16);

					// 烟花像素点在窗口上的坐标
					int xx = (int)(fire[i].x + fire[i].r * cos(a));
					int yy = (int)(fire[i].y - fire[i].r * sin(a));

					// 较暗的像素点不输出、防止越界
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 800 && yy > 0 && yy < 600)
					{
						pMem[yy * 800 + xx] = BGR(fire[i].xy[x1][y1]);	// 显存操作绘制烟花
					}
				}
				fire[i].isdraw = false;
			}
		}
	}
}



int main()
{
	welcome();

	

	DWORD t1 = timeGetTime();
	DWORD st1 = timeGetTime();
	DWORD *pMem = GetImageBuffer();
	for (int  i = 0; i < NUM; i++)
	{
		init(i);
	}

	load();
	BeginBatchDraw();

	while (!kbhit())
	{
		Sleep(10);

		for (int clr = 0; clr < 1000; clr++)
		{
			for (int j = 0; j < 2; j++)
			{
				int px1 = rand() % 800;
				int py1 = rand() % 600;

				if (py1 < 599)				// 防止越界
					pMem[py1 * 800 + px1] = pMem[py1 * 800 + px1 + 1] = BLACK;	// 对显存赋值擦出像素点
			}
		}

		chose(t1);
		shoot();
		show(pMem);
		style(st1);
		FlushBatchDraw();
	}

	getchar();
	return 0;
}
#endif

#if 1
/********************************************************
*														*
*	程序名称：	新年祝福烟花							*
*	编译环境：	VS2013 && easyx(冬至版)					*
*	作者相关：	TanZhouEDU—九夏老师					*
*	最后修改：	2016年5月19日							*
*														*
*********************************************************/
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <Mmsystem.h>		
#pragma comment ( lib, "Winmm.lib" )

/***** 宏定义区 ******/
#define pheight 600
#define pwidth 800
#define NUM		13			// 烟花种类数量宏定义
#define PI      3.1415926548

/***** 结构定义区 **********/

// 烟花结构
struct FIRE
{
	int r;					// 当前爆炸半径
	int max_r;				// 爆炸中心距离边缘最大半径
	int x, y;				// 爆炸中心在窗口的坐标
	int cen_x, cen_y;		// 爆炸中心相对图片左上角的坐标
	int width, height;		// 图片的宽高
	int xy[240][240];		// 储存图片像素点

	bool show;				// 是否绽放
	bool draw;				// 开始输出像素点
	DWORD t1, t2, dt;		// 绽放速度
}Fire[NUM];

// 烟花弹结构
struct JET
{
	int x, y;				// 喷射点坐标
	int hx, hy;				// 最高点坐标------将赋值给 FIRE 里面的 x, y
	int height;				// 烟花高度
	bool shoot;				// 是否可以发射

	DWORD t1, t2, dt;		// 发射速度
	IMAGE img[2];			// 储存花弹一亮一暗图片
	byte n : 1;				// 图片下标
}Jet[NUM];

int lo = 0;
int tm = 0;
/**** 函数申明区 ****/

void welcome();
void Init(int);		// 初始化烟花
void Load();		// 加载烟花图片
void Shoot();		// 发射烟花
void Chose(DWORD&);		// 筛选烟花
void Style(DWORD&);		// 发射样式
void Show(DWORD*);		// 绽放烟花
void end();
void proend();


// 主函数
void main()
{
	initgraph(800, 600);
	srand(time(0));

	// 播放背景音乐
	mciSendString(L"open ./fire/小幸运.mp3 alias bk", 0, 0, 0);
	mciSendString(L"play bk repeat", 0, 0, 0);
	//mciSendString(L"open ./fire/今天你要嫁给我.mp3 alias bk", 0, 0, 0);
	//mciSendString(L"play bk repeat", 0, 0, 0);
	//mciSendString(L"open ./fire/bk1.mp3 alias bk", 0, 0, 0);
	//mciSendString(L"play bk repeat", 0, 0, 0);

	welcome();

	end();

	DWORD t1 = timeGetTime();			// 筛选烟花计时
	DWORD st1 = timeGetTime();			// 播放花样计时
	DWORD* pMem = GetImageBuffer();		// 获取窗口显存指针

	for (int i = 0; i < NUM; i++)		// 初始化烟花
	{
		Init(i);
	}
	Load();								// 将烟花图片信息加载进相应结构中
	BeginBatchDraw();					// 开始批量绘图

	int en = 0;
	while (!kbhit())
	{
		Sleep(7);

		// 随机选择 4000 个像素点擦除
		for (int clr = 0; clr < 1000; clr++)
		{
			for (int j = 0; j < 2; j++)
			{
				int px1 = rand() % pwidth;
				int py1 = rand() % pheight;

				if (py1 < 599)				// 防止越界
					pMem[py1 * pwidth + px1] = pMem[py1 * pwidth + px1 + 1] = BLACK;	// 对显存赋值擦出像素点
			}
		}
		Chose(t1);			// 筛选烟花
		Shoot();			// 发射烟花
		Show(pMem);			// 绽放烟花
		Style(st1);			// 花样发射
		FlushBatchDraw();	// 显示前面的所有绘图操作
		tm++;
		if (tm>=1000)
		{
			switch (lo)
			{
			case 0:
				setcolor(GREEN);
				settextstyle(50, 0, L"楷体");
				outtextxy(300, 250, L"我");
				Sleep(100);
				break;
			case 1:
				setcolor(WHITE);
				settextstyle(50, 0, L"楷体");
				outtextxy(350, 250, L"爱");
				Sleep(100);
				break;
			case 2:
				setcolor(RED);
				settextstyle(50, 0, L"楷体");
				outtextxy(400, 250, L"大");
				Sleep(100);
				break;
			case 3:
				setcolor(BLUE);
				settextstyle(50, 0, L"楷体");
				outtextxy(450, 250, L"王");
				Sleep(100);
				break;
			case 4:
				settextstyle(50, 0, L"楷体");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(300, 250, L"我");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(350, 250, L"喜");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(400, 250, L"欢");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(450, 250, L"大");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(500, 250, L"王");
				Sleep(100);
				break;
			case 5:
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				settextstyle(50, 0, L"楷体");
				outtextxy(450, 250, L"以后------");
				Sleep(100);
				break;
			case 6:
				settextstyle(50, 0, L"楷体");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(300, 250, L"愿");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(350, 250, L"携");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(400, 250, L"手");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(450, 250, L"不");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(500, 250, L"离");
				Sleep(100);
				break;
			case 7:
				exit(0);
			default:
				break;
			}
			lo++;
			tm = 0;
		}
		//if (en==1)
		//{
		//	break;
		//}
	}
	
}


void welcome()
{
	//setfillstyle(0);
	//setcolor(YELLOW);

	for (int i = 0; i < 50; i++)
	{
		setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		int x = 600 + int(180 * sin(PI * 2 * i / 60));
		int y = 200 + int(180 * cos(PI * 2 * i / 60));
		cleardevice();
		settextstyle(i, 0, L"楷体");
		outtextxy(200, 250, L"愿大王天天开心！");
		outtextxy(x - 10, y + 100, L"——王雨洁");
		Sleep(25);
	}

	Sleep(5000);
	cleardevice();
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	settextstyle(25, 0,L"楷体");
	outtextxy(200, 100, L"我喜欢你");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 150, L"不是因为你是什么样的人");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 200, L"而是因为");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 250, L"喜欢");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 300, L"和你在一起的");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 350, L"感觉");
	//outtextxy(300, 400, L"----《小幸运》");
	Sleep(5000);
}

// 初始化烟花参数
void Init(int i)
{
	// 分别为：烟花中心到图片边缘的最远距离、烟花中心到图片左上角的距离 (x、y) 两个分量
	int r[13] = { 80, 80, 90, 90, 130, 89, 87, 86, 85, 89, 84, 83, 82 };
	int x[13] = { 120, 120, 110, 117, 110, 93, 102, 102, 110, 105, 100, 108, 110 };
	int y[13] = { 120, 120, 85, 118, 120, 103, 105, 110, 110, 120, 120, 104, 85 };

	/**** 初始化烟花 *****/

	Fire[i].x = 0;				// 烟花中心坐标
	Fire[i].y = 0;
	Fire[i].width = 240;				// 图片宽
	Fire[i].height = 240;				// 图片高
	Fire[i].max_r = r[i];				// 最大半径
	Fire[i].cen_x = x[i];				// 中心距左上角距离
	Fire[i].cen_y = y[i];
	Fire[i].show = false;			// 是否绽放
	Fire[i].dt = 5;				// 绽放时间间隔
	Fire[i].t1 = timeGetTime();
	Fire[i].r = 0;				// 从 0 开始绽放

	/**** 初始化烟花弹 *****/

	Jet[i].x = -240;				// 烟花弹左上角坐标
	Jet[i].y = -240;
	Jet[i].hx = -240;				// 烟花弹发射最高点坐标
	Jet[i].hy = -240;
	Jet[i].height = 0;				// 发射高度
	Jet[i].t1 = timeGetTime();
	Jet[i].dt = rand() % 10;		// 发射速度时间间隔
	Jet[i].n = 0;				// 烟花弹闪烁图片下标
	Jet[i].shoot = false;			// 是否发射
}


// 加载图片
void Load()
{
	/**** 储存烟花的像素点颜色 ****/
	IMAGE fm, gm;
	loadimage(&fm, L"./fire/flower.jpg", 3120, 240);

	for (int i = 0; i < 13; i++)
	{
		SetWorkingImage(&fm);
		getimage(&gm, i * 240, 0, 240, 240);
		SetWorkingImage(&gm);

		for (int a = 0; a < 240; a++)
		for (int b = 0; b < 240; b++)
			Fire[i].xy[a][b] = getpixel(a, b);
	}

	/**** 加载烟花弹 ************/
	IMAGE sm;
	loadimage(&sm, L"./fire/shoot.jpg", 200, 50);

	for (int i = 0; i < 13; i++)
	{
		SetWorkingImage(&sm);
		int n = rand() % 5;

		getimage(&Jet[i].img[0], n * 20, 0, 20, 50);			// 暗
		getimage(&Jet[i].img[1], (n + 5) * 20, 0, 20, 50);		// 亮
	}


	SetWorkingImage();		// 设置回绘图窗口
}


// 在一定范围内筛选可发射的烟花，并初始化发射参数，输出烟花弹到屏幕，播放声音
void Chose(DWORD& t1)
{
	DWORD t2 = timeGetTime();

	if (t2 - t1 > 100)
	{
		int n = rand() % 20;

		if (n < 13 && Jet[n].shoot == false && Fire[n].show == false)
		{
			/**** 重置烟花弹，预备发射 *****/
			Jet[n].x = rand() % pwidth;
			Jet[n].y = rand() % 100 + 600;
			Jet[n].hx = Jet[n].x;
			Jet[n].hy = rand() % 400;
			Jet[n].height = Jet[n].y - Jet[n].hy;
			Jet[n].shoot = true;
			putimage(Jet[n].x, Jet[n].y, &Jet[n].img[Jet[n].n], SRCINVERT);

			/**** 播放每个烟花弹的声音 *****/
			/*char c1[50], c2[30], c3[30];
			sprintf(c1, "open ./fire/shoot.mp3 alias s%d", n);
			sprintf(c2, "play s%d", n);
			sprintf(c3, "close n%d", n);

			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);*/
		}
		t1 = t2;
	}
}


// 扫描烟花弹并发射
void Shoot()
{
	for (int i = 0; i < 13; i++)
	{
		Jet[i].t2 = timeGetTime();

		if (Jet[i].t2 - Jet[i].t1 > Jet[i].dt && Jet[i].shoot == true)
		{
			/**** 烟花弹的上升 *****/
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			if (Jet[i].y > Jet[i].hy)
			{
				Jet[i].n++;
				Jet[i].y -= 5;
			}

			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			/**** 上升到高度的 3 / 4，减速 *****/
			if ((Jet[i].y - Jet[i].hy) * 4 < Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			/**** 上升到最大高度 *****/
			if (Jet[i].y <= Jet[i].hy)
			{
				// 播放爆炸声
				/*char c1[50], c2[30], c3[30];
				sprintf(c1, "open ./fire/bomb.wav alias n%d", i);
				sprintf(c2, "play n%d", i);
				sprintf(c3, "close s%d", i);

				mciSendString(c3, 0, 0, 0);
				mciSendString(c1, 0, 0, 0);
				mciSendString(c2, 0, 0, 0);*/

				putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// 擦掉烟花弹
				Fire[i].x = Jet[i].hx + 10;											// 在烟花弹中间爆炸
				Fire[i].y = Jet[i].hy;												// 在最高点绽放
				Fire[i].show = true;					// 开始绽放
				Jet[i].shoot = false;					// 停止发射

			}
			Jet[i].t1 = Jet[i].t2;
		}
	}
}



// 显示花样
void Style(DWORD& st1)
{
	DWORD st2 = timeGetTime();

	if (st2 - st1 >20000)		// 一首歌的时间
	{
		// 心形坐标
		int x[13] = { 60 * 0.66, 75 * 0.66, 91 * 0.66, 100 * 0.66, 95 * 0.66, 75 * 0.66, 60 * 0.66, 45 * 0.66, 25 * 0.66, 15 * 0.66, 25 * 0.66, 41 * 0.66, 60 * 0.66 };
		int y[13] = { 65 * 0.75, 53 * 0.75, 40 * 0.75, 22 * 0.75, 5 * 0.75, 4 * 0.75, 20, 4 * 0.75, 5 * 0.75, 22 * 0.75, 40 * 0.75, 53 * 0.75, 65 * 0.75 };
		for (int i = 0; i < NUM; i++)
		{
			//cleardevice();
			/**** 规律分布烟花弹 ***/
			Jet[i].x = x[i] * 10;
			Jet[i].y = (y[i] + 75) * 10;
			Jet[i].hx = Jet[i].x;
			Jet[i].hy = y[i] * 10;
			Jet[i].height = Jet[i].y - Jet[i].hy;
			Jet[i].shoot = true;
			Jet[i].dt = 7;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// 显示烟花弹

			/**** 设置烟花参数 ***/
			Fire[i].x = Jet[i].x + 10;
			Fire[i].y = Jet[i].hy;
			Fire[i].show = false;
			Fire[i].r = 0;


			/**** 播放发射声音 ***/
		/*	char c1[50], c2[30], c3[30];
			sprintf(c1, "open ./fire/shoot.mp3 alias s%d", i);
			sprintf(c2, "play s%d", i);
			sprintf(c3, "close n%d", i);

			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);*/

		}
		st1 = st2;
	}
}


// 绽放烟花
void Show(DWORD* pMem)
{
	// 烟花个阶段绽放时间间隔，制作变速绽放效果
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };

	for (int i = 0; i < NUM; i++)
	{
		Fire[i].t2 = timeGetTime();

		// 增加爆炸半径，绽放烟花，增加时间间隔做变速效果
		if (Fire[i].t2 - Fire[i].t1 > Fire[i].dt && Fire[i].show == true)
		{
			if (Fire[i].r < Fire[i].max_r)
			{
				Fire[i].r++;
				Fire[i].dt = drt[Fire[i].r / 10];
				Fire[i].draw = true;
			}

			if (Fire[i].r >= Fire[i].max_r - 1)
			{
				Fire[i].draw = false;
				Init(i);
			}
			Fire[i].t1 = Fire[i].t2;
		}

		// 如果该号炮花可爆炸，根据当前爆炸半径画烟花，颜色值接近黑色的不输出。
		if (Fire[i].draw)
		{
			for (double a = 0; a <= 6.28; a += 0.01)
			{
				int x1 = (int)(Fire[i].cen_x + Fire[i].r * cos(a));				// 相对于图片左上角的坐标
				int y1 = (int)(Fire[i].cen_y - Fire[i].r * sin(a));

				if (x1 > 0 && x1 < Fire[i].width && y1 > 0 && y1 < Fire[i].height)	// 只输出图片内的像素点
				{
					int b = Fire[i].xy[x1][y1] & 0xff;
					int g = (Fire[i].xy[x1][y1] >> 8) & 0xff;
					int r = (Fire[i].xy[x1][y1] >> 16);

					// 烟花像素点在窗口上的坐标
					int xx = (int)(Fire[i].x + Fire[i].r * cos(a));
					int yy = (int)(Fire[i].y - Fire[i].r * sin(a));

					// 较暗的像素点不输出、防止越界
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < pwidth && yy > 0 && yy < pheight)
						pMem[yy * pwidth + xx] = BGR(Fire[i].xy[x1][y1]);	// 显存操作绘制烟花
				}
			}
			Fire[i].draw = false;
		}
	}
}



void end()
{
	for (int i = 0; i < 50; ++i)
	{
		settextstyle(30, 0, L"华文彩云");

		//在一个圆上取60个点
		int x = 200 + int(120 * sin(2 * 3.1415926*i / 60));
		int y = 200 + int(120 * cos(2 * 3.1415926*i / 60));

		setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));

		outtextxy(x, y, L"I will never never never give you up forever!");
		outtextxy(x = 300, y + 50, L"---I promise!");
		Sleep(25);
		if (i != 49)
		{
			cleardevice();
		}
	}
}
#endif