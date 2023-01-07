#include <iostream>
#include <iomanip>
#include <vector>
#include <magic_enum.hpp>
#include <complex>
#include <fstream>
#include <streambuf>
#include <Eigen/Dense>
#include <Truss/Common/PlaneNode.hpp>
#include <Truss/Element/Element_Bar.hpp>
#include <Truss/Material//Material_Elastic.hpp>
#include <TrussDocument/TrussDocument.hpp>
#include <TrussDocument/Tokenizer.hpp>
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

    TrussSolver<> solver;
    TrussDocument doc = TrussDocument::Parse(input);
    solver.LoadTrussDocument(doc);
    auto K = solver.GetKSimplified();
    auto F = solver.GetFSimplified();
    VectorXf D = K.colPivHouseholderQr().solve(F);
    cout << "[K] = " << endl;
    cout << K << endl;
    cout << "{F} = " << endl;
    cout << F << endl;
    cout << "{D} = " << endl;
    cout << D << endl;

	return 0;
}
