//Kathleen Liebscher and Logan Yokum 
//Section 1
//Interface for Spaceship in Lunar Landing

#include <array>
#include <vector>
using namespace std;

// global variables
enum direction{north, east, south, west};

class Spaceship
{
  public:
    Spaceship();
    ~Spaceship();

    bool CheckCollision(vector<array<int, 2>>);
    bool GetSafe();

    void Thrust(direction);

    void Update(int, int);
    void DrawShip(int, int); 

  private:
    double xAcc = 0, yAcc = 0;
    double xVel = 10, yVel = 0;
    double xPos = 0, yPos = 0;
    double radius = 8;

    const int minStartVel = 3;
    const int maxStartVel = 20;

    double fuel = 250;
    double fuel_change = -10; // fuel units per second

    double gravityAcc = 0.2; // pixels per second^2
    double rocketAcc = 1.2; // pixels per second^2

    array<int, 3> flameColor;
    bool isFiring = false;
    direction flameDirection;

    array<int, 3> shipColor;
    array<array<int, 2>, 4> shipCoords;

    bool isSafe = true;
    bool isLanded = false;
    const double maxSafeSpeed = 8;
};

