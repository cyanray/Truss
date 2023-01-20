#include <iostream>
#include <vector>
#include <magic_enum.hpp>
#include <fstream>
#include <Eigen/Dense>
#include <Truss/Solver.hpp>

using namespace std;
using namespace Truss;
using namespace Eigen;


int main()
{
#ifdef _WIN32
    system("chcp 65001");
#endif

    ifstream tfile("example_truss/hello.truss");
    std::string input((std::istreambuf_iterator<char>(tfile)), std::istreambuf_iterator<char>());

    TrussSolver solver;
    TrussDocument doc = TrussDocument::Parse(input);
    solver.LoadTrussDocument(doc);
    auto K = solver.GetSimplifiedK();
    auto F = solver.GetSimplifiedF();
    VectorXf D = K.colPivHouseholderQr().solve(F);

    cout << "[K] = " << endl;
    cout << K << endl;
    cout << "{F} = " << endl;
    cout << F << endl;
    cout << "{D} = " << endl;
    cout << D << endl;

    return 0;
}
