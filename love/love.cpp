
#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<math.h>
#include<mmsystem.h>//�������֣���ý���豸
#pragma comment(lib, "winmm.lib")//��������

#if 0

#define NUM 13//��������Ļ�ڣ��������13���̻���


struct jet
{
	int x, y;//�̻���������
	int hx, hy;//�̻������������
	int height;//�߶�
	bool isshoot;//�Ƿ���

	DWORD t1, t2, dt;//�����ٶ�
	IMAGE img[2];//���ͼƬ������
	byte n : 1;//ͼƬ���±�
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
	initgraph(800, 600);//���ô���
	srand((unsigned)time(NULL));//�����

	mciSendString(L"open ./fire/bk1.mp3 alias music", 0, 0, 0); //mci Send String �����ַ�����ָ����ײ����������
	mciSendString(L"play music", 0, 0, 0);//��������


	////ʵ����ɫ�ĸı�
	//while (!kbhit())
	//{
	//	setbkcolor(RGB(rand() % 255, rand() % 255, rand() % 255));//���������ɫ��rgbֵ��
	//	cleardevice();//ˢ����Ļ
	//	Sleep(100);//����
	//}
	settextcolor(YELLOW);
	for (int i = 0; i < 50;++i)
	{
		settextstyle(i, 0, L"���Ĳ���");

		//��һ��Բ��ȡ60����
		int x = 300 + int(120 * sin(2 * 3.1415926*i / 60));
		int y = 200 + int(120 * cos(2 * 3.1415926*i / 60));
		
		outtextxy(x, y, L"���˽ڿ���");
		//outtextxy(50, 100, L"���˽ڿ���");
		Sleep(25);
	}


	getchar();
	cleardevice();


	settextstyle(25, 0, L"����");
	outtextxy(400, 200, L"���쿪��");
	outtextxy(440, 240, L"����������");


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

	for (int  i = 0; i < NUM; i++)//���ͬʱ����13���̻�
	{
		SetWorkingImage(&jetimg);//���õ�ǰ���豸Ϊjetimg
		int n = rand() % 5;
		getimage(&Jet[i].img[0], n * 20, 0, 20, 50);//�ӵ�ǰ�豸��ȡͼƬ
		getimage(&Jet[i].img[1], (n+5) * 20, 0, 20, 50);//�ӵ�ǰ�豸��ȡͼƬ
	}


