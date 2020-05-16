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
        FileName.append("-" + GetDateString());
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
    DcMotor DcMotor;
    Controller Pid(8, 4, 0.001);
    
    /**
     * Preparing simulation parameters
    */
    Ctx.SetSimulationTimeSec(5);
    Ctx.SetProbesCountPerSec(1000);


    DcMotor.st.AngularVelocity = 0.0;
    DcMotor.st.RotorCurrent = 0.0;
    DcMotor.ext.U = 0;
    vector<double> DcHistory[2];
    double *Dc_x;

    for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
    {
        DcMotor.ext.U = Pid.CalculateOutput(150, DcMotor.st.AngularVelocity, 0.001);

        if(i == 1000){ DcMotor.ext.Tl = 80;}
        if(i == 2500){ DcMotor.ext.Tl = 0;}
        if(i == 3500){ DcMotor.ext.Tl = -10;}

        Dc_x = DcMotor.ComputeNextState(0.001, &DcMotor);


        DcHistory[0].push_back(Dc_x[0]);
        DcHistory[1].push_back(Dc_x[1]);
    }
    WriteToFile(DcHistory);

    std::cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    return 0;
}
