#include <iostream>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

const int WINDOW_HEIGHT = 21;
const int WINDOW_WIDTH = 19;

const int tileSize = 25;

int q = 0;
bool iterationSolved = true;

String tileMap[WINDOW_HEIGHT] = {
"AAAAAAAAAAAAAAAAAAA",
"A1       A       2A",
"A AA AAA A AAA AA A",
"A        A        A",
"A AA A AAAAA A AA A",
"A    A   A   A    A",
"AAAA AAA A AAA AAAA",
"BBBA A       A ABBB",
"AAAA A AAAAA A AAAA",
"BBBB   ABBBA   BBBB",
"AAAA A AAAAA A AAAA",
"BBBA A       A ABBB",
"AAAA A AAAAA A AAAA",
"A        A        A",
"A AA AAA A AAA AA A",
"A  A     C     A  A",
"AA A A AAAAA A A AA",
"A    A   A   A    A",
"A AAAAAA A AAAAAA A",
"A3               4A",
"AAAAAAAAAAAAAAAAAAA",
};

class Player
{
public:
	float frame = 0;
	int x = 9, y = 15;
	int newx = 0, newy = 0;
	int rotate = 1, ti = 0;

	void update()
	{
		frame += 0.01;
		if (frame > 5)
			frame -= 5;

		ti++;
		if (ti >= 300) 
		{
			switch (rotate)
			{
			case 1:
				if (tileMap[y][newx + 1] != 'A')
					newx += 1;
				break;
			case 2:
				if (tileMap[y][newx - 1] != 'A')
					newx -= 1;
				break;
			case 3:
				if (tileMap[newy - 1][x] != 'A')
					newy -= 1;
				break;
			case 4:
				if (tileMap[newy + 1][x] != 'A')
					newy += 1;
				break;
			}

			ti = 0;
		}

		if (tileMap[newy][newx] == ' ' || tileMap[newy][newx] == 'B')
		{
			if (tileMap[newy][newx] == ' ')
				q++;

			if (tileMap[newy][newx] == '1'	|| tileMap[newy][newx] == '2' || tileMap[newy][newx] == '3' || tileMap[newy][newx] == '4')
				iterationSolved = false;

			tileMap[y][x] = 'B';

			tileMap[newy][newx] = 'C';

			x = newx;
			y = newy;
		}

		if (newy == 9 && (newx == 0 || newx == 18))
		{
			if (newx == 0)
				newx = 17;
			else
				newx = 1;

			tileMap[y][x] = 'B';
			tileMap[newy][newx] = 'C';

			x = newx;
			y = newy;
		}
	}
};

class Enemy
{
public:
	int x[4] = { 1, 17 , 1, 17 }, y[4] = { 1, 1, 19, 19 };
	int newx[4] = { 0 , 0 , 0, 0 }, newy[4] = { 0, 0, 0, 0 };
	int rotate[4] = { 1, 1, 1, 1 }, ti = 0;

	void update()
	{
		ti++;

		if (ti >= 300)
		{
			for (int i = 0; i < 4; i++) 
			{
				rotate[i] = rand() % 4 + 1;

				newx[i] = x[i];
				newy[i] = y[i];

				switch (rotate[i])
				{
				case 1:
					if (tileMap[y[i]][newx[i] + 1] != 'A')
						newx[i] += 1;
					break;
				case 2:
					if (tileMap[y[i]][newx[i] - 1] != 'A')
						newx[i] -= 1;
					break;
				case 3:
					if (tileMap[newy[i] - 1][x[i]] != 'A')
						newy[i] -= 1;
					break;
				case 4:
					if (tileMap[newy[i] + 1][x[i]] != 'A')
						newy[i] += 1;
					break;
				}
			}

			ti = 0;
		}

		for (int i = 0; i < 4; i++) 
		{
			if (tileMap[newy[i]][newx[i]] == ' ' || tileMap[newy[i]][newx[i]] == 'B' ||	tileMap[newy[i]][newx[i]] == 'C')
			{
				if (tileMap[newy[i]][newx[i]] == 'B')
					tileMap[y[i]][x[i]] = 'B';
				else if (tileMap[newy[i]][newx[i]] == ' ')
					tileMap[y[i]][x[i]] = ' ';
				else if (tileMap[newy[i]][newx[i]] == 'C')
					iterationSolved = false;

				if (i == 0)
					tileMap[newy[i]][newx[i]] = '1';
				if (i == 1)
					tileMap[newy[i]][newx[i]] = '2';
				if (i == 2)
					tileMap[newy[i]][newx[i]] = '3';
				if (i == 3)
					tileMap[newy[i]][newx[i]] = '4';

				x[i] = newx[i];
				y[i] = newy[i];
			}

			if (newy[i] == 9 && (newx[i] == 0 || newx[i] == 18)) 
			{
				if (newx[i] == 0)
					newx[i] = 17;
				else
					newx[i] = 1;

				tileMap[y[i]][x[i]] = 'B';

				if (i == 0)
					tileMap[newy[i]][newx[i]] = '1';
				if (i == 1)
					tileMap[newy[i]][newx[i]] = '2';
				if (i == 2)
					tileMap[newy[i]][newx[i]] = '3';
				if (i == 3)
					tileMap[newy[i]][newx[i]] = '4';

				x[i] = newx[i];
				y[i] = newy[i];
			}
		}
	}
};

