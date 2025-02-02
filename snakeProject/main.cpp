#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <cstring>
#include <cmath>
#include <fstream>

// all the varaibles that are required to run the game

// the game only works for a resolution of 1920x1080

const int WIDTH = 1920;
const int HEIGHT = 1080;

const int GRIDSIZE = 30;

const int NUM_CELLS_X = WIDTH / GRIDSIZE;
const int NUM_CELLS_Y = HEIGHT / GRIDSIZE;

const int MAX_SNAKE_LENGTH = 200;

int snakelength = 1;

int snakeX[MAX_SNAKE_LENGTH] = { NUM_CELLS_X / 2 };
int snakeY[MAX_SNAKE_LENGTH] = { NUM_CELLS_Y / 2 };

int score = 0;

int powerupx, powerupy;

int fruitAmount = 1;
const int maxFruit = 10;
int fruitX[maxFruit], fruitY[maxFruit];

int dir = 0;

int FPS = 17;

bool gameover = false;

int fruitChoice = 0;
int colorChoice = 0;
const int TOTAL_COLORS = 6;

int modeChoice = 1;

int lvlChangeThreshold = 100;

int lvl = 0;

bool obstacleGrid[NUM_CELLS_X][NUM_CELLS_Y] = { false }; // True if the cell is an obstacle

bool customGrid[NUM_CELLS_X][NUM_CELLS_Y] = { false };

bool isTelepathic = false;

int prevDir;
bool pause = false;
bool universalPause = false;

int leaderBoardScores[4];
std::string leaderBoardNames[4];

bool isGradient = true;

sf::SoundBuffer eatenBuffer;
sf::Sound fruitEatenSound(eatenBuffer);

void colorSetter(sf::RectangleShape& block);
void gradientSetter(sf::RectangleShape& block, int i, int s = snakelength);
void saveGame(void);
bool loadSave(void);
void spawnLineObstacles(bool dir = true);
void spawnFruit(int num);
void moveSnake(void);
void handleWallCollision(void);
bool checkWallCollision(void);
bool checkobstacleCollision(void);
bool checkcustomCollision(void);
void checkFruitCollision(void);
bool checkBodyCollision(void);
void lvlIncrease(void);
void loadGame(sf::RenderWindow& window);
void levelSelectMenu(sf::RenderWindow& window);
bool checkSave(void);
void loadSaveMenu(sf::RenderWindow& window);
void levelChangePauseMenu(sf::RenderWindow& window);
void customlvldesginer(sf::RenderWindow& window);
void mainGame(sf::RenderWindow& window);
void sortLeaderBoard(void);
int findPlayerScore(void);
std::string inputNameScreen(sf::RenderWindow& window);
void saveLeaderBoard(void);
void loadLeaderBoard(sf::RenderWindow& window);
void gameOverWindow(sf::RenderWindow& window);
void waitKeyPress(sf::RenderWindow& window);
std::string loadEndlessLeaderBoard(void);
std::string loadSimpleLeaderBoard(void);
void powerUpMenu(sf::RenderWindow& window);


int main()
{
	srand(time(0));
	eatenBuffer.loadFromFile("fruiteaten.ogg");

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "sf::snake", sf::Style::Fullscreen);
	srand(time(0));

	while (window.isOpen())
	{
		loadGame(window);
		levelSelectMenu(window);
		loadSaveMenu(window);
		if (modeChoice == 2) powerUpMenu(window);
		if (modeChoice == 3) customlvldesginer(window);
		mainGame(window);
		gameOverWindow(window);
	}

	return 0;
}

void spawnPowerUp()
{
	powerupx = rand() % NUM_CELLS_X;
	powerupy = rand() % NUM_CELLS_Y;

}

bool checkPowerUpCollision()
{
	if (snakeX[0] == powerupx && snakeY[0] == powerupy)
	{
		snakelength = 1;
		return true;
	}
	return false;
}

void colorSetter(sf::RectangleShape& block)
{
	// sets the color if a block according to the users choice

	sf::Color colors[] = { sf::Color::Green, sf::Color::Cyan, sf::Color::Magenta, sf::Color(255,43,174), sf::Color::Yellow, sf::Color(246,45,45) };

	block.setFillColor(colors[colorChoice]);
}

void gradientSetter(sf::RectangleShape& block, int i, int s)
{
	// sets the gradient of a block on the basis of its position in the snake

	float color1red = 0;
	float color1green = 0;
	float color1blue = 0;

	float color2red = 0;
	float color2green = 0;
	float color2blue = 0;

	if (colorChoice == 0)
	{

		// rgb(0, 255, 0) green
		color1red = 0;
		color1green = 255;
		color1blue = 0;


		// rgb(20, 219, 245) cyan

		color2red = 20;
		color2green = 219;
		color2blue = 245;
	}
	else if (colorChoice == 1)
	{
		// rgb(20, 219, 245) Cyan
		color1red = 20;
		color1green = 219;
		color1blue = 245;


		// rgb(255, 0, 255) Magenta

		color2red = 255;
		color2green = 0;
		color2blue = 255;
	}
	else if (colorChoice == 2)
	{
		// rgb(255, 0, 255) Magenta
		color1red = 255;
		color1green = 0;
		color1blue = 255;

		// rgb(255,43,174) Pink

		color2red = 255;
		color2green = 43;
		color2blue = 174;
	}
	else if (colorChoice == 3)
	{
		// rgb(255,43,174) Pink

		color1red = 255;
		color1green = 43;
		color1blue = 174;

		// rgb(255, 188, 10) Yellow

		color2red = 255;
		color2green = 188;
		color2blue = 110;
	}
	else if (colorChoice == 4)
	{
		// rgb (254, 246, 92) Yellow

		color1red = 254;
		color1green = 246;
		color1blue = 92;

		// rgb(255, 154, 0)Green

		color2red = 255;
		color2green = 154;
		color2blue = 0;
	}
	else if (colorChoice == 5)
	{
		// rgb(1, 96, 201)cyan

		color2red = 1;
		color2green = 96;
		color2blue = 201;

		// rgb(0,255,0) Green

		color1red = 246;
		color1green = 45;
		color1blue = 45;
	}

	float resultRed, resultGreen, resultBlue;

	float percent = ((float)i / s);
	resultRed = color1red + percent * (color2red - color1red);
	resultGreen = color1green + percent * (color2green - color1green);
	resultBlue = color1blue + percent * (color2blue - color1blue);

	block.setFillColor(sf::Color(resultRed, resultGreen, resultBlue));

}

void saveGame(void)
{
	// saves the game on the basis of the user mode choice that has been selected
	// each game mode has seperate save file

	if (modeChoice == 1)
	{
		// saves all the stuff in simpleSave.txt

		std::ofstream save("simpleSave.txt");

		save << snakelength << std::endl;

		for (int i = 0; i < snakelength; i++) save << snakeX[i] << std::endl;
		for (int i = 0; i < snakelength; i++) save << snakeY[i] << std::endl;

		save << score << std::endl;

		save << fruitX[0] << std::endl;
		save << fruitY[0] << std::endl;

		if (!pause)
			save << dir << std::endl;
		else
			save << prevDir << std::endl;

		save << lvl << std::endl;

		for (int i = 0; i < NUM_CELLS_X; i++)
		{
			for (int j = 0; j < NUM_CELLS_Y; j++)
				save << obstacleGrid[i][j] << std::endl;
		}

		save.close();
	}
	else if (modeChoice == 2)
	{
		std::ofstream save("endlessSave.txt");

		save << snakelength << std::endl;

		for (int i = 0; i < snakelength; i++) save << snakeX[i] << std::endl;
		for (int i = 0; i < snakelength; i++) save << snakeY[i] << std::endl;

		save << fruitX[0] << std::endl;
		save << fruitY[0] << std::endl;

		save << score << std::endl;

		if (!pause)
			save << dir << std::endl;
		else
			save << prevDir << std::endl;

		save.close();
	}
	else if (modeChoice == 3)
	{
		std::ofstream save("customSave.txt");

		save << FPS << std::endl;
		save << isTelepathic << std::endl;
		save << fruitAmount << std::endl;

		for (int i = 0; i < NUM_CELLS_X; i++)
		{
			for (int j = 0; j < NUM_CELLS_Y; j++)
			{
				save << customGrid[i][j] << std::endl;
			}
		}

		save.close();
	}

	return;
}

