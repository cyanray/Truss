#include <fstream>
#include <iostream>
#include <vector>

#include <Eigen/Dense>
#include <Truss/Solver.hpp>
#include <TrussDocument/TrussDocument.hpp>

#ifdef _WIN32
#include "UTF8CodePage.hpp"
#endif

using namespace std;
using namespace Truss;
using namespace Eigen;


int main(int argc, char* argv[])
{
#ifdef _WIN32
    UTF8CodePage _use_utf8_codepage;
#endif

    if (argc < 2)
    {
        cout << "Usage: TrussCLI.exe <input_file_path>" << endl;
        ::exit(-1);
    }
    string input_file_path = argv[1];
    ifstream input_file(input_file_path);
    std::string input((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());


    try
    {
        TrussDocument doc = TrussDocument::Parse(input);
        TrussSolver solver;
        solver.LoadTrussDocument(doc);
        auto index = solver.GetFreedomIndex();
        auto K = solver.GetK(index);
        auto F = solver.GetF(index);
        VectorXf D = K.colPivHouseholderQr().solve(F);

        cout << "[K] = " << endl;
        cout << K << endl;
        cout << "{F} = " << endl;
        cout << F << endl;
        cout << "{D} = " << endl;
        cout << D << endl;
    }
    catch (const exception& exception)
    {
        cout << "Error: " << exception.what() << endl;
    }

    return 0;
}
