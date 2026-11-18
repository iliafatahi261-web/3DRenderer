#include <raylib.h>
#include <vector>
#include <cmath>
#include <string>

struct point
{
	public:
	float x, y, z;
	int sx, sy, s;
};

void rotateY(point& p, float angle)
{
	float cos_a = cos(angle);
	float sin_a = sin(angle);
	float x = (p.x * cos_a - p.z * sin_a);
	float z = (p.x * sin_a + p.z * cos_a);
	p.x = x;
	p.z = z;
}

void rotateX(point& p, float angle)
{
	float cos_a = cos(angle);
	float sin_a = sin(angle);
	float y = (p.y * cos_a - p.z * sin_a);
	float z = (p.y * sin_a + p.z * cos_a);
	p.y = y;
	p.z = z;
}

bool clipLine(point& a , point& b, float nearPlane)
{
	bool aBehind = a.z < nearPlane;
	bool bBehind = b.z < nearPlane;

	if(aBehind && bBehind)
		return false;
	if(aBehind || bBehind)
	{
		float t = (nearPlane - a.z) / (b.z - a.z);

		point inter;

		inter.x = a.x + t * (b.x - a.x);
		inter.y = a.y + t * (b.y - a.y);
		inter.z = nearPlane;

		if(aBehind)
			a = inter;
		else 
			b = inter;
	}
	return true;
}
std::vector<point> points{
    // Floor corners: 0–7
    { 0.000f,-2.5f, 5.000f},
    { 3.535f,-2.5f, 3.535f},
    { 5.000f,-2.5f, 0.000f},
    { 3.535f,-2.5f,-3.535f},
    { 0.000f,-2.5f,-5.000f},
    {-3.535f,-2.5f,-3.535f},
    {-5.000f,-2.5f, 0.000f},
    {-3.535f,-2.5f, 3.535f},

    // Ceiling corners: 8–15
    { 0.000f, 2.5f, 5.000f},
    { 3.535f, 2.5f, 3.535f},
    { 5.000f, 2.5f, 0.000f},
    { 3.535f, 2.5f,-3.535f},
    { 0.000f, 2.5f,-5.000f},
    {-3.535f, 2.5f,-3.535f},
    {-5.000f, 2.5f, 0.000f},
    {-3.535f, 2.5f, 3.535f},

    // Floor midpoints of each wall: 16–23
    { 1.767f,-2.5f, 4.267f},
    { 4.267f,-2.5f, 1.767f},
    { 4.267f,-2.5f,-1.767f},
    { 1.767f,-2.5f,-4.267f},
    {-1.767f,-2.5f,-4.267f},
    {-4.267f,-2.5f,-1.767f},
    {-4.267f,-2.5f, 1.767f},
    {-1.767f,-2.5f, 4.267f},

    // Ceiling midpoints of each wall: 24–31
    { 1.767f, 2.5f, 4.267f},
    { 4.267f, 2.5f, 1.767f},
    { 4.267f, 2.5f,-1.767f},
    { 1.767f, 2.5f,-4.267f},
    {-1.767f, 2.5f,-4.267f},
    {-4.267f, 2.5f,-1.767f},
    {-4.267f, 2.5f, 1.767f},
    {-1.767f, 2.5f, 4.267f}
};

std::vector<std::vector<int>> lines{
    // Floor outline
    {0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,0},

    // Ceiling outline
    {8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31,8},

    // Vertical room corners
    {0,8}, {1,9}, {2,10}, {3,11},
    {4,12}, {5,13}, {6,14}, {7,15},

    // Vertical center lines on the eight walls
    {16,24}, {17,25}, {18,26}, {19,27},
    {20,28}, {21,29}, {22,30}, {23,31}
};

int main()
{

	const int ScreenHeight = 480;
	const int ScreenWidth = 480;
	InitWindow(ScreenWidth, ScreenHeight, "Basic 3DRenderer");
	SetTargetFPS(60);

	std::vector<Color> frameBuffer(ScreenHeight * ScreenWidth);

	float px = 0, py = 0, pz = 0, pa = 0, pms = 3.0f, prs = 1.5f; //player speed
	float pitch = 0;
	const float nearPlane = 0.1f;
	std::vector<point> transPoints(points.size());
	while(!WindowShouldClose())
	{
		int maxPitch = 1.4f;
		for(int i = 0; i < frameBuffer.size(); ++i)
			frameBuffer[i] = BLACK;
		//GetInput
		if(IsKeyDown(KEY_LEFT))
			pa -= prs * GetFrameTime();
		if(IsKeyDown(KEY_RIGHT))
			pa += prs * GetFrameTime();
		if(IsKeyDown(KEY_W))
		{
			px += (pms * GetFrameTime()) * sin(pa);
			pz += (pms * GetFrameTime()) * cos(pa);
		}
		if(IsKeyDown(KEY_S))
		{
			px -= (pms * GetFrameTime()) * sin(pa); 
			pz -= (pms * GetFrameTime()) * cos(pa);
		}
		if(IsKeyDown(KEY_UP))
			pitch += prs * GetFrameTime();
		if(IsKeyDown(KEY_DOWN))
			pitch -= prs * GetFrameTime();
		if(pitch > maxPitch) pitch = maxPitch;
		if(pitch < -maxPitch) pitch = -maxPitch;
		if(pa > (3.14 * 2)) pa -= (3.14 * 2);
		if(pa < -(3.14 * 2)) pa += (3.14 * 2);
		//DoCalculations
		for(int i = 0; i < points.size(); ++i)
		{
			point p = points[i];

			p.x -= px;
			p.y -= py;
			p.z -= pz;
			
			rotateY(p, pa);
			rotateX(p, pitch);

			transPoints[i] = p;
		}
		//DrawStuff
		BeginDrawing();
		ClearBackground(BLACK);
		for(int i = 0; i < lines.size(); i++)
		{
			for(int j = 0; j < lines[i].size(); j++)
			{
				int pOne = lines[i][j];
				int pTwo = lines[i][(j + 1) % lines[i].size()];

				point a = transPoints[pOne];
				point b = transPoints[pTwo];
				
				if(!clipLine(a,b,nearPlane))
					continue;

				float x1 = (a.x / a.z + 0.5f) * ScreenWidth;
				float y1 = (0.5f - a.y / a.z ) * ScreenHeight;

				float x2 = (b.x / b.z + 0.5f) * ScreenWidth;
				float y2 = (0.5f - b.y / b.z ) * ScreenHeight;

				DrawLineEx(
						{x1, y1},
						{x2, y2},
					 	3.0f,
					 	GREEN);
			}
		}
		DrawText(std::to_string(pitch).c_str(),10,30,20,GREEN);
		DrawText(std::to_string(pa).c_str(),10,50,20,GREEN);
		DrawFPS(10,10);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