bool loadSave(void)
{
	// loads stuff according to each game mode

	if (modeChoice == 1)
	{
		std::ifstream save("simpleSave.txt");

		if (!save)
		{
			return false;
		}

		save >> snakelength;
		for (int i = 0; i < snakelength; i++) save >> snakeX[i];
		for (int i = 0; i < snakelength; i++) save >> snakeY[i];

		save >> score;

		save >> fruitX[0];
		save >> fruitY[0];

		save >> prevDir;
		save >> lvl;

		for (int i = 0; i < NUM_CELLS_X; i++)
			for (int j = 0; j < NUM_CELLS_Y; j++)
				save >> obstacleGrid[i][j];

		universalPause = true;

		save.close();

	}
	else if (modeChoice == 2)
	{
		std::ifstream save("endlessSave.txt");

		if (!save)
		{
			return false;
		}

		save >> snakelength;

		for (int i = 0; i < snakelength; i++) save >> snakeX[i];
		for (int i = 0; i < snakelength; i++) save >> snakeY[i];

		save >> fruitX[0];
		save >> fruitY[0];

		save >> score;

		save >> prevDir;

		universalPause = true;

		save.close();
	}
	else if (modeChoice == 3)
	{
		std::ifstream save("customSave.txt");

		if (!save)
			return false;

		save >> FPS;
		save >> isTelepathic;
		save >> fruitAmount;

		for (int i = 0; i < NUM_CELLS_X; i++)
		{
			for (int j = 0; j < NUM_CELLS_Y; j++)
			{
				save >> customGrid[i][j];
			}
		}

		save.close();
	}

	return true;
}

void spawnLineObstacles(bool dir)
{
	const int max = 10; // Maximum line length

	// decide from the given dir var
	bool isHorizontal = dir;

	// Random starting position
	int beginX = rand() % (NUM_CELLS_X - max);
	int beginY = rand() % (NUM_CELLS_Y - max);

	int lineLength = 8 + rand() % max;

	// Random direction for horizontal lines (true = left to right, false = right to left)
	bool moveRight = true;

	// not random anymore cuz it sucks

	for (int i = 0; i < lineLength; ++i) {
		int x, y;

		if (isHorizontal) {
			if (moveRight) {
				x = beginX + i; // Move right
			}
			else {
				x = beginX - i; // Move left
			}
			y = beginY; // Horizontal lines stay at the same y
		}
		else {
			x = beginX; // Vertical lines stay at the same x
			y = beginY + i; // Move down for vertical lines
		}

		// Ensure the line stays within bounds
		if (x < 0 || x >= NUM_CELLS_X || y >= NUM_CELLS_Y) {
			break;
		}

		obstacleGrid[x][y] = true; // Mark as obstacle
	}

	// Skip a random cell in the line to avoid forming a closed square
	int skipIndex = rand() % lineLength;
	int skipX, skipY;

	if (isHorizontal) {
		if (moveRight) {
			skipX = beginX + skipIndex; // Skip moving right
		}
		else {
			skipX = beginX - skipIndex; // Skip moving left
		}
		skipY = beginY; // Horizontal lines stay at the same y
	}
	else {
		skipX = beginX; // Vertical lines stay at the same x
		skipY = beginY + skipIndex; // Skip moving down
	}

	if (skipX >= 0 && skipX < NUM_CELLS_X && skipY < NUM_CELLS_Y) {
		obstacleGrid[skipX][skipY] = false; // Clear one cell
	}
}

void spawnFruit(int num)
{
	// spawn fruits
	do
	{
		fruitX[num] = rand() % NUM_CELLS_X;
		fruitY[num] = rand() % NUM_CELLS_Y;


		// Check if the fruit overlaps with the snake's body
		bool isOnSnake = false;
		for (int i = 0; i < snakelength; i++)
		{
			if (snakeX[i] == fruitX[num] && snakeY[i] == fruitY[num])
			{
				isOnSnake = true;
				break;
			}
		}

		// Check mode-specific conditions
		if (modeChoice == 1 && (isOnSnake || obstacleGrid[fruitX[num]][fruitY[num]]))
			continue;
		if (modeChoice == 2 && isOnSnake)
			continue;
		if (modeChoice == 3 && (isOnSnake || customGrid[fruitX[num]][fruitY[num]]))
			continue;

		// If no conditions are violated, break the loop
		break;

	} while (true);
}

void moveSnake(void)
{
	// moves the snake body

	for (int i = snakelength; i > 0; i--)
	{
		snakeX[i] = snakeX[i - 1];
		snakeY[i] = snakeY[i - 1];
	}

	// moves the snake head

	if (dir == 1)
	{
		snakeY[0]--;
	}
	else if (dir == 2)
	{
		snakeY[0]++;
	}
	else if (dir == 3)
	{
		snakeX[0]++;
	}
	else if (dir == 4)
	{
		snakeX[0]--;
	}

}

void handleWallCollision(void)
{
	// allows the snake to move across the walls of the window

	if (snakeX[0] < 0)
	{
		snakeX[0] = NUM_CELLS_X - 1;
	}
	if (snakeX[0] >= NUM_CELLS_X)
	{
		snakeX[0] = 0;
	}
	if (snakeY[0] < 0)
	{
		snakeY[0] = NUM_CELLS_Y - 1;
	}
	if (snakeY[0] >= NUM_CELLS_Y)
	{
		snakeY[0] = 0;
	}
}


bool checkWallCollision(void)
{
	// checks wall collision in accordance to the game mode

	if (modeChoice == 1 || (modeChoice == 3 && !isTelepathic)) // SIMPLE mode
	{
		if (snakeX[0] < 0 || snakeX[0] >= NUM_CELLS_X || snakeY[0] < 0 || snakeY[0] >= NUM_CELLS_Y)
			return true;
	}
	else if (modeChoice == 2 || (modeChoice == 3 && isTelepathic)) // ENDLESS mode
	{
		handleWallCollision(); // Wrap the snake around the walls
		return false;          // No collision in ENDLESS mode
	}
	return false;
}

bool checkobstacleCollision(void)
{
	// checks if the snake has collided with a line obstacle or not spawned in modechoice 1

	for (int j = 0; j < NUM_CELLS_Y; j++)
	{
		for (int i = 0; i < NUM_CELLS_X; i++)
		{
			if (obstacleGrid[i][j] == 1 && i == snakeX[0] && j == snakeY[0])
				return true;
		}
	}
	return false;
}

bool checkcustomCollision(void)
{
	// checks collision with the custom grid that the player has drawn

	for (int j = 0; j < NUM_CELLS_Y; j++)
	{
		for (int i = 0; i < NUM_CELLS_X; i++)
		{
			if (customGrid[i][j] && i == snakeX[0] && j == snakeY[0])
				return true;
		}
	}
	return false;
}

void checkFruitCollision(void)
{
	// increments score and snake's length if the fruit has collided and plays the sound effect

	for (int i = 0; i < fruitAmount; i++)
	{
		if (snakeX[0] == fruitX[i] && snakeY[0] == fruitY[i])
		{
			score += 10;
			spawnFruit(i);
			fruitEatenSound.setPitch(1.5f);
			fruitEatenSound.play();
			if (snakelength < MAX_SNAKE_LENGTH)
				snakelength++;
		}
	}

}

