#include <iostream>
#include <cmath>
#include <chrono>
#include <sstream>

#include "laba.h"
#include "../math/optimization.h"
#include "../utils/fileUtils.h"

struct laba3_params
{
    file_utils::OutputData simulationOutput;
    file_utils::OutputData targetFuncOutput;
    std::string targetFuncName;
};

std::tuple<math_labs::SimulationParams, math_labs::BorderParams, laba3_params> getInputParams(int argc, char* argv []);

math_labs::TargetFunc getTargetFunc(const std::string funcName);

void countFuncMatrix(const math_labs::TargetFunc targetFunc, const math_labs::BorderParams borders, file_utils::OutputData& output);

using SimulationFunc = std::function<std::pair<double, double>()>;
double benchmarkFunc(const SimulationFunc simulationFunc, std::stringstream& output);


int cpp_labs::startLab3(int argc, char* argv [])
{

    auto[simulation, borders, params] = getInputParams(argc, argv);

    math_labs::TargetFunc targetFunc = getTargetFunc(params.targetFuncName);
    countFuncMatrix(targetFunc, borders, params.targetFuncOutput);

    std::stringstream simulationCsv;
    simulationCsv << "label,x,y,time\n";

    simulationCsv << "boltzmann,";
    SimulationFunc b = std::bind(&math_labs::simulateBoltzmann, targetFunc, simulation, borders);
    benchmarkFunc(b, simulationCsv);
    
    simulationCsv << "\ncauchy,";
    SimulationFunc c = std::bind(&math_labs::simulateCauchy, targetFunc, simulation, borders);
    benchmarkFunc(c, simulationCsv);
    simulationCsv << "\n";
    
    params.simulationOutput.output = simulationCsv.str();
    file_utils::writeOutput(params.simulationOutput);

    return 0;
}


std::tuple<math_labs::SimulationParams, math_labs::BorderParams, laba3_params> getInputParams(int argc, char* argv [])
{
    math_labs::SimulationParams simulation;
    simulation.alpha = 0.99;
    simulation.T_start = 100;
    simulation.T_end = 1e-6;
    simulation.iterations_per_temp = 1000;
    simulation.max_iterations = 30000;

    math_labs::BorderParams borders;
    borders.x_min = -5.12;
    borders.x_max = 5.12;
    borders.y_min = -5.12;
    borders.y_max = 5.12;

    laba3_params params;
    params.targetFuncName = "rostrigin";

    params.simulationOutput.directory = "./laba3_results";
    params.simulationOutput.fileName = "simulationResults.csv";

    params.targetFuncOutput.directory = "./laba3_results";
    params.targetFuncOutput.fileName = "targetFunc.csv";

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-alpha" && i+1 < argc)
            simulation.alpha = std::stod(argv[i+1]);
        else if (arg == "-Tstart")
            simulation.T_start = std::stod(argv[i+1]);
        else if (arg == "-Tend")
            simulation.T_end = std::stod(argv[i+1]);
        else if (arg == "-iter")
            simulation.iterations_per_temp = std::stoi(argv[i+1]);
        else if (arg == "-maxIter")
            simulation.max_iterations = std::stoi(argv[i+1]);
        else if (arg == "-xmin")
            borders.x_min = std::stod(argv[i+1]);
        else if (arg == "-xmax")
            borders.x_max = std::stod(argv[i+1]);
        else if (arg == "-ymin")
            borders.y_min = std::stod(argv[i+1]);
        else if (arg == "-ymax")
            borders.y_max = std::stod(argv[i+1]);
        else if (arg == "-dir") {
            params.simulationOutput.directory = argv[i+1];
            params.targetFuncOutput.directory = argv[i+1];
        }
        else if (arg == "-tf")
            params.targetFuncOutput.fileName = argv[i+1];
        else if (arg == "-sr")
            params.simulationOutput.fileName = argv[i+1];
        else if (arg == "-tft")
            params.targetFuncName = argv[i+1];
    }
    
    std::cout << "Input params:\n"
            << "  alpha = " << simulation.alpha << "\n"
            << "  T_start = " << simulation.T_start << "\n"
            << "  T_end = " << simulation.T_end << "\n"
            << "  iterations_per_temp = " << simulation.iterations_per_temp << "\n"
            << "  max_iterations = " << simulation.max_iterations << "\n"
            << "  x: [" << borders.x_min << ", " << borders.x_max << "]\n"
            << "  y: [" << borders.y_min << ", " << borders.y_max << "]\n";

    return {simulation, borders, params};
}

math_labs::TargetFunc getTargetFunc(const std::string funcName)
{
    if (funcName == "rosenbrock") {
        return math_labs::rosenbrock;
    }
    else if (funcName == "ackley") {
        return math_labs::ackley;
    }
    else if (funcName == "beale") {
        return math_labs::beale;
    }
    else if (funcName == "rastrigin") {
        return math_labs::rastrigin;
    }

    std::cout << "Using default target func: rastrigin\n";
    return math_labs::rastrigin;
}

void countFuncMatrix(const math_labs::TargetFunc targetFunc, const math_labs::BorderParams borders, file_utils::OutputData& data)
{
    std::stringstream output;
    output << "x,y,z\n";

    double yStep = 0.1;
    double xStep = 0.1;
    for (double y = borders.y_min; y < borders.y_max; y+=yStep)
    {
        for (double x = borders.x_min; x < borders.x_max; x+=xStep)
        {
            double z = targetFunc(x, y);
            output << std::to_string(x) << "," << std::to_string(y) << "," << std::to_string(z) << "\n";
        }
    }

    data.output = output.str();
    file_utils::writeOutput(data);
}

double benchmarkFunc(const SimulationFunc simulationFunc, std::stringstream& output)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;

    start = std::chrono::high_resolution_clock::now();
    auto[x, y] = simulationFunc();    
    end = std::chrono::high_resolution_clock::now();

    std::cout << "x: " << std::to_string(x) << "\ny: " << std::to_string(y) << "\n";

    double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Executed in " << std::to_string(milliseconds) << " mls\n";
    
    output << std::to_string(x) << "," << std::to_string(y) << "," << std::to_string(milliseconds);
    return milliseconds;
}

