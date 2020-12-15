#include "parser.h"
#include <chrono>

void mono ::parser(int argc, char **argv)
{
    //check argument count
    if (argc != 3)
    {
        cout << "Usage: ./mono <input_file_name> <output_file_name>" << endl;
        exit(1);
    }

    fstream fin;
    fin.open(argv[1], fstream::in);

    //check argument open
    if (!fin.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        exit(1);
    }

    char buffer[100];

    //parser start
    while (!fin.eof())
    {
        fin >> buffer;
        if (!strcmp(buffer, "BoundaryIndex"))
        {
            //input Bx1
            fin >> buffer;
            Bx1 = atoi(buffer);
            //input By1
            fin >> buffer;
            By1 = atoi(buffer);
            //input Bx2
            fin >> buffer;
            Bx2 = atoi(buffer);
            //input By2
            fin >> buffer;
            By2 = atoi(buffer);
        }

        Node_num = (Bx2 + 1) * (By2 + 1);
        Adjacent.resize(Node_num);
        GridLength = (By2 + 1);
        extra_cost.resize(Node_num);
        Init(0);

        if (!strcmp(buffer, "DefaultCost"))
        {
            //input default cost
            fin >> buffer;
            default_cost = atoi(buffer);
        }
        if (!strcmp(buffer, "NumNonDefaultCost"))
        {
            //set non default cost size
            fin >> buffer;
            int size = atoi(buffer);
            NDcosts.resize(size);

            for (int i = 0; i < size; ++i)
            {
                //input non default x1 y1 x2 y2 cost
                fin >> buffer;
                NDcosts[i].setX1(atoi(buffer));
                fin >> buffer;
                NDcosts[i].setY1(atoi(buffer));
                fin >> buffer;
                NDcosts[i].setX2(atoi(buffer));
                fin >> buffer;
                NDcosts[i].setY2(atoi(buffer));
                fin >> buffer;
                NDcosts[i].setValue(atoi(buffer));
                int End_Num = Coord2Num(std::pair<int, int>(NDcosts[i].getX1(), NDcosts[i].getY1()));
            }
        }
        if (!strcmp(buffer, "Source"))
        {
            //input start x y
            fin >> buffer;
            sx = atoi(buffer);
            fin >> buffer;
            sy = atoi(buffer);
        }
        if (!strcmp(buffer, "Target"))
        {
            //input target x y
            fin >> buffer;
            tx = atoi(buffer);
            fin >> buffer;
            ty = atoi(buffer);
        }
    }

    fin.close();

    // The following demonstrates how to use the parser
    cout << "Boundary Index: " << Bx1 << " " << By1 << " " << Bx2 << " " << By2 << endl;
    cout << "Default cost: " << default_cost << endl;
    cout << "# non-default costs: " << NDcosts.size() << endl;
    // for (int i = 0; i < NDcosts.size(); ++i)
    // {
    //     cout << NDcosts[i].getX1() << " " << NDcosts[i].getY1() << " "
    //          << NDcosts[i].getX2() << " " << NDcosts[i].getY2() << " "
    //          << NDcosts[i].getValue() << endl;
    // }

    return;
}

std::pair<std::vector<int>, int> mono ::routing()
{
    //////////////////////////////////////////////////////////
    ///////////////write your algorithm///////////////////////
    //////////////////////////////////////////////////////////
    auto t3 = std::chrono::high_resolution_clock::now();
    GetGridconncet();

    std::pair<std::vector<int>, int> path;
    int Start_Num = Coord2Num(std::pair<int, int>(sx, sy));
    int End_Num = Coord2Num(std::pair<int, int>(tx, ty));

    cal_cost();

    path = GetPath(End_Num);
    auto t4 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration3 = t4 - t3;
    printf("Waited %f ms.\n", duration3.count());

    return path;
}

//////////////////////////////////////////////////////////
//////////////////write your output///////////////////////
//////////////////////////////////////////////////////////