bool checkBodyCollision(void)
{
	// checks if a snake collides with it's body or not

	for (int i = 1; i < snakelength; i++)
	{
		if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
		{
			return true;
		}
	}

	return false;
}

void lvlIncrease(void)
{
	// increases the user's level

	lvl++;
	score = 0;
	spawnLineObstacles(true);
	spawnLineObstacles(false);
}

std::string loadSimpleLeaderBoard(void)
{
	std::ifstream fin;

	fin.open("simpleLeaderBoard.txt");

	std::string leadText = "Simple Leader Board:\n";

	if (fin)
	{
		int score;
		std::string name;
		for (int i = 0; i < 3; i++)
		{
			fin >> name >> score;
			if (name == "")
				name = "----------";
			leadText += std::to_string(i + 1) + ". " + name + ": " + std::to_string(score) + "\n";
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
			leadText += std::to_string(i + 1) + ". ----------: 0\n";
	}

	fin.close();

	return leadText;
}

std::string loadEndlessLeaderBoard(void)
{
	std::ifstream fin;

	fin.open("endlessLeaderBoard.txt");

	std::string leadText = "Endless Leader Board:\n";

	if (fin)
	{
		int score;
		std::string name;
		for (int i = 0; i < 3; i++)
		{
			fin >> name >> score;
			if (name == "")
				name = "----------";
			leadText += std::to_string(i + 1) + ". " + name + ": " + std::to_string(score) + "\n";
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
			leadText += std::to_string(i + 1) + ". ----------: 0\n";
	}

	fin.close();

	return leadText;
}

void loadGame(sf::RenderWindow& window)
{
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("bg.jpg"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite backgroundSprite(backgroundTexture);

	float xdir = window.getSize().x / backgroundSprite.getGlobalBounds().width;
	float ydir = window.getSize().y / backgroundSprite.getGlobalBounds().height;
	backgroundSprite.setScale(xdir, ydir);

	sf::Texture fruitTexture;
	if (!fruitTexture.loadFromFile("fruit.png"))
		std::cout << "Error loading fruit image" << std::endl;
	sf::Sprite fruitSprite(fruitTexture);



	sf::Font font;

	if (!font.loadFromFile("PressStart2P-Regular.ttf"))
	{
		std::cout << "fonts didn't load" << std::endl;
	}

	sf::Text gamename;
	gamename.setFont(font);
	gamename.setString("sf::Snake");
	gamename.setCharacterSize(55);
	gamename.setPosition(window.getSize().x / 2 - 250, window.getSize().y / 5);
	gamename.setFillColor(sf::Color::Green);
	gamename.setStyle(sf::Text::Bold);

	sf::Clock clock;
	float amplitude = 35.0f;
	float speed = 200.0f;
	float frequency = 2.0f;

	sf::Text ColorSelect;
	ColorSelect.setFont(font);
	ColorSelect.setCharacterSize(40);
	ColorSelect.setFillColor(sf::Color::Green);
	ColorSelect.setString("Press left or right keys to change color");
	ColorSelect.setPosition(window.getSize().x / 2 - 800, window.getSize().y / 3 + 10);

	sf::Text gradientTag;
	gradientTag.setFont(font);
	gradientTag.setString("Press G to toggle GRADIENT");
	gradientTag.setCharacterSize(40);
	gradientTag.setFillColor(sf::Color::White);
	gradientTag.setPosition(WIDTH / 2 - 550, HEIGHT / 2 + 150);

	sf::Text fruitTag;
	fruitTag.setFont(font);
	fruitTag.setString("Press F to toggle Fruit Selection");
	fruitTag.setCharacterSize(40);
	fruitTag.setFillColor(sf::Color::Green);
	fruitTag.setPosition(WIDTH / 2 - 675, HEIGHT / 2 + 250);

	sf::Text EnterKeyIntimidation;
	EnterKeyIntimidation.setFont(font);
	EnterKeyIntimidation.setCharacterSize(40);
	EnterKeyIntimidation.setFillColor(sf::Color::Green);
	EnterKeyIntimidation.setString("Press Enter Key to continue\n\n  Press Q to Exit anytime");
	EnterKeyIntimidation.setPosition(window.getSize().x / 2 - 550, window.getSize().y - window.getSize().y / 3 + 150);

	sf::RectangleShape block(sf::Vector2f(GRIDSIZE, GRIDSIZE));
	block.setFillColor(sf::Color::Green);

	int X_OFFSET = 650;

	sf::CircleShape rightKey(GRIDSIZE, 3);
	rightKey.setPosition(sf::Vector2f(window.getSize().x / 2 + 375, window.getSize().y / 2 - 5));
	rightKey.setRotation(90);
	rightKey.setFillColor(sf::Color::Transparent);

	sf::CircleShape leftKey(GRIDSIZE, 3);
	leftKey.setRotation(270);
	leftKey.setPosition(sf::Vector2f(X_OFFSET - 85, window.getSize().y / 2 + 56));
	leftKey.setFillColor(sf::Color::Transparent);

	int keyDirection = 2;
	bool isFruit = false;

	sf::Text simpleLeaderBoardTag;
	simpleLeaderBoardTag.setFont(font);
	simpleLeaderBoardTag.setCharacterSize(40);
	simpleLeaderBoardTag.setString(loadSimpleLeaderBoard());
	simpleLeaderBoardTag.setLineSpacing(1.25f);

	sf::Text endlessLeaderBoardTag;
	endlessLeaderBoardTag.setFont(font);
	endlessLeaderBoardTag.setCharacterSize(40);
	endlessLeaderBoardTag.setString(loadEndlessLeaderBoard());
	endlessLeaderBoardTag.setPosition(WIDTH - 800, 0);
	endlessLeaderBoardTag.setLineSpacing(1.25f);


	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Enter) return;
				if (event.key.code == sf::Keyboard::Q) window.close();
				if (event.key.code == sf::Keyboard::G) isGradient = !isGradient;
				if ((event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) && !isFruit)
				{
					colorChoice = (colorChoice + 1) % TOTAL_COLORS;
					keyDirection = 1;
				}
				else if ((event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) && !isFruit)
				{
					colorChoice--;
					if (colorChoice < 0) colorChoice = 5;
					keyDirection = 0;
				}
				else
				{
					keyDirection = -1;
				}
				if (event.key.code == sf::Keyboard::F)
				{
					isFruit = !isFruit;
				}
				if (isFruit)
				{
					if ((event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) && isFruit)
					{
						fruitChoice++;
						if (fruitChoice > 4) fruitChoice = 0;
						keyDirection = 1;

					}
					else if ((event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) && isFruit)
					{
						fruitChoice--;
						if (fruitChoice < 0) fruitChoice = 4;
						keyDirection = 0;
					}
					else
					{
						keyDirection = -1;
					}
				}
			}
		}

		if (fruitChoice == 0)
		{
			if (!fruitTexture.loadFromFile("fruit.png"))
				std::cout << "Error loading fruit image" << std::endl;
		}
		else if (fruitChoice == 1)
		{
			if (!fruitTexture.loadFromFile("fruit2.png"))
				std::cout << "Error loading fruit image" << std::endl;
		}
		else if (fruitChoice == 2)
		{
			if (!fruitTexture.loadFromFile("fruit3.png"))
				std::cout << "Error loading fruit image" << std::endl;
		}
		else if (fruitChoice == 3)
		{
			if (!fruitTexture.loadFromFile("fruit4.png"))
				std::cout << "Error loading fruit image" << std::endl;
		}
		else if (fruitChoice == 4)
		{
			if (!fruitTexture.loadFromFile("fruit5.png"))
				std::cout << "Error loading fruit image" << std::endl;
		}

		sf::Sprite fruitSprite(fruitTexture);

		float xdir1 = 4;
		float ydir1 = 4;
		fruitSprite.setScale(xdir1, ydir1);
		fruitSprite.setPosition(930.0f, 530.0f);

		float elapsed = clock.getElapsedTime().asSeconds();
		float x = speed * elapsed;
		float y = amplitude * std::sin(frequency * x * 0.01f);

		if (!isFruit)
		{
			leftKey.setPosition(sf::Vector2f(X_OFFSET - 85 + x, window.getSize().y / 2 + 56 + y));
			rightKey.setPosition(sf::Vector2f(window.getSize().x / 2 + 375 + x, window.getSize().y / 2 - 5 + amplitude * std::sin(frequency * x * 0.01f + 0.5f * 19)));

			int right_x = rightKey.getPosition().x;

			if (right_x > WIDTH)
			{
				do
				{
					right_x -= WIDTH;
				} while (right_x > WIDTH);
				rightKey.setPosition(right_x, window.getSize().y / 2 - 5 + amplitude * std::sin(frequency * x * 0.01f + 0.5f * 19));
			}

			int left_x = leftKey.getPosition().x;

			if (left_x > WIDTH)
			{
				do
				{
					left_x -= WIDTH;
				} while (left_x > WIDTH);
				leftKey.setPosition(left_x, window.getSize().y / 2 + 56 + y);
			}
		}
		else
		{
			rightKey.setPosition(1195, 555);
			leftKey.setPosition(765, 616);
		}

		window.clear();

		window.draw(backgroundSprite);

		colorSetter(block);

		gamename.setFillColor(block.getFillColor());
		ColorSelect.setFillColor(block.getFillColor());
		EnterKeyIntimidation.setFillColor(block.getFillColor());
		gradientTag.setFillColor(block.getFillColor());
		fruitTag.setFillColor(block.getFillColor());
		simpleLeaderBoardTag.setFillColor(block.getFillColor());
		endlessLeaderBoardTag.setFillColor(block.getFillColor());

		window.draw(gamename);
		window.draw(ColorSelect);
		window.draw(simpleLeaderBoardTag);
		window.draw(endlessLeaderBoardTag);


		if (keyDirection == 1)
		{
			rightKey.setOutlineThickness(5);
			rightKey.setOutlineColor(sf::Color(153, 144, 144));
			leftKey.setOutlineThickness(0);
		}
		else if (keyDirection == 0)
		{
			leftKey.setOutlineThickness(5);
			leftKey.setOutlineColor(sf::Color(153, 144, 144));
			rightKey.setOutlineThickness(0);
		}
		else
		{
			rightKey.setOutlineThickness(0);
			leftKey.setOutlineThickness(0);
		}

		window.draw(leftKey);

		if (isFruit)
		{
			window.draw(fruitSprite);
		}
		else
		{
			for (int i = 0; i < 20; i++)
			{
				float y = amplitude * std::sin(frequency * x * 0.01f + 0.5f * i);
				block.setPosition(X_OFFSET + GRIDSIZE * i + x, 550 + y);
				if (isGradient)
					gradientSetter(block, 20 - i, 20);
				int block_x;
				if (block.getPosition().x > WIDTH)
				{
					block_x = block.getPosition().x;
					do
					{
						block_x -= WIDTH;
					} while (block_x > WIDTH);
					block.setPosition(block_x, 550 + y);
				}
				window.draw(block);
			}
		}

		window.draw(rightKey);

		window.draw(gradientTag);

		window.draw(fruitTag);

		window.draw(EnterKeyIntimidation);

		window.display();
	}
}