void drawMaze(Sprite& mainSprite, RenderWindow& window, Player& p, Enemy& en)
{
	for (int i = 0; i < WINDOW_HEIGHT; i++)
	{
		for (int j = 0; j < WINDOW_WIDTH; j++)
		{
			if (tileMap[i][j] == 'A')
				mainSprite.setTextureRect(IntRect(0, 0, tileSize, tileSize));
			if (tileMap[i][j] == 'C')
				mainSprite.setTextureRect(IntRect(tileSize * int(p.frame), tileSize * p.rotate, tileSize, tileSize));
			if (tileMap[i][j] == ' ')
				mainSprite.setTextureRect(IntRect(tileSize, 0, tileSize, tileSize));
			if (tileMap[i][j] == '1')
				mainSprite.setTextureRect(IntRect(tileSize * 5, tileSize * en.rotate[0], tileSize, tileSize));
			if (tileMap[i][j] == '2')
				mainSprite.setTextureRect(IntRect(tileSize * 5, tileSize * en.rotate[1], tileSize, tileSize));
			if (tileMap[i][j] == '3')
				mainSprite.setTextureRect(IntRect(tileSize * 5, tileSize * en.rotate[2], tileSize, tileSize));
			if (tileMap[i][j] == '4')
				mainSprite.setTextureRect(IntRect(tileSize * 5, tileSize * en.rotate[3], tileSize, tileSize));
			if (tileMap[i][j] == 'B')
				continue;

			mainSprite.setPosition(j * tileSize, i * tileSize);
			window.draw(mainSprite);
		}
	}
}

int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(WINDOW_WIDTH * tileSize, WINDOW_HEIGHT * tileSize), "Maze!");

	Texture pacmanTextureMap;
	pacmanTextureMap.loadFromFile("tiles/maze-pacman-title.png");
	Sprite pacmanSprite(pacmanTextureMap);

	Texture wonTextureMap;
	wonTextureMap.loadFromFile("tiles/maze-pacman-youwin.png");
	Sprite won(wonTextureMap);
	won.setPosition(100, 210);

	Texture loseTextureMap;
	loseTextureMap.loadFromFile("tiles/maze-pacman-youlose.png");
	Sprite lose(loseTextureMap);
	lose.setPosition(100, 210);

	Player p;
	Enemy en;

	// Main game loop.
	while (window.isOpen())
	{
		// Handle key events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (q < 171 && iterationSolved)
				if (event.type == Event::KeyPressed)
				{
					p.newx = p.x;
					p.newy = p.y;

					if (event.key.code == Keyboard::Right)
						p.rotate = 1;
					if (event.key.code == Keyboard::Left)
						p.rotate = 2;
					if (event.key.code == Keyboard::Up)
						p.rotate = 3;
					if (event.key.code == Keyboard::Down)
						p.rotate = 4;
				}
		}

		// Update player and enemy positions.
		if (q < 171 && iterationSolved) {
			p.update();
			en.update();
		}

		// Redraw.
		window.clear(Color::Black);

		drawMaze(pacmanSprite, window, p, en);

		if (q == 171)
			window.draw(won);
		if (!iterationSolved)
			window.draw(lose);

		window.display();
	}

	return 0;
}