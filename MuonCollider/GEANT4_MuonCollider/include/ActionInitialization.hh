#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "globals.hh"


class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization(bool debugMode, G4int seedNumber);
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

private:
    bool fDebugMode;
    G4int fseedNumber;
};

#endif // ACTION_INITIALIZATION_HH

