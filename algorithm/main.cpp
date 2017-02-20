#include <fstream>
#include <cstring>
using namespace std;
ifstream fin ("input.in");
ofstream fout ("output.out");
struct point {
    double x,y;
} robots[9999], obstacles[999][999];
int numRobots, numObstacles, numPointsObst[999];

double getCoordinate(char *p, int length)
{
    double num = 0.0;
    int sign = 1;
    int power = 1;
    int isFloat = 0;
    int i = 0;

    if(p[0] == '-')
    {
        sign = -1;
        i = 1;
    }

    for(; i<length; i++)
    {
        if(p[i]=='.')
        {
            isFloat = 1;
            power=10;
            continue;
        }
        if(isFloat == 0)
            num = num*10 + p[i] - '0';
        else
        {
            num = num + ((double)(p[i]-'0'))/power;
            power *= 10;
        }
    }

    return sign*num;
}
void printRobots()
{
    int i;
    for(i=1;i<=numRobots;i++)
        fout<<'('<<robots[i].x<<','<<robots[i].y<<"),";
}
void printObstacles()
{
    int i,j;
    if(numObstacles>0)
        fout<<'#';
    for(i=1;i<=numObstacles;i++)
    {
        for(j=1;j<=numPointsObst[i];j++)
            fout<<'('<<obstacles[i][j].x<<','<<obstacles[i][j].y<<"),";
        fout<<';';
    }
    fout<<'\n';
}
int main()
{
    char s[55000],*p,*s1,*s2,*s3;
    double num1,num2;
    int i,j;
    for(i=1;i<=5;i++)
    {
        fin.getline(s,55000);
        p=strchr(s,':');
        p+=2;
        //fout<<p<<'\n';

        numRobots = 0;
        numObstacles = 0;


        while(p-s < strlen(s))
        {
            if(p[0] == '#')
            {
                numObstacles = 1;
                break;
            }

            s1 = strchr(p,'(');
            s1++;
            s2 = strchr(s1,',');
            s3 = strchr(s2,')');
            num1 = getCoordinate(s1, s2-s1);
            s2+=2;
            num2 = getCoordinate(s2, s3-s2);

            robots[++numRobots].x = num1;
            robots[numRobots].y = num2;

            p=s3+1;
        }

        while(p-s < strlen(s))
        {
            if(p[0] == ';')
                numObstacles++;

            s1 = strchr(p,'(');
            s1++;
            s2 = strchr(s1,',');
            s3 = strchr(s2,')');
            num1 = getCoordinate(s1, s2-s1);
            s2+=2;
            num2 = getCoordinate(s2, s3-s2);

            obstacles[numObstacles][++numPointsObst[numObstacles]].x = num1;
            obstacles[numObstacles][numPointsObst[numObstacles]].y = num2;

            p=s3+1;
        }
        printRobots();
        printObstacles();
        for(j=1;j<=numObstacles;j++)
            numPointsObst[j]=0;
    }
    fin.close();
    fout.close();
    return 0;
}
