/*

== TO DO ==
1. Create a blank screen and a game loop - DONE
2. Draw the paddles and the ball - DONE
3. Move the ball around - DONE
4. Check for a collision with all edges - DONE
5. Move the player's paddle - DONE
6. Move the CPU paddle with AI - DONE
7. Check for a collision with the paddles - DONE
8. Add score - DONE
9. Add Shift Speed - DONE

*/

#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>

// raylib
#include <raylib.h>

//screen
int screenX = 1280;
int screenY = 800;
int centerX = (screenX / 2);
int centerY = (screenY / 2);
Color bgColor = BLACK;

int playerScore = 0,
	enemyScore = 0;


bool shiftPressed = false;

class cColors {
public:
	Color playerSide = Color{ 0, 71, 164, 255 };
	Color enemySide = Color{ 0, 92, 212, 255 };

	Color enemyPaddle = Color{ 180, 0, 0, 255 };
	Color playerPaddleNormal = Color{ 0, 180, 0, 255 };
	Color playerPaddleShift = Color{ 70, 255, 70, 255 };
};
inline cColors colors;

class cBall {
public:
	// All of these variables are set as a default
	float x = centerX,
		y = centerY;

	int speedX = 6.f,
		speedY = 6.f;

	float radius = 15.f;
	Color color = YELLOW;

	void Draw()
	{
		DrawCircle(x, y, radius, color);
	}

	void ResetBall()
	{
		x = centerX;
		y = centerY;

		int speed_choices[2] = { -1, 1 };
		speedX *= speed_choices[GetRandomValue(0, 1)];
		speedY *= speed_choices[GetRandomValue(0, 1)];
	}

	void Update()
	{
		x += speedX;
		y += speedY;

		if (y + radius >= screenY || y - radius <= 0) {
			speedY *= -1;
		}

		if (x + radius >= screenX || x - radius <= 0) {
			speedX *= -1;
		}

		if (x + radius >= screenX)
		{
			enemyScore++;
			ResetBall();
		}

		if (x - radius <= 0)
		{
			playerScore++;
			ResetBall();
		}

	}



};
inline cBall ball;

class cPaddle {
protected:
	void LimitMovement()
	{
		if (y <= 0)
		{
			y = 0;
		}
		if (y + Height >= screenY)
		{
			y = screenY - Height;
		}
	}
public:
	// Some are default, some no

	float Width = 20.f,
		Height = 100.f;

	int HalfWidth = (Width / 2),
		HalfHeight = (Height / 2);

	int Spacing = 5;
	
	float x,
		  y;

	Color color;

	float speed = 7.9f;
	int ShiftSpeed = speed * 2;

	void Draw()
	{
		DrawRectangle(x, y, Width, Height, color); // enemy
	}

	void Update()
	{
		shiftPressed = false;

		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		{
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				shiftPressed = true;
				y -= ShiftSpeed;
			}
			else
			{
				y -= speed;
			}
		}

		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
		{
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				shiftPressed = true;
				y += ShiftSpeed;
			}
			else
			{
				y += speed;
			}
		}

		LimitMovement();
	}

};
inline cPaddle player;

class cCpuPaddle : public cPaddle {
public:


	void Update()
	{
		if (y + HalfHeight <= ball.y )
		{
			y  += speed;
		}

		if (y + HalfHeight >= ball.y )
		{
			y -= speed;
		}

		LimitMovement();

	}


};
inline cCpuPaddle cpu;

int main()
{
	std::cout << "Starting the game..." << std::endl;

	InitWindow(screenX, screenY, "pong game");
	SetTargetFPS(60);
	


	// setup player (left side) - me
	player.x = player.Spacing; // x = 0, so just add spacing so it starts drawing after the spacing, we want the code clean thats why we do this
	player.y = centerY - player.HalfHeight;

	// setup enemy (right side)
	cpu.x = screenX - cpu.Spacing - cpu.Width;
	cpu.y = centerY - cpu.HalfHeight;
	cpu.color = colors.enemyPaddle;



	while (!WindowShouldClose()) // I dont like using bool == false, I prefer <statement> ( boolTrue() ) - if bool is true or <statement> ( !boolTrue() ) - if bool is false.
	{
		if (shiftPressed)
		{
			player.color = colors.playerPaddleShift;

		}
		else {
			player.color = colors.playerPaddleNormal;
		}
		BeginDrawing();
		{
			// update:
			ball.Update();

			player.Update();
			cpu.Update();

			// background color
			ClearBackground(colors.playerSide);// player side
			DrawRectangle(centerX, 0, centerX, screenY, colors.enemySide); // enemy side

			// check for collisions
			if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x, player.y, player.Width, player.Height }))
			{
				ball.speedX *= -1;
			} // player (me)

			if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.Width, cpu.Height }))
			{
				ball.speedX *= -1;
			} // enemy



			// drawing:
			ball.Draw();

			player.Draw();
			cpu.Draw();
			DrawText(TextFormat("%i", playerScore), 3 * screenX / 4 - 20, 25, 60, RED);
			DrawText(TextFormat("%i", enemyScore), screenX / 4 - 20, 25, 60, GREEN);

			DrawLine(centerX, 0, centerX, screenY, WHITE);


		}
		EndDrawing();
	}
	
	CloseWindow();

	return 0;
}