#define INF 9999999
#include <fstream>
#include <cstring>
#include <cmath>
using namespace std;
ifstream fin("input.in");
ofstream fout("output.out");

struct node
{
	double x, y;
	int obstacleNo;
} nodes[10000];

struct line 
{
	double a, b, c;
} ;

int numNodes;
int numRobots;
int numObstacles;

double distances[10000][10000];
int nextNode[10000][10000];

double getCoordinate(char *p, int length)
{
	double num = 0.0;
	int sign = 1;
	int power = 1;
	int isFloat = 0;
	int i = 0;

	if (p[0] == '-')
	{
		sign = -1;
		i = 1;
	}

	for (; i<length; i++)
	{
		if (p[i] == '.')
		{
			isFloat = 1;
			power = 10;
			continue;
		}
		if (isFloat == 0)
			num = num * 10 + p[i] - '0';
		else
		{
			num = num + ((double)(p[i] - '0')) / power;
			power *= 10;
		}
	}

	return sign*num;
}

void read()
{
	char s[55000], *p, *s1, *s2, *s3;
	double num1, num2;
	int j;
	fin.getline(s, 55000);
	p = strchr(s, ':');
	p += 2;

	numRobots = 0;
	numObstacles = 0;
	numNodes = 0;

	while (p - s < strlen(s))
	{
		if (p[0] == '#')
		{
			numObstacles = 1;
			break;
		}

		s1 = strchr(p, '(');
		s1++;
		s2 = strchr(s1, ',');
		s3 = strchr(s2, ')');
		num1 = getCoordinate(s1, s2 - s1);
		s2++;
		if (s2[0] == ' ')
			s2++;
		num2 = getCoordinate(s2, s3 - s2);

		numRobots++;

		nodes[++numNodes].x = num1;
		nodes[numNodes].y = num2;
		nodes[numNodes].obstacleNo = -1;

		p = s3 + 1;
	}

	while (p - s < strlen(s))
	{
		if (p[0] == ';')
			numObstacles++;

		s1 = strchr(p, '(');
		s1++;
		s2 = strchr(s1, ',');
		s3 = strchr(s2, ')');
		num1 = getCoordinate(s1, s2 - s1);
		s2++;
		if (s2[0] == ' ')
			s2++;
		num2 = getCoordinate(s2, s3 - s2);

		nodes[++numNodes].x = num1;
		nodes[numNodes].y = num2;
		nodes[numNodes].obstacleNo = numObstacles;

		p = s3 + 1;
	}
}

int equalDoubles(double x, double y)
{
	if (abs(x - y) < 0.00001)
		return 1;
	return 0;
}