void mono ::output(int argc, char **argv, std::pair<std::vector<int>, int> array)
{
    fstream fout;

    fout.open(argv[2], fstream::out);

    if (!fout.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        exit(1);
    }

    std::pair<int, int> StartCoord;
    std::pair<int, int> EndCoord;
    fout << "RoutingCost " << array.second << endl;
    fout << "RoutingPath " << array.first.size() - 1 << endl;

    for (int i = 1; i < array.first.size() - 1; ++i)
    {
        StartCoord = Num2Coord(array.first[i]);
        EndCoord = Num2Coord(array.first[i + 1]);
        if (i == 1)
        {
            fout << StartCoord.first << " " << StartCoord.second << endl;
        }

        fout << EndCoord.first << " " << EndCoord.second << std::endl;
    }

    fout.close();

    return;
}
std::pair<int, int> mono::Num2Coord(int Num)
{
    int GridLength = this->By2 + 1;
    std::pair<int, int> Coord;

    Coord.first = Num / GridLength;
    Coord.second = Num % GridLength;
    return Coord;
}

// /////////////////////////////////////////////////////////////////////////////////

void mono::Init(int Start)
{
    dis.resize(Node_num);
    pred.resize(Node_num);
    for (int i = 0; i < Node_num; ++i)
    {
        dis[i] = Max_Dis;
        pred[i] = -1;
    }
    dis[Start] = 0;
}

void mono::Relaxation(int from, int to, int weight)
{
    if (dis[to] > dis[from] + weight)
    {
        dis[to] = dis[from] + weight;
        pred[to] = from;
    }
}

void mono::AddConnect(int from, int to, int weight)
{
    std::pair<int, int> Node = std::make_pair(to, weight);
    Adjacent[from].push_back(Node);
}

std::pair<std::vector<int>, int> mono::GetPath(int end)
{
    std::pair<std::vector<int>, int> result;
    int size = 1;
    bool Done = false;
    std::vector<int> Path;
    int Endnode = end;
    Path.push_back(Endnode);
    while (!Done)
    {
        Path.push_back(pred[Endnode]);
        // std::cout << pred[Endnode] << std::endl;
        if (pred[Endnode] == StartNode || pred[Endnode] == -1)
        {
            size++;
            Done = true;
            break;
        }
        else
        {
            Endnode = pred[Endnode];
            size++;
        }
    }
    Path.resize(size);

    std::reverse(Path.begin(), Path.end());

    result.first = Path;
    result.second = dis[dis.size() - 1];
    return result;
}

int mono::Coord2Num(std::pair<int, int> pos)
{
    return pos.first * GridLength + pos.second;
}

void mono::GetGridconncet()
{
    int Max = (Bx2 + 1) * (By2 + 1);

    for (int i = 0; i < Max - 1; ++i)
    {
        if (i + (By2 + 1) < Max)
        {
            Adjacent[i].push_back(std::make_pair(i + (By2 + 1), mono::default_cost));
        }
        if ((i + 1) % (By2 + 1))
        {
            Adjacent[i].push_back(std::make_pair(i + 1, mono::default_cost));
        }
    }
    // int i = 0;
    // for (std::vector<std::list<std::pair<int, int>>>::iterator itr = Adjacent.begin(); itr != Adjacent.end(); ++itr)
    // {
    //     if (i + (By2 + 1) < Node_num)
    //     {
    //         Adjacent[i].push_back(std::make_pair(i + (By2 + 1), mono::default_cost));
    //     }
    //     if ((i + 1) % (By2 + 1))
    //     {
    //         Adjacent[i].push_back(std::make_pair(i + 1, mono::default_cost));
    //     }
    //     ++i;
    // }

    UpdateCost_fix();
}

void mono::UpdateCost_fix()
{
    for (std::vector<NDcost>::iterator itr = NDcosts.begin(); itr != NDcosts.end(); ++itr)
    {
        int St = Coord2Num(std::pair<int, int>((*itr).getX1(), (*itr).getY1()));
        int To = Coord2Num(std::pair<int, int>((*itr).getX2(), (*itr).getY2()));
        int cost = (*itr).getValue();
        for (std::list<std::pair<int, int>>::iterator itra = Adjacent[St].begin(); itra != Adjacent[St].end(); ++itra)
        {
            if ((*itra).first == To)
            {
                (*itra).second += cost;
                break;
            }
        }
    }
}

void mono::cal_cost()
{
    for (int i = 0; i < (Bx2 + 1) * (By2 + 1); ++i)
    {
        for (std::list<std::pair<int, int>>::iterator itr = Adjacent[i].begin(); itr != Adjacent[i].end(); itr++)
        {
            Relaxation(i, (*itr).first, (*itr).second);
        }
    }
}