void levelSelectMenu(sf::RenderWindow& window)
{
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("bg.jpg"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite backgroundSprite(backgroundTexture);

	float xdir = window.getSize().x / backgroundSprite.getGlobalBounds().width;
	float ydir = window.getSize().y / backgroundSprite.getGlobalBounds().height;
	backgroundSprite.setScale(xdir, ydir);

	window.setFramerateLimit(0);

	sf::Font font;

	if (!font.loadFromFile("PressStart2P-Regular.ttf"))
	{
		std::cout << "font not loaded" << std::endl;
	}

	sf::Text titletag;
	titletag.setFont(font);
	titletag.setString("Select Game Mode");
	titletag.setCharacterSize(70);
	titletag.setFillColor(sf::Color::White);
	titletag.setPosition(sf::Vector2f(window.getSize().x / 2 - 550, 130));


	sf::Text simpleTag;
	simpleTag.setFont(font);
	simpleTag.setString("SIMPLE");
	simpleTag.setCharacterSize(60);
	simpleTag.setFillColor(sf::Color::White);
	simpleTag.setPosition(sf::Vector2f(window.getSize().x / 2 - 150, window.getSize().y / 2 - 200));

	sf::Text endlessTag;
	endlessTag.setFont(font);
	endlessTag.setCharacterSize(60);
	endlessTag.setString("ENDLESS");
	endlessTag.setFillColor(sf::Color::White);
	endlessTag.setPosition(sf::Vector2f(window.getSize().x / 2 - 175, window.getSize().y / 2));

	sf::Text customTag;
	customTag.setFont(font);
	customTag.setCharacterSize(60);
	customTag.setString("CUSTOM");
	customTag.setFillColor(sf::Color::White);
	customTag.setPosition(sf::Vector2f(window.getSize().x / 2 - 155, window.getSize().y / 2 + 200));

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Q) window.close();
				if (event.key.code == sf::Keyboard::Enter) return;

				if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
				{
					modeChoice--;
					if (modeChoice < 1)
						modeChoice = 3;
				}

				if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
				{
					modeChoice++;
					if (modeChoice > 3)
						modeChoice = 1;
				}
			}
		}

		window.clear();

		window.draw(backgroundSprite);

		if (modeChoice == 1)
		{
			simpleTag.setFillColor(sf::Color::Green);
			endlessTag.setFillColor(sf::Color::White);
			customTag.setFillColor(sf::Color::White);
		}
		else if (modeChoice == 2)
		{
			endlessTag.setFillColor(sf::Color::Green);
			simpleTag.setFillColor(sf::Color::White);
			customTag.setFillColor(sf::Color::White);
		}
		else if (modeChoice == 3)
		{
			endlessTag.setFillColor(sf::Color::White);
			simpleTag.setFillColor(sf::Color::White);
			customTag.setFillColor(sf::Color::Green);
		}

		window.draw(titletag);
		window.draw(simpleTag);
		window.draw(endlessTag);
		window.draw(customTag);

		window.display();

	}
}

bool checkSave(void)
{
	if (modeChoice == 1)
	{
		std::ifstream save("simpleSave.txt");
		if (!save) return false;
		return true;
	}
	if (modeChoice == 2)
	{
		std::ifstream save("endlessSave.txt");
		if (!save) return false;
		return true;
	}
	return false;
}

void loadSaveMenu(sf::RenderWindow& window)
{
	if (!checkSave())
	{
		spawnFruit(0);
		return;
	}

	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("bg.jpg"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite backgroundSprite(backgroundTexture);

	float xdir = window.getSize().x / backgroundSprite.getGlobalBounds().width;
	float ydir = window.getSize().y / backgroundSprite.getGlobalBounds().height;
	backgroundSprite.setScale(xdir, ydir);


	sf::Font font;
	font.loadFromFile("PressStart2P-Regular.ttf");

	sf::Text headerTag;
	headerTag.setFont(font);
	headerTag.setString("Wanna load the save file???");
	headerTag.setCharacterSize(50);
	headerTag.setPosition(WIDTH / 2 - 625, HEIGHT / 2 - 100);

	sf::Text choiceTag;
	choiceTag.setFont(font);
	choiceTag.setString("Y/N");
	choiceTag.setCharacterSize(50);
	choiceTag.setPosition(WIDTH / 2 - 50, HEIGHT / 2);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Q) window.close();
				if (event.key.code == sf::Keyboard::Y)
				{
					loadSave();
					return;
				}
				if (event.key.code == sf::Keyboard::N)
				{
					spawnFruit(0);
					return;
				}
			}
		}

		window.clear();

		window.draw(backgroundSprite);

		window.draw(headerTag);
		window.draw(choiceTag);

		window.display();
	}
}

