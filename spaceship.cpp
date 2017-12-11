//Kathleen Liebscher and Logan Yokum
//Section 1
//Implementation for Spaceship of lunar landing
#define _USE_MATH_DEFINES
#define deltat 0.3
#include <iostream>
#include "spaceship.h"
#include "gfx.h"
#include <cmath>
#include <array>
#include <vector>
#include <stdlib.h>
#include <time.h>

Spaceship :: Spaceship()
{
  // set color
  shipColor[0] = 2;
  shipColor[1] = 244;
  shipColor[2] = 236;

  flameColor[0] = 237;
  flameColor[1] = 84;
  flameColor[2] = 18;

  // set random start speed
  srand(time(0));
  xVel = (rand() % (maxStartVel - minStartVel)) + minStartVel;
}

Spaceship :: ~Spaceship()
{
}

bool Spaceship :: CheckCollision(vector<array<int, 2>> points)
{
  array<int, 2> point1;
  array<int, 2> point2;
  bool isCollided = false;

  // cycle through array
  for(int i = 0; i < points.size()-1; i++)
  {
    point1 = points[i];
    point2 = points[i+1];

    // slope
    double slope = double(point2[1] - point1[1]) / (point2[0] - point1[0]);

    double check_x_left = xPos - radius / sqrt(2);
    double check_x_right = xPos + radius / sqrt(2);

    double check_y_left = (slope * (check_x_left - point1[0])) + point1[1];
    double check_y_right = (slope * (check_x_right - point1[0])) + point1[1];

    
    // check if within bounds on left point
    if(point1[0] < check_x_left && check_x_left < point2[0])
    {
      // check for collision
      if(yPos + (radius / sqrt(2)) > check_y_left) isCollided = true;
    }
    
    // check if within bounds on right point
    if(point1[0] < check_x_right && check_x_right < point2[0])
    {
      if(yPos + (radius / sqrt(2)) > check_y_right) isCollided = true;
    }

    if(isCollided)
    {
      // check if crashed or landed
      double speed = sqrt(pow(xVel, 2) + pow(yVel, 2));
      if(floor(speed) > maxSafeSpeed || abs(slope) > 0.001)
      {
        isSafe = false;
      }

      isLanded = true;     
      return true;
    }
  }

  return false;
}

bool Spaceship :: GetSafe() { return isSafe;}

void Spaceship :: Thrust(direction myDirection)
{
  // fire in direction if has fuel
  if(fuel > 0)
  {
    isFiring = true;

    switch(myDirection)
    {
      case north:
      {
        yAcc = -rocketAcc;
        flameDirection = south;

        break;
      }

      case south:
      {
        yAcc = rocketAcc;
        flameDirection = north;

        break;
      }

      case west:
      {
        xAcc = -rocketAcc;
        flameDirection = east;

        break;
      }

      case east:
      {
        xAcc = rocketAcc;
        flameDirection = west;

        break;
      }
    }
  }
}

void Spaceship :: Update(int windowWidth, int windowHeight)
{
  // use acceleration to calculate new velocity
  xVel += xAcc * deltat;
  yVel += yAcc * deltat;
  yVel += gravityAcc * deltat;

  // use up fuel
  if(isFiring) 
  {
    if(fuel + fuel_change > 0) fuel += fuel_change * deltat;
    else fuel = 0;
  }

  // reset accelerations to zero
  xAcc = 0;
  yAcc = 0;

  // keep ship within bounds of screen
  if(xPos < 0)
  {
    xVel = 0;
    xPos = 2; 
  }

  if(xPos > windowWidth)
  {
    xVel = 0;
    xPos = windowWidth - 2;
  }

  if(yPos < 0)
  {
    yVel = 0;
    yPos = 2;
  }

  if(yPos > windowHeight)
  {
    yVel = 0;
    yPos = windowHeight - 2;
  }

  // stop ship movement if landed
  if(isLanded)
  {
    xVel = 0;
    yVel = 0;
  }

  // use velocity to calculate new positions
  xPos += xVel * deltat;
  yPos += yVel * deltat;

  // update coordinates
  double offset = -M_PI_4;
  double theta = -(2 * M_PI) / 4;
  double x1, y1;

  for(int i = 0; i < 4; i++)
  {
    // establish coordinates
    x1 = xPos + radius * cos((i * theta) + offset);
    y1 = yPos + radius * sin((i * theta) + offset);

    // add coordinates to arrow
    shipCoords[i][0] = x1;
    shipCoords[i][1] = y1;
  }

  // draw ship
  DrawShip(windowWidth, windowHeight);
}

