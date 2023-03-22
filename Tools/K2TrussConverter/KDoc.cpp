#include "KDoc.hpp"
#include <sstream>
using namespace std;

namespace Truss::Tools
{
    vector<KDoc> KDoc::Parse(const string& kdoc)
    {
        vector<KDoc> result;
        KDoc* last = nullptr;
        istringstream iss(kdoc);
        string line;
        while (getline(iss, line))
        {
            // remove leading space
            line.erase(0, line.find_first_not_of(' '));
            if (line.empty()) continue;
            char first_char = line[0];
            if (first_char == '*')
            {
                last = &result.emplace_back(KDoc{line.substr(1)});
                continue;
            }
            else if (::isdigit(first_char))
            {
                if (last == nullptr) throw runtime_error("KDoc::Parse: Data without keyword.");
                istringstream row(line);
                vector<double> data;
                double value;
                while (row >> value) data.push_back(value);
                last->Data.push_back(data);
                continue;
            }
            else if (::isalpha(first_char))
            {
                if (last == nullptr) throw runtime_error("KDoc::Parse: Title without keyword.");
                last->Title = line;
                continue;
            }
        }
        return result;
    }
}// namespace Truss::Tools
