#include <iostream>
#include <string>
#include "TString.h"
#include <fstream>
#include <vector>

using namespace std;

	
vector<string> ReadData(string FileName){
 
  vector<string> FileList;
  ifstream file(FileName);
  string str; 
  
  bool debug=false;
  
  if(!file.is_open()){std::cout << "file non trovato" << std::endl;}

  while (std::getline(file, str))
    {
      FileList.push_back(str);
    }
  //  if(debug){
  //  for(int i=0; i< (int)FileList.size();i++){
  //   std::cout << FileList.at(i) << std::endl;
  // }
  //}
  return FileList;
}
