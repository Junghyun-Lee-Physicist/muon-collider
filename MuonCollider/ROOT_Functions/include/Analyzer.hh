
#ifndef ANALYZER_HH
#define ANALYZER_HH

//#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TChain.h>

#include <vector>
#include <cmath>
#include <iostream>
#include <map>

using namespace std;


struct Point {
   
    // This Struct is the conatainer for Step point information
    // which are stored at n-tuple

    double x, y, z, energy, time;
    bool visited = false;
    int clusterID = -1;
    // clusterID = -1 means [ un-assigne ]
};


class Analyzer {

public:
  
    Analyzer(const std::string& fPath, const int randomSeedNumber);
    ~Analyzer();

    void AnalyzeNtuple(const int randomSeedNumber);

    void PrintLogicOfThisClass();

private:

    void SetHistograms();
    double distance(const Point& a, const Point& b);
    std::vector<Point*> regionQuery(const Point& point, double eps);
    void expandCluster(Point& point, std::vector<Point*>& neighbors, int clusterID, double eps, int minPts);
    void DBSCAN(double eps, int minPts);
 
    std::vector<Point> points;
    TChain* chain;
    std::string outputDirPath;   

    // ROOT histograms
    TH1D* hTotalEnergyDeposit;
    TH1D* hTotalEdiff;
    TH3D* hClusterPositions;
    TH1D* hClusterEdiff;
    TH2D* hClusterPosiEdiff;
    TH1D* hClusterPositions_z;
    TH1D* hStepEnergyDeposit;
    TH1D* hStepEdiff;
    TH2D* hAllStepPosiEdep;
    TH2D* hAllStepPosiEdiff;

};

#endif // ANALYZER_HH
