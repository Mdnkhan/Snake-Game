#include <iostream>
#include <Windows.h>
#include <list>
#include <thread>
using namespace std;

int ScreenHeight = 40;
int ScreenWidth = 120;
int score = 0;
int xFood = 62;
int yFood = 26;
bool sdead = false;

enum directio_values { UP, DOWN, LEFT, RIGHT };
int sdirection = UP; /*UP = 0, DOWN = 1. LEFT = 2, RIGHT = 3*/
int prDirection = UP;

struct SnakeSection {
	int x;
	int y;
};
int main() {

	wchar_t *screen = new wchar_t[ScreenHeight*ScreenWidth];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	screen[ScreenHeight*ScreenWidth - 1] = '\0';

	while (1) {
		sdead = false;
		sdirection = LEFT;
		score = 0;
		xFood = 22;
		yFood = 10;
		list<SnakeSection> snake = { { 65,22 },{ 65,23 },{ 65,24 } };

		while (!sdead) {
			//Timing
			auto t1 = chrono::system_clock::now();
			while ((chrono::system_clock::now() - t1) < ((sdirection % 2 == 1) ? 120ms : 200ms))
			{
				this_thread::sleep_for(200ms);
				//Input
				if (GetAsyncKeyState(VK_UP)) { sdirection = UP; }
				if (GetAsyncKeyState(VK_DOWN)) { sdirection = DOWN; }
				if (GetAsyncKeyState(VK_LEFT)) { sdirection = LEFT; }
				if (GetAsyncKeyState(VK_RIGHT)) { sdirection = RIGHT; }
			}
			//Game logic
			//Adding new head
			switch (sdirection)
			{
			case UP://UP
				if (prDirection == DOWN) {
					snake.push_front({ snake.front().x, snake.front().y + 1 });
				}
				else {
					snake.push_front({ snake.front().x, snake.front().y - 1 });
					prDirection = sdirection;
				}
				break;
			case DOWN: //DOWN
				if (prDirection == UP) {
					snake.push_front({ snake.front().x, snake.front().y - 1 });
				}
				else {
					snake.push_front({ snake.front().x, snake.front().y + 1 });
					prDirection = sdirection;
				}
				break;
			case LEFT: //LEFT
				if (prDirection == RIGHT) {
					snake.push_front({ snake.front().x + 1, snake.front().y });
				}
				else
				{
					snake.push_front({ snake.front().x - 1, snake.front().y });
					prDirection = sdirection;
				}
				break;
			case RIGHT: //RIGHT
				if (prDirection == LEFT) {
					snake.push_front({ snake.front().x - 1, snake.front().y });
				}
				else
				{
					snake.push_front({ snake.front().x + 1, snake.front().y });
					prDirection = sdirection;
				}

				break;
			default:
				break;
			}

			//Collision
			//Colision with wall
			if (snake.front().x <= 0 || snake.front().x >= ScreenWidth - 1) {
				sdead = true;
			}
			if (snake.front().y < 4 || snake.front().y >= ScreenHeight - 1) {
				sdead = true;
			}

			//Collision with food
			if (snake.front().x == xFood && snake.front().y == yFood) {
				score++;
				while (screen[yFood*ScreenWidth + xFood] != L' ') {
					xFood = rand() % (ScreenWidth - 1) + 1;
					yFood = rand() % (ScreenHeight - 1) + 4;
				}

				snake.push_back({ snake.back().x, snake.back().y });
			}

			//Coliding with own body
			for (list<SnakeSection>::iterator i = snake.begin(); i != snake.end(); i++) {
				if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y) { sdead = true; }
			}
			//choping of old tail
			snake.pop_back();

			//Display 
			//clearing the screen
			for (int i = 0; i < ScreenHeight*ScreenWidth; i++) screen[i] = L' ';
			//displaying the arena
			for (int i = 0; i < ScreenWidth; i++)
			{
				screen[i] = L'=';
				screen[2 * ScreenWidth + i] = L'=';
				screen[3 * ScreenWidth + i] = L'=';
				screen[(ScreenHeight - 1) * ScreenWidth + i] = L'=';
			}
			for (int i = 0; i < ScreenHeight; i++) {
				screen[i*ScreenWidth] = L'|';
				screen[i*ScreenWidth + ScreenWidth - 1] = L'|';
			}
			wsprintf(&screen[ScreenWidth + 35], L"Sakib's - S N A K E ! !                SCORE:%d", score);

			//drawing Snake body
			for (auto s : snake) {
				screen[s.y*ScreenWidth + s.x] = sdead ? L'+' : L'O';
			}

			if (sdead) {
				wsprintf(&screen[20 * ScreenWidth + 45], L"---PRESS SPACE TO PLAY AGAIN---");
			}
			//Drawing snake head
			screen[snake.front().y * ScreenWidth + snake.front().x] = sdead ? L'X' : L'0';
			screen[yFood*ScreenWidth + xFood] = L'*';
			//outputing to console
			WriteConsoleOutputCharacter(hConsole, screen, ScreenHeight*ScreenWidth, { 0,0 }, &dwBytesWritten);
		}

		while ( !GetAsyncKeyState(VK_SPACE) );
	}
	return 0;
}