	SetWorkingImage(NULL);//���õ�ǰ���豸Ϊ����


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

void chose(DWORD &t1)//ѡ���̻�������
{
	DWORD t2 = timeGetTime();

	if (t2-t1>100)//����һ���̻���
	{
		int n = rand() % 20;

		if (n < 13 && Jet[n].isshoot == false)
		{
			//�����̻���������
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
				// ���ű�ը��
				/*char c1[50], c2[30], c3[30];
				sprintf(c1, "open ./fire/bomb.wav alias n%d", i);
				sprintf(c2, "play n%d", i);
				sprintf(c3, "close s%d", i);

				mciSendString(c3, 0, 0, 0);
				mciSendString(c1, 0, 0, 0);
				mciSendString(c2, 0, 0, 0);*/

				putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// �����̻���
				fire[i].x = Jet[i].hx + 10;											// ���̻����м䱬ը
				fire[i].y = Jet[i].hy;												// ����ߵ�����
				fire[i].isshow = true;					// ��ʼ����
				Jet[i].isshoot = false;					// ֹͣ����

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
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// ��ʾ�̻���

			/**** �����̻����� ***/
			fire[i].x = Jet[i].x + 10;
			fire[i].y = Jet[i].hy;
			fire[i].isshow = false;
			fire[i].r = 0;

			/**** ���ŷ������� ***/
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
				int x1 = (int)(fire[i].cen_x + fire[i].r * cos(a));				// �����ͼƬ���Ͻǵ�����
				int y1 = (int)(fire[i].cen_y - fire[i].r * sin(a));

				if (x1 > 0 && x1 < fire[i].width && y1 > 0 && y1 < fire[i].height)	// ֻ���ͼƬ�ڵ����ص�
				{
					int b = fire[i].xy[x1][y1] & 0xff;
					int g = (fire[i].xy[x1][y1] >> 8) & 0xff;
					int r = (fire[i].xy[x1][y1] >> 16);

					// �̻����ص��ڴ����ϵ�����
					int xx = (int)(fire[i].x + fire[i].r * cos(a));
					int yy = (int)(fire[i].y - fire[i].r * sin(a));

					// �ϰ������ص㲻�������ֹԽ��
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 800 && yy > 0 && yy < 600)
					{
						pMem[yy * 800 + xx] = BGR(fire[i].xy[x1][y1]);	// �Դ���������̻�
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

				if (py1 < 599)				// ��ֹԽ��
					pMem[py1 * 800 + px1] = pMem[py1 * 800 + px1 + 1] = BLACK;	// ���Դ渳ֵ�������ص�
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
*	�������ƣ�	����ף���̻�							*
*	���뻷����	VS2013 && easyx(������)					*
*	������أ�	TanZhouEDU��������ʦ					*
*	����޸ģ�	2016��5��19��							*
*														*
*********************************************************/
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <Mmsystem.h>		
#pragma comment ( lib, "Winmm.lib" )

/***** �궨���� ******/
#define pheight 600
#define pwidth 800
#define NUM		13			// �̻����������궨��
#define PI      3.1415926548

/***** �ṹ������ **********/

// �̻��ṹ
struct FIRE
{
	int r;					// ��ǰ��ը�뾶
	int max_r;				// ��ը���ľ����Ե���뾶
	int x, y;				// ��ը�����ڴ��ڵ�����
	int cen_x, cen_y;		// ��ը�������ͼƬ���Ͻǵ�����
	int width, height;		// ͼƬ�Ŀ��
	int xy[240][240];		// ����ͼƬ���ص�

	bool show;				// �Ƿ�����
	bool draw;				// ��ʼ������ص�
	DWORD t1, t2, dt;		// �����ٶ�
}Fire[NUM];

// �̻����ṹ
struct JET
{
	int x, y;				// ���������
	int hx, hy;				// ��ߵ�����------����ֵ�� FIRE ����� x, y
	int height;				// �̻��߶�
	bool shoot;				// �Ƿ���Է���

	DWORD t1, t2, dt;		// �����ٶ�
	IMAGE img[2];			// ���滨��һ��һ��ͼƬ
	byte n : 1;				// ͼƬ�±�
}Jet[NUM];

int lo = 0;
int tm = 0;
/**** ���������� ****/

void welcome();
void Init(int);		// ��ʼ���̻�
void Load();		// �����̻�ͼƬ
void Shoot();		// �����̻�
void Chose(DWORD&);		// ɸѡ�̻�
void Style(DWORD&);		// ������ʽ
void Show(DWORD*);		// �����̻�
void end();
void proend();


// ������
void main()
{
	initgraph(800, 600);
	srand(time(0));

	// ���ű�������
	mciSendString(L"open ./fire/С����.mp3 alias bk", 0, 0, 0);
	mciSendString(L"play bk repeat", 0, 0, 0);
	//mciSendString(L"open ./fire/������Ҫ�޸���.mp3 alias bk", 0, 0, 0);
	//mciSendString(L"play bk repeat", 0, 0, 0);
	//mciSendString(L"open ./fire/bk1.mp3 alias bk", 0, 0, 0);
	//mciSendString(L"play bk repeat", 0, 0, 0);

	welcome();

	end();

	DWORD t1 = timeGetTime();			// ɸѡ�̻���ʱ
	DWORD st1 = timeGetTime();			// ���Ż�����ʱ
	DWORD* pMem = GetImageBuffer();		// ��ȡ�����Դ�ָ��

	for (int i = 0; i < NUM; i++)		// ��ʼ���̻�
	{
		Init(i);
	}
	Load();								// ���̻�ͼƬ��Ϣ���ؽ���Ӧ�ṹ��
	BeginBatchDraw();					// ��ʼ������ͼ

	int en = 0;
	while (!kbhit())
	{
		Sleep(7);

		// ���ѡ�� 4000 �����ص����
		for (int clr = 0; clr < 1000; clr++)
		{
			for (int j = 0; j < 2; j++)
			{
				int px1 = rand() % pwidth;
				int py1 = rand() % pheight;

				if (py1 < 599)				// ��ֹԽ��
					pMem[py1 * pwidth + px1] = pMem[py1 * pwidth + px1 + 1] = BLACK;	// ���Դ渳ֵ�������ص�
			}
		}
		Chose(t1);			// ɸѡ�̻�
		Shoot();			// �����̻�
		Show(pMem);			// �����̻�
		Style(st1);			// ��������
		FlushBatchDraw();	// ��ʾǰ������л�ͼ����
		tm++;
		if (tm>=1000)
		{
			switch (lo)
			{
			case 0:
				setcolor(GREEN);
				settextstyle(50, 0, L"����");
				outtextxy(300, 250, L"��");
				Sleep(100);
				break;
			case 1:
				setcolor(WHITE);
				settextstyle(50, 0, L"����");
				outtextxy(350, 250, L"��");
				Sleep(100);
				break;
			case 2:
				setcolor(RED);
				settextstyle(50, 0, L"����");
				outtextxy(400, 250, L"��");
				Sleep(100);
				break;
			case 3:
				setcolor(BLUE);
				settextstyle(50, 0, L"����");
				outtextxy(450, 250, L"��");
				Sleep(100);
				break;
			case 4:
				settextstyle(50, 0, L"����");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(300, 250, L"��");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(350, 250, L"ϲ");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(400, 250, L"��");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(450, 250, L"��");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(500, 250, L"��");
				Sleep(100);
				break;
			case 5:
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				settextstyle(50, 0, L"����");
				outtextxy(450, 250, L"�Ժ�------");
				Sleep(100);
				break;
			case 6:
				settextstyle(50, 0, L"����");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(300, 250, L"Ը");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(350, 250, L"Я");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(400, 250, L"��");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(450, 250, L"��");
				setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
				outtextxy(500, 250, L"��");
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
		settextstyle(i, 0, L"����");
		outtextxy(200, 250, L"Ը�������쿪�ģ�");
		outtextxy(x - 10, y + 100, L"���������");
		Sleep(25);
	}

	Sleep(5000);
	cleardevice();
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	settextstyle(25, 0,L"����");
	outtextxy(200, 100, L"��ϲ����");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 150, L"������Ϊ����ʲô������");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 200, L"������Ϊ");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 250, L"ϲ��");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 300, L"������һ���");
	setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
	outtextxy(200, 350, L"�о�");
	//outtextxy(300, 400, L"----��С���ˡ�");
	Sleep(5000);
}

// ��ʼ���̻�����
void Init(int i)
{
	// �ֱ�Ϊ���̻����ĵ�ͼƬ��Ե����Զ���롢�̻����ĵ�ͼƬ���Ͻǵľ��� (x��y) ��������
	int r[13] = { 80, 80, 90, 90, 130, 89, 87, 86, 85, 89, 84, 83, 82 };
	int x[13] = { 120, 120, 110, 117, 110, 93, 102, 102, 110, 105, 100, 108, 110 };
	int y[13] = { 120, 120, 85, 118, 120, 103, 105, 110, 110, 120, 120, 104, 85 };

	/**** ��ʼ���̻� *****/

	Fire[i].x = 0;				// �̻���������
	Fire[i].y = 0;
	Fire[i].width = 240;				// ͼƬ��
	Fire[i].height = 240;				// ͼƬ��
	Fire[i].max_r = r[i];				// ���뾶
	Fire[i].cen_x = x[i];				// ���ľ����ϽǾ���
	Fire[i].cen_y = y[i];
	Fire[i].show = false;			// �Ƿ�����
	Fire[i].dt = 5;				// ����ʱ����
	Fire[i].t1 = timeGetTime();
	Fire[i].r = 0;				// �� 0 ��ʼ����

	/**** ��ʼ���̻��� *****/

	Jet[i].x = -240;				// �̻������Ͻ�����
	Jet[i].y = -240;
	Jet[i].hx = -240;				// �̻���������ߵ�����
	Jet[i].hy = -240;
	Jet[i].height = 0;				// ����߶�
	Jet[i].t1 = timeGetTime();
	Jet[i].dt = rand() % 10;		// �����ٶ�ʱ����
	Jet[i].n = 0;				// �̻�����˸ͼƬ�±�
	Jet[i].shoot = false;			// �Ƿ���
}


// ����ͼƬ
void Load()
{
	/**** �����̻������ص���ɫ ****/
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

	/**** �����̻��� ************/
	IMAGE sm;
	loadimage(&sm, L"./fire/shoot.jpg", 200, 50);

	for (int i = 0; i < 13; i++)
	{
		SetWorkingImage(&sm);
		int n = rand() % 5;

		getimage(&Jet[i].img[0], n * 20, 0, 20, 50);			// ��
		getimage(&Jet[i].img[1], (n + 5) * 20, 0, 20, 50);		// ��
	}


	SetWorkingImage();		// ���ûػ�ͼ����
}


// ��һ����Χ��ɸѡ�ɷ�����̻�������ʼ���������������̻�������Ļ����������
void Chose(DWORD& t1)
{
	DWORD t2 = timeGetTime();

	if (t2 - t1 > 100)
	{
		int n = rand() % 20;

		if (n < 13 && Jet[n].shoot == false && Fire[n].show == false)
		{
			/**** �����̻�����Ԥ������ *****/
			Jet[n].x = rand() % pwidth;
			Jet[n].y = rand() % 100 + 600;
			Jet[n].hx = Jet[n].x;
			Jet[n].hy = rand() % 400;
			Jet[n].height = Jet[n].y - Jet[n].hy;
			Jet[n].shoot = true;
			putimage(Jet[n].x, Jet[n].y, &Jet[n].img[Jet[n].n], SRCINVERT);

			/**** ����ÿ���̻��������� *****/
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


// ɨ���̻���������
void Shoot()
{
	for (int i = 0; i < 13; i++)
	{
		Jet[i].t2 = timeGetTime();

		if (Jet[i].t2 - Jet[i].t1 > Jet[i].dt && Jet[i].shoot == true)
		{
			/**** �̻��������� *****/
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			if (Jet[i].y > Jet[i].hy)
			{
				Jet[i].n++;
				Jet[i].y -= 5;
			}

			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			/**** �������߶ȵ� 3 / 4������ *****/
			if ((Jet[i].y - Jet[i].hy) * 4 < Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			/**** ���������߶� *****/
			if (Jet[i].y <= Jet[i].hy)
			{
				// ���ű�ը��
				/*char c1[50], c2[30], c3[30];
				sprintf(c1, "open ./fire/bomb.wav alias n%d", i);
				sprintf(c2, "play n%d", i);
				sprintf(c3, "close s%d", i);

				mciSendString(c3, 0, 0, 0);
				mciSendString(c1, 0, 0, 0);
				mciSendString(c2, 0, 0, 0);*/

				putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// �����̻���
				Fire[i].x = Jet[i].hx + 10;											// ���̻����м䱬ը
				Fire[i].y = Jet[i].hy;												// ����ߵ�����
				Fire[i].show = true;					// ��ʼ����
				Jet[i].shoot = false;					// ֹͣ����

			}
			Jet[i].t1 = Jet[i].t2;
		}
	}
}



// ��ʾ����
void Style(DWORD& st1)
{
	DWORD st2 = timeGetTime();

	if (st2 - st1 >20000)		// һ�׸��ʱ��
	{
		// ��������
		int x[13] = { 60 * 0.66, 75 * 0.66, 91 * 0.66, 100 * 0.66, 95 * 0.66, 75 * 0.66, 60 * 0.66, 45 * 0.66, 25 * 0.66, 15 * 0.66, 25 * 0.66, 41 * 0.66, 60 * 0.66 };
		int y[13] = { 65 * 0.75, 53 * 0.75, 40 * 0.75, 22 * 0.75, 5 * 0.75, 4 * 0.75, 20, 4 * 0.75, 5 * 0.75, 22 * 0.75, 40 * 0.75, 53 * 0.75, 65 * 0.75 };
		for (int i = 0; i < NUM; i++)
		{
			//cleardevice();
			/**** ���ɷֲ��̻��� ***/
			Jet[i].x = x[i] * 10;
			Jet[i].y = (y[i] + 75) * 10;
			Jet[i].hx = Jet[i].x;
			Jet[i].hy = y[i] * 10;
			Jet[i].height = Jet[i].y - Jet[i].hy;
			Jet[i].shoot = true;
			Jet[i].dt = 7;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// ��ʾ�̻���

			/**** �����̻����� ***/
			Fire[i].x = Jet[i].x + 10;
			Fire[i].y = Jet[i].hy;
			Fire[i].show = false;
			Fire[i].r = 0;


			/**** ���ŷ������� ***/
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


// �����̻�
void Show(DWORD* pMem)
{
	// �̻����׶�����ʱ������������������Ч��
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };

	for (int i = 0; i < NUM; i++)
	{
		Fire[i].t2 = timeGetTime();

		// ���ӱ�ը�뾶�������̻�������ʱ����������Ч��
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

		// ����ú��ڻ��ɱ�ը�����ݵ�ǰ��ը�뾶���̻�����ɫֵ�ӽ���ɫ�Ĳ������
		if (Fire[i].draw)
		{
			for (double a = 0; a <= 6.28; a += 0.01)
			{
				int x1 = (int)(Fire[i].cen_x + Fire[i].r * cos(a));				// �����ͼƬ���Ͻǵ�����
				int y1 = (int)(Fire[i].cen_y - Fire[i].r * sin(a));

				if (x1 > 0 && x1 < Fire[i].width && y1 > 0 && y1 < Fire[i].height)	// ֻ���ͼƬ�ڵ����ص�
				{
					int b = Fire[i].xy[x1][y1] & 0xff;
					int g = (Fire[i].xy[x1][y1] >> 8) & 0xff;
					int r = (Fire[i].xy[x1][y1] >> 16);

					// �̻����ص��ڴ����ϵ�����
					int xx = (int)(Fire[i].x + Fire[i].r * cos(a));
					int yy = (int)(Fire[i].y - Fire[i].r * sin(a));

					// �ϰ������ص㲻�������ֹԽ��
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < pwidth && yy > 0 && yy < pheight)
						pMem[yy * pwidth + xx] = BGR(Fire[i].xy[x1][y1]);	// �Դ���������̻�
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
		settextstyle(30, 0, L"���Ĳ���");

		//��һ��Բ��ȡ60����
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