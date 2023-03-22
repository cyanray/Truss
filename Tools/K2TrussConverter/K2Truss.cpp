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
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <kdoc file>" << endl;
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
        string kdoc((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
        auto result = KDoc::Parse(kdoc);
        for (auto& doc: result)
        {
            if (doc.Keyword == "NODE")
            {
                auto nodes = GetNodes(doc);
                cout << nodes.ToString() << endl;
            }
            else if (doc.Keyword == "ELEMENT_SHELL")
            {
                auto triangles = GetTriangles(doc);
                cout << triangles.ToString() << endl;
            }
        }
    }
    catch (exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}