void levelChangePauseMenu(sf::RenderWindow& window)
{
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("bg.jpg"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite backgroundSprite(backgroundTexture);

	float xdir = window.getSize().x / backgroundSprite.getGlobalBounds().width;
	float ydir = window.getSize().y / backgroundSprite.getGlobalBounds().height;
	backgroundSprite.setScale(xdir, ydir);

	sf::SoundBuffer lvlBuffer;

	if (!lvlBuffer.loadFromFile("lvlup.ogg"))
	{
		std::cout << "Couldnt load lvlup.ogg" << std::endl;
		exit(2);
	}

	sf::Sound lvlupSound(lvlBuffer);
	lvlupSound.setPitch(1.5f);

	lvlupSound.play();

	sf::Font font;
	font.loadFromFile("PressStart2P-Regular.ttf");

	sf::Text lvlText;
	lvlText.setFont(font);
	lvlText.setString("LEVEL " + std::to_string(lvl));
	lvlText.setCharacterSize(44);
	lvlText.setFillColor(sf::Color::White);
	lvlText.setPosition(window.getSize().x / 2 - 175, window.getSize().y / 2 - 200);


	snakeX[0] = NUM_CELLS_X / 2;
	snakeY[0] = NUM_CELLS_Y / 2;

	snakelength = 1;

	dir = 0;

	sf::Text continueText;
	continueText.setFont(font);
	continueText.setCharacterSize(44);
	continueText.setString("Press SpaceBar to continue");
	continueText.setFillColor(sf::Color::White);
	continueText.setPosition(window.getSize().x / 2 - 550, window.getSize().y / 2 + 100);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space) return;
				if (event.key.code == sf::Keyboard::Q) window.close();
			}
		}

		window.clear();

		window.draw(backgroundSprite);

		window.draw(lvlText);
		window.draw(continueText);

		window.display();
	}
}

void customlvldesginer(sf::RenderWindow& window)
{
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("bg.jpg"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite backgroundSprite(backgroundTexture);

	float xdir = window.getSize().x / backgroundSprite.getGlobalBounds().width;
	float ydir = window.getSize().y / backgroundSprite.getGlobalBounds().height;
	backgroundSprite.setScale(xdir, ydir);

	int start = 0;

	sf::Font font;
	font.loadFromFile("PressStart2P-Regular.ttf");


	sf::Text mainTag;
	mainTag.setFont(font);
	mainTag.setCharacterSize(70);
	mainTag.setFillColor(sf::Color(255, 241, 64));
	mainTag.setPosition(WIDTH / 2 - 700, 40);
	mainTag.setString("Custom Level Designer");

	sf::Text keyTag;
	keyTag.setFont(font);
	keyTag.setCharacterSize(40);
	keyTag.setFillColor(sf::Color(255, 241, 64));
	keyTag.setString("Click and hold left mouse button to add blocks\n   or right mouse button to remove blocks\n    Press T to toggle Telepathic mode");
	keyTag.setPosition(window.getSize().x / 2 - 925, 250);
	keyTag.setLineSpacing(1.5f);

	sf::Text changeAmount;
	changeAmount.setFont(font);
	changeAmount.setCharacterSize(43);
	changeAmount.setFillColor(sf::Color(255, 241, 64));
	changeAmount.setPosition(window.getSize().x / 2 - 700, 500);
	changeAmount.setString("Press Left or Right Key to change\n     the amount of fruit");


	sf::Text speedChangeTag;
	speedChangeTag.setFont(font);
	speedChangeTag.setCharacterSize(43);
	speedChangeTag.setFillColor(sf::Color(255, 241, 64));
	speedChangeTag.setPosition(window.getSize().x / 2 - 700, 650);
	speedChangeTag.setString("Press Up or Down Key to change\n    the speed of the snake");

	sf::Text saveLoadTagInstructions;
	saveLoadTagInstructions.setFont(font);
	saveLoadTagInstructions.setCharacterSize(43);
	saveLoadTagInstructions.setFillColor(sf::Color(255, 241, 64));
	saveLoadTagInstructions.setString("Press S\\L to save\\load design");
	saveLoadTagInstructions.setPosition(WIDTH / 2 - 600, 800);

	sf::Text buttonTag;
	buttonTag.setFont(font);
	buttonTag.setCharacterSize(60);
	buttonTag.setFillColor(sf::Color(255, 241, 64));
	buttonTag.setString("Press Enter to start drawing");
	buttonTag.setPosition(WIDTH / 8 - 100, window.getSize().y / 2 + 400);

	sf::Text fruitCountTag;
	fruitCountTag.setFont(font);
	fruitCountTag.setCharacterSize(20);
	fruitCountTag.setFillColor(sf::Color::White);
	fruitCountTag.setPosition(0, 0);

	sf::Text telepathicTag;
	telepathicTag.setFont(font);
	telepathicTag.setCharacterSize(20);
	telepathicTag.setPosition(200, 0);

	sf::Text speedTag;
	speedTag.setFont(font);
	speedTag.setFillColor(sf::Color::White);
	speedTag.setCharacterSize(20);
	speedTag.setPosition(640, 0);

	sf::Text saveLoadTag;
	saveLoadTag.setFont(font);
	saveLoadTag.setString("Saved the Design");
	saveLoadTag.setFillColor(sf::Color::White);
	saveLoadTag.setCharacterSize(20);
	saveLoadTag.setPosition(850, 0);




	int centerX = NUM_CELLS_X / 2;
	int centerY = NUM_CELLS_Y / 2;

	bool displaySaveLoad = false;

	bool save = false;
	bool load = false;
	bool loadSuccess = false;

	sf::RectangleShape block(sf::Vector2f(GRIDSIZE - 1, GRIDSIZE - 1));
	block.setPosition(sf::Vector2f(centerX * GRIDSIZE, centerY * GRIDSIZE));

	colorSetter(block);

	int enterCount = 0;

	int hover_block_x = 0, hover_block_y = 0;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Q) window.close();
				if (event.key.code == sf::Keyboard::Enter)
				{
					start = 1;
				}
				if (start == 1)
				{
					if (event.key.code == sf::Keyboard::Enter)
					{
						if (enterCount == 0) enterCount = 1;
						else
						{
							for (int i = 0; i < fruitAmount; i++)
							{
								spawnFruit(i);
							}
							return;
						}
					}
					if (event.key.code == sf::Keyboard::Right)
					{
						fruitAmount++;
						if (fruitAmount > 10)
							fruitAmount = 1;
						displaySaveLoad = false;
					}
					if (event.key.code == sf::Keyboard::Left)
					{
						fruitAmount--;
						if (fruitAmount < 1)
							fruitAmount = 10;
						displaySaveLoad = false;
					}
					if (event.key.code == sf::Keyboard::T)
					{
						isTelepathic = !isTelepathic;
						displaySaveLoad = false;
					}
					if (event.key.code == sf::Keyboard::Up)
					{
						FPS++;
						if (FPS > 60)
							FPS = 1;
						displaySaveLoad = false;
					}
					if (event.key.code == sf::Keyboard::Down)
					{
						FPS--;
						if (FPS < 1)
							FPS = 60;
						displaySaveLoad = false;
					}
					if (event.key.code == sf::Keyboard::S)
					{
						saveGame();
						displaySaveLoad = true;
						save = true;
						load = false;
					}
					if (event.key.code == sf::Keyboard::L)
					{
						loadSuccess = loadSave();
						displaySaveLoad = true;
						save = false;
						load = true;
					}
				}
			}
		}

		if (start == 1)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);

				int x = mousePos.x / GRIDSIZE;
				int y = mousePos.y / GRIDSIZE;

				if (x != centerX && y != centerY)
					customGrid[x][y] = true;
				displaySaveLoad = false;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);

				int x = mousePos.x / GRIDSIZE;
				int y = mousePos.y / GRIDSIZE;

				customGrid[x][y] = false;

				displaySaveLoad = false;
			}
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			hover_block_x = mousePos.x / GRIDSIZE;
			hover_block_y = mousePos.y / GRIDSIZE;

		}

		window.clear();


		fruitCountTag.setString("Fruits: " + std::to_string(fruitAmount));
		speedTag.setString("Speed: " + std::to_string(FPS));

		if (isTelepathic)
		{
			telepathicTag.setFillColor(sf::Color::Green);
			telepathicTag.setString("Telepathic Walls: ON");
		}
		else
		{
			telepathicTag.setFillColor(sf::Color::Red);
			telepathicTag.setString("Telepathic Walls: OFF");
		}

		if (start == 0)
		{
			window.draw(backgroundSprite);
			window.draw(mainTag);
			window.draw(keyTag);
			window.draw(speedChangeTag);
			window.draw(buttonTag);
			window.draw(saveLoadTagInstructions);
			window.draw(changeAmount);
		}
		else
		{
			sf::RectangleShape hoverBlock;
			hoverBlock.setPosition(hover_block_x * GRIDSIZE, hover_block_y * GRIDSIZE);
			hoverBlock.setSize(sf::Vector2f(GRIDSIZE - 1, GRIDSIZE - 1));
			hoverBlock.setFillColor(sf::Color(150, 150, 150));
			window.draw(hoverBlock);

			window.draw(fruitCountTag);
			window.draw(telepathicTag);
			window.draw(speedTag);

			if (displaySaveLoad)
			{
				if (save)
				{
					saveLoadTag.setString("Saved the Design");
					saveLoadTag.setFillColor(sf::Color::Green);
				}
				if (load)
				{
					if (loadSuccess)
					{
						saveLoadTag.setString("Loaded the Design");
						saveLoadTag.setFillColor(sf::Color::Green);
					}
					else
					{
						saveLoadTag.setString("No Design Available");
						saveLoadTag.setFillColor(sf::Color::Red);
					}
				}
				window.draw(saveLoadTag);
			}

			window.draw(block);
			for (int j = 0; j < NUM_CELLS_Y; j++)
			{
				for (int i = 0; i < NUM_CELLS_X; i++)
				{
					if (customGrid[i][j])
					{
						sf::RectangleShape obs(sf::Vector2f(GRIDSIZE - 1, GRIDSIZE - 1));
						obs.setFillColor(sf::Color::White);
						obs.setPosition(i * GRIDSIZE, j * GRIDSIZE);
						window.draw(obs);
					}
				}
			}
		}

		window.display();
	}

}

