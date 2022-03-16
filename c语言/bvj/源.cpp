#include "acllib.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h> 
const int wk = 1200, wh = 750;
const int m = 10000;
const int f = 5000;
const int w = 50;
const int ik = 160, ih = 100;
const char* pStr = "菜鸡，你凉了";
const char* pStr1 = "社，社会，你赢了";
ACL_Image img[5], joker,heartp,bg;
ACL_Sound bj, gj, gl,ch;
char name1[] = "b1.jpg", joke[] = "j1.jpg", name2[] = "b2.jpg", name3[] = "b3.jpg", bre[] = "break.jpeg", name[] = "hhh.jpg",heart[]="heart.jpg",beij[]="gtm.jpg";
char bgm[] = "bj.mp3", tx1[] = "gj.mp3", tx2[] = "gl.mp3", tx3[] = "chi.mp3";
int mid = 0, mti = 500;
int cid = 1, cti = 1000;
int bid = 2, bti = 15000;
int hid = 3, hti = 30000;
int x, y;
int life = 5;
int judge_1 = 1, judge_2 = 2;
int score = 0;
int level = 1;
void timerevent(int id);
void keye(int key, int e);
const char* start_screen = "start_screen.jpg";
const char* explannation = "explannation.jpg";
void paint();
typedef struct rect
{
	int x, y;
	int w, h;
}rect;
int collision(rect r1, rect r2);
struct tp
{
	int w;
	int h;
	int x;
	int y;
	int d1 = 5;
	int d2 = 5;
};
struct tp b, * j[m] = { NULL }, * d[f] = { NULL }, * h[w] = { NULL };
int jn = 0,dn=0,o=0,hn=0;
int Setup()
{
	initWindow("B VS J", 0, 0, wk, wh);
	srand((unsigned)time(NULL));
	if (life == 0)
	{
		beginPaint();
		paintText(580, 380, pStr);
		endPaint();
		return 0;;
	}
	if (life != 0 && score == 100)
	{
		beginPaint();
		paintText(580, 380, pStr1);
		endPaint();
		return 0;
	}
	loadImage(name1, &img[0]);
	loadImage(name2, &img[1]);
	loadImage(name3, &img[2]);
	loadImage(bre, &img[3]);
	loadImage(name, &img[4]);
	loadImage(joke, &joker);
	loadImage(heart, &heartp);
	loadImage(beij, &bg);
	loadSound(bgm, &bj);
	loadSound(tx1, &gj);
	loadSound(tx2, &gl);
	loadSound(tx3, &ch);
	playSound(bj, 10);
	b.x = rand() % 840;
	b.y = rand() % 500;
	if (score == 10 && level == 1)
	{
		score = 0;
		level++;
	}
	if (score == 30 && level == 2)
	{
		score = 0;
		level++;
	}
	beginPaint();
	putImageScale(&bg, 0, 0, wk, wh);
	endPaint();
	for ( int i = 0; i < m; i++) j[i] = NULL;
	for (int i = 0; i < f; i++) d[i] = NULL;
	for (int i = 0; i < w; i++) h[i] = NULL;
	if (jn <m)
	{
		j[jn] = (struct tp*)malloc(sizeof(struct tp));
		j[jn]->x = rand() % (wk - 100);
		j[jn]->y = rand() % (wh - 60);
		j[jn]->w = 100;
		j[jn]->h = 60;
		j[jn]->d1 = 15;
		j[jn]->d2 = 15;
	}
	if (dn < f)
	{
		d[dn] = (struct tp*)malloc(sizeof(struct tp));
		d[dn]->x = rand() % (wk - 200);
		d[dn]->y = rand() % (wh - 120);
		d[dn]->w = 200;
		d[dn]->h = 120;
		d[dn]->d1 = 15;
		d[dn]->d2 = 15;
	}
	if (hn < w)
	{
		h[hn] = (struct tp*)malloc(sizeof(struct tp));
		h[hn]->x = rand() % (wk - 100);
		h[hn]->y = rand() % (wh - 60);
		h[hn]->w = 100;
		h[hn]->h = 60;
		h[hn]->d1 = 30;
		h[hn]->d2 = 30;
	}
	b.w = 160;
	b.h = 100;
	registerTimerEvent(timerevent);
	startTimer(mid, mti);
	startTimer(cid, cti);
	startTimer(bid, bti);
	startTimer(hid, hti);
	registerKeyboardEvent(keye);
	paint();

	return 0;
}
void paint()
{
	if (life > 0 && score < 100)
	{
		beginPaint();
		clearDevice();
		putImageScale(&bg, 0, 0, wk, wh);
		putImageScale(&img[level - 1], b.x, b.y, b.w, b.h);
		for (int i = 0; i <= jn; i++)
		{
			if (j[i] != NULL)
			{
				putImageScale(&joker, j[i]->x, j[i]->y, j[i]->w, j[i]->h);
			}
		}
		for (int i = 0; i <= hn; i++)
		{
			if (h[i] != NULL)
			{
				putImageScale(&heartp, h[i]->x, h[i]->y, h[i]->w, h[i]->h);
			}
		}
		for (int i = 0; i <= dn; i++)
		{
			if (d[i] != NULL)
			{
				putImageScale(&img[3], d[i]->x, d[i]->y, d[i]->w, d[i]->h);
			}
		}
		char str1[20], str2[10], str3[20];
		sprintf_s(str1, "score:%d", score);
		setTextSize(30);
		paintText(10, 10, str1);
		sprintf_s(str2, "life:%d", life);
		setTextSize(30);
		paintText(450, 10, str2);
		sprintf_s(str3, "level:%d", level);
		setTextSize(30);
		paintText(900, 10, str3);
		endPaint();
	}
	else if (life == 0)
	{
		beginPaint();
		paintText(580, 380, pStr);
		endPaint();
		cancelTimer(1);
		cancelTimer(2);
		cancelTimer(3);
		stopSound(bj);
		stopSound(gj);
		stopSound(gl);
		return;
	}
	else if (life!=0&&score == 100)
	{
		beginPaint();
		paintText(580, 380, pStr1);
		endPaint();
		cancelTimer(1);
		cancelTimer(2);
		cancelTimer(3);
		return;
	}
	if (score == 10 && level == 1)
	{
		score = 0;
		level++;
	}
	if (score == 30 && level == 2)
	{
		score = 0;
		level++;
	}
}
void timerevent(int id)
{
		int i;
		if (id == 0)
		{
			for (i = 0; i <= jn; i++)
			{
				if (j[i] != NULL)
				{
					j[i]->x += j[i]->d1;
					j[i]->y += j[i]->d2;
					if (j[i]->y >= wh - j[i]->h)
					{
						j[i]->y = wh - j[i]->h;
						j[i]->d2 = j[i]->d2 * -1;
					}
					if (j[i]->y <= 0)
					{
						j[i]->y = 0;
						j[i]->d2 = j[i]->d2 * -1;
					}
					if (j[i]->x >= wk - j[i]->w)
					{
						j[i]->x = wk - j[i]->w;
						j[i]->d1 = j[i]->d1 * -1;
					}
					if (j[i]->x <= 0)
					{
						j[i]->x = 0;
						j[i]->d1 = j[i]->d1 * -1;
					}
				}
			}
			for (i = 0; i <= dn; i++)
			{
				if (d[i] != NULL)
				{
					d[i]->x += d[i]->d1;
					d[i]->y += d[i]->d2;
					if (d[i]->y >= wh - d[i]->h)
					{
						d[i]->y = wh - d[i]->h;
						d[i]->d2 = d[i]->d2 * -1;
					}
					if (d[i]->y <= 0)
					{
						d[i]->y = 0;
						d[i]->d2 = d[i]->d2 * -1;
					}
					if (d[i]->x >= wk - d[i]->w)
					{
						d[i]->x = wk - d[i]->w;
						d[i]->d1 = d[i]->d1 * -1;
					}
					if (d[i]->x <= 0)
					{
						d[i]->x = 0;
						d[i]->d1 = d[i]->d1 * -1;
					}
				}
			}
			for (i = 0; i <= hn; i++)
			{
				if (h[i] != NULL)
				{
					h[i]->x -= h[i]->d1;
					h[i]->y -= h[i]->d2;
					if (h[i]->y >= wh - h[i]->h)
					{
						h[i]->y = wh - h[i]->h;
						h[i]->d2 = h[i]->d2 * -1;
					}
					if (h[i]->y <= 0)
					{
						h[i]->y = 0;
						h[i]->d2 = h[i]->d2 * -1;
					}
					if (h[i]->x >= wk - h[i]->w)
					{
						h[i]->x = wk - h[i]->w;
						h[i]->d1 = h[i]->d1 * -1;
					}
					if (h[i]->x <= 0)
					{
						h[i]->x = 0;
						h[i]->d1 = h[i]->d1 * -1;
					}
				}
			}
		}
		if (id == 1)
		{
			jn++;
			if (jn < m)
			{
				j[jn] = (struct tp*)malloc(sizeof(struct tp));
				j[jn]->x = rand() % (wk - 100);
				j[jn]->y = rand() % (wh - 60);
				j[jn]->w = 100;
				j[jn]->h = 60;
				j[jn]->d1 = 15;
				j[jn]->d2 = 15;
			}
		}
		if (id == 2)
		{
			if (dn - o < 4)
			{
				dn++;
				if (dn < f)
				{
					d[dn] = (struct tp*)malloc(sizeof(struct tp));
					d[dn]->x = rand() % (wk - 200);
					d[dn]->y = rand() % (wh - 120);
					d[dn]->w = 200;
					d[dn]->h = 120;
					d[dn]->d1 = 15;
					d[dn]->d2 = 15;
				}
			}
		}
		if (id ==3)
		{
			hn++;
			if (hn < w)
			{
				h[hn] = (struct tp*)malloc(sizeof(struct tp));
				h[hn]->x = rand() % (wk - 100);
				h[hn]->y = rand() % (wh - 60);
				h[hn]->w = 100;
				h[hn]->h = 60;
				h[hn]->d1 = 15;
				h[hn]->d2 = 15;
			}
		}
		int c, v,z;
		rect r1, r2;
		r1.x = b.x;
		r1.y = b.y;
		r1.w = b.w;
		r1.h = b.h;
		for (i = 0; i <= jn; i++)
		{
			if (j[i] != NULL)
			{
				r2.x = j[i]->x;
				r2.y = j[i]->y;
				r2.h = j[i]->h;
				r2.w = j[i]->w;
				c = collision(r1, r2);
				if (c)
				{
					playSound(gj, 0);
					score++;
					b.w = b.w * 1.01;
					b.h = b.h * 1.01;
					delete  (j[i]);
					j[i] = NULL;
				}
			}
		}
		for (i = 0; i <= dn; i++)
		{
			if (d[i] != NULL)
			{
				r2.x = d[i]->x;
				r2.y = d[i]->y;
				r2.h = d[i]->h;
				r2.w = d[i]->w;
				v = collision(r1, r2);
				if (v)
				{
					o++;
					playSound(gl, 0);
					score = score - 5;
					life--;
					b.w = b.w / 1.01;
					b.h = b.h / 1.01;
					//delete  (d[i]);
					d[i] = NULL;
				}
			}
		}
		for (i = 0; i <= hn; i++)
		{
			if (h[i] != NULL)
			{
				r2.x = h[i]->x;
				r2.y = h[i]->y;
				r2.h = h[i]->h;
				r2.w = h[i]->w;
				z = collision(r1, r2);
				if (z)
				{
					playSound(ch, 0);
					life++;
					//delete  (h[i]);
					h[i] = NULL;
				}
			}
		}
		if (score == 10 && level == 1)
		{
			score = 0;
			level++;
		}
		if (score == 30 && level == 2)
		{
			score = 0;
			level++;
		}
		paint();
}

