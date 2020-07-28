#ifndef USEFUL_FUNCTIONS
#define USEFUL_FUNCTIONS

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

#endif