void waitKeyPress(sf::RenderWindow& window)
{
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
				return;
		}
	}

	return;
}

void mainGame(sf::RenderWindow& window)
{
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("bg.jpg"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite backgroundSprite(backgroundTexture);

	sf::Texture powerUpTexture;
	if (!powerUpTexture.loadFromFile("powerup.png"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite powerup(powerUpTexture);

	float xdir = window.getSize().x / backgroundSprite.getGlobalBounds().width;
	float ydir = window.getSize().y / backgroundSprite.getGlobalBounds().height;
	backgroundSprite.setScale(xdir, ydir);

	window.setFramerateLimit(FPS);

	srand(time(0));

	sf::RectangleShape snakeSegement(sf::Vector2f(GRIDSIZE, GRIDSIZE));
	colorSetter(snakeSegement);

	sf::Texture fruitTexture;

	if (!fruitTexture.loadFromFile("fruit.png"))
		std::cout << "Error loading fruit image" << std::endl;
	sf::Sprite fruit(fruitTexture);

	if (fruitChoice == 0)
	{
		if (!fruitTexture.loadFromFile("fruit.png"))
			std::cout << "Error loading fruit image" << std::endl;
		sf::Sprite fruit(fruitTexture);

	}
	else if (fruitChoice == 1)
	{
		if (!fruitTexture.loadFromFile("fruit2.png"))
			std::cout << "Error loading fruit image" << std::endl;
		sf::Sprite fruit(fruitTexture);

	}
	else if (fruitChoice == 2)
	{
		if (!fruitTexture.loadFromFile("fruit3.png"))
			std::cout << "Error loading fruit image" << std::endl;
		sf::Sprite fruit(fruitTexture);

	}
	else if (fruitChoice == 3)
	{
		if (!fruitTexture.loadFromFile("fruit4.png"))
			std::cout << "Error loading fruit image" << std::endl;
		sf::Sprite fruit(fruitTexture);

	}
	else if (fruitChoice == 4)
	{
		if (!fruitTexture.loadFromFile("fruit5.png"))
			std::cout << "Error loading fruit image" << std::endl;
		sf::Sprite fruit(fruitTexture);
	}

	/*sf::RectangleShape fruit(sf::Vector2f(GRIDSIZE, GRIDSIZE));
	fruit.setFillColor(sf::Color::Red)*/;

	sf::Font font;

	if (!font.loadFromFile("PressStart2P-Regular.ttf"))
	{
		std::cout << "fonts didn't load" << std::endl;
	}

	int timer = 0;
	bool startTimer = false;

	sf::Text scoreDisplay;
	scoreDisplay.setFont(font);
	scoreDisplay.setCharacterSize(30);
	scoreDisplay.setFillColor(sf::Color::Cyan);
	scoreDisplay.setPosition(sf::Vector2f(10, 10));

	sf::Text pauseDisplay;
	pauseDisplay.setFont(font);
	pauseDisplay.setString("||");
	pauseDisplay.setStyle(sf::Text::Bold);
	pauseDisplay.setFillColor(sf::Color::White);
	pauseDisplay.setPosition(window.getSize().x - 70, 20);
	pauseDisplay.setLetterSpacing(0.3f);

	sf::RectangleShape wallBlock(sf::Vector2f(GRIDSIZE, GRIDSIZE));
	wallBlock.setFillColor(sf::Color::Blue);

	sf::Text saveTag;
	saveTag.setFont(font);
	saveTag.setString("Press S to Save Game");
	saveTag.setCharacterSize(50);
	saveTag.setPosition(WIDTH / 2 - 500, HEIGHT / 2 - 50);

	sf::Text continueTag;
	continueTag.setString("Press Any Key to continue");
	continueTag.setFont(font);
	continueTag.setCharacterSize(50);
	continueTag.setPosition(WIDTH / 2 - 600, HEIGHT / 2 - 50);


	bool userPause = false;
	int saveCount = 0;

	sf::Clock clock;
	bool isDraw = false;
	int num = 0;

	bool startPowerUpTimer = false;
	int powerUpTimer = 0;

	while (window.isOpen() && !gameover)
	{
		sf::Event event;

		bool directionChanged = false;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed && !directionChanged)
			{
				if (universalPause)
				{
					universalPause = false;
					dir = prevDir;
				}
				if ((event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) && !pause)
				{
					if (snakelength == 1)
					{
						dir = 1;
						directionChanged = true;
					}
					else if (dir != 2)
					{
						dir = 1;
						directionChanged = true;
					}
				}
				else if ((event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) && !pause)
				{
					if (snakelength == 1)
					{
						dir = 2;
						directionChanged = true;
					}
					else if (dir != 1)
					{
						dir = 2;
						directionChanged = true;
					}
				}
				else if ((event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) && !pause)
				{
					if (snakelength == 1)
					{
						dir = 3;
						directionChanged = true;
					}
					else if (dir != 4)
					{
						dir = 3;
						directionChanged = true;
					}
				}
				else if ((event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) && !pause)
				{
					if (snakelength == 1)
					{
						dir = 4;
						directionChanged = true;
					}
					else if (dir != 3)
					{
						dir = 4;
						directionChanged = true;
					}
				}
				else if (event.key.code == sf::Keyboard::S && userPause)
				{
					saveGame();
					saveCount++;
					timer = 0;
					startTimer = true;
				}
				else if (event.key.code == sf::Keyboard::Q)
				{
					if (modeChoice != 3)
						saveGame();
					window.close();
				}

				if (event.key.code == sf::Keyboard::Escape)
				{
					if (!pause)
					{
						pause = true;
						prevDir = dir;
						dir = 0;
						if (modeChoice != 3)
							userPause = true;
						saveCount = 0;
						timer = 0;
					}
					else
					{
						pause = false;
						dir = prevDir;
						if (modeChoice != 3)
							userPause = false;
						saveCount = 0;
					}
				}
			}
		}

		if (startTimer)
		{
			timer++;
			if (timer >= FPS)
			{
				startTimer = false;
				timer = 0;
			}
		}

		if (!pause && !universalPause)
		{
			moveSnake();

			if (checkWallCollision()) // SIMPLE mode triggers gameover
			{
				gameover = true;
				break;
			}

			if (checkBodyCollision())
			{
				gameover = true;
				break;
			}

			checkFruitCollision();
		}
		if (modeChoice == 2)
		{
			if (checkPowerUpCollision())
			{
				powerup.setPosition(2000, 2000);
			}

		}

		if (modeChoice == 1)
		{
			if (score % lvlChangeThreshold == 0 && score != 0)
			{
				lvlIncrease();
				levelChangePauseMenu(window);
			}

			if (checkobstacleCollision())
			{
				gameover = true;
				break;
			}
		}

		if (modeChoice == 3)
		{
			if (checkcustomCollision())
			{
				gameover = true;
				break;
			}
		}

		window.clear();


		window.draw(backgroundSprite);

		std::string scoreText = "Score: " + std::to_string(score + lvl * lvlChangeThreshold);

		scoreDisplay.setString(scoreText);


		for (int i = 0; i < fruitAmount; i++)
		{
			fruit.setPosition(sf::Vector2f(GRIDSIZE * fruitX[i], GRIDSIZE * fruitY[i]));
			window.draw(fruit);
		}

		for (int i = 0; i < snakelength; i++)
		{
			if (isGradient)
				gradientSetter(snakeSegement, i);
			snakeSegement.setPosition(sf::Vector2f(GRIDSIZE * snakeX[i], GRIDSIZE * snakeY[i]));
			window.draw(snakeSegement);
		}

		if (modeChoice == 1)
		{
			for (int j = 0; j < NUM_CELLS_Y; j++) { // Rows (y-axis)
				for (int i = 0; i < NUM_CELLS_X; i++) { // Columns (x-axis)
					if (obstacleGrid[i][j])
					{ // Grid at (i, j)
						sf::RectangleShape block(sf::Vector2f(GRIDSIZE - 1, GRIDSIZE - 1));
						block.setFillColor(sf::Color::Blue);
						block.setPosition(i * GRIDSIZE, j * GRIDSIZE); // Corrected x and y
						window.draw(block);
					}
				}
			}
		}

		if (modeChoice == 3)
		{
			for (int j = 0; j < NUM_CELLS_Y; j++)
			{
				for (int i = 0; i < NUM_CELLS_X; i++)
				{
					if (customGrid[i][j])
					{
						sf::RectangleShape block(sf::Vector2f(GRIDSIZE - 1, GRIDSIZE - 1));
						block.setFillColor(sf::Color::Blue);
						block.setPosition(i * GRIDSIZE, j * GRIDSIZE);
						window.draw(block);
					}
				}
			}
		}

		if (pause)
		{
			if (userPause)
			{
				if (saveCount > 0)
					saveTag.setFillColor(sf::Color::Green);
				if (!startTimer)
					saveTag.setFillColor(sf::Color::White);
				window.draw(saveTag);
			}
			window.draw(pauseDisplay);
		}

		window.draw(scoreDisplay);

		if (universalPause)
		{
			window.draw(continueTag);
		}



		if (modeChoice == 2)
		{
			if (!startPowerUpTimer)
			{
				if (clock.getElapsedTime().asSeconds() > 5)
				{
					int chance = rand() % 2;
					if (chance)
					{
						spawnPowerUp();
						powerup.setPosition(powerupx * GRIDSIZE, powerupy * GRIDSIZE);
						isDraw = true;
						startPowerUpTimer = true;
						powerUpTimer = 0;
					}
					clock.restart();
				}
			}
			if (startPowerUpTimer)
			{
				if (checkPowerUpCollision())
				{
					startPowerUpTimer = false;
					powerupx = 2000;
					powerupy = 2000;
					isDraw = false;
					powerUpTimer = 0;
				}
				if (powerUpTimer < FPS * 3)
				{
					powerUpTimer++;
				}
				else
				{
					powerUpTimer = 0;
					startPowerUpTimer = false;
					powerupx = 2000;
					powerupy = 2000;
					isDraw = false;
					clock.restart();
				}
			}


			if (isDraw)
			{
				window.draw(powerup);
			}

		}
		window.display();
	}
}

void sortLeaderBoard(void)
{
	for (int i = 0; i < 4; i++)
	{
		int largest = i;
		for (int j = i + 1; j < 4; j++)
		{
			if (leaderBoardScores[j] > leaderBoardScores[largest])
				largest = j;
		}
		std::swap(leaderBoardScores[largest], leaderBoardScores[i]);
		std::swap(leaderBoardNames[largest], leaderBoardNames[i]);
	}
}

int findPlayerScore(void)
{
	for (int i = 3; i >= 0; i--)
	{
		if (leaderBoardScores[i] == (score + lvl * lvlChangeThreshold))
			return i;
	}
}

std::string inputNameScreen(sf::RenderWindow& window)
{
	std::string playerName = "";

	sf::Font font;
	font.loadFromFile("PressStart2P-Regular.ttf");

	sf::Text enterTag;
	enterTag.setFont(font);
	enterTag.setString("Enter your name for leaderboard");
	enterTag.setCharacterSize(45);
	enterTag.setFillColor(sf::Color::White);
	enterTag.setPosition(window.getSize().x / 2 - 725, window.getSize().y / 2 - 400);


	sf::Text nameTag;
	nameTag.setFont(font);
	nameTag.setCharacterSize(45);
	nameTag.setFillColor(sf::Color::White);
	nameTag.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 100);


	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 8)
				{
					if (!playerName.empty()) playerName.pop_back();
				}
				else if (event.text.unicode < 128)
				{
					if (playerName.size() < 15)
						playerName += static_cast<char>(event.key.code);
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Enter)
					return playerName;
				if (event.key.code == sf::Keyboard::Escape)
				{
					return "";
					window.close();
				}
			}

		}

		nameTag.setString(playerName);
		nameTag.setPosition(sf::Vector2f(window.getSize().x / 2 - 150 - 10 * playerName.size(), window.getSize().y / 2 - 100));

		window.clear();

		window.draw(enterTag);
		window.draw(nameTag);

		window.display();
	}
}

