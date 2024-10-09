#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

struct cell {
	int val = -1;
	sf::Color col = sf::Color::Transparent;
};

//############################################	SAND BOX	#########################################################
class Sandbox {
private:
	//## Data
	//General
	int width = 2;
	int height = 2;
	int cellSize = 1;
	sf::Vector2f pos = { 0,0 };
	sf::Vector2i cursorPos = { 0,0 };
	std::vector<std::vector<cell>> currState;
	std::vector<std::vector<cell>> nextState;
	bool isBorderDrawn = 0;

	//Brush
	int brushSize = 1;
	int maxBrush = 10;
	int brushIncrement = 1;
	int brushOpacity = 80;
	bool isBrushShadowDrawn = 1;
	sf::Color brushColor = sf::Color::Red;

	

	//## Util Functions
	sf::Vector2i ClickToCell(int x, int y);

public:
	//## Constructor and Destructor
	Sandbox();
	~Sandbox();

	//## Primary Functions
	void AddGrain(int x, int y, int val, sf::Color color);
	void AddOnClick(int x, int y);
	void AddBrushSize(float amt);

	void Update(sf::RenderWindow& win);
	void Draw(sf::RenderWindow& win);

	

	//## Getters and Setters
	//# Cell Size
	void SetCellSize(int newSize) {
		cellSize = newSize;
	}

	//# Dimensions
	void SetDims(int x, int y) {
		width = x;
		height = y;

		//Create empty array
		std::vector<std::vector<cell>> empty;

		for (int i = 0; i < height; i++) {
			std::vector<cell> temp;
			for (int j = 0; j < width; j++) {
				cell t;
				temp.push_back(t);
			}
			empty.push_back(temp);
		}

		//Replace current array
		currState = empty;
		nextState = empty;

	}

	sf::Vector2i GetDims() {
		sf::Vector2i dims = { width, height };
		return dims;
	}

	//# Position
	void SetPos(int x, int y) {
		pos.x = x;
		pos.y = y;
	}

	void SetBorderDrawn(bool state) {
		isBorderDrawn = state;
	}

	void SetBrushColor(sf::Color newColor);
};


