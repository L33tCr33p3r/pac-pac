#include<iostream>
#include "SFML/Graphics.hpp"
using namespace std;

enum DIRECTIONS { LEFT, RIGHT, UP, DOWN }; //left is 0, right is 1, up is 2, down is 3

int main() {
	//game set up (you'll need these lines in every game)
	sf::RenderWindow screen(sf::VideoMode(1280, 720), "Pacman"); //set up screen
	sf::Event event; //set up event queue
	sf::Clock clock; //set up the clock (needed for game timing)
	const float FPS = 60.0f; //FPS
	screen.setFramerateLimit(FPS); //set FPS

	//load in images
	sf::Texture brick;
	brick.loadFromFile("brick.jpg");
	sf::Sprite wall;
	wall.setTexture(brick);

	int map[18][32] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
		1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
		1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,1,1,
		2,2,2,0,1,0,1,0,0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,0,0,1,0,1,0,2,2,2,
		1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,1,0,1,0,1,0,1,0,1,1,1,
		1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,1,
		1,0,1,1,1,1,1,0,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,1,0,1,
		1,0,1,1,1,1,1,0,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,1,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,1,
		1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,1,0,1,0,1,0,1,0,1,1,1,
		2,2,2,0,1,0,1,0,0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,0,0,1,0,1,0,2,2,2,
		1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,1,1,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
		1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	//player setup
	int xpos = 40;
	int ypos = 40;
	int vx = 0;
	int vy = 0;
	int radius = 20;
	sf::CircleShape player(radius);
	player.setFillColor(sf::Color(250, 250, 0)); //using RGB value for color here (hex also works)
	player.setPosition(xpos, ypos); //top left "corner" of circle (not center!)
	bool keys[] = { false, false, false, false };

	sf::CircleShape dot(4);
	dot.setFillColor(sf::Color(250, 250, 250));

	//################### HOLD ONTO YOUR BUTTS, ITS THE GAME LOOP###############################################################
	while (screen.isOpen()) {//keep window open until user shuts it down

		while (screen.pollEvent(event)) { //look for events-----------------------

			//this checks if the user has clicked the little "x" button in the top right corner
			if (event.type == sf::Event::EventType::Closed) {
				screen.close();
			}
			//KEYBOARD INPUT 
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				keys[LEFT] = true;
			}
			else keys[LEFT] = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				keys[RIGHT] = true;
			}
			else keys[RIGHT] = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				keys[UP] = true;
			}
			else keys[UP] = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				keys[DOWN] = true;
			}
			else keys[DOWN] = false;
		}//end event loop---------------------------------------------------------------

		//use the Keyboard Input
		if (keys[LEFT] == true) vx = -3;
		else if (keys[RIGHT] == true) vx = 3;
		//else vx = 0;

		if (keys[UP] == true) vy = -3;
		else if (keys[DOWN] == true) vy = 3;
		//else vy = 0;

		//Collision
		if ((vx > 0 && map[(ypos + 3) / 40][(xpos + (radius * 2)) / 40] == 1) || (vx > 0 && map[(ypos + (radius * 2) - 3) / 40][(xpos + (radius * 2)) / 40] == 1)) //RIGHT
			vx = 0;
		if ((vx < 0 && map[(ypos + 3) / 40][(xpos - 3) / 40] == 1) || (vx < 0 && map[(ypos + (radius * 2) - 3) / 40][(xpos - 3) / 40] == 1)) //LEFT
			vx = 0;
		if ((vy < 0 && map[(ypos - 3) / 40][(xpos + 3) / 40] == 1) || (vy < 0 && map[(ypos - 3) / 40][(xpos + (radius * 2) - 3) / 40] == 1)) //UP
			vy = 0;
		if ((vy > 0 && map[(ypos + (radius * 2)) / 40][(xpos + 3) / 40] == 1) || (vy > 0 && map[(ypos + (radius * 2)) / 40][(xpos + (radius * 2) - 3) / 40] == 1)) //DOWN
			vy = 0;

		//move Mr. Pac
		xpos += vx;
		ypos += vy;
		player.setPosition(xpos, ypos);

		if (map[(ypos + radius) / 40][(xpos + radius) / 40] == 0)
			map[(ypos + radius) / 40][(xpos + radius) / 40] = 2;

		//render section-----------------------------------------
		screen.clear(); //wipes screen, without this things smear

		//draw map
		for (int rows = 0; rows < 32; rows++) {
			for (int cols = 0; cols < 18; cols++) {
				if (map[cols][rows] == 0) {
					dot.setPosition(rows * 40 + 18, cols * 40 + 18);
					screen.draw(dot);
				}
				if (map[cols][rows] == 1) {
					wall.setPosition(rows * 40, cols * 40);
					screen.draw(wall);
				}
			}
		}
		screen.draw(player); //draw player
		screen.display(); //flips memory drawings onto screen

	}//######################## end game loop ###################################################################################

	cout << "goodbye!" << endl;
} //end of main