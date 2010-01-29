/* @doc \file Object to generate a file following AMELIF format.

   Copyright (c) 2010
   @author Olivier Stasse
   JRL-Japan, CNRS/AIST
 
   All rights reserved.

   Please refers to file License.txt for details on the license.

*/
#include <string>
#include <iostream>
#include <fstream>

#include <dynamicsJRLJapan/dynamicsJRLJapanFactory.h>
#include "GenerateRobotForAMELIF.h"
#include "GenerateRobotForVRML1.h"

using namespace std;

void ExportToAMELIF(CjrlHumanoidDynamicRobot *aHDR,
		    std::vector<std::string> &aVectorOfURLs)
{
  dynamicsJRLJapan::GenerateRobotForAMELIF aGenerateRobotForAMELIF;

  string aHRP2Normalized("HRP2Normalized");
  aGenerateRobotForAMELIF.SetAccessToData(aVectorOfURLs);
  aGenerateRobotForAMELIF.GenerateRobot(aHRP2Normalized,aHDR);

}


int main(int argc, char *argv[])
{
  string aSpecificitiesFileName;
  string aPath;
  string aName;
  string aMapFromJointToRank;

  ofstream tcout("output.txt",ofstream::out);

  if (argc!=5)
    {
      aPath="./";
      aName="sample.wrl";
      aSpecificitiesFileName = "sampleSpecificities.xml";
      aMapFromJointToRank = "sampleLinkJointRank.xml";
    }
  else 
    {
      aSpecificitiesFileName = argv[3];
      aPath=argv[1];
      aName=argv[2];
      aMapFromJointToRank=argv[4];
    }
  dynamicsJRLJapan::ObjectFactory aRobotDynamicsObjectConstructor;
  
  CjrlHumanoidDynamicRobot * aHDR = aRobotDynamicsObjectConstructor.createHumanoidDynamicRobot();

  if (aHDR==0)
    { 
      cerr<< "Dynamic cast on HDR failed " << endl;
      exit(-1);
    }
  string RobotFileName = aPath+aName;
  cout << "RobotFileNAme :" << RobotFileName << endl;
  vector<string> aVectorOfURLs;
  dynamicsJRLJapan::parseOpenHRPVRMLFile(*aHDR,RobotFileName,
					 aMapFromJointToRank,
					 aSpecificitiesFileName,
					 aVectorOfURLs);

  ExportToAMELIF(aHDR,aVectorOfURLs);
}
