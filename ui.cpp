/*
#include  <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <easyx.h>
#include <iostream>
using namespace std;



void textBox(int startX, int startY, int width, int height, int lineHeight,
	COLORREF bgColor, COLORREF fontColor, char* msg, int* msgLen)
{
	// 画文本框
	//rectangle(10, 18, 630, 42);
	//rectangle(10, 18, 630, 126);

	int x = startX;				// 文字输出的位置。每输出一个，坐标相应增加
	int y = startY;
	int ncursor = 0;		// 实现光标闪烁的计数器
	//char msg[1024] = "";
	*msgLen = 0;
	int lineWidth[10] = { 0 };
	int lineIndex = 0;
	int lineMax = (height) / lineHeight;

	while (1) {
		if (*msgLen == 0) {
			x = startX;
			y = startY;
			memset(lineWidth, 0, sizeof(lineWidth));
			lineIndex = 0;
			setbkcolor(bgColor);
			clearrectangle(startX, startY, startX + width, startY + height);
		}

		if (_kbhit()) {
			setbkcolor(bgColor);
			setbkmode(OPAQUE);

			// ASCII 字符输入
			wchar_t c = _getwch();
			if (c == '\r') {
				if (lineIndex >= lineMax - 1) continue;

				lineWidth[lineIndex++] = x;
				setlinecolor(bgColor); //抹掉原来的光标线
				//line(x + 2, 23, x + 2, 37);
				line(x + 2, y + 1, x + 2, y + 15);

				x = startX;
				y += lineHeight;

				msg[*msgLen] = '\n';
				*msgLen += 1;
			}
			else if (c == 8) {  //退格键
				if (x == startX) { //到了最左边
					if (lineIndex > 0) {
						//抹掉原来的光标线
						setlinecolor(bgColor);
						line(x + 2, y + 1, x + 2, y + 15);

						lineIndex--;
						x = lineWidth[lineIndex];
						y -= lineHeight;
						if (msg[*msgLen - 1] == '\n') {
							*msgLen--;
						}
					}
				}
				else {
					if (isHZ(msg, *msgLen - 1)) { //中文字符
						setbkcolor(bgColor);
						int w = textwidth('8') * 2;
						clearrectangle(x - w, y, x + 2, y + 25);
						x -= w;
						*msgLen -= 2;
					}
					else {
						setbkcolor(bgColor);
						int w = textwidth(msg[*msgLen - 1]); //'1'和'w'不等宽
						clearrectangle(x - w, y, x + 2, y + 25);
						x -= w;
						*msgLen -= 1;
					}
				}
			}
			else {
				//outtextxy(x, 22, c);
				int maxW = startX + width - 2 * textwidth('a');

				if (lineIndex >= lineMax - 1 && x >= maxW) {
					continue;
				}
				setbkcolor(bgColor);
				setbkmode(OPAQUE);
				outtextxy(x, y, c);
				x += textwidth(c);

				msg[*msgLen] = c;
				*msgLen += 1;

				if (x >= startX + width - 2 * textwidth('a')) {
					if (lineIndex >= lineMax - 1 && x >= maxW) {
						continue;
					}

					lineWidth[lineIndex++] = x;
					x = startX;
					y += lineHeight;
				}
			}
		}
		else // 除此之外，检测是否有 IME 输入，如果有，输出
		{
			wstring s = GetIMEString();
			string s2 = wstring2string(s);
			if (lineIndex >= lineMax - 1) {
				if (x + textwidth(s2.c_str()) >= startX + width - 2 * textwidth('a'))
					continue;
			}

			if (s.size() > 0) {
				setbkcolor(bgColor);
				setbkmode(OPAQUE);

				int rightWidth = startX + width - x;
				int rightCount = rightWidth / (textwidth('8') * 2);

				setbkcolor(bgColor);
				setbkmode(OPAQUE);

				if (rightCount == 0) {
					lineWidth[lineIndex++] = x;
					x = startX;
					y += lineHeight;
					outtextxy(x, y, s2.c_str());
					x += textwidth(s2.c_str());
				}
				else if (rightCount < s.size()) {
					wstring tmp1 = s.substr(0, rightCount);
					wstring tmp2 = s.substr(rightCount);
					string tmp1s = wstring2string(tmp1);
					string tmp2s = wstring2string(tmp2);
					outtextxy(x, y, tmp1s.c_str());
					x += textwidth(tmp1s.c_str());
					lineWidth[lineIndex++] = x;
					outtextxy(startX, y + lineHeight, tmp2s.c_str());
					x = startX + textwidth(tmp2s.c_str());
					y += lineHeight;
				}
				else {
					outtextxy(x, y, s2.c_str());
					x += textwidth(s2.c_str());
				}

				string tmp = wstring2string(s);
				strcpy(&msg[*msgLen], tmp.c_str());
				*msgLen += strlen(tmp.c_str());
			}
		}

		setlinecolor((ncursor++ / 10 % 2 == 0) ? fontColor : bgColor); // 绘制光标
		line(x + 2, y + 1, x + 2, y + 15);

		Sleep(20); // 延时
	}
}
void drawText(int x1, int y1, int width, int lineHeight, char* msg) {
	int len = strlen(msg);
	int i = 0;
	int x = x1 + 10;
	int y = y1 + 10;
	char tmp[3] = { 0 };

	setbkmode(TRANSPARENT);

	while (i < len) {
		if (msg[i] > 0) {
			if (msg[i] == '\n') {
				x = x1 + 10;
				y += lineHeight;
			}
			else {
				outtextxy(x, y, msg[i]);
				x += textwidth(msg[i]);
			}
			i++;
		}
		else {
			tmp[0] = msg[i++];
			tmp[1] = msg[i++];
			outtextxy(x, y, tmp);
			x += textwidth(tmp);
		}

		if (x >= x1 + width - 15) {
			x = x1 + 10;
			if (msg[i] == '\n') i++; //如果行满的时候准备换行时，对于的字符正好是回车符，就跳过回车符，否则会连跳两行
			y += lineHeight;
		}
	}
}



*/
