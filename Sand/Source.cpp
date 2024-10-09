#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <SFML/Graphics.hpp>
#include "pallette.hpp"
#include "Sandbox.h"


/*
Title: SFML Sand Drop
Created: 26 SEP 2024
Updated: 9 OCT 2024
Author: Sam Daly

Description:
	This is a simple(ish) program that lets you drop sand particles into a box. Each particle follows two main rules:
		1. Vertical Gravity:
			If there is no particle below, and the lower bound has not been reached, then move down.

		2. Diagonal Gravity:
			If there is a particle below, but no particle diagonally below, and at least one diagonal is within bounds
			then slide diagonally down. If both diagonals are open, then choose at random.

	The other portion of this program is the color picker. The color picker allow the user to choose any color represented by the
	HSV color system to apply to the sand particles. The two sections of this tool are:
		1. Hue Slider:
			The Hue slider is comprised of 6 colors, red, yellow, green, cyan, blue, and magenta. The X coordinate of the slider
			is converted to the Hue value, which is an angle from 0-360 degrees

		2. The value and saturation box.
			The Value and Saturation box displays a gradient of black on the bottom and white and the current hue at the top.
			The selected X coordinate is converted to the saturation value
			The selected Y coordinate is converted to the value value

		The HSV value determined by these controls is based on a cylindrical coordinate system where hue is the angle around the
		y axis of the cylinder, saturation is the horizontal distance away from the center of the cylinder (ie a fraction of the radius), and 
		value is the vertical distance from the bottom of the cylinder (ie a fraction of the height). This HSV value is then converted to
		an RGB value, which is then applied to the sand particles.

*/


/*
SFML Sand Drop
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




//############################################	MAIN	#########################################################
int main() {
	//Vars
	sf::RenderWindow win(sf::VideoMode(800,500), "Sand");
	sf::Event e;
	sf::Clock frameClock;
	float updateThreshold = 0.0;
	float updateTimer = 0.0;
	float dt = 0.0;

	int grainSize = 10;
	sf::Color activeColor = sf::Color::Red;
	int padding = 20;
	Pallette pallette;
	pallette.setPos({10,50});
	sf::RectangleShape palleteOutput;
	palleteOutput.setSize({50,50});
	sf::Vector2f pos;
	pos.x = ((pallette.getBounds().width + pallette.getBounds().left) / 2) - (palleteOutput.getSize().x / 2);
	pos.y = pallette.getBounds().height + pallette.getBounds().top + padding;
	palleteOutput.setPosition(pos);
	palleteOutput.setFillColor(sf::Color::Red);

	
	Sandbox box;
	box.SetBorderDrawn(1);
	box.SetCellSize(grainSize);
	box.SetDims((win.getSize().x - (pallette.getBounds().width + padding - 2)) / grainSize, (win.getSize().y / grainSize) + 1);
	box.SetPos(pallette.getBounds().width + padding + 2, 1);

	//## Init
	srand(time(0));

	//## Window Loop
	while (win.isOpen()) {
		//Update Delta Time
		dt = frameClock.getElapsedTime().asSeconds();
		frameClock.restart();

		//Event poll
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}

			if (e.type == sf::Event::MouseWheelScrolled) {
				box.AddBrushSize(e.mouseWheelScroll.delta);
			}

			if (e.type == sf::Event::KeyReleased) {
				if (e.key.code == sf::Keyboard::R) {
					box.SetDims(box.GetDims().x, box.GetDims().y);
				}
			}

		}

		//Non Polling Updates
		updateTimer += dt;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && pallette.Sample(sf::Mouse::getPosition(win).x, sf::Mouse::getPosition(win).y)) {
			activeColor = pallette.getCanvasSelect();
			palleteOutput.setFillColor(pallette.getCanvasSelect());
		}
		pallette.Update(dt);
		
		if (updateTimer >= updateThreshold) {
			box.SetBrushColor(activeColor);
			box.Update(win);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				sf::Vector2i clickPos = sf::Mouse::getPosition(win);
				box.AddOnClick(clickPos.x, clickPos.y);
			}
			
			updateTimer = 0;
		}

		//Draw
		win.clear();
		box.Draw(win);
		pallette.Draw(win);
		win.draw(palleteOutput);
		win.display();
	}

	return 0;
}