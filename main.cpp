#define _CRT_SECURE_NO_WARNINGS

#define NOTFOUND -1

#include <stdlib.h>
#include <iostream>
#include <string.h>

template <typename T>
struct Queue {
	T* q;
	int size;
	int first;

	Queue(int maxSize);
	~Queue();
	bool isEmpty();
	void push(T item);
	T pop();
};


struct PathPoint {
	int x, y, distance;
	PathPoint();
	PathPoint(int x, int y, int d);
};


typedef struct City {
	const char* name;
	int x, y;
};

typedef struct jakDojade {
	int width, height;
	char** rawMap;
	int cityCount;
	City* cities;
	int** costMatrix;

	~jakDojade();

	void readMap();
	void readFlights();
	void readQuestions();

	void findCities();
	char* findName(int x, int y);
	char* completeName(char* row, int start, bool end = false);
	int getCity(int x, int y);
	int getCity(const char* name);

	void findCosts();

	void dijkstraPathCost(int start, int end, bool readPath);
};

bool isMapTile(char c);
bool isRoadTile(char c);


int main()
{
	jakDojade engine;
	engine.readMap();

	engine.findCities();
	engine.findCosts();

	engine.readFlights();

	engine.readQuestions();


	return 0;
}

jakDojade::~jakDojade()
{
	for (int i = 0; i < height; i++)
		delete[] rawMap[i];
	delete[] rawMap;

	delete[] cities;

	for (int i = 0; i < cityCount; i++)
		delete[] costMatrix[i];
	delete[] costMatrix;
}

void jakDojade::readMap()
{
	std::cin >> width >> height;

	rawMap = new char*[height];
	for (int i = 0; i < height; i++)
		rawMap[i] = new char[width];

	char temp;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			std::cin >> temp;
			rawMap[i][j] = temp;
		}
	}
}

void jakDojade::readFlights()
{
	int connectionCount;
	std::cin >> connectionCount;

	char* str = new char[100];

	int start, end, distance;
	for (int i = 0; i < connectionCount; i++)
	{
		std::cin >> str;
		start = getCity(str);

		std::cin >> str;
		end = getCity(str);

		std::cin >> distance;

		if (costMatrix[start][end] > distance || costMatrix[start][end] == 0) costMatrix[start][end] = distance;
	}

}

void jakDojade::readQuestions()
{
	int questionCount;
	std::cin >> questionCount;

	char* str = new char[100];

	int start, end, type;
	for (int i = 0; i < questionCount; i++)
	{
		std::cin >> str;
		start = getCity(str);

		std::cin >> str;
		end = getCity(str);

		std::cin >> type;

		if (type == 0)
		{
			dijkstraPathCost(start, end, false);
		}
		else
			dijkstraPathCost(start, end, true);
		std::cout << "\n";
	}
}

void jakDojade::findCities()
{
	int counter = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (rawMap[i][j] == '*')
				counter++;

	cityCount = counter;
	cities = new City[counter];
	City city;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (rawMap[i][j] == '*')
			{
				city.name = findName(j, i);
				city.x = j;
				city.y = i;
				cities[cityCount - counter--] = city;
			}
}

