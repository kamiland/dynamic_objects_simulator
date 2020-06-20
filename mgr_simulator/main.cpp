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

    /**
     * Declaration of elementary objects and variables
    */
    DcMotor DcMotor;
    Controller Pid(0, 0, 0);
    PidSettingsFromJson(j, Pid);


    if (true == j["pid"]["saturation"]["set"])
    {
        Pid.SetSaturation(j["pid"]["saturation"]["min"], j["pid"]["saturation"]["max"]);
    }

    SeriesRLC Rlc;
    /**
     * Execution of simulation and research
    */

    Rlc.InitParameters(j["series_rlc"]["parameters"]["R"], j["series_rlc"]["parameters"]["L"], j["series_rlc"]["parameters"]["C"]);
    Rlc.st.CapacitorVoltage = j["series_rlc"]["init_state"]["capacitor_voltage"];
    Rlc.st.CircuitCurrent = j["series_rlc"]["init_state"]["circuit_current"];
    vector<double> RlcHistory[2];
    double *Rlc_x;

    vector <double> Time = j["series_rlc"]["external_forces"]["U"]["time"];
    vector <double> Value = j["series_rlc"]["external_forces"]["U"]["value"];

    for (unsigned int t = 0; t < Ctx.GetProbesCountTotal(); t++)
    {
        for(unsigned int j = 0; j < Time.size(); j++)
        {
            if(TimeCheck(t, Time[j], Ctx.GetStep()))
            {
                Rlc.ext.U = Value[j];
                Time.erase(Time.begin() + j);
                Value.erase(Value.begin() + j);
            }
        }   

        Rlc_x = Rlc.ComputeNextState(0.001, &Rlc);

        RlcHistory[0].push_back(Rlc_x[0]);
        RlcHistory[1].push_back(Rlc_x[1]);
    }
    WriteToFile(RlcHistory, "rlc");

    std::cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    return 0;
}
