class Player
{
private:
	float X, Y, Z;
	float Vx, Vy;
	float Rotation, Tilt;
	float RotationVelocity, TiltVelocity;
	float WalkDistance;
public:
	Player();
	Player(float x, float y);
	Player(float x, float y, float rotation);

	void UpdateInput(float movementSpeed, float rotationSpeed, float movementSmoothness, float rotationSmoothness, float collisionDistance);

	float getX();
	float getY();
	float getZ();
	float getVx();
	float getVy();
	float getRotation();
	float getTilt();
	float getRotationVelocity();
	float getTiltVelocity();

	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setVx(float x);
	void setVy(float y);
	void setRotation(float rotation);
	void setTilt(float tilt);
	void setRotationVelocity(float rotationVelocity);
	void setTiltVelocity(float tiltVelocity);

	friend class Engine;
	friend class Graphics;
};