char* jakDojade::findName(int x, int y)
{
	if (y > 0 && x > 0 && isMapTile(rawMap[y - 1][x - 1]) && !isMapTile(rawMap[y - 1][x]))
		return completeName(rawMap[y - 1], x);

	if (y > 0 && x < width - 1 && isMapTile(rawMap[y - 1][x + 1]) && !isMapTile(rawMap[y - 1][x]))
		return completeName(rawMap[y - 1], x, true);

	if (y > 0 && x > 0 && !isMapTile(rawMap[y - 1][x - 1]) && isMapTile(rawMap[y - 1][x]))
		return completeName(rawMap[y - 1], x-1, true);

	if (y > 0 && x < width - 1 && !isMapTile(rawMap[y - 1][x + 1]) && isMapTile(rawMap[y - 1][x]))
		return completeName(rawMap[y - 1], x + 1);

	if (y < height - 1 && x > 0 && isMapTile(rawMap[y + 1][x - 1]) && !isMapTile(rawMap[y + 1][x]))
		return completeName(rawMap[y + 1], x);

	if (y < height - 1 && x < width - 1 && isMapTile(rawMap[y + 1][x + 1]) && !isMapTile(rawMap[y + 1][x]))
		return completeName(rawMap[y + 1], x, true);

	if (y < height - 1 && x > 0 && !isMapTile(rawMap[y + 1][x - 1]) && isMapTile(rawMap[y + 1][x]))
		return completeName(rawMap[y + 1], x - 1, true);

	if (y < height - 1 && x < width - 1 && !isMapTile(rawMap[y + 1][x + 1]) && isMapTile(rawMap[y + 1][x]))
		return completeName(rawMap[y + 1], x + 1);


	if (x == 0 && y < height-1 && !isMapTile(rawMap[y + 1][x]))
		return completeName(rawMap[y + 1], x);

	if (x == width - 1  && y < height - 1 && !isMapTile(rawMap[y + 1][x]))
		return completeName(rawMap[y + 1], x, true);

	if (x == 0 && y > 0 && !isMapTile(rawMap[y - 1][x]))
		return completeName(rawMap[y - 1], x);

	if (x == width - 1 && y > 0 && !isMapTile(rawMap[y - 1][x]))
		return completeName(rawMap[y - 1], x, true);

	if (x > 0 && !isMapTile(rawMap[y][x - 1]))
		return completeName(rawMap[y], x - 1, true);

	if (x < width -1 && !isMapTile(rawMap[y][x + 1]))
		return completeName(rawMap[y], x + 1);


	if (!isMapTile(rawMap[y + 1][x]))
	{
		while (!isMapTile(rawMap[y + 1][x]) && x > 0)
			x--;
		return completeName(rawMap[y + 1], x+1);
	}

	if (!isMapTile(rawMap[y - 1][x]))
	{
		while (!isMapTile(rawMap[y - 1][x]) && x > 0)
			x--;
		return completeName(rawMap[y - 1], x+1);
	}
	return nullptr;
}

char* jakDojade::completeName(char* row, int start, bool end)
{
	char* str = nullptr;
	int counter = 0;
	int pos = start;
	char c = row[pos];
	while (!isMapTile(c) && pos < width && pos >= 0)
	{
		if (end) pos--;
		else pos++;

		counter++;
		c = row[pos];
	}
	str = new char[counter+1];
	str[counter] = '\0';
	
	for (int i = 0; i < counter; i++)
	{
		if (end)
		{
			str[i] = row[pos + i + 1];
		}
		else
		{
			str[i] = row[start + i];
		}
	}
	return str;
}

int jakDojade::getCity(int x, int y)
{
	for (int i = 0; i < cityCount; i++)
	{
		if (cities[i].x == x && cities[i].y == y)
			return i;
	}
	return NOTFOUND;
}

int jakDojade::getCity(const char* name)
{
	for (int i = 0; i < cityCount; i++)
	{
		if (strcmp(cities[i].name, name) == 0)
			return i;
	}
	return NOTFOUND;
}



