//Katie Liebscher and Logan Yokum
//Driver for Lunar Lander
//Section 1
#define deltat 0.3

#include <cmath>
#include <iostream>
#include "spaceship.h"
#include "gfx.h"
#include <stdlib.h>
#include <unistd.h>
#include <ctime>

using namespace std;

// prototypes
vector<array<int, 2>> CreateSurface(int, int);
void DrawSurface(vector<array<int, 2>>);

int main()
{
  int windowHeight = 700;
  int windowWidth = 1000;
  char input;
  bool isPlaying = true;
 
  gfx_open(windowWidth, windowHeight, "SSRamzi Maiden Voyage");

  // Title Screen
  int width, height;
  int char_width = 6, char_height = 9;

  string begin_str = "Land your mighty vessel, the SSRamzi, on the Moon!";
  string control_str = "Use arrow keys to fly.";
  string choice_str = "Press p to begin.";

  gfx_color(188, 188, 188);

  // display begin string
  width = windowWidth/2 - (begin_str.size()/2 * char_width);
  height = windowHeight/2;
  gfx_text(width, height, begin_str.c_str());

  // display control string
  width = windowWidth/2 - (control_str.size()/2 * char_width);
  height += char_height * 2;
  gfx_text(width, height, control_str.c_str());

  // display control string
  width = windowWidth/2 - (choice_str.size()/2 * char_width);
  height += char_height * 2;
  gfx_text(width, height, choice_str.c_str());

  char user_choice;
  bool isValid = false;

  // take in user input
  while(!isValid)
  {
    user_choice = gfx_wait();

    switch(user_choice)
    {
      case 'p':
      {
        isValid = true;
        break;
      }
    }
  }

  while(isPlaying)
  {
    // create points for background
    vector<array<int, 2>> points = CreateSurface(windowWidth, windowHeight);

    Spaceship SSRamzi;

    while(true)
    {
      // get user input
      if(gfx_event_waiting())
      {
        input = gfx_wait();

        switch(input)
        {
          // up arrow
          case 'R':
          {
            SSRamzi.Thrust(north);
            
            break;
          }

          // right arrow
          case 'S':
          {
            SSRamzi.Thrust(east);
            
            break;
          }

          // left arrow
          case 'Q':
          {
            SSRamzi.Thrust(west);
            
            break;
          }
        }
      }

      // update and draw screen
      gfx_clear();
      SSRamzi.Update(windowWidth, windowHeight);
      DrawSurface(points);

      // check for collision
      if(SSRamzi.CheckCollision(points)) 
      {
        break;
      }

      gfx_flush();
      usleep(deltat * 100000);
    }

    // End Screen
    string end_str = (SSRamzi.GetSafe() ? "You landed!!! Yaaay!" : "You crashed!!! Boooo!");
    choice_str = "Press q to quit or p to play again.";

    gfx_color(188, 188, 188);

    // display end string
    width = windowWidth/2 - (end_str.size()/2 * char_width);
    height = char_height * 2 * 6;
    gfx_text(width, height, end_str.c_str());

    // display choice string
    width = windowWidth/2 - (choice_str.size()/2 * char_width);
    height += char_height * 2;
    gfx_text(width, height, choice_str.c_str());

    isValid = false;

    // take in user input
    while(!isValid)
    {
      user_choice = gfx_wait();

      switch(user_choice)
      {
        case 'q':
        {
          isValid = true;
          isPlaying = false;

          break;
        }
        
        case 'p':
        {
          isValid = true;

          break;
        }
      }
    }
      
  }

  return 0;
}

vector<array<int, 2>> CreateSurface(int windowWidth, int windowHeight)
{
  vector<array<int, 2>> points;
  array<int, 2> point;

  int num_piece = 100;
  int dx = std:: ceil(windowWidth / num_piece);
  
  int padding = windowHeight / 3;
  int yMax = windowHeight - (padding / 2);
  int yMin = padding;
  int x = 0;
  int y;

  int land_size = 40 / dx; // number of dxs needed
  int num_land = 3;

  int range_land = windowWidth / num_land / dx;
  int max_land = range_land - land_size;

  int rand_min, rand_max;

  vector<int> landing_points;

  double rand_constant = time(0);

  for(int i = 0; i < num_land; i++)
  {
    // ranges for random x
    rand_min = i * range_land;
    rand_max = rand_min + max_land;

    srand(rand_constant);
    rand_constant++;
    landing_points.push_back((rand() % (rand_max - rand_min) + rand_min) * dx);
  }
  
  // create vector of points for surface
  int curr_landing = 0;
  
  for(int i = 0; i < num_piece; i++)
  {
    srand(rand_constant);
    rand_constant++;
    y = (rand() % (yMax - yMin)) + yMin;
    point[0] = x;
    point[1] = y;
    points.push_back(point);
    
    // add landing zone
    if(x == landing_points[curr_landing])
    {
      // add both points of landing zone
      point[0] = x + land_size * dx;
      points.push_back(point);
      
      curr_landing++;
      x += land_size * dx;
      i += land_size-1;
    }

    x += dx;
  }
   
  return points;
}

void DrawSurface(vector<array<int, 2>> points)
{
  array<int, 2> point1, point2;
  double slope;
  vector<array<int, 2>> landing_zones;

  // loop through points and draw lines for non-landing_zones
  gfx_color(188, 188, 188);
  
  for(int i = 0; i < points.size()-1; i++)
  {
    point1 = points[i];
    point2 = points[i+1];

    slope = double(point2[1] - point1[1]) / (point2[0] - point1[0]);

    if(abs(slope) < 0.001)
    {
      landing_zones.push_back(point1);
      landing_zones.push_back(point2);
    }
    else
    {
      gfx_line(point1[0], point1[1], point2[0], point2[1]);
    }
  }

  // loop through points and draw lines for landing zones
  gfx_color(188, 244, 86);

  for(int i = 0; i < landing_zones.size(); i += 2)
  {
    point1 = landing_zones[i];
    point2 = landing_zones[i+1];
    gfx_line(point1[0], point1[1], point2[0], point2[1]);
  }
}
