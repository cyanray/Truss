#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <initializer_list>

#include <TrussDocument/TrussDocument.hpp>

#include "K2Truss.hpp"
#include "KDoc.hpp"

using namespace std;
using namespace Truss;
using namespace Truss::Tools;


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "Usage: K2Truss <input.k> <output.truss>" << endl;
        return 1;
    }

    try
    {
        ifstream ifs(argv[1]);
        if (!ifs.is_open())
        {
            cout << "Failed to open file: " << argv[1] << endl;
            return 1;
        }
        ofstream ofs(argv[2]);
        if (!ofs.is_open())
        {
            cout << "Failed to open file: " << argv[2] << endl;
            return 1;
        }

        string kdoc((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
        auto result = KDoc::Parse(kdoc);

        auto tdoc = TrussDocument::Object();

        for (auto& doc: result)
        {
            if (doc.Keyword == "NODE")
            {
                auto nodes = GetNodes(doc);
                tdoc.Add("Node", nodes);
            }
            else if (doc.Keyword == "ELEMENT_SHELL")
            {
                auto triangles = GetTriangles(doc);
                tdoc.Add("Element", triangles);
            }
        }

        ofs << tdoc.ToString() << endl;
        cout << "Done." << endl;

    }
    catch (exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}