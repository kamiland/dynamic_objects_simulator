/**
 * Kamil Tomasz Andrzejewski - 2020
*/

#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <windows.h>
#include "GlobalContext.hpp"
#include "AllModels.hpp"
#include "json/json.hpp"
#include "UsefulFunctions.hpp"
using namespace std;
using json = nlohmann::json;


int main() 
{
    /**
     * Reading JSON with configuration of simulation
     * 
    */
    json j;
    string RawJsonString = ReadFromFile("./json/simulation_context.json");
    j = json::parse(RawJsonString);


    /**
     * Preparing simulation context and parameters
     * 
    */
    GlobalContext Ctx;
    Ctx.SetSimulationTimeSec(j["context"]["time"]);
    Ctx.SetProbesCountPerSec(j["context"]["probes_per_second"]);

    auto Now = chrono::system_clock::now();
    auto DateStamp = chrono::system_clock::to_time_t(Now);
    srand((unsigned int) DateStamp);


    /**
     * Declaration of elementary objects and variables
     * 
    */
    vector <unsigned int> _NodesCount = j["neural_network"]["nodes_count"];
    vector <ActivationFunction> _ActivationFunctions = j["neural_network"]["activation_functions"];
    NeuralNetwork NN(_NodesCount, _ActivationFunctions);
    
    NN.Randomization(j["neural_network"]["weights_range"], j["neural_network"]["biases_range"], 
                     j["neural_network"]["control_constant_range"]);

    NeuralRegulator NR(NN);


    /**
     * Execution of simulation and research
     * 
    */
 
    EvolutionaryAlgorithm EVA(90, NN);

    for (int i = 0; i < 55; i++)
    {
        NR = EVA.EvolveNextGeneration();
    }


        // Run simulation
    DcMotor DcMotor;

    DcMotor.st.AngularVelocity = 0.0;
    DcMotor.st.RotorCurrent = 0.0;
    DcMotor.ext.U = 0;
    vector<double> DcHistory[3];
    double *Dc_x;
    double Input[] = {0, 0, 0};
    double ErrorIntegral = 0.0;
    double Setpoint = 150;

    for (int i = 0; i < 450; i++)
    {
        Input[0] = (Setpoint - DcMotor.st.AngularVelocity);
        Input[1] = DcMotor.st.AngularVelocity;
        Input[1] = DcMotor.st.RotorCurrent;
        DcMotor.ext.U = NR.CalculateOutput(Input)[0];

        // if(i == 80){ DcMotor.ext.Tl = 100;}
        // if(i == 190){ DcMotor.ext.Tl = 0;}
        // if(i == 310){ DcMotor.ext.Tl = -30;}

        Dc_x = DcMotor.ComputeNextState(0.01, &DcMotor);

        DcHistory[0].push_back(Dc_x[0]);
        DcHistory[1].push_back(Dc_x[1]);
        DcHistory[2].push_back(DcMotor.ext.U);
    }
    WriteToFile(DcHistory, "dc");

    cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    return 0;
}