void saveLeaderBoard(void)
{
	std::ofstream fout;

	if (modeChoice == 1)
		fout.open("simpleLeaderBoard.txt");
	else if (modeChoice == 2)
		fout.open("endlessLeaderBoard.txt");

	for (int i = 0; i < 3; i++)
	{
		fout << leaderBoardNames[i] << std::endl << leaderBoardScores[i] << std::endl;
	}

	fout.close();
}

void loadLeaderBoard(sf::RenderWindow& window)
{
	std::ifstream fin;
	if (modeChoice == 1)
	{
		fin.open("simpleLeaderBoard.txt");
	}
	else if (modeChoice == 2)
	{
		fin.open("endlessLeaderBoard.txt");
	}

	if (fin)
	{
		for (int i = 0; i < 3; i++)
		{
			fin >> leaderBoardNames[i] >> leaderBoardScores[i];
			if (leaderBoardNames[i] == "")
				leaderBoardNames[i] = "_";
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			leaderBoardNames[i] = "_";
			leaderBoardScores[i] = 0;
		}
	}


	leaderBoardScores[3] = score + lvl * lvlChangeThreshold;

	sortLeaderBoard();

	if ((score + lvl * lvlChangeThreshold) > 0)
	{
		int pos = findPlayerScore();

		if (pos < 3)
		{
			if (window.isOpen())
				leaderBoardNames[pos] = inputNameScreen(window);
		}
	}

	fin.close();

	saveLeaderBoard();

	fin.close();
}

