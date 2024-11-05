#ifndef DIMENSIONSMANAGER_HH
#define DIMENSIONSMANAGER_HH

#include "G4Exception.hh"

class DimensionsManager {

public:
    static DimensionsManager* GetInstance();
    static void DeleteInstance();

    // Getter methods
    const double* GetWorldSize_Width()          const;
    const double* GetWorldSize_Height()         const;
    const double* GetWorldSize_Length()         const;
    const double* GetModuleSize_Width()         const;
    const double* GetModuleSize_Height()        const;
    const double* GetModuleSize_Length()        const;
    const int*    GetNumberOfModule_Row()       const;
    const int*    GetNumberOfModule_Column()    const;
    const double* GetFiberCladdingSize_Radius() const;
    const double* GetFiberCladdingSize_Length() const;
    const double* GetCherenFiberSize_Radius()   const;
    const double* GetCherenFiberSize_Length()   const;
    const double* GetScintFiberSize_Radius()    const;
    const double* GetScintFiberSize_Length()    const;


    // Setter methods
    void SetWorldSize_Width_Height_Length(double width, double height, double length);
    void SetModuleSize_Width_Height_Length(double width, double height, double length);
    void SetNumberOfModule_Row_Column(int row, int column);
    void SetFiberCladdingSize_Radius_Length(double radius, double length);
    void SetCherenFiberSize_Radius_Length(double radius, double length);
    void SetScintFiberSize_Radius_Length(double radius, double length);

private:
    DimensionsManager();
    ~DimensionsManager();

    static DimensionsManager* fInstance;
    static bool isDeleted;

    double WorldSize_Width;
    double WorldSize_Height;
    double WorldSize_Length;

    double ModuleSize_Width;
    double ModuleSize_Height;
    double ModuleSize_Length;
    int    NumberOfModule_Row;
    int    NumberOfModule_Column;  

    double FiberCladdingSize_Radius;
    double FiberCladdingSize_Length;
    double CherenFiberSize_Radius;
    double CherenFiberSize_Length;
    double ScintFiberSize_Radius; 
    double ScintFiberSize_Length;  

};

#endif