void jakDojade::findCosts()
{

	int costMatrixSize = cityCount;

	costMatrix = new int* [costMatrixSize];
	for (int i = 0; i < costMatrixSize; i++)
		costMatrix[i] = new int[costMatrixSize];

	for (int i = 0; i < costMatrixSize; i++)
		for (int j = 0; j < costMatrixSize; j++)
			costMatrix[i][j] = 0;


	bool** visited = new bool*[height];
	for (int j = 0; j < height; j++)
		visited[j] = new bool[width];


	PathPoint start(0, 0, 0);

	Queue<PathPoint>* queue = nullptr;

	for (int startCityIndex = 0; startCityIndex < cityCount; startCityIndex++)
	{
		

		start.x = cities[startCityIndex].x;
		start.y = cities[startCityIndex].y;

		for (int endCityIndex = 0; endCityIndex < cityCount; endCityIndex++)
		{
			if (startCityIndex == endCityIndex) continue;

			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					visited[i][j] = false;

			for (int i = 0; i < height; i++) 
			{
				for (int j = 0; j < width; j++)
				{
					if (!isMapTile(rawMap[i][j]) || rawMap[i][j] == '.' || rawMap[i][j] == '*')
						visited[i][j] = true;
					else
						visited[i][j] = false;
				}
			}
			visited[cities[endCityIndex].y][cities[endCityIndex].x] = false;

			queue = new Queue<PathPoint>(width*height);
			queue->push(start);

			while (!queue->isEmpty()) {
				PathPoint point = queue->pop();

				if (point.x == cities[endCityIndex].x && point.y == cities[endCityIndex].y)
				{
					costMatrix[startCityIndex][endCityIndex] = point.distance;
					break;
				}

				if (point.x > 0 && !visited[point.y][point.x - 1])
				{
					queue->push(PathPoint(point.x - 1, point.y, point.distance + 1));
					visited[point.y][point.x - 1] = true;
				}
				if (point.y > 0 && !visited[point.y - 1 ][point.x])
				{
					queue->push(PathPoint(point.x , point.y - 1, point.distance + 1));
					visited[point.y - 1][point.x] = true;
				}
				if (point.x < width - 1 && !visited[point.y][point.x + 1])
				{
					queue->push(PathPoint(point.x + 1, point.y, point.distance + 1));
					visited[point.y][point.x + 1] = true;
				}
				if (point.y < height - 1 && !visited[point.y + 1][point.x])
				{
					queue->push(PathPoint(point.x, point.y + 1, point.distance + 1));
					visited[point.y + 1][point.x] = true;
				}

			}
			delete queue;
		}
	}

	for (int j = 0; j < height; j++)
		delete[] visited[j];
	delete[] visited;

}

void jakDojade::dijkstraPathCost(int start, int end, bool readPath)
{
	if (start == end)
	{
		std::cout << 0;
		return;
	}

	const int infinity = INT32_MAX;

	bool* visited = new bool[cityCount];
	for (int i = 0; i < cityCount; i++)
		visited[i] = false;

	int* distance = new int[cityCount];
	for (int i = 0; i < cityCount; i++)
		distance[i] = infinity;

	int* previous = new int[cityCount];
	for (int i = 0; i < cityCount; i++)
		previous[i] = -1;

	int current = start;

	distance[start] = 0;
	previous[start] = start;

	while (true)
	{
		for (int i = 0; i < cityCount; i++)
		{
			if (costMatrix[current][i] == 0)
				continue;

			if (distance[i] > distance[current] + costMatrix[current][i])
			{
				distance[i] = distance[current] + costMatrix[current][i];
				previous[i] = current;
			}
		}

		visited[current] = true;

		int smallestDistance = NOTFOUND;
		for (int i = 0; i < cityCount; i++)
		{
			if (!visited[i] && ( (smallestDistance != NOTFOUND && distance[i] < distance[smallestDistance]) || (smallestDistance == NOTFOUND) ) )
				smallestDistance = i;
		}

		if (smallestDistance == NOTFOUND)
		{
			std::cout << distance[end];
			

			if (readPath)
			{
				int nodeCount = 0;
				int* path = new int[cityCount];
				for (int i = 0; i < cityCount; i++)
				{
					path[i] = -1;
				}
				current = end;
				while (current != start)
				{
					path[cityCount - nodeCount - 1] = previous[current];
					current = previous[current];
					nodeCount++;
				}
				
				for (int i = cityCount - nodeCount + 1; i < cityCount; i++)
				{
					std::cout << " " << cities[path[i]].name;
				}
				
				delete[] path;
			}

			break;
		}

		current = smallestDistance;
	}
	delete[] visited;
	delete[] distance;
	delete[] previous;

}

bool isMapTile(char c)
{
	return c == '*' || c == '#' || c == '.';
}

bool isRoadTile(char c)
{
	return c == '*' || c == '#';
}

template<typename T>
Queue<T>::Queue(int maxSize)
{
	q = new T[maxSize];
	first = 0;
	size = 0;
}

template<typename T>
Queue<T>::~Queue()
{
	delete q;
}

template<typename T>
bool Queue<T>::isEmpty()
{
	return size == 0;
}

template<typename T>
void Queue<T>::push(T item)
{
	q[first + size] = item;
	size++;
}

template<typename T>
T Queue<T>::pop()
{
	size--;
	return q[first++];
}

PathPoint::PathPoint()
{
	x = 0;
	y = 0;
	distance = 0;
}

PathPoint::PathPoint(int x, int y, int d)
{
	this->x = x;
	this->y = y;
	this->distance = d;
}
