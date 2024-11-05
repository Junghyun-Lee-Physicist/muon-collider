#include <TFile.h>
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
    double x, y, z, energy, time;
    bool visited = false;
    int clusterID = -1; // -1 means unassigned
};

double distance(const Point& a, const Point& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

std::vector<Point*> regionQuery(std::vector<Point>& points, const Point& point, double eps) {
    std::vector<Point*> neighbors;
    for (auto& p : points) {
        if (distance(point, p) <= eps) {
            neighbors.push_back(&p);
        }
    }
    return neighbors;
}

void expandCluster(std::vector<Point>& points, Point& point, std::vector<Point*>& neighbors, int clusterID, double eps, int minPts) {
    point.clusterID = clusterID;
    std::vector<Point*> seeds = neighbors;

    for (size_t i = 0; i < seeds.size(); ++i) {
        Point* currentP = seeds[i];
        if (!currentP->visited) {
            currentP->visited = true;
            std::vector<Point*> currentNeighbors = regionQuery(points, *currentP, eps);
            if (currentNeighbors.size() >= minPts) {
                seeds.insert(seeds.end(), currentNeighbors.begin(), currentNeighbors.end());
            }
        }
        if (currentP->clusterID == -1) {
            currentP->clusterID = clusterID;
        }
    }
}

void DBSCAN(std::vector<Point>& points, double eps, int minPts) {
    int clusterID = 0;
    for (auto& point : points) {
        if (point.visited) {
            continue;
        }
        point.visited = true;
        std::vector<Point*> neighbors = regionQuery(points, point, eps);
        if (neighbors.size() < minPts) {
            point.clusterID = 0; // Mark as noise
        } else {
            clusterID++;
            expandCluster(points, point, neighbors, clusterID, eps, minPts);
        }
    }
}

void AnalyzeNtuple() {
  
    TString path = "/u/user/jhlee/Storage/MuonCollider";
    ////TFile* file = TFile::Open(path + "ntupleMuonCollider_1.root" );
    ////TTree* tree = (TTree*)file->Get("MuonSim");

    TChain* tree = new TChain("MuonSim");
    for (int i = 0; i < 1; ++i) {
        TString filePath = path + TString::Format("/ntupleMuonCollider_%d.root", i);
        tree->Add(filePath);
    }


    std::vector<double>* Position_x = nullptr;
    std::vector<double>* Position_y = nullptr;
    std::vector<double>* Position_z = nullptr;
    std::vector<double>* KineticEnergy = nullptr;
    std::vector<double>* EnergyDeposit = nullptr;
    std::vector<double>* LocalTime = nullptr;
    std::vector<double>* GlobalTime = nullptr;
    std::vector<int>* ProcessNumber = nullptr;

    tree->SetBranchAddress("Position_x", &Position_x);
    tree->SetBranchAddress("Position_y", &Position_y);
    tree->SetBranchAddress("Position_z", &Position_z);
    tree->SetBranchAddress("KineticEnergy", &KineticEnergy);
    tree->SetBranchAddress("EnergyDeposit", &EnergyDeposit);
    tree->SetBranchAddress("LocalTime", &LocalTime);
    tree->SetBranchAddress("GlobalTime", &GlobalTime);
    tree->SetBranchAddress("ProcessNumber", &ProcessNumber);

    TH1D* hTotalEnergyDeposit    = new TH1D("hTotalEnergyDeposit"    , ";Total Energy Deposit [GeV]", 50, 0, 150);
    TH3D* hClusterPositions      = new TH3D("hClusterPositions"      , ";x [m];y [m];z [m]", 10, -0.01, 0.01, 10, -0.01, 0.01, 50, 0, 50.0);
    TH1D* hClusterEnergyDeposits = new TH1D("hClusterEnergyDeposits" , ";Energy Deposit per Cluster [MeV]", 50, 0, 250.0);
    TH2D* hClusterPosiEdep       = new TH2D("hClusterPosiEdep"       , ";Clustered Position [m];Edep [MeV]", 50, 0, 50, 50, 0, 250);
    TH1D* hClusterPositions_z    = new TH1D("hClusterPositions_z"    , ";Cluster Z Positions [m]", 50, 0, 50.0);
    TH1D* hStepEnergyDeposit     = new TH1D("hStepEnergyDeposit"     , ";Step Energy Deposit [MeV]", 50, 0, 150.0);
    TH2D* hAllStepPosiEdep       = new TH2D("hAllStepPosiEdep"       , ";Step Position [m];Edep [MeV]", 50, 0, 50, 50, 0, 150);

    Long64_t nEntries = tree->GetEntries();
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
        tree->GetEntry(entry);

        if (Position_x->size() != Position_y->size() || Position_x->size() != Position_z->size() ||
            Position_x->size() != KineticEnergy->size() || Position_x->size() != EnergyDeposit->size() ||
            Position_x->size() != LocalTime->size() || Position_x->size() != GlobalTime->size() ||
            Position_x->size() != ProcessNumber->size()) {
            std::cerr << "Error: Vector sizes do not match!" << std::endl;
            return;
        }

        std::vector<Point> points;
        double totalEnergyDeposit = 0.0;

        for (size_t i = 0; i < Position_x->size(); ++i) {

            if( (*Position_z)[i] > 50.0 ) continue;

            points.push_back(Point{
                (*Position_x)[i], (*Position_y)[i], (*Position_z)[i],
                (*EnergyDeposit)[i], (*GlobalTime)[i]
            });
            hStepEnergyDeposit->Fill((*EnergyDeposit)[i]);
            totalEnergyDeposit += (*EnergyDeposit)[i];
            hAllStepPosiEdep->Fill( (*Position_z)[i], (*EnergyDeposit)[i] );
           
        }

        hTotalEnergyDeposit->Fill(totalEnergyDeposit / 1000.0); // MeV to GeV

        double eps = 0.01; // 1 cm
        int minPts = 1; 
        DBSCAN(points, eps, minPts);

        std::map<int, double> clusterEnergyMap;
        std::map<int, std::vector<Point>> clusters;

        for (const auto& point : points) {
            if (point.clusterID > 0) {
                clusters[point.clusterID].push_back(point);
                clusterEnergyMap[point.clusterID] += point.energy;
            }
        }

        for (const auto& cluster : clusters) {
            double weightedSumX = 0.0, weightedSumY = 0.0, weightedSumZ = 0.0, totalEnergy = 0.0;
            for (const auto& point : cluster.second) {
                weightedSumX += point.x * point.energy;
                weightedSumY += point.y * point.energy;
                weightedSumZ += point.z * point.energy;
                totalEnergy += point.energy;
            }
            double meanX = weightedSumX / totalEnergy;
            double meanY = weightedSumY / totalEnergy;
            double meanZ = weightedSumZ / totalEnergy;

            if(meanZ > 50.01) cout<<"over 51 meanZ : "<<meanZ<<", and it's total energy : "<<totalEnergy<<endl;
            if(meanZ < 50.01 && meanZ > 50.0) meanZ = 50.0;

            hClusterPositions->Fill(meanX, meanY, meanZ);
            hClusterPositions_z->Fill(meanZ);
            hClusterEnergyDeposits->Fill(totalEnergy);
            hClusterPosiEdep->Fill(meanZ, totalEnergy);
        }

        if (entry % (nEntries / 10) == 0) { // 10% 진행 상황 출력
            std::cout << "Progress: " << (100 * entry / nEntries) << "%" << std::endl;
        }
    }

    TFile* outFile = new TFile("results.root", "RECREATE");
    //TFile* outFile = new TFile("results_10.root", "RECREATE");

    hTotalEnergyDeposit->Write();
    hClusterPositions->Write();
    hClusterEnergyDeposits->Write();
    hClusterPosiEdep->Write();
    hClusterPositions_z->Write();
    hStepEnergyDeposit->Write();
    hAllStepPosiEdep->Write();


    outFile->Close();

    delete tree;
}

int main() {
    AnalyzeNtuple();
    return 0;
}

