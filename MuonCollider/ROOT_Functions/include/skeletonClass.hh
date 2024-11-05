
#ifndef SKELETONCLASS_HH
#define SKELETONCLASS_HH

#include <iostream>
#include <string>

using namespace std;

class skeletonClass {

public:
  
    skeletonClass();
    ~skeletonClass();

    void PrintMessage();
    void setMsg(string msg);

private:
    
    string msgContent;

};

#endif

