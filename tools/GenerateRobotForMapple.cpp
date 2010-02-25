/* @doc \file Object to generate a file following VRML 1.0 format.

   Copyright (c) 2010
   @author Olivier Stasse
   JRL-Japan, CNRS/AIST
 
   All rights reserved.

   Please refers to file License.txt for details on the license.

*/
/* System includes */
#include <iostream>
#include <fstream>

/*! Includes of the framework */
#include "GenerateRobotForMaple.h"

using namespace std;

namespace dynamicsJRLJapan {

  Tools::GenerateRobotForMaple::GenerateRobotForMaple()
  {
  }

  Tools::GenerateRobotForMaple::~GenerateRobotForMaple()
  {
  }

  void Tools::GenerateRobotForMaple::GenerateGPLv2License(ostream &aof)
  {
    aof << "# Copyright (C) "<< endl
	<< "# This program is free software; you can redistribute it and/or modify it" << endl
	<< "# under the terms of the GNU General Public License version 2 as published" << endl
	<< "# by the Free Software Foundation." << endl
	<< "#" << endl
	<< "# This program is distributed in the hope that it will be useful, but" << endl
	<< "# WITHOUT ANY WARRANTY; without even the implied warranty of" << endl
	<< "# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General" << endl
	<< "# Public License for more details." << endl
	<< "# " << endl
	<< "# You should have received a copy of the GNU General Public License along" << endl
	<< "# with this program; if not, write to the Free Software Foundation, Inc.," << endl
	<< "# 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA." << endl;
  }

  void Tools::GenerateRobotForMaple::GenerateHeader(ostream &aof,
						    CjrlHumanoidDynamicRobot *aHDR)
  {
    aof << "# Number of Degree of freedom" << endl;
    aof << "NDDL := " << aHDR->numberDof() << endl;
    
    aof << "# Definitions of coordinates and generalized speeds " << endl;
    aof << "q := vector(NDDL):" << endl;
    aof << "qdot := vector(NDDL):" << endl;
    
    aof << "#Cardan rotation: we begin with rotation around x, and after around y and" << endl;
    aof << "#finally around z." << endl;

    

  }
  /*! \brief Generate hard coded robot */
  void Tools::GenerateRobotForMaple::GenerateRobot(std::string &RobotName,
						    CjrlHumanoidDynamicRobot *aHDR)
  {
    m_HDR = aHDR;
    ofstream aof;
    string FileName = RobotName + "Data.maple";
    aof.open((char *)FileName.c_str(),ofstream::out);

    if(!aof.is_open())
      return;

    GenerateGPLv2License(aof);
    GenerateHeader(aof,aHDR);

    string empty("");
    GenerateJoints(aof,empty,aHDR);

    aof.close();
  }


  void Tools::GenerateRobotForMaple::GenerateJoints(ostream &os,
						     string shifttab,
						     CjrlHumanoidDynamicRobot *aHDR)
  {
    CjrlJoint *RootJoint = m_HDR->rootJoint();
    unsigned int gindex=1;

    m_Indexes[RootJoint] = gindex;
    
    unsigned int IndexBase = aHDR->numberDof()+1;
    os << "#Frame "<< gindex << endl;
    os << "Rx_"<< gindex << " := q["<< IndexBase+3<< "]:" << endl;
    os << "Ry_"<< gindex << " :=-q["<< IndexBase+5<< "]:" << endl;
    os << "Rz_"<< gindex << " := q["<< IndexBase+4<< "]:" << endl;
    os << "Tx_"<< gindex << " := q["<< IndexBase<< "]:" << endl;
    os << "Ty_"<< gindex << " :=-q["<< IndexBase+2<< "]:" << endl;
    os << "Tz_"<< gindex << " := q["<< IndexBase+1<< "]:" << endl;

    GenerateJoint(RootJoint,os,shifttab,gindex);

  }
  
  /*! \brief Take the links towards the geometrical information */
  void Tools::GenerateRobotForMaple::SetAccessToData(std::vector<BodyGeometricalData> &AccessToData)
  {
    m_AccessToData = AccessToData;
  }