void keye(int key, int e)
{
	if (e != KEY_DOWN)
	{
		return;
	}
	switch (key)
	{
	case VK_UP:
		b.y = b.y - 5;
		if (b.y <= 0)
		{
			b.y = 0;
		}
		break;
	case VK_DOWN:
		b.y = b.y + 5;
		if (b.y >= wh - b.h)
		{
			b.y = wh - b.h;
		}
		break;
	case VK_LEFT:
		b.x = b.x - 5;
		if (b.x <= 0)
		{
			b.x = 0;
		}
		break;
	case VK_RIGHT:
		b.x = b.x + 5;
		if (b.x >= wk - b.w)
		{
			b.x = wk - b.w;
		}
		break;
	default:
		break;
	}
	paint();
}
int collision(rect r1, rect r2)
{
	int c = 1;
	if ((r1.x <= r2.x) && ((r1.x + r1.w) >= r2.x))
	{
		if ((r1.y < r2.y) && ((r1.y + r1.h) > r2.y)) return c;
		else if ((r1.y > r2.y) && (r1.y < (r2.y + r2.h))) return c;
	}
	else  if ((r1.x >= r2.x) && (r1.x <= (r2.x + r2.w)))
	{
		if ((r1.y < r2.y) && ((r1.y + r1.h) > r2.y)) return c;
		else if ((r1.y > r2.y) && (r1.y < (r2.y + r2.h))) return c;
	}
	c = 0;
	return c;
}