void Spaceship :: DrawShip(int windowWidth, int windowHeight)
{
  double x1, y1, x2, y2;
  
  // draw square
  for(int i = 0; i < 3; i++)
  {
    // establish coordinates
    x1 = shipCoords[i][0];
    y1 = shipCoords[i][1];
    x2 = shipCoords[i+1][0];
    y2 = shipCoords[i+1][1];

    gfx_color(shipColor[0], shipColor[1], shipColor[2]);
    gfx_line(x1, y1, x2, y2);
  }

  // create last line
  x1 = shipCoords[3][0];
  y1 = shipCoords[3][1];
  x2 = shipCoords[0][0];
  y2 = shipCoords[0][1];

  gfx_color(shipColor[0], shipColor[1], shipColor[2]);
  gfx_line(x1, y1, x2, y2);

  // draw thruster
  if(isFiring)
  {
    double theta = -(2 * M_PI) / 3;
    double length = shipCoords[1][0] - shipCoords[0][0];
    array<int, 2> point1, point2;

    gfx_color(flameColor[0], flameColor[1], flameColor[2]); 

    switch(flameDirection)
    {
      case north:
      {
        
        break;
      }
      
      case south:
      {
        theta = -M_PI / 3;

        // get points of flame
        point1 = shipCoords[2]; 
        point2 = shipCoords[3]; 

        // get middle point
        x2 = point2[0] + length * cos(theta);
        y2 = point2[1] + length * sin(theta);

        // draw lines
        gfx_line(point1[0], point1[1], x2, y2);
        gfx_line(point2[0], point2[1], x2, y2);
        
        break;
      }

      case east:
      {
        theta = -5 * M_PI / 6;

        // get points of flame
        point1 = shipCoords[0]; 
        point2 = shipCoords[3]; 

        // get middle point
        x2 = point1[0] + length * cos(theta);
        y2 = point1[1] + length * sin(theta);

        // draw lines
        gfx_line(point1[0], point1[1], x2, y2);
        gfx_line(point2[0], point2[1], x2, y2);
        
        break;
      }

      case west:
      {
        theta = -M_PI / 6;

        // get points of flame
        point1 = shipCoords[1]; 
        point2 = shipCoords[2]; 

        // get middle point
        x2 = point1[0] + length * cos(theta);
        y2 = point1[1] + length * sin(theta);

        // draw lines
        gfx_line(point1[0], point1[1], x2, y2);
        gfx_line(point2[0], point2[1], x2, y2);
        
        break;
      }
    }

    isFiring = false;
  }

  // display statistics
  double speed = sqrt(pow(xVel, 2) + pow(yVel, 2));
  string fuel_str = "Fuel: " + to_string(int(fuel));
  string speed_str = "Speed: " + to_string(int(speed));
  string max_speed_str = "Max Safe Speed: " + to_string(int(maxSafeSpeed));
  int width, height;
  int char_width = 6, char_height = 9;
  int w_padding = 6;

  gfx_color(188, 188, 188);

  // display fuel
  width = windowWidth - (fuel_str.size() * char_width) - w_padding;
  height = char_height * 2;
  gfx_text(width, height, fuel_str.c_str());

  // display speed
  width = windowWidth - (speed_str.size() * char_width) - w_padding;
  height += char_height * 2;
  gfx_text(width, height, speed_str.c_str());

  // display safe speed
  width = windowWidth - (max_speed_str.size() * char_width) - w_padding;
  height += char_height * 2;
  gfx_text(width, height, max_speed_str.c_str());

}