  void Tools::GenerateRobotForMaple::GenerateJoint(CjrlJoint *aJoint, 
						    ostream &os,
						    string shifttab, unsigned int &gindex)
  {

    m_Indexes[aJoint] = gindex;
    os << "#Frame "<< gindex << endl;
    
    MAL_S4x4_MATRIX(aTransformation,double);
    aTransformation = aJoint->currentTransformation();
    unsigned int indexparent=0;

    CjrlJoint *parentJoint = aJoint->parentJoint();
    if (parentJoint!=0)
      {
	MAL_S4x4_MATRIX(parentTransformation,double);
	parentTransformation = parentJoint->currentTransformation();

	MAL_S4x4_MATRIX(invParentTransformation,double);
	MAL_S4x4_INVERSE(parentTransformation,invParentTransformation,double);
	indexparent = m_Indexes[parentJoint];
	aTransformation = MAL_S4x4_RET_A_by_B(invParentTransformation,aTransformation);
      }

    matrix3d aRotation;
    vector3d anAxis, aRealAxis;
    for(unsigned int i=0;i<3;i++)
      {
	for(unsigned int j=0;j<3;j++)
	  MAL_S3x3_MATRIX_ACCESS_I_J(aRotation,i,j) = 
	    MAL_S4x4_MATRIX_ACCESS_I_J(aTransformation,i,j);
      }
    anAxis(0) = 1.0; anAxis(1) = 0.0; anAxis(2) = 0.0;
    MAL_S3x3_C_eq_A_by_B(aRealAxis,aRotation,anAxis);
    
    os << "ref_"<<gindex << " := "<< indexparent << ":" << endl;
    if (fabs(fabs(aRealAxis(0))-1.0)<1e-8)
      os << "Rx_"<< gindex << " := q["<< aJoint->rankInConfiguration() << "]:" << endl;
    else 
      os << "Rx_"<< gindex << " := 0:" << endl;

    if (fabs(fabs(aRealAxis(1))-1.0)<1e-8)
      os << "Ry_"<< gindex << " := q["<< aJoint->rankInConfiguration() << "]:" << endl;
    else 
      os << "Ry_"<< gindex << " := 0:" << endl;

    if (fabs(fabs(aRealAxis(2))-1.0)<1e-8)
      os << "Rz_"<< gindex << " := q["<< aJoint->rankInConfiguration() << "]:" << endl;
    else 
      os << "Rz_"<< gindex << " := 0:" << endl;

    if (fabs(fabs(aRealAxis(2))-1.0)<1e-8)
      os << "Tx_"<< gindex << " := " << 
	MAL_S4x4_MATRIX_ACCESS_I_J(aTransformation,0,3)<<  ":" << endl;
    else 
      os << "Tx_"<< gindex << " := 0:" << endl;

    if (fabs(fabs(aRealAxis(2))-1.0)<1e-8)
      os << "Ty_"<< gindex << " := " << 
	MAL_S4x4_MATRIX_ACCESS_I_J(aTransformation,1,3)<<  ":" << endl;
    else 
      os << "Ty_"<< gindex << " := 0:" << endl;

    if (fabs(fabs(aRealAxis(2))-1.0)<1e-8)
      os << "Tz_"<< gindex << " := " << 
	MAL_S4x4_MATRIX_ACCESS_I_J(aTransformation,2,3) <<  ":" << endl;
    else 
      os << "Tz_"<< gindex << " := 0:" << endl;


    gindex++;
    // Call the sons.
    for(unsigned int i=0;i<aJoint->countChildJoints();i++)
      {
	GenerateJoint(aJoint->childJoint(i),os,shifttab,gindex);
      }

  }  

  void Tools::GenerateBody(CjrlJoint *aJoint,
			   ostream &os,
			   string &shifttab,
			   unsigned int &gindex)
  {
    CjrlBody *aBody = aJoint->linkedBody();
    os << "m_" << gindex << " := "<<  aBody->mass() << ":"<<endl;
    
    vector3d aCom=aBody->localCenterOfMass();
    os << "G_" << gindex 
       << " := vector(["
       << aCom(0) << " , "
       << aCom(1) << " , " 
       << aCom(2) << "])" << endl;
    
    matrix3d IG=aBody->inertiaMatrix();
    os << "IG_" << gindex 
       << " := matrix([[" << 
       << MATRIX_3x3_ACCESS_I_J(IG,0,0) << " , " 
       << MATRIX_3x3_ACCESS_I_J(IG,0,1) << " , " 
       << MATRIX_3x3_ACCESS_I_J(IG,0,2) << " ],[" 
       << MATRIX_3x3_ACCESS_I_J(IG,1,0) << " , " 
       << MATRIX_3x3_ACCESS_I_J(IG,1,1) << " , " 
       << MATRIX_3x3_ACCESS_I_J(IG,1,2) << " ],[" 
       << MATRIX_3x3_ACCESS_I_J(IG,2,0) << " , " 
       << MATRIX_3x3_ACCESS_I_J(IG,2,1) << " , " 
       << MATRIX_3x3_ACCESS_I_J(IG,2,2) << " ]]):" 
       << endl << endl;


    gindex++;
    // Call the sons.
    for(unsigned int i=0;i<aJoint->countChildJoints();i++)
      {
	GenerateBody(aJoint->childJoint(i),os,shifttab,gindex);
      }
    
  }

  void Tools::GenerateRobotForMaple::GenerateBodies(ostream &os,
						     CjrlHumanoidDynamicRobot *aHDR)
  {
    GenerateHeader(aos,aHDR);
    os << "Gravity := vector([0, -9.81, 0]):"  << endl;
    unsigned int gindex=0;
    CjrlJoint *RootJoint = aHDR->rootJoint();
    string empty("");
    
    GenerateBody(RootJoint,os,shifttab,gindex);

  }

}
