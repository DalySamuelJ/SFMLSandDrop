#include <iostream>
#include <SFML/Graphics.hpp>

/*
Title: SFML Based Color Picker
Created: 26 SEP 2024
Updated: 26 SEP 2024
Author: Sam Daly

Description:

*/

/*
SFML Color Picker
	Copyright (C) 2024  Samuel Daly

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/




//############################	UTILITY	###########################################################################
float dist(sf::Vector2f a, sf::Vector2f b) {
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

sf::Color HSVtoRGB(sf::Vector3f in) {
	float s = in.y / 100;
	float v = in.z / 100;
	float C = s * v;
	float X = C * (1 - fabs(fmod(in.x / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;

	if (in.x >= 0 && in.x < 60) {
		r = C, g = X, b = 0;
	}
	else if (in.x >= 60 && in.x < 120) {
		r = X, g = C, b = 0;
	}
	else if (in.x >= 120 && in.x < 180) {
		r = 0, g = C, b = X;
	}
	else if (in.x >= 180 && in.x < 240) {
		r = 0, g = X, b = C;
	}
	else if (in.x >= 240 && in.x < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}

	int R = (r + m) * 255;
	int G = (g + m) * 255;
	int B = (b + m) * 255;

	return sf::Color(R, G, B, 255);
}


//############################	PALLETTE	###########################################################################
class Pallette {
private:
	//##### Data
	//General
	sf::FloatRect bounds = { 0,0,1,1 };
	float diagonal = 1;
	sf::Vector3f hsv = { 0,0,0 };

	//Canvas
	sf::Vertex canvas[4];
	sf::Vector2i canvasSize = { 200,300 };
	sf::Vector2f canvasPos = { 20,20 };
	sf::Color canvas_Select = sf::Color::White;
	sf::RectangleShape canvas_cursor;
	int canvas_cursorSize = 5;

	//Slider
	sf::Vertex slider[12];
	sf::Vector2i sliderSize = { 200,50 };
	sf::Vector2f sliderPos = { 20, 330 };
	sf::Color slider_Select = sf::Color::Red;
	float slider_Tick_Pos = 0;
	sf::Vertex slider_Tick[2];
	sf::Color slider_Gradient[6];
	float slider_tickSpeed = 50;

	//Layout
	int canvas_slider_padding = 10;

public:
	//##### Constructor and Destructor
	Pallette() {
		//Initialize

		//HSV
		hsv = { 0,100,100 }; //Red

		//Canvas Veticies
		canvas[0] = sf::Vertex(canvasPos, sf::Color::White, sf::Vector2f(100.f, 100.f));
		canvas[1] = sf::Vertex(sf::Vector2f(canvasPos.x + canvasSize.x, canvasPos.y), slider_Select, sf::Vector2f(100.f, 100.f));
		canvas[2] = sf::Vertex(sf::Vector2f(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), sf::Color::Black, sf::Vector2f(100.f, 100.f));
		canvas[3] = sf::Vertex(sf::Vector2f(canvasPos.x, canvasPos.y + canvasSize.y), sf::Color::Black, sf::Vector2f(100.f, 100.f));

		//Canvas Cursor
		canvas_cursor.setFillColor(sf::Color::Transparent);
		canvas_cursor.setOutlineColor(sf::Color::White);
		canvas_cursor.setOutlineThickness(2);
		canvas_cursor.setPosition({ canvasPos.x + canvasSize.x - canvas_cursorSize, canvasPos.y + canvas_cursorSize });
		canvas_cursor.setSize({ (float)canvas_cursorSize, (float)canvas_cursorSize });

		//Slider Color Gradient
		slider_Gradient[0] = sf::Color::Red;
		slider_Gradient[1] = sf::Color::Yellow;
		slider_Gradient[2] = sf::Color::Green;
		slider_Gradient[3] = sf::Color::Cyan;
		slider_Gradient[4] = sf::Color::Blue;
		slider_Gradient[5] = sf::Color::Magenta;

		//Slider Pos
		sliderPos.x = canvasPos.x;
		sliderPos.y = canvasPos.y + canvasSize.y + canvas_slider_padding;

		//Slider Verticies
		float w = sliderSize.x / 6;
		slider[0] = sf::Vertex(sf::Vector2f(sliderPos.x, sliderPos.y), slider_Gradient[0], sf::Vector2f(100, 100));
		slider[1] = sf::Vertex(sf::Vector2f(sliderPos.x, sliderPos.y + sliderSize.y), slider_Gradient[0], sf::Vector2f(100, 100));
		slider[2] = sf::Vertex(sf::Vector2f(sliderPos.x + (2 * w), sliderPos.y), slider_Gradient[1], sf::Vector2f(100, 100));
		slider[3] = sf::Vertex(sf::Vector2f(sliderPos.x + (2 * w), sliderPos.y + sliderSize.y), slider_Gradient[1], sf::Vector2f(100, 100));
		slider[4] = sf::Vertex(sf::Vector2f(sliderPos.x + (3 * w), sliderPos.y), slider_Gradient[2], sf::Vector2f(100, 100));
		slider[5] = sf::Vertex(sf::Vector2f(sliderPos.x + (3 * w), sliderPos.y + sliderSize.y), slider_Gradient[2], sf::Vector2f(100, 100));
		slider[6] = sf::Vertex(sf::Vector2f(sliderPos.x + (4 * w), sliderPos.y), slider_Gradient[3], sf::Vector2f(100, 100));
		slider[7] = sf::Vertex(sf::Vector2f(sliderPos.x + (4 * w), sliderPos.y + sliderSize.y), slider_Gradient[3], sf::Vector2f(100, 100));
		slider[8] = sf::Vertex(sf::Vector2f(sliderPos.x + (5 * w), sliderPos.y), slider_Gradient[4], sf::Vector2f(100, 100));
		slider[9] = sf::Vertex(sf::Vector2f(sliderPos.x + (5 * w), sliderPos.y + sliderSize.y), slider_Gradient[4], sf::Vector2f(100, 100));
		slider[10] = sf::Vertex(sf::Vector2f(sliderPos.x + sliderSize.x, sliderPos.y), slider_Gradient[5], sf::Vector2f(100, 100));
		slider[11] = sf::Vertex(sf::Vector2f(sliderPos.x + sliderSize.x, sliderPos.y + sliderSize.y), slider_Gradient[5], sf::Vector2f(100, 100));

		//Slider Tick Vertices
		slider_Tick[0] = sf::Vertex(sf::Vector2f(sliderPos.x + slider_Tick_Pos, sliderPos.y), sf::Color::White, sf::Vector2f(100, 100));
		slider_Tick[1] = sf::Vertex(sf::Vector2f(sliderPos.x + slider_Tick_Pos, sliderPos.y + sliderSize.y), sf::Color::White, sf::Vector2f(100, 100));

		//Pallette Bounds
		bounds.left = canvasPos.x;
		bounds.top = canvasPos.y;
		bounds.width = canvasSize.x;
		bounds.height = canvasSize.y + sliderSize.y + canvas_slider_padding;

		//Diagonal
		diagonal = sqrt(pow(canvasSize.y, 2) + pow(bounds.width, 2));
	}

	~Pallette() {}

	//##### Primary Fuctions

	//Sample
	int Sample(int mouseX, int mouseY) {

		sf::Vector2f mousePos = { (float)mouseX, (float)mouseY };

		//Gaurd for click outside pallette bounds
		if (!bounds.contains(mousePos)) {
			return -1;
		}



		//Determine whether click is in canvas or slider (if else to exclude padding area)
		if (mousePos.y <= canvas[3].position.y) {
			//Then its a click in the canvas
			canvas_cursor.setPosition(mousePos);
			UpdateCanvas();
		}
		else if (mousePos.y >= slider[0].position.y) {
			//Then its a click in the slider
			slider_Tick_Pos = mousePos.x;

			UpdateSlider();

		}

		return 1;
	}

	//Update Canvas
	void UpdateCanvas() {
		//Saturation (x)
		hsv.y = 100 * (canvas_cursor.getPosition().x / (canvasPos.x + canvasSize.x));

		//Value (y)
		hsv.z = 100 - (100 * ((canvas_cursor.getPosition().y / (canvasPos.y + canvasSize.y))));

		canvas_Select = HSVtoRGB(hsv);
	}

	//Update Slider (BUG: left clip 32deg)
	void UpdateSlider() {
		slider_Tick[0].position.x = slider_Tick_Pos;
		slider_Tick[1].position.x = slider_Tick_Pos;

		//Hue = 360 * tickVal / sliderWidth
		hsv.x = 360 * (slider_Tick_Pos / slider[11].position.x);

		int normalized = int((slider_Tick_Pos / (sliderSize.x + sliderPos.x)) * 256 * 6);
		int region = normalized / 256;
		int x = normalized % 256;
		switch (region)
		{
		case 0: slider_Select.r = 255; slider_Select.g = 0;   slider_Select.b = 0;   slider_Select.g += x; break;
		case 1: slider_Select.r = 255; slider_Select.g = 255; slider_Select.b = 0;   slider_Select.r -= x; break;
		case 2: slider_Select.r = 0;   slider_Select.g = 255; slider_Select.b = 0;   slider_Select.b += x; break;
		case 3: slider_Select.r = 0;   slider_Select.g = 255; slider_Select.b = 255; slider_Select.g -= x; break;
		case 4: slider_Select.r = 0;   slider_Select.g = 0;   slider_Select.b = 255; slider_Select.r += x; break;
		case 5: slider_Select.r = 255; slider_Select.g = 0;   slider_Select.b = 255; slider_Select.b -= x; break;
		default:
			break;
		}

		canvas[1].color = slider_Select;
	}

	//Update
	void Update(float dt) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			slider_Tick_Pos += slider_tickSpeed * dt;
			if (slider_Tick_Pos > sliderPos.x + sliderSize.x) {
				slider_Tick_Pos = sliderPos.x;
			}
			UpdateSlider();
			UpdateCanvas();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			slider_Tick_Pos -= slider_tickSpeed * dt;
			if (slider_Tick_Pos < sliderPos.x) {
				slider_Tick_Pos = sliderPos.x + sliderSize.x;
			}
			UpdateSlider();
			UpdateCanvas();
		}

	}

	//Draw
	void Draw(sf::RenderWindow& win) {
		//Canvas
		win.draw(&canvas[0], 4, sf::Quads);
		win.draw(canvas_cursor);

		win.draw(&slider[0], 12, sf::TrianglesStrip);

		win.draw(&slider_Tick[0], 2, sf::LinesStrip);
	}

	//##### Getters and Setters
	sf::Color getCanvasSelect() {
		return canvas_Select;
	}

	sf::FloatRect getBounds() {
		return bounds;
	}

	void setPos(sf::Vector2f newPos) {
		sf::Vector2f offset = newPos - canvasPos;
		canvasPos = newPos;
		UpdateCanvas();

		//Canvas Cursor
		canvas_cursor.move(offset);

		//Canvas Veticies
		canvas[0].position += offset;
		canvas[1].position += offset;
		canvas[2].position += offset;
		canvas[3].position += offset;

		//Slider Pos
		sliderPos += offset;

		//Slider Verticies
		for (int i = 0; i < 12; i++) {
			slider[i].position += offset;
		}

		//Slider Tick Vertices
		slider_Tick[0].position += offset;
		slider_Tick[1].position += offset;

		//Pallette Bounds
		bounds.left = canvasPos.x;
		bounds.top = canvasPos.y;
	}

};
