
#include "skeletonClass.hh"

skeletonClass::skeletonClass() {

  cout << "[ Constructor of skeletonClass ]" << endl;
  msgContent = "";

}

skeletonClass::~skeletonClass() {

  cout << "[ Destructor of skeletonClass ]" << endl;

}

void skeletonClass::PrintMessage() {

  cout << "Print content : " << msgContent << endl;

}

void skeletonClass::setMsg(string msg) {

  msgContent = msg;

}

