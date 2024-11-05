
#include "Analyzer.hh"

Analyzer::Analyzer(const std::string& fPath, const int randomSeedNumber) : outputDirPath(fPath){

    chain = new TChain("MuonSim");
    if (outputDirPath.back() != '/') {
        outputDirPath += '/';
        cout << " -- [ Analyzer Log ] There is no [ / ] at the end of output directory path" << endl;
        cout << " -- [ Analyzer Log ] So [ / ] will be added at the output path" << endl;
    }
    TString outputPath = outputDirPath + TString::Format("ntupleMuonCollider_%d.root", randomSeedNumber);
    cout << " -- [ Analyzer Log ] Output file path : "<< outputPath << endl;
    chain->Add( outputPath );

    SetHistograms();
    AnalyzeNtuple(randomSeedNumber);
}


Analyzer::~Analyzer() {}

void PrintLogicOfThisClass(){
    cout << " -- [ Analyzer Log ] Information for meachin: TempTempTmepTmep";
}

void Analyzer::SetHistograms(){

    hTotalEnergyDeposit    = new TH1D("hTotalEnergyDeposit"    , ";Total Energy Deposit [MeV]", 100, 11000., 12000.);
    hTotalEdiff            = new TH1D("hTotalEdiff"            , ";Total Energy Diff [MeV]", 400, 0., 450000.);
    hClusterPositions      = new TH3D("hClusterPositions"      , ";x [cm];y [cm];z [cm]", 10, -5., 5., 10, -5., 5., 50, 0., 500.0);
    hClusterEdiff          = new TH1D("hClusterEdiff"          , ";Energy Diff per Cluster [MeV]", 50, 0., 2000.0);
    hClusterPosiEdiff      = new TH2D("hClusterPosiEdiff"      , ";Clustered Position [cm];Ediff [MeV]", 51, 0, 510., 50, 0., 2000.);
    hClusterPositions_z    = new TH1D("hClusterPositions_z"    , ";Cluster Z Positions [cm]", 51, 0, 510.0);
    hStepEnergyDeposit     = new TH1D("hStepEnergyDeposit"     , ";Step Energy Deposit [MeV]", 50, 0, 200.0);
    hStepEdiff             = new TH1D("hStepEdiff"             , ";Step Energy Diff [MeV]", 50, 0, 2000.0);
    hAllStepPosiEdep       = new TH2D("hAllStepPosiEdep"       , ";Step Position [cm];Edep [MeV]", 51, 0., 510., 50, 0, 200.);
    hAllStepPosiEdiff      = new TH2D("hAllStepPosiEiff"       , ";Step Position [cm];Ediff [MeV]", 51, 0., 510., 50, 0, 2000.);

}


double Analyzer::distance(const Point& a, const Point& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}


std::vector<Point*> Analyzer::regionQuery(const Point& point, double eps) {
    std::vector<Point*> neighbors;
    for (auto& p : points) {
        if (distance(point, p) <= eps) {
            neighbors.push_back(&p);
        }
    }
    return neighbors;
}


void Analyzer::expandCluster(Point& point, std::vector<Point*>& neighbors, int clusterID, double eps, int minPts) {
    point.clusterID = clusterID;
    std::vector<Point*> seeds = neighbors;

    for (size_t i = 0; i < seeds.size(); ++i) {
        Point* currentP = seeds[i];
        if (!currentP->visited) {
            currentP->visited = true;
            std::vector<Point*> currentNeighbors = regionQuery(*currentP, eps);
            if (currentNeighbors.size() >= minPts) {
                seeds.insert(seeds.end(), currentNeighbors.begin(), currentNeighbors.end());
            }
        }
        if (currentP->clusterID == -1) {
            currentP->clusterID = clusterID;
        }
    }
}


void Analyzer::DBSCAN(double eps, int minPts) {

//DBSCAN(Density-based spatial clustering of applications with noise) 

    int clusterID = 0;
    for (auto& point : points) {
        if (point.visited) {
            continue;
        }
        point.visited = true;
        std::vector<Point*> neighbors = regionQuery(point, eps);
        if (neighbors.size() < minPts) {
            point.clusterID = 0; // Mark as noise
        } else {
            clusterID++;
            expandCluster(point, neighbors, clusterID, eps, minPts);
        }
    }
}


