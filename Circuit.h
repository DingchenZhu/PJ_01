#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct RINFO {
    string Name;
    string N1;
    string N2;
    double Value;
};

struct CINFO {
    string Name;
    string N1;
    string N2;
    double Value;
};

struct LINFO {
    string Name;
    string N1;
    string N2;
    double Value;
};

struct SourceINFO {
    string SourceName;
    string SourceN1;
    string SourceN2;
    string Sourcetype;
    double SourceDcValue;
    double SourceAcValue;
    double SourceFreq;
    double SourcePhase;
};

struct ControlledSource
{
    string ControlledName;
    string N1;
    string N2;
    string ctrlN1;
    string ctrlN2;
    double value;
    char type;      // E:VCVS,G:VCCS,F:CCCS,H:CCVS
};

struct MOSINFO {
    string MOSName;
    string MOSN1;
    string MOSN2;
    string MOSN3;
    string MOStype;
    double MOSW;
    double MOSL;
    int MOSID;
    string MOSMODEL;
};

struct ModelINFO {
    int ID;
    double VT;
    double MU;
    double COX;
    double LAMBDA;
    double CJ0;
};

// 数据存储结构
struct CircuitData {
    vector<RINFO> rinfo;
    vector<CINFO> cinfo;
    vector<LINFO> linfo;
    vector<SourceINFO> sourceinfo;
    vector<MOSINFO> mosinfo;
    vector<ModelINFO> modelinfo;
};
#endif
