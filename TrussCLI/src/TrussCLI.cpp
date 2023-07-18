#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "CsvHeaders.hpp"
#include "csv_to_string.hpp"
#include "Csv/CsvWriter.hpp"
#include <Eigen/Dense>

#include <Truss/Solver.hpp>
#include <TrussDocument/TrussDocument.hpp>
#include <backward.hpp>

#ifdef _WIN32
#include "UTF8CodePage.hpp"
#endif

using namespace std;
using namespace Truss;
using namespace Eigen;
using namespace Csv;


int main(int argc, char* argv[])
{
#ifdef _WIN32
    UTF8CodePage _use_utf8_codepage;
#endif

    if (argc < 3)
    {
        cout << "Usage: TrussCLI.exe <input_file_path> <output_folder>" << endl;
        ::exit(-1);
    }
    string input_file_path = argv[1];
    string output_folder = argv[2];
    ifstream input_file(input_file_path);
    if (!input_file.is_open())
    {
        cout << "Failed to open input file." << endl;
        ::exit(-1);
    }
    std::string input((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    if (input.empty())
    {
        cout << "Input file is empty." << endl;
        ::exit(1);
    }
    ofstream basic_output_file(output_folder + "/basic_output.txt", fstream::out | fstream::trunc);
    if (!basic_output_file.is_open())
    {
        cout << "Failed to open basic output file." << endl;
        ::exit(-1);
    }

    ofstream nodes_output_file(output_folder + "/nodes.csv", fstream::out | fstream::trunc);
    if (!nodes_output_file.is_open())
    {
        cout << "Failed to open nodes output file (nodes.csv)." << endl;
        ::exit(-1);
    }
    auto nodes_csv_file = CsvWriter(nodes_output_file);
    ofstream elements_output_file(output_folder + "/elements.csv", fstream::out | fstream::trunc);
    if (!elements_output_file.is_open())
    {
        cout << "Failed to open elements output file (elements.csv)." << endl;
        ::exit(-1);
    }
    auto elements_csv_file = CsvWriter(elements_output_file);

    nodes_csv_file << NodesCsvHeader;
    elements_csv_file << ElementsCsvHeader;

    try
    {
        TrussDocument doc = TrussDocument::Parse(input);
        TrussSolver solver;
        solver.LoadTrussDocument(doc);
        auto index = solver.GetFreedomIndex();
        auto K = solver.GetK(index);
        auto F = solver.GetF(index);

        basic_output_file << "[K] = " << endl;
        basic_output_file << K << endl;
        basic_output_file << "{F} = " << endl;
        basic_output_file << F << endl;
        // VectorXf D = K.colPivHouseholderQr().solve(F);
        // VectorXf D = K.fullPivLu().solve(F);
        Truss::VectorX D = K.bdcSvd(ComputeThinU | ComputeThinV).solve(F);
        basic_output_file << "{D} = " << endl;
        basic_output_file << D << endl;

        Truss::VectorX D_full = Truss::VectorX::Zero(solver.GetKSize());
        for (int i = 0; i < index.size(); ++i)
        {
            D_full(index[i]) = D(i);
        }

        auto& res = solver.GetResources();

        // nodes data
        for (auto& [key, node]: res.Nodes)
        {
            DisplacementVector node_displacement = D_full.segment<ALL_DOF>(node.Id * ALL_DOF);
            nodes_csv_file << std::make_tuple(node.Id, node.Key, node.X, node.Y, node.Z, node_displacement);
        }
        // Elements data
        for (auto& [key, element]: res.Elements)
        {
            auto node_count = element->GetNodeCount();
            vector<ID> nodes = element->GetNodeIds();
            // Compute displacement for each node
            Truss::VectorX displacement = Truss::VectorX::Zero(ALL_DOF * node_count);
            for (int i = 0; i < node_count; ++i)
            {
                auto node = nodes[i];
                auto node_displacement = D_full.segment<ALL_DOF>(node * ALL_DOF);
                displacement.segment<ALL_DOF>(i * ALL_DOF) = node_displacement;
            }
            // Compute stress
            auto stress = element->CalculateStress(displacement);
            // Write to csv
            Eigen::Vector<int, 8> node_keys(nodes.data());
            node_keys.segment(node_count, 8 - node_count).fill(-1);
            elements_csv_file << std::make_tuple(element->Key, element->GetElementName(), node_count, node_keys, stress);
        }

        cout << "Done." << endl;
    }
    catch (const exception& exception)
    {
        using namespace backward;
        std::ostringstream ss;

        StackTrace stackTrace;
        TraceResolver resolver;
        stackTrace.load_here();
        resolver.load_stacktrace(stackTrace);

        for (std::size_t i = 0; i < stackTrace.size(); ++i)
        {
            const ResolvedTrace trace = resolver.resolve(stackTrace[i]);

            ss << "#" << i << " at " << trace.object_function << ", " << trace.addr << ";\n";
        }

        cout << ss.str() << endl;
        cout << "Error: " << exception.what() << endl;
    }

    return 0;
}