double getDistanceNodeToNode(node p1, node p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

line getLine(node p1, node p2)
{
	line l;

	l.a = p2.y - p1.y;
	l.b = p1.x - p2.x;
	l.c = p1.y*p2.x - p1.x*p2.y;
	return l;
}

int getIntersection(node &p, line l1, line l2)
{
	if (l2.a == 0 || l2.b == 0)
	{
		if (l1.a == 0 && l2.a == 0)
			return 0;
		if (l1.b == 0 && l2.b == 0)
			return 0;
	}
	else
		if (l1.a / l2.a == l1.b / l2.b)
			return 0;

	p.x = (l1.b*l2.c - l2.b*l1.c) / (l1.a*l2.b - l2.a*l1.b);
	if (l2.b == 0)
		p.y = (-1)*(l1.a*p.x + l1.c) / l1.b;
	else
		p.y = (-1)*(l2.a*p.x + l2.c) / l2.b;
	return 1;
}

int checkIntersectionOnSegment(node intersection, node p1, node p2)
{
	node aux, cp1 = p1, cp2 = p2;
	if (p1.x > p2.x)
	{
		aux = p1;
		p1 = p2;
		p2 = aux;
	}

	if (!equalDoubles(p1.x, p2.x))
		if (intersection.x <= p1.x || intersection.x >= p2.x || equalDoubles(intersection.x, p1.x) || equalDoubles(intersection.x, p2.x))
			return 0;

	if (p1.y > p2.y)
	{
		aux = p1;
		p1 = p2;
		p2 = aux;
	}

	if (!equalDoubles(p1.y, p2.y))
		if (intersection.y <= p1.y || intersection.y >= p2.y || equalDoubles(intersection.y, p1.y) || equalDoubles(intersection.y, p2.y))
			return 0;

	return 1;
}

int checkPointOnSegment(node p, node p1, node p2)
{
	line l = getLine(p1, p2);
	return equalDoubles(0.0, (l.a*p.x+l.b*p.y+l.c)) && checkIntersectionOnSegment(p, p1, p2);
}

int intersectedLines(node line1p1, node line1p2, node line2p1, node line2p2)
{
	line l1 = getLine(line1p1, line1p2);
	line l2 = getLine(line2p1, line2p2);
	node intersection;
	int intersected = getIntersection(intersection, l1, l2);

	if (checkIntersectionOnSegment(intersection, line1p1, line1p2) && checkIntersectionOnSegment(intersection, line2p1, line2p2))
		return 1;
	return 0;
}

bool nodeInsideObstacle(node n, int a)
{
	if (nodes[a].obstacleNo == -1)
		return 0;
	int ca = a;
	int last = a;
	while (nodes[a].obstacleNo == nodes[a - 1].obstacleNo)
		a--;
	while (nodes[last].obstacleNo == nodes[last + 1].obstacleNo)
		last++;

	int first = a;
	int i;
	int j = last;

	int n1, n2;
	if (ca == first)
	{
		n1 = last;
		n2 = ca + 1;
	}
	else
		if (ca == last)
		{
			n1 = ca - 1;
			n2 = first;
		}
		else
		{
			n1 = ca - 1;
			n2 = ca + 1;
		}
	if (checkPointOnSegment(n, nodes[n1], nodes[ca]))
		return 0;
	if (checkPointOnSegment(n, nodes[ca], nodes[n2]))
		return 0;

	bool oddNodes = false;

	for (i = first; i <= last; j = i++)
	{
		if (((nodes[i].y >= n.y) != (nodes[j].y >= n.y)) && (n.x <= (nodes[j].x - nodes[i].x) * (n.y - nodes[i].y) / (nodes[j].y - nodes[i].y) + nodes[i].x))
			oddNodes = !oddNodes;
	}

	return oddNodes;
}

int lineThroughObstacle(int a, int b)
{
	if (nodes[a].obstacleNo == -1 && nodes[b].obstacleNo == -1)
		return 0;
	node newNode1, newNode2, node1 = nodes[a], node2 = nodes[b];
	newNode1.x = node1.x + (node2.x - node1.x) / 50;
	newNode1.y = node1.y + (node2.y - node1.y) / 50;
	newNode2.x = node2.x + (node1.x - node2.x) / 50;
	newNode2.y = node2.y + (node1.y - node2.y) / 50;
	return nodeInsideObstacle(newNode1, a) || nodeInsideObstacle(newNode2, b);
}

int intersectsObstacle(int a, int b)
{
	if (lineThroughObstacle(a,b))
	{
		return 1;
	}
	int i, firstNodeObst;
	node obstLineP1, obstLineP2;
	for (i = numRobots + 1; i <= numNodes; i++)
	{
		if (nodes[i].obstacleNo != nodes[i - 1].obstacleNo)
			firstNodeObst = i;
		obstLineP1 = nodes[i];
		if(nodes[i].obstacleNo == nodes[i + 1].obstacleNo)
			obstLineP2 = nodes[i + 1];
		else
			obstLineP2 = nodes[firstNodeObst];
		int aa = intersectedLines(nodes[a], nodes[b], obstLineP1, obstLineP2);
		if (intersectedLines(nodes[a], nodes[b], obstLineP1, obstLineP2))
			return 1;
	}
	return 0;
}

int main()
{
	int i,j,k,m;
	
	for (i = 1; i <= 30; i++)
	{
		read();
		if (i == 3)
		{
			for (j = 1; j < numNodes; j++)
			{
				for (k = j + 1; k <= numNodes; k++)
				{
					if (!intersectsObstacle(j, k))
					{
						distances[j][k] = getDistanceNodeToNode(nodes[j], nodes[k]);
						nextNode[j][k] = k;
						distances[k][j] = distances[j][k];
						nextNode[k][j] = j;
					}
					else
					{
						distances[j][k] = INF;
						nextNode[j][k] = 0;
						distances[k][j] = INF;
						nextNode[k][j] = 0;
					}
				}
			}
			fout << '\n';

			for (m = 1; m <= numNodes; m++)
				for (j = 1; j <= numNodes; j++)
					for (k = 1; k <= numNodes; k++)
						if (distances[j][m] != INF && distances[m][k] != INF && (distances[j][k] > distances[j][m] + distances[m][k]) && j != k)
						{
							distances[j][k] = distances[j][m] + distances[m][k];
							nextNode[j][k] = nextNode[j][m];
						}

			for (j = 1; j <= numNodes; j++)
			{
				for (k = 1; k <= numNodes; k++)
					fout << j << ", " << k << ":\ndistance: " << distances[j][k] << "\nthrough: " << nextNode[j][k] << '\n';
				fout << '\n';
			}
		}
	}
}