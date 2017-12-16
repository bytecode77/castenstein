#include "Castenstein.h"

Player::Player()
{
	X = 0;
	Y = 0;
	Z = 0;
	Vx = 0;
	Vy = 0;
	Rotation = 0;
	Tilt = 0;
	RotationVelocity = 0;
	TiltVelocity = 0;
	WalkDistance = 0;
}
Player::Player(float x, float y)
{
	X = x;
	Y = y;
	Z = 0;
	Vx = 0;
	Vy = 0;
	Rotation = 0;
	Tilt = 0;
	RotationVelocity = 0;
	TiltVelocity = 0;
	WalkDistance = 0;
}
Player::Player(float x, float y, float rotation)
{
	X = x;
	Y = y;
	Z = 0;
	Vx = 0;
	Vy = 0;
	Rotation = rotation;
	Tilt = 0;
	RotationVelocity = 0;
	TiltVelocity = 0;
	WalkDistance = 0;
}

void Player::UpdateInput(float movementSpeed, float rotationSpeed, float movementSmoothness, float rotationSmoothness, float collisionDistance)
{
	if (Input::getKeyDown(SDLK_LSHIFT)) movementSpeed *= 1.5f;

	RotationVelocity = (RotationVelocity + Input::getMouseXSpeed()) * rotationSmoothness;
	TiltVelocity = (TiltVelocity + Input::getMouseYSpeed() / 1.5f) * rotationSmoothness;
	float ox = float(Input::getKeyDown(SDLK_d) - Input::getKeyDown(SDLK_a));
	float oy = float(Input::getKeyDown(SDLK_s) - Input::getKeyDown(SDLK_w));
	if (ox != 0 && oy != 0)
	{
		float d = sqrt(2.f);
		ox /= d;
		oy /= d;
	}
	Vx = (Vx + ox * cosd(Rotation) - oy * sind(Rotation)) * movementSmoothness;
	Vy = (Vy + ox * sind(Rotation) + oy * cosd(Rotation)) * movementSmoothness;

	float dx = Vx * movementSpeed, dy = Vy * movementSpeed;
	int mapX = int(X), mapY = int(Y);
	int colX = int(X + dx + copysign(collisionDistance, dx));
	int colY = int(Y + dy + copysign(collisionDistance, dy));
	float newX = X, newY = Y;
	if (Engine::CurrentMap->Blocks[mapY][colX]->IsWall || Y - floor(Y) < collisionDistance && Engine::CurrentMap->Blocks[mapY - 1][colX]->IsWall || ceil(Y) - Y < collisionDistance && Engine::CurrentMap->Blocks[mapY + 1][colX]->IsWall)
	{
		if (dx > 0) newX = ceil(X) - collisionDistance; else if (dx < 0) newX = floor(X) + collisionDistance;
	}
	else
	{
		newX = X + dx;
	}
	if (Engine::CurrentMap->Blocks[colY][mapX]->IsWall || X - floor(X) < collisionDistance && Engine::CurrentMap->Blocks[colY][mapX - 1]->IsWall || ceil(X) - X < collisionDistance && Engine::CurrentMap->Blocks[colY][mapX + 1]->IsWall)
	{
		if (dy > 0) newY = ceil(Y) - collisionDistance; else if (dy < 0) newY = floor(Y) + collisionDistance;
	}
	else
	{
		newY = Y + dy;
	}
	X = clip(newX, 1 + collisionDistance, Engine::CurrentMap->Size.X - 1 - collisionDistance);
	Y = clip(newY, 1 + collisionDistance, Engine::CurrentMap->Size.Y - 1 - collisionDistance);

	Z = sin(WalkDistance += sqrt(Vx * Vx + Vy * Vy) * movementSpeed * 4) * .05f - .2f;
	Rotation += RotationVelocity * rotationSpeed;
	Tilt += TiltVelocity * rotationSpeed;
	Tilt = clip(Tilt, -45, 45);
}

float Player::getX()
{
	return X;
}
float Player::getY()
{
	return Y;
}
float Player::getZ()
{
	return Z;
}
float Player::getVx()
{
	return Vx;
}
float Player::getVy()
{
	return Vy;
}
float Player::getRotation()
{
	return Rotation;
}
float Player::getTilt()
{
	return Tilt;
}
float Player::getRotationVelocity()
{
	return RotationVelocity;
}
float Player::getTiltVelocity()
{
	return TiltVelocity;
}

void Player::setX(float x)
{
	X = x;
}
void Player::setY(float y)
{
	Y = y;
}
void Player::setZ(float z)
{
	Z = z;
}
void Player::setVx(float vx)
{
	Vx = vx;
}
void Player::setVy(float vy)
{
	Vy = vy;
}
void Player::setRotation(float rotation)
{
	Rotation = rotation;
}
void Player::setTilt(float tilt)
{
	Tilt = tilt;
}
void Player::setRotationVelocity(float rotationVelocity)
{
	RotationVelocity = rotationVelocity;
}
void Player::setTiltVelocity(float tiltVelocity)
{
	TiltVelocity = tiltVelocity;
}