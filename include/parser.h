#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <list>
#include <climits>
#include <algorithm>


using namespace std;

class NDcost
{
public:
    //////////////////set////////////////////
    void setX1(int x1) { _x1 = x1; }
    void setY1(int y1) { _y1 = y1; }
    void setX2(int x2) { _x2 = x2; }
    void setY2(int y2) { _y2 = y2; }
    void setValue(int value) { _value = value; }

    //////////////////get////////////////////
    int getX1() { return _x1; }
    int getY1() { return _y1; }
    int getX2() { return _x2; }
    int getY2() { return _y2; }
    int getValue() { return _value; }

private:
    int _x1;
    int _y1;
    int _x2;
    int _y2;
    int _value;
};

class mono
{
public:
    /////////////////////////////functions/////////////////////////////

    void parser(int argc, char **argv);
    std::pair<std::vector<int>, int> routing();
    void output(int argc, char **argv, std::pair<std::vector<int>, int> array);
    /////////////////////////////declaration/////////////////////////////

    //boundary x1 y1 x2 y2
    int Bx1;
    int By1;
    int Bx2;
    int By2;

    //default cost
    static int default_cost;

    //non default cost
    vector<NDcost> NDcosts;

    //start x y;
    int sx;
    int sy;

    //target x y;
    int tx;
    int ty;

    ////////////////////////////////////////////////////////////////////////
    std::vector<std::list<std::pair<int, int>>> Adjacent;
    std::vector<std::list<int>> extra_cost;

    std::vector<int> pred, dis;

    int Node_num;
    int StartNode;
    int GridLength;
    int capacity;
    const int Max_Dis = INT_MAX;
    int ClosePathCnt = 0;

    std::pair<std::vector<int>, int> GetPath(int end);
    void AddConnect(int from, int to, int weight);
    void GetGridconncet();

    void Init(int Start);
    void Relaxation(int X, int Y, int weight);

    std::pair<int, int> Num2Coord(int Num);
    int Coord2Num(std::pair<int, int> Coord); 
    
    void cal_cost();
    void UpdateCost_fix();

};

#endif // PARSER_H
