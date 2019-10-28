#include "Point.h"
#include <cstdlib>
#include <climits>

#define N 5
#define L 20
#define PI 3.14159265358979323846

struct Path{
    Point path[L];
    int size;
    float distance;
    bool feasible;
    Path(){
        size = distance = 0;
        feasible = true;
    }
};

class Pathplanning
{
private:
    int radio;
    Point position;
    Point target;
    int isCollisionFree(Point a, Point b);
    void calculateParameters(Point p1, Point p2, Point p3, float &a, float &b, float &c);
    Point searchPoint(Point a, Point b, bool d, bool &feasible);
    void recPath(Point dep, Point obj, bool dir, Path &path);

public:
    Pathplanning(Point pos, Point tgt, int r);
    void printData();
    void shortPath(Point dep, Point tgt);
    Point obstacles[N];
    Path trajectory;
};

Pathplanning::Pathplanning(Point pos, Point tgt, int r)
{
    position = pos;
    target = tgt;
    radio = r;
    for(int i = 0; i < N; i++){
        obstacles[i].x = rand() % L;
        obstacles[i].y = rand() % L;
    }
}

void Pathplanning::calculateParameters(Point p1, Point p2, Point p3, float &a, float &b, float &c){
    if(p1.x == p2.x){
        a = 1;
        b = -2*p3.y;
        c = pow(p1.x-p3.x,2) - radio*radio + p3.y*p3.y;
    }
    else{
        float m = float((p2.y-p1.y)/(p2.x-p1.x));
        float d = float(p1.y - m*p1.x);
        a = float(1+m*m);
        b = float(2*(m*(d-p3.y)-p3.x));
        c = float(pow(d-p3.y, 2) + pow(p3.x, 2) - radio*radio);
    }
}

int Pathplanning::isCollisionFree(Point p1, Point p2){
    float a, b, c;
    for(int i = 0; i < N; i++){
        calculateParameters(p1, p2, obstacles[i], a, b, c);
        if(b*b - 4*a*c > 0){
            cout << i << endl;
            return i;
        }

    }
    cout << -1 << endl;
    return -1;
}

Point Pathplanning::searchPoint(Point a, Point b, bool dir, bool &feasible){
    float angle = PI/2;
    if(a.y != b.y)
        angle = atan(-1*(b.x-a.x)/(b.y-a.y));
    do{
        if(dir){
            b.x += (radio+1)*cos(angle);
            b.y += (radio+1)*sin(angle);
        }
        else{
            b.x -= (radio+1)*cos(angle);
            b.y += (radio+1)*sin(-1*angle);
        }
        if(b.x < 0 || b.x > L || b.y < 0 || b.y > L){
            feasible = false;
            break;
        }
    }while(isCollisionFree(a, b) != -1);
    return b;
}

void Pathplanning::recPath(Point dep, Point obj, bool dir, Path &path){
    int isCol = isCollisionFree(dep, obj); 
    bool feasible = true;
    if(isCol >= 0){
        Point p1 = searchPoint(dep, obstacles[isCol], dir, feasible);
        if(feasible){
            recPath(dep, p1, dir, path);
            recPath(p1, obj, dir, path);
        }
        else
            path.feasible = false;
        
    }
    else if(path.size < L){
        path.distance += dep.distance(obj);
        path.path[path.size++] = obj; 
    }
}

void Pathplanning::shortPath(Point dep, Point obj){
    Path p1, p2;
    recPath(dep, obj, true, p1);
    recPath(dep, obj, false, p2);
    trajectory = p1.distance < p2.distance ? p1 : p2;
}

void Pathplanning::printData(){
    cout << "Position = ";
    position.printData();
    cout << "Target = ";
    target.printData();
    cout << "Radio = " << radio << endl;
    cout << "Obstacles = " << endl;
    for(int i = 0; i < N; i++)
        obstacles[i].printData();   
    for(int i = 0; i < trajectory.size; i++)
        trajectory.path[i].printData();
    cout << "Distance = " << trajectory.distance << endl;
    cout << "Feasible = " << trajectory.feasible << endl;
}