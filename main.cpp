#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <ctime>
#include <windows.h>
#include <iostream>
#include <fstream>
using namespace std;
const double g = 1.3;
const int WIDTH = 900, HEIGHT = 600, N = 1024, BALL_WIDTH = 50, UP_V = -18;
int score = -1, block_v = 10;
double v = 0;
bool run = true;
struct actor {
	int x, y, h;
	bool show;
	void draw() {
		fillrectangle(x, y, x + BALL_WIDTH, y + h);
	}
} upBlocks[N], downBlocks[N], ball = actor{WIDTH / 3, HEIGHT / 2, BALL_WIDTH, true}, block, topb;
void welcome() {
	settextstyle(150, 0, "Calibri");
	outtextxy(130, 220, "Bird PC Game");
	settextstyle(50, 0, "Calibri");
	outtextxy(220, 350, "Developed by Peter Lu in 2023.");
	Sleep(1000);
	cleardevice();
}
void drawScore() {
	char s[N];
	_stprintf(s, "%d", score);
	settextstyle(70, 0, "Calibri");
	outtextxy(70, 70, s);
}
bool check_collide() {
	if (ball.y + BALL_WIDTH >= block.y && ball.x + BALL_WIDTH <= block.x + BALL_WIDTH  && ball.x + BALL_WIDTH >= block.x) return true;
	if (ball.y <= topb.h && ball.x + BALL_WIDTH >= topb.x && ball.x + BALL_WIDTH <= topb.x + BALL_WIDTH) return true;
	return false;
}
DWORD t1, t2;
int maxScore;
bool endFlag = true;
void drawMaxScore() {
	settextstyle(70, 0, "Calibri");
	char s[N];
	_stprintf(s, "%d", maxScore);
	outtextxy(650, 70, "RC:"); 
	outtextxy(750, 70, s);
}
void update() {
	t2 = GetTickCount();
	if (t2 - t1 >= 5000) {
		t1 = GetTickCount();
		block_v += 5;
	}
	if (kbhit()) {
		char c = _getch();
		if (c == ' ') v = UP_V;
	} else {
		v += g;
		ball.y += v; 
		if (ball.y > HEIGHT - BALL_WIDTH) {
			ball.y = HEIGHT - BALL_WIDTH;
			v = 0;
		}
		if (ball.y < 0) {
			ball.y = 0;
			v = 0;
		} 
	}
	drawScore();
	drawMaxScore();
	if (score > maxScore) {
		settextstyle(70, 0, "Î¢ÈíÑÅºÚ");
		outtextxy(220, 70, "´òÆÆ¼ÍÂ¼£¡");
	}
	if (check_collide()) {
		cleardevice();
		drawScore();
		drawMaxScore();
		settextstyle(150, 0, "Calibri");
		outtextxy(180, 220, "Game Over");
		settextstyle(50, 0, "Calibri");
		outtextxy(250, 350, "Press any key to continue.");
		FlushBatchDraw();
		endFlag = false;
		_getch();
		if (score > maxScore) {
			maxScore = score;
			fstream newRecord;
			newRecord.open("record.txt", ios::out);
			char s[N];
			_stprintf(s, "%d", maxScore);
			newRecord << s; 
		}
		score = -1;
		block = topb = actor{-1, HEIGHT / 3, HEIGHT - HEIGHT / 3, true};
		block_v = 10;
	}
	block.x -= block_v;
	topb.x -=  block_v;
	if (block.x + BALL_WIDTH < 0) {
		block.x = WIDTH;
		block.h = int(rand()) % (HEIGHT / 2);
		block.y = HEIGHT - block.h;
		topb.y = 0;
		topb.x = WIDTH;
		if (HEIGHT - block.h - 3 * BALL_WIDTH < 0) topb.h = 0; 
		else topb.h = HEIGHT - block.h - (int(rand()) % 3 + 5) * BALL_WIDTH;
		++score;
	}
	ball.draw();
	block.draw();
	topb.draw();
}
int main() {
	srand(time(NULL));
	initgraph(WIDTH, HEIGHT);
	cleardevice();
	welcome();
	block = topb = actor{-200, HEIGHT / 3, HEIGHT - HEIGHT / 3, true};
	t1 = GetTickCount();
	fstream record;
	record.open("record.txt", ios::in);
	record >> maxScore;
	record.close();
	while (run) {
		BeginBatchDraw();
		update();
		if (endFlag) FlushBatchDraw();
		endFlag = true;
		Sleep(25);
		cleardevice();
	}
	EndBatchDraw();
	closegraph();
	return 0;
}

