#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace Truss::Tools
{
    class KDoc
    {
    public:
        string Keyword{};
        string Title{};
        vector<vector<double>> Data{};

        static vector<KDoc> Parse(const string& kdoc);
    };
}