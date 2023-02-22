#include <queue>
#include <vector>
#include <iostream>
#include <functional>
#include <stack>

using namespace std;

struct Point
{
	int x;
	int y;
	bool operator==(const Point& other) const
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const Point& other) const
	{
		return !(*this == other);
	}
};

typedef vector<Point> Points;
typedef vector<vector<int>> Matrix;

float distance(Point a, Point b)
{
	int x = a.x - b.x;
	int y = a.y - b.y;
	return sqrt(x * x + y * y);
}

float distance(const Points& points, Point target)
{
	return points.size() + distance(points[points.size() - 1], target);
}

bool isValidPosition(const Matrix m, const Point& newPoint, const Point& start, const Points& path)
{
	if (newPoint == start) {
		return false;
	}
	if (find(path.begin(), path.end(), newPoint) != path.end())
	{
		return false;
	}
	if (newPoint.x >= m.size() || newPoint.x < 0 || newPoint.y >= m[0].size() || newPoint.y < 0)
	{
		return false;
	}
	return m[newPoint.y][newPoint.x] == 0;
}


Points FindPath(const Matrix m, Point start, Point end)
{
	auto cmp = [=](const Points& left, const Points& right)
	{
		return distance(left, end) > distance(right, end);
	};

	priority_queue<Points, vector<Points>, decltype(cmp)> q(cmp);

	int directions[4][2] =
	{
		{1, 0},
		{0, 1},
		{0, -1},
		{-1, 0}
	};

	for (int i = 0; i < 4; i++)
	{
		Point p = { start.x + directions[i][0], start.y + directions[i][1] };
		Points path;

		if (isValidPosition(m, p, start, path)) 
		{
			path.push_back(p);
			q.push(path);
		}
	}

	while (true)
	{
		if (q.empty())
		{
			cout << "No valid path!" << endl;
			return Points();
		}
		Points path = q.top();
		q.pop();

		if (path[path.size() - 1] == end)
		{
			return path;
		}
		Point last = path[path.size() - 1];

		for (int i = 0; i < 4; i++)
		{
			Point p = { last.x + directions[i][0], last.y + directions[i][1] };

			if (isValidPosition(m, p, start, path))
			{
				Points newPath = path;
				newPath.push_back(p);
				q.push(newPath);
			}
		}
	}
}

void PrintMatrix(Matrix m)
{
	int zeros = 0;
	int ones = 0;
	for (int i = 0; i < m.size(); i++)
	{
		for (int j = 0; j < m[0].size(); j++)
		{
			cout << m[i][j] << "  ";
			zeros += m[i][j] == 0 ? 1 : 0;
			ones += m[i][j] == 1 ? 1 : 0;
		}
		cout << endl;
	}
	cout << "Ones: " << ones << endl;
	cout << "Zeros: " << zeros << endl;
}

void PrintPath(const Points& path)
{
	cout << "Path Length: " << path.size() << endl;
	for (int i = 0; i < path.size(); i++)
	{
		cout << "X: " << path[i].x << "\tY: " << path[i].y << endl;
	}
}

int main()
{
	const Matrix m0 = {
		{ 0,0,0,1,0 },
		{ 0,1,0,0,0 },
		{ 0,1,1,1,0 },
		{ 0,0,1,1,0 },
		{ 0,0,0,0,0 }
	};
	const Matrix m1 = {
		{ 0,0,0,1,0 },
		{ 0,1,0,0,0 },
		{ 1,1,1,1,0 },
		{ 0,0,1,1,0 },
		{ 0,0,0,1,0 }
	};

	srand(0);
	const float wallProbability = 0.3f;
	Matrix m2;
	m2.resize(20);

	for (auto& row : m2)
	{
		row.resize(20);
		for (int& element : row) 
		{
			float r = float(rand() % 1000) / 1000.0f;
			element = r <= wallProbability ? 1 : 0;
		}
	}

	PrintMatrix(m2);

	Point start = { 1, 0 };
	Point end = { 18, 18 };
	Points points = FindPath(m2, start, end);
	PrintPath(points);

	start = { 1, 3 };
	end = { 4, 0 };
	Points points = FindPath(m1, start, end);
	PrintPath(points);
}
