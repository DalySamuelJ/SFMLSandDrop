#include "Sandbox.h"


sf::Vector2i Sandbox::ClickToCell(int x, int y)
{
	sf::Vector2i cell = { -1,-1 };
	sf::IntRect bounds = { (int)pos.x, (int)pos.y, width * cellSize, height * cellSize };

	//# If the click is in bounds
	if (bounds.contains(x, y)) {
		//# Then Get the cell
		//Correct for position
		x -= pos.x;
		y -= pos.y;

		//Divide by cell size
		cell.x = floor(x / cellSize);
		cell.y = floor(y / cellSize);

		//std::cout << "Clicked Cell: " << cell.x << ", " << cell.y << std::endl;
	}


	return cell;
}

Sandbox::Sandbox()
{
	this->SetDims(width, height);
}

Sandbox::~Sandbox()
{
}

void Sandbox::AddGrain(int x, int y, int val, sf::Color color)
{
	//Adds a grain at the specified cell

		//Gaurd against out of bounds
	if (y >= height || x >= width) {
		std::cout << "Cannot add grain, invalid cell" << std::endl;
		return;
	}

	currState[y][x].val = val;
	currState[y][x].col = color;
}

void Sandbox::AddOnClick(int x, int y)
{
	sf::Vector2i targetCell = ClickToCell(x, y);
	if (targetCell.x >= 0 && targetCell.y >= 0) {
		for (int i = targetCell.y - brushSize; i < targetCell.y + brushSize; i++) {
			for (int j = targetCell.x - brushSize; j < targetCell.x + brushSize; j++) {
				if (i >= 0 && i < height && j >= 0 && j < width && rand() % 2 == 0) {
					this->AddGrain(j, i, 1, brushColor);
				}
			}
		}
	}
}

void Sandbox::AddBrushSize(float amt)
{
	brushSize = floor(amt + brushSize);

	if (brushSize > maxBrush) {
		brushSize = maxBrush;
	}

	if (brushSize < 1) {
		brushSize = 1;
	}
}

void Sandbox::Update(sf::RenderWindow& win)
{
	cursorPos = sf::Mouse::getPosition(win);
	sf::IntRect bounds = { (int)pos.x, (int)pos.y, width * cellSize, height * cellSize };
	if (bounds.contains(cursorPos.x, cursorPos.y)) {
		isBrushShadowDrawn = true;
	}
	else {
		isBrushShadowDrawn = false;
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//Drop if empty below
			if (currState[i][j].val >= 0) {
				//If there is a grain in this spot then apply rules

				//Rule #1: Gravity Drop
				if (i + 1 < height && currState[i + 1][j].val < 0) {
					nextState[i + 1][j] = currState[i][j];
					nextState[i][j].val = -1;
				}

				//Rule #2: Gravity Slide
				if (i + 1 < height && currState[i + 1][j].val >= 0) {
					//Pick random and try, slide the other way if already full
					int r = -1;
					if (rand() % 2 == 0) {
						r = 1;
					}
					if (j + r < width && j + r >= 0 && currState[i + 1][j + r].val < 0) {
						nextState[i + 1][j + r] = currState[i][j];
						nextState[i][j].val = -1;
					}
				}

				//Rule #3: None
			}
		}
	}

	currState = nextState;
}

void Sandbox::Draw(sf::RenderWindow& win)
{
	//# Border
	if (isBorderDrawn) {
		sf::RectangleShape border;
		border.setSize({(float)cellSize * width + 1,(float)cellSize * height + 1 });
		border.setPosition(pos.x - 1, pos.y - 1);
		border.setFillColor(sf::Color::Transparent);
		border.setOutlineColor(sf::Color::Green);
		border.setOutlineThickness(1);
		win.draw(border);
	}

	//# Cells
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (currState[i][j].val >= 0) {
				sf::RectangleShape cell;
				cell.setSize({ (float)cellSize,(float)cellSize });
				cell.setPosition(pos.x + (cellSize * j), pos.y + (cellSize * i));
				cell.setFillColor(currState[i][j].col);
				win.draw(cell);
			}
		}
	}

	//# Brush Shadow
	if (isBrushShadowDrawn) {
		sf::CircleShape shadow(brushSize * cellSize);
		shadow.setFillColor(sf::Color(255, 255, 255, brushOpacity));
		shadow.setPosition({ (float)(cursorPos.x - shadow.getRadius()),(float)(cursorPos.y - shadow.getRadius()) });
		win.draw(shadow);
	}
}

void Sandbox::SetBrushColor(sf::Color newColor)
{
	brushColor = newColor;
}