void gameOverWindow(sf::RenderWindow& window)
{
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("bg.jpg"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite backgroundSprite(backgroundTexture);

	float xdir = window.getSize().x / backgroundSprite.getGlobalBounds().width;
	float ydir = window.getSize().y / backgroundSprite.getGlobalBounds().height;
	backgroundSprite.setScale(xdir, ydir);
	window.setFramerateLimit(0);

	FPS = 17;

	for (int i = 0; i < NUM_CELLS_X; i++)
	{
		for (int j = 0; j < NUM_CELLS_Y; j++)
		{
			obstacleGrid[i][j] = false;
		}
	}

	fruitAmount = 1;

	gameover = false;

	if (modeChoice != 3)
		loadLeaderBoard(window);

	snakelength = 1;

	snakeX[0] = NUM_CELLS_X / 2;
	snakeY[0] = NUM_CELLS_Y / 2;

	dir = 0;

	sf::Font font;

	if (!font.loadFromFile("PressStart2P-Regular.ttf"))
	{
		std::cout << "unable to laod the font!!!!" << std::endl;
	}

	sf::Text gameoverText;
	gameoverText.setFont(font);
	gameoverText.setCharacterSize(44);
	gameoverText.setPosition(sf::Vector2f(window.getSize().x / 2 - 175, window.getSize().y / 2 - 500));
	gameoverText.setFillColor(sf::Color::Red);
	gameoverText.setString("Gameover");

	sf::Text scoreDisplay;
	scoreDisplay.setFont(font);
	scoreDisplay.setCharacterSize(40);
	scoreDisplay.setPosition(sf::Vector2f(window.getSize().x / 2 - 155, window.getSize().y / 2 + 200));
	scoreDisplay.setFillColor(sf::Color::White);
	scoreDisplay.setString("Score: " + std::to_string(score + lvl * lvlChangeThreshold));

	sf::Text enterKeyIntimidation;
	enterKeyIntimidation.setFont(font);
	enterKeyIntimidation.setCharacterSize(35);
	enterKeyIntimidation.setPosition(sf::Vector2f(window.getSize().x / 2 - 450, window.getSize().y / 2 + 350));
	enterKeyIntimidation.setFillColor(sf::Color::Green);
	enterKeyIntimidation.setString("Press SpaceBar to continue");

	sf::Text leaderBoardTag;
	leaderBoardTag.setFont(font);
	leaderBoardTag.setCharacterSize(45);
	leaderBoardTag.setFillColor(sf::Color::White);
	leaderBoardTag.setPosition(sf::Vector2f(window.getSize().x / 2 - 250, window.getSize().y / 2 - 350));
	leaderBoardTag.setString("LEADERBOARD");


	sf::Text leaderBoardNameTag;
	leaderBoardNameTag.setFont(font);
	leaderBoardNameTag.setCharacterSize(45);
	leaderBoardNameTag.setFillColor(sf::Color::White);

	sf::Text leaderBoardScoreTag;
	leaderBoardScoreTag.setFont(font);
	leaderBoardScoreTag.setCharacterSize(45);
	leaderBoardScoreTag.setFillColor(sf::Color::White);

	if (modeChoice == 3)
	{
		gameoverText.setPosition(window.getSize().x / 2 - 175, window.getSize().y / 2 - 300);
		scoreDisplay.setPosition(window.getSize().x / 2 - 155, window.getSize().y / 2 - 100);
		enterKeyIntimidation.setPosition(window.getSize().x / 2 - 450, window.getSize().y / 2 + 100);
	}

	score = 0;
	lvl = 0;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					modeChoice = 1;
					return;
				}
				if (event.key.code == sf::Keyboard::Q) window.close();
			}
		}


		window.clear();

		window.draw(backgroundSprite);


		window.draw(gameoverText);

		if (modeChoice != 3)
		{
			window.draw(leaderBoardTag);

			for (int i = 0; i < 3; i++)
			{
				leaderBoardNameTag.setString(leaderBoardNames[i]);
				leaderBoardScoreTag.setString(std::to_string(leaderBoardScores[i]));

				leaderBoardNameTag.setPosition(sf::Vector2f(window.getSize().x / 2 - 400, window.getSize().y / 2 + i * 100 - 200));
				leaderBoardScoreTag.setPosition(sf::Vector2f(window.getSize().x / 2 + 350, window.getSize().y / 2 + i * 100 - 200));

				window.draw(leaderBoardNameTag);
				window.draw(leaderBoardScoreTag);
			}
		}

		window.draw(scoreDisplay);
		window.draw(enterKeyIntimidation);

		window.display();

	}

}

void powerUpMenu(sf::RenderWindow& window)
{
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("bg.jpg"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite backgroundSprite(backgroundTexture);

	float xdir = window.getSize().x / backgroundSprite.getGlobalBounds().width;
	float ydir = window.getSize().y / backgroundSprite.getGlobalBounds().height;
	backgroundSprite.setScale(xdir, ydir);

	sf::Texture powerUpTexture;
	if (!powerUpTexture.loadFromFile("powerup.png"))
		std::cout << "Error loading background image" << std::endl;

	sf::Sprite powerUpSprite(powerUpTexture);

	float xdir1 = 8;
	float ydir1 = 8;
	powerUpSprite.setScale(xdir1, ydir1);
	powerUpSprite.setPosition(850.0f, 375.0f);

	sf::Font font;
	font.loadFromFile("PressStart2P-Regular.ttf");


	sf::Text mainTag;
	mainTag.setFont(font);
	mainTag.setCharacterSize(70);
	mainTag.setFillColor(sf::Color(255, 241, 64));
	mainTag.setPosition(WIDTH / 2 - 270, 40);
	mainTag.setString("Power Up");

	sf::Text powerTag;
	powerTag.setFont(font);
	powerTag.setCharacterSize(45);
	powerTag.setFillColor(sf::Color(255, 241, 64));
	powerTag.setString("Look out for this power up!");
	powerTag.setPosition(350, 200);


	sf::Text powerupTag;
	powerupTag.setFont(font);
	powerupTag.setCharacterSize(40);
	powerupTag.setFillColor(sf::Color(255, 241, 64));
	powerupTag.setString("It reduces your snakes body to just 1 making\n    it easier to gain more points!");
	powerupTag.setPosition(75, 750);

	sf::Text buttonTag;
	buttonTag.setFont(font);
	buttonTag.setCharacterSize(60);
	buttonTag.setFillColor(sf::Color(255, 241, 64));
	buttonTag.setString("Press Enter to continue");
	buttonTag.setPosition(WIDTH / 8, window.getSize().y / 2 + 400);
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Q) window.close();
				if (event.key.code == sf::Keyboard::Enter) return;
			}
		}
		window.clear();

		window.draw(backgroundSprite);
		window.draw(powerUpSprite);

		window.draw(mainTag);
		window.draw(powerTag);
		window.draw(powerupTag);
		window.draw(buttonTag);


		window.display();

	}

}