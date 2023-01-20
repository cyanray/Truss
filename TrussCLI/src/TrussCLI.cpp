#include <iostream>
#include <vector>
#include <magic_enum.hpp>
#include <fstream>
#include <Eigen/Dense>
#include <Truss/Solver.hpp>

using namespace std;
using namespace Truss;
using namespace Eigen;


int main(int argc, char* argv[])
{
#ifdef _WIN32
    system("chcp 65001");
#endif

    if (argc < 2)
    {
        cout << "Usage: TrussCLI.exe <input_file_path>" << endl;
        ::exit(-1);
    }
    string input_file_path = argv[1];
    ifstream input_file(input_file_path);
    std::string input((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

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
