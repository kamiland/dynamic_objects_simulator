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
using namespace std;

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
    SS << std::put_time(std::localtime(&DateStamp), "%y.%m.%d-%H_%M_%S");
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


int main() 
{
    /**
     * Declaration of elementary objects and variables
    */
    GlobalContext Ctx;
    Controller Pid(1, 1, 0);
    ReferenceDcMotor ReferenceMotor(0, 0);
    vector<DcMotorState> DcHistory;
    DoublePendulum DIP;
    DIP.ext.U = 0;
    DIP.ext.Z0 = 0;
    DIP.ext.Z1 = 0;
    DIP.ext.Z2 = 0;
    memset(DIP.st.State, 0, sizeof(DIP.st.State));
    DIP.ComputeNextState(0.001);
    
    /**
     * Preparing simulation parameters
    */
    Ctx.SetSimulationTimeSec(1);
    Ctx.SetProbesCountPerSec(1000);

    DcHistory = ReferenceMotor.Simulate(Ctx.GetProbesCountTotal(), 0.001);

    DcMotor dcmotor;
    dcmotor.st.State[0] = 0.0;
    dcmotor.st.State[1] = 0.0;
    dcmotor.ext.U = 230;
    vector<double> history[2];
    double *x;

    for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
    {
        x = dcmotor.ComputeNextState(0.001);
        history[0].push_back(x[0]);
        history[1].push_back(x[1]);
    }

    for(int i = 0; i < DcHistory.size(); i++)
    {
        // cout << DcHistory[i].rotorCurrent - history[i].rotorCurrent << ", " << DcHistory[i].angularVelocity - history[i].angularVelocity << endl;
    }

    WriteToFile(history);

    std::cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    return 0;
}
