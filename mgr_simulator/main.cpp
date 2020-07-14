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
using namespace std;
using json = nlohmann::json;

bool DirExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
  {
    return false;  //something is wrong with your path!
  }

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
  {
    return true;   // this is a directory!
  }

  return false;    // this is not a directory!
}

string GetDateString()
{
    std::stringstream SS;
    auto Now = std::chrono::system_clock::now();
    auto DateStamp = std::chrono::system_clock::to_time_t(Now);
    SS << std::put_time(std::localtime(&DateStamp), "%d.%m.%y-%H_%M_%S");
    return SS.str();
}

template <typename T, size_t N>
void WriteToFile(T (&History)[N], string FileName = "", string FileType = "txt", uint16_t Precision = 8)
{
    ofstream fout;
    uint16_t StateCount = N;
    const string Path = "./data/";

    if(0 == FileName.compare(""))
    {
        FileName = GetDateString();
    }
    else
    {
        // FileName.append("-" + GetDateString());
    }
    
    FileName.append("." + FileType);

    if (CreateDirectory(Path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
    {
        fout.open(Path + FileName);

        for (uint32_t i = 0; i < History[0].size(); i++)
        {
            for(uint16_t s = 0; s < StateCount; s++)
            {
                fout << setprecision(Precision) << History[s].at(i); if(s < StateCount-1) {fout << ", ";}; 
            }
            fout << "\n"; 
        }

        fout.close();
    }
    else
    {
        cout << "[ERROR] Failed to create directory." << endl;
    }
}

string ReadFromFile(string FileName)
{
    ifstream Fin(FileName);
    string Str((istreambuf_iterator<char>(Fin)), istreambuf_iterator<char>());
    return Str;
}

bool TimeCheck(int i, double Time, double Step)
{
    if(i * Step == Time)
    {
        return true;
    }
    else if ((i * Step - Step) < Time < (i * Step + Step))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PidSettingsFromJson(const json& j, Controller& p) 
{
    j.at("pid").at("kp").get_to(p.Kp);
    j.at("pid").at("ki").get_to(p.Ki);
    j.at("pid").at("kd").get_to(p.Kd);
}

template <class T>
void PrintVector1D(T Vector, string Name)
{
    cout << Name << " size: " << Vector.size() << endl;
    for (auto it = Vector.begin(); it != Vector.end(); ++it)
    {                         
        cout << *it << " ";                                
    }
    cout << endl;
}

template <class T>
void PrintVector2D(T Vector, string Name)
{
    cout << Name << " size: " << Vector.size() << endl;
    for ( auto it = Vector.begin(); it != Vector.end(); ++it)
    {                                    
        for ( auto it2 = it->begin(); it2 != it->end(); ++it2)
        {                         
            cout << *it2 << " ";                                
        }              
        cout << endl;                                                                              
    }      
    cout << endl;
}

template <class T>
void PrintVector3D(T Vector, string Name)
{
    cout << Name << " size: " << Vector.size() << endl;
    for ( auto it = Vector.begin(); it != Vector.end(); ++it)
    {                                    
        for ( auto it2 = it->begin(); it2 != it->end(); ++it2)
        {                         
            for ( auto it3 = it2->begin(); it3 != it2->end(); ++it3)
            {                         
                cout << *it3 << " ";                                
            }                      
            cout << endl;          
        }              
        cout << endl;                                                                              
    }   
}

int main() 
{
    /**
     * Reading JSON with configuration of simulation
    */
    json j;
    string RawJsonString = ReadFromFile("./json/simulation_context.json");
    j = json::parse(RawJsonString);

    /**
     * Preparing simulation context and parameters
    */
    GlobalContext Ctx;
    Ctx.SetSimulationTimeSec(j["context"]["time"]);
    Ctx.SetProbesCountPerSec(j["context"]["probes_per_second"]);

    auto Now = std::chrono::system_clock::now();
    auto DateStamp = std::chrono::system_clock::to_time_t(Now);
    srand((unsigned int) DateStamp);

    /**
     * Declaration of elementary objects and variables
    */
    SeriesRLC Rlc;

    vector <unsigned int> _NodesCount = j["neural_network"]["nodes_count"];
    vector <ActivationFunction> _ActivationFunctions = j["neural_network"]["activation_functions"];
    
    NeuralNetwork NN(_NodesCount, _ActivationFunctions);
    
    NN.Randomization(j["neural_network"]["weights_range"],
                     j["neural_network"]["biases_range"],
                     j["neural_network"]["control_constant_range"]);

    NeuralRegulator NR(NN);

    cout << "***************************************" << endl;
    PrintVector2D(NR.NeuralNet.Biases, "NR Biases");
    PrintVector3D(NR.NeuralNet.Weights, "NR Weights");

    double Input4[] = {1.5, 2.5};
    vector <double> Output4;
    Output4 = NR.NeuralNet.Feedforward(Input4);

    PrintVector1D(Output4, "NR-NN output");

    vector <double> NRoutput = NR.CalculateOutput(Input4);

    PrintVector1D(NRoutput, "NR-NN output");
    cout << "***************************************" << endl;


    // Rlc.InitParameters(j["series_rlc"]["parameters"]["R"], j["series_rlc"]["parameters"]["L"], j["series_rlc"]["parameters"]["C"]);
    // Rlc.st.CapacitorVoltage = j["series_rlc"]["init_state"]["capacitor_voltage"];
    // Rlc.st.CircuitCurrent = j["series_rlc"]["init_state"]["circuit_current"];
    // vector<double> RlcHistory[2];
    // double *Rlc_x;
    // vector <double> Time = j["series_rlc"]["external_forces"]["U"]["time"];
    // vector <double> Value = j["series_rlc"]["external_forces"]["U"]["value"];
    // if(Time.size() > Value.size())
    // {
    //     cout << "[ERROR] Time vector is longer than Value vector. Terminatig the simulator." << endl;
    //     return (1);
    // }

    /**
     * Execution of simulation and research
    */

    // for (unsigned int t = 0; t < Ctx.GetProbesCountTotal(); t++)
    // {
    //     if(false == Time.empty() && TimeCheck(t, Time[0], Ctx.GetStep()))
    //     {
    //         Rlc.ext.U = Value[0];
    //         Time.erase(Time.begin());
    //         Value.erase(Value.begin());
    //     }
    //     Rlc_x = Rlc.ComputeNextState(0.001, &Rlc);
    //     RlcHistory[0].push_back(Rlc_x[0]);
    //     RlcHistory[1].push_back(Rlc_x[1]);
    // }
    // WriteToFile(RlcHistory, "rlc");

    DcMotor DcMotor;
    Controller Pid(j["pid"]["kp"], j["pid"]["ki"], j["pid"]["kd"]);
    Pid.SetSaturation(j["pid"]["saturation"]["min"], j["pid"]["saturation"]["max"]);

    DcMotor.st.AngularVelocity = 0.0;
    DcMotor.st.RotorCurrent = 0.0;
    DcMotor.ext.U = 0;
    vector<double> DcHistory[2];
    double *Dc_x;
 
    EvolutionaryAlgorithm EVA(70, NN);

    for (int i = 0; i < 30; i++)
    {
        EVA.EvolveNextGeneration();
    }

    // for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
    // {
    //     DcMotor.ext.U = Pid.CalculateOutput(150, DcMotor.st.AngularVelocity, 0.001);

    //     if(i == 1000){ DcMotor.ext.Tl = 80;}
    //     if(i == 2500){ DcMotor.ext.Tl = 0;}
    //     if(i == 3500){ DcMotor.ext.Tl = -10;}

    //     Dc_x = DcMotor.ComputeNextState(0.001, &DcMotor);


    //     DcHistory[0].push_back(Dc_x[0]);
    //     DcHistory[1].push_back(Dc_x[1]);
    // }
    // WriteToFile(DcHistory, "dc");




    cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    return 0;
}
