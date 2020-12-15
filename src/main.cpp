#include "parser.h"
#include <chrono>
int mono::default_cost = 0;

int main(int argc, char **argv)
{
    cout << "############################################" << endl;
    cout << "#            Monotonic Routing             #" << endl;
    cout << "############################################" << endl;

    mono monotonic;
    /////////////////////parser/////////////////////
    monotonic.parser(argc, argv);
    cout << "............parser done!!!!!!.............." << endl;
    cout << "............parser done!!!!!!.............." << endl;
    // auto t3 = std::chrono::high_resolution_clock::now();
    
    std::pair<std::vector<int>, int> path = monotonic.routing();

    // auto t4 = std::chrono::high_resolution_clock::now();

    // std::chrono::duration<double, std::milli> duration3 = t4 - t3;
    // printf("Waited %f ms.\n", duration3.count());
    cout << "............monotonic routing done!!!!!!.............." << endl;

    /////////////////////output/////////////////////
    monotonic.output(argc, argv, path);
    cout << "............output done!!!!!!.............." << endl;

    return 0;
}