void Analyzer::AnalyzeNtuple(const int randomSeedNumber) {

    std::vector<double>* Position_x = nullptr;
    std::vector<double>* Position_y = nullptr;
    std::vector<double>* Position_z = nullptr;
    std::vector<double>* KineticEnergy = nullptr;
    std::vector<double>* KineticEnergyDiff = nullptr;
    std::vector<double>* EnergyDeposit = nullptr;
    std::vector<double>* LocalTime = nullptr;
    std::vector<double>* GlobalTime = nullptr;
    std::vector<int>* ProcessNumber = nullptr;

    chain->SetBranchAddress("Position_x", &Position_x);
    chain->SetBranchAddress("Position_y", &Position_y);
    chain->SetBranchAddress("Position_z", &Position_z);
    chain->SetBranchAddress("KineticEnergy", &KineticEnergy);
    chain->SetBranchAddress("KineticEnergyDiff", &KineticEnergyDiff);
    chain->SetBranchAddress("EnergyDeposit", &EnergyDeposit);
    chain->SetBranchAddress("LocalTime", &LocalTime);
    chain->SetBranchAddress("GlobalTime", &GlobalTime);
    chain->SetBranchAddress("ProcessNumber", &ProcessNumber);

    Long64_t nEntries = chain->GetEntries();
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
        chain->GetEntry(entry);

        if (Position_x->size() != Position_y->size() || Position_x->size() != Position_z->size() ||
            Position_x->size() != KineticEnergy->size() || Position_x->size() != KineticEnergyDiff->size() || Position_x->size() != EnergyDeposit->size() ||
            Position_x->size() != LocalTime->size() || Position_x->size() != GlobalTime->size() ||
            Position_x->size() != ProcessNumber->size()) {
            std::cerr << "Error: Vector sizes do not match!" << std::endl;
            return;
        }

        points.clear();
        double totalEnergyDeposit = 0.0;
        double totalEdiff = 0.0;

        for (size_t i = 0; i < Position_x->size(); ++i) {

            //if( (*Position_z)[i] > 50.0 ) continue;
            if( (*KineticEnergyDiff)[i] <= 0.0 ) continue;

            points.push_back(Point{
                (*Position_x)[i], (*Position_y)[i], (*Position_z)[i],
                //(*EnergyDeposit)[i], (*GlobalTime)[i]
                (*KineticEnergyDiff)[i], (*GlobalTime)[i]
            });
            hStepEnergyDeposit->Fill((*EnergyDeposit)[i]);
            hStepEdiff->Fill((*KineticEnergyDiff)[i]);
            totalEnergyDeposit += (*EnergyDeposit)[i];
            totalEdiff         += (*KineticEnergyDiff)[i];
            hAllStepPosiEdep->Fill( (*Position_z)[i], (*EnergyDeposit)[i] );
            hAllStepPosiEdiff->Fill( (*Position_z)[i], (*KineticEnergyDiff)[i] );
            // This energy deposit is not perfect because muon can experice the pair production
            // Then muon's mass energy could be the secandaries's kinetic energy

        }

        hTotalEnergyDeposit->Fill(totalEnergyDeposit);
        hTotalEdiff->Fill(totalEdiff);


        double eps = 1.0; // 1 cm, eps means epsilon
        int minPts = 1; // This value could make the noise points which are not closed to anyone!
        DBSCAN(eps, minPts);

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

            //if(meanZ > 50.01) cout<<"over 51 meanZ : "<<meanZ<<", and it's total energy : "<<totalEnergy<<endl;
            //if(meanZ < 50.01 && meanZ > 50.0) meanZ = 50.0;

            hClusterPositions->Fill(meanX, meanY, meanZ);
            hClusterPositions_z->Fill(meanZ);
            hClusterEdiff->Fill(totalEnergy);
            hClusterPosiEdiff->Fill(meanZ, totalEnergy);
        }

        if ( (nEntries > 10) && (entry % (nEntries / 10) == 0) ) { // 10% 진행 상황 출력
            std::cout << "Progress: " << (100 * entry / nEntries) << "%" << std::endl;
        }
    }

    TString nameOfOutput = TString::Format("analyzerResult_%d.root", randomSeedNumber);
    //TFile* outFile = new TFile( nameOfOutput, "RECREATE" );

    hTotalEnergyDeposit->Write();
    hTotalEdiff->Write();
    hClusterPositions->Write();
    hClusterEdiff->Write();
    hClusterPosiEdiff->Write();
    hClusterPositions_z->Write();
    hStepEnergyDeposit->Write();
    hStepEdiff->Write();
    hAllStepPosiEdep->Write();
    hAllStepPosiEdiff->Write();

    //outFile->Close();

    delete chain;
}




