/*
 * Copyright 2010,
 *
 * Oussama Kanoun
 * Francois Keith
 * Florent Lamiraux
 * Olivier Stasse
 *
 * JRL/LAAS, CNRS/AIST
 *
 * This file is part of dynamicsJRLJapan.
 * dynamicsJRLJapan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dynamicsJRLJapan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Lesser Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with dynamicsJRLJapan.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Research carried out within the scope of the Associated
 *  International Laboratory: Joint Japanese-French Robotics
 *  Laboratory (JRL)
 *
 */
/* @doc Object used to handle hand
*/

#include "Debug.h"

#include "DynMultiBodyPrivate.h"
#include "jrl/dynamics/hand.hh"
#include "HumDynMultiBodyPrivate.h"
#include "HumanoidSpecificities.h"


using namespace dynamicsJRLJapan;



HumDynMultiBodyPrivate::HumDynMultiBodyPrivate() : DynMultiBodyPrivate(),
						   m_LeftWristJoint(0),
						   m_RightWristJoint(0),
						   m_LeftFoot(0),
						   m_RightFoot(0),
						   m_LeftAnkleJoint(0),
						   m_RightAnkleJoint(0),
						   m_leftHand(0),
						   m_rightHand(0),
						   m_GazeJoint(0),
						   m_WaistJoint(0),
						   m_ChestJoint(0),
						   m_HS(0)
{
}


HumDynMultiBodyPrivate::HumDynMultiBodyPrivate(const DynMultiBodyPrivate& inDynamicMultiBody,
						   string aFileNameForHumanoidSpecificities) :
  DynMultiBodyPrivate(inDynamicMultiBody),
  m_LeftWristJoint(0),
  m_RightWristJoint(0),
  m_LeftFoot(0),
  m_RightFoot(0),
  m_LeftAnkleJoint(0),
  m_RightAnkleJoint(0),
  m_leftHand(0),
  m_rightHand(0),
  m_GazeJoint(0),
  m_WaistJoint(0),
  m_ChestJoint(0),
  m_HS(0)
{
  SetHumanoidSpecificitiesFile(aFileNameForHumanoidSpecificities);
}

void HumDynMultiBodyPrivate::SetHumanoidSpecificitiesFile(string &aFileNameForHumanoidSpecificities)
{
  m_HS = new HumanoidSpecificities();

  if (m_HS!=0)
    {
      m_HS->ReadXML(aFileNameForHumanoidSpecificities);

      double AnklePosition[3];
      // Take the right ankle position (should be equivalent)
      m_HS->GetAnklePosition(-1,AnklePosition);
      m_AnkleSoilDistance = AnklePosition[2];
      ODEBUG("AnkleSoilDistance =" << m_AnkleSoilDistance);

      // Lenght of the hip (necessary for
      double HipLength[3];
      // Takes the left one.
      m_HS->GetHipLength(1,HipLength);

      ODEBUG(HipLength[0] << " "
	      << HipLength[1] << " "
	      << HipLength[2] << " ");
      m_Dt(0) = HipLength[0];
      m_Dt(1) = HipLength[1];
      m_Dt(2) = HipLength[2];

      MAL_S3_VECTOR(StaticToTheLeftHip,double);
      MAL_S3_VECTOR(StaticToTheRightHip,double);

      // Displacement between the hip and the waist.
      double WaistToHip[3];
      m_HS->GetWaistToHip(1,WaistToHip);
      m_StaticToTheLeftHip(0) = WaistToHip[0];
      m_StaticToTheLeftHip(1) = WaistToHip[1];
      m_StaticToTheLeftHip(2) = WaistToHip[2];

      m_TranslationToTheLeftHip = m_StaticToTheLeftHip;

      m_HS->GetWaistToHip(-1,WaistToHip);
      m_StaticToTheRightHip(0) = WaistToHip[0];
      m_StaticToTheRightHip(1) = WaistToHip[1];
      m_StaticToTheRightHip(2) = WaistToHip[2];
      m_TranslationToTheRightHip = m_StaticToTheRightHip;


      // If the Dynamic Multibody object is already loaded
      // create the link between the joints and the effector semantic.

      if (numberDof()!=0)
	LinkBetweenJointsAndEndEffectorSemantic();
      else
	{
	  throw "No degree of freedom";
	}
    }
  else
    {
      cerr << "Warning: No appropriate definition of Humanoid Specifities" << endl;
      cerr << "Use default value: " << 0.1 << endl;
      m_AnkleSoilDistance = 0.1;

      // Displacement between the hip and the waist.
      m_Dt(0) = 0.0;
      m_Dt(1) = 0.04;
      m_Dt(2) = 0.0;

      LTHROW("Warning:No appropriate definition of Humanoid Specifities");
    }

}

HumDynMultiBodyPrivate::~HumDynMultiBodyPrivate()
{
  if (m_HS!=0)
    delete m_HS;

  delete m_rightHand;
  delete m_leftHand;
  delete m_RightFoot;
  delete m_LeftFoot;
}

void HumDynMultiBodyPrivate::LinkBetweenJointsAndEndEffectorSemantic()
{
  if (m_HS==0)
    return;

  // Link the correct joints.

  // Get the left hand.
  std::vector<int> JointForOneLimb = m_HS->GetArmJoints(1);
  int ListeJointsSize = JointForOneLimb.size();
  int EndIndex(-1);
  if (ListeJointsSize > 1)
  {
    std::vector<int> JointsForWrist = m_HS->GetWrists(1);
    ODEBUG("JointsForWrist:" << JointsForWrist.size());
    if (JointsForWrist.size()>0)
      {
	m_LeftWristJoint = GetJointFromActuatedID(JointsForWrist[0]);
	ODEBUG("JointsForWrist[0]: " << JointsForWrist[0]);
      }
    else
      LTHROW("No left wrist");
  }

  // Get the right hand.
  JointForOneLimb.clear();
  JointForOneLimb = m_HS->GetArmJoints(-1);
  ListeJointsSize = JointForOneLimb.size();
  if (ListeJointsSize > 1)
  {
    std::vector<int> JointsForWrist = m_HS->GetWrists(-1);
    ODEBUG("JointsForWrist:" << JointsForWrist.size());
    if (JointsForWrist.size()>0)
      {
	m_RightWristJoint = GetJointFromActuatedID(JointsForWrist[0]);
	ODEBUG("JointsForWrist[0]: " << JointsForWrist[0]);
      }
  }

  // Get the left foot.
  JointForOneLimb.clear();
  JointForOneLimb = m_HS->GetFootJoints(1);
  ListeJointsSize = JointForOneLimb.size();
  CjrlJoint* theLeftAnkle(0x0);
  if(ListeJointsSize > 0)
  {
    EndIndex = JointForOneLimb[ListeJointsSize-1];
    theLeftAnkle = GetJointFromActuatedID(EndIndex);
    ODEBUG("Joints for the left foot:" << EndIndex);
  }

  Foot *theLeftFoot = new Foot();
  theLeftFoot->setAssociatedAnkle(theLeftAnkle);
  double AnklePosition[3];
  m_HS->GetAnklePosition(1,AnklePosition);
  vector3d AnklePositionInLocalFrame;
  AnklePositionInLocalFrame(0) = AnklePosition[0];
  AnklePositionInLocalFrame(1) = AnklePosition[1];
  AnklePositionInLocalFrame(2) = AnklePosition[2];
  theLeftFoot->setAnklePositionInLocalFrame(AnklePositionInLocalFrame);

  double lFootWidth,lFootHeight,lFootDepth;
  m_HS->GetFootSize(1,lFootDepth, lFootWidth, lFootHeight);
  theLeftFoot->setSoleSize(lFootDepth,lFootWidth);
  leftFoot(theLeftFoot);
  leftAnkle(theLeftAnkle);

  // Get the right foot.
  JointForOneLimb.clear();
  JointForOneLimb = m_HS->GetFootJoints(-1);
  ListeJointsSize = JointForOneLimb.size();
  CjrlJoint* theRightAnkle(0x0);
  if (ListeJointsSize > 0)
  {
    EndIndex = JointForOneLimb[ListeJointsSize-1];
    theRightAnkle = GetJointFromActuatedID(EndIndex);
    ODEBUG("Joints for the right foot:" << EndIndex);
  }

  Foot *theRightFoot = new Foot();
  theRightFoot->setAssociatedAnkle(theRightAnkle);

  m_HS->GetAnklePosition(-1,AnklePosition);
  AnklePositionInLocalFrame(0) = AnklePosition[0];
  AnklePositionInLocalFrame(1) = AnklePosition[1];
  AnklePositionInLocalFrame(2) = AnklePosition[2];
  theRightFoot->setAnklePositionInLocalFrame(AnklePositionInLocalFrame);

  m_HS->GetFootSize(-1,lFootDepth, lFootWidth, lFootHeight);
  theRightFoot->setSoleSize(lFootDepth,lFootWidth);

  rightFoot(theRightFoot);
  rightAnkle(theRightAnkle);

  // Get the gaze joint (head) of the humanoid.
  JointForOneLimb.clear();
  JointForOneLimb = m_HS->GetHeadJoints();
  ListeJointsSize = JointForOneLimb.size();
  if (ListeJointsSize > 0)
  {
    EndIndex = JointForOneLimb[ListeJointsSize-1];
    m_GazeJoint = GetJointFromActuatedID(EndIndex);
  }

  // Get the waist joint of the humanoid.
  std::vector<int> JointsForWaist = m_HS->GetWaistJoints();
  if (JointsForWaist.size()==1)
    m_WaistJoint = GetJointFromActuatedID(JointsForWaist[0]);


  // Get the chest joint of the humanoid.
  std::vector<int> JointsForChest = m_HS->GetChestJoints();
  unsigned NbChestJoints = JointsForChest.size();
  if (NbChestJoints>0)
    m_ChestJoint = GetJointFromActuatedID(JointsForChest[NbChestJoints-1]);

  // Take care of the hands information.
  HandsData HumHands = m_HS->GetHandsData();

  Hand* hand=new Hand();
  hand->setAssociatedWrist(rightWrist());
  hand->setCenter(HumHands.Center[0]);
  hand->setThumbAxis(HumHands.okayAxis[0]);
  hand->setForeFingerAxis(HumHands.showingAxis[0]);
  hand->setPalmNormal(HumHands.palmAxis[0]);
  rightHand(hand);

  hand=new Hand();
  hand->setAssociatedWrist(leftWrist());
  hand->setCenter(HumHands.Center[1]);
  hand->setThumbAxis(HumHands.okayAxis[1]);
  hand->setForeFingerAxis(HumHands.showingAxis[1]);
  hand->setPalmNormal(HumHands.palmAxis[1]);
  leftHand(hand);

}

const MAL_S3_VECTOR_TYPE(double) & HumDynMultiBodyPrivate::zeroMomentumPoint() const
{

  return m_ZeroMomentumPoint;
}

void HumDynMultiBodyPrivate::ComputingZeroMomentumPoint()
{
  m_ZeroMomentumPoint = getZMP();
}

/***************************************************/
/* Implementation of the proxy design pattern for  */
/* the part inherited from jrlDynamicRobot.        */
/***************************************************/

bool HumDynMultiBodyPrivate::computeForwardKinematics()
{
  bool r;
  r= DynMultiBodyPrivate::computeForwardKinematics();
  ComputingZeroMomentumPoint();
  return r;

}


bool HumDynMultiBodyPrivate::jacobianJointWrtFixedJoint(CjrlJoint*,
							MAL_MATRIX_TYPE(double) & )
{
  cerr<< " The method HumDynMultiBodyPrivate::jacobianJointWrtFixedJoint " <<endl
      << " is not implemented yet. " << endl;
  return true;
}


void HumDynMultiBodyPrivate::waist(CjrlJoint * inWaist)
{
  m_WaistJoint = inWaist;
}

CjrlJoint* HumDynMultiBodyPrivate::waist() const
{
  return m_WaistJoint;
}

double HumDynMultiBodyPrivate::getHandClench(CjrlHand* )
{
    // default implementation. always returns 0
    return 0;
}

bool HumDynMultiBodyPrivate::setHandClench(CjrlHand* , double )
{
    // default implementation. always returns false
    return false;
}

void HumDynMultiBodyPrivate::leftWrist(CjrlJoint *inLeftWrist)
{
  m_LeftWristJoint = inLeftWrist;
}

CjrlJoint *HumDynMultiBodyPrivate::leftWrist() const
{
  return m_LeftWristJoint;
}

void HumDynMultiBodyPrivate::rightWrist(CjrlJoint *inRightWrist)
{
  m_RightWristJoint = inRightWrist;
}

CjrlJoint *HumDynMultiBodyPrivate::rightWrist() const
{
  return m_RightWristJoint;
}

void HumDynMultiBodyPrivate::rightHand(CjrlHand* inRightHand)
{
  m_rightHand = inRightHand;
}

CjrlHand * HumDynMultiBodyPrivate::rightHand() const
{
  return m_rightHand;
}


void HumDynMultiBodyPrivate::leftHand(CjrlHand* inLeftHand)
{
  m_leftHand = inLeftHand;
}

CjrlHand * HumDynMultiBodyPrivate::leftHand() const
{
  return m_leftHand;
}

void HumDynMultiBodyPrivate::leftAnkle(CjrlJoint *inLeftAnkle)
{
  m_LeftAnkleJoint = inLeftAnkle;
}

CjrlJoint *HumDynMultiBodyPrivate::leftAnkle() const
{
  return m_LeftAnkleJoint;
}

void HumDynMultiBodyPrivate::rightAnkle(CjrlJoint *inRightAnkle)
{
  m_RightAnkleJoint = inRightAnkle;
}

CjrlJoint *HumDynMultiBodyPrivate::rightAnkle() const
{
  return m_RightAnkleJoint;
}

void HumDynMultiBodyPrivate::leftFoot(CjrlFoot *inLeftFoot)
{
  m_LeftFoot = inLeftFoot;
}

CjrlFoot * HumDynMultiBodyPrivate::leftFoot() const
{
  return m_LeftFoot;
}

void HumDynMultiBodyPrivate::rightFoot(CjrlFoot *inRightFoot)
{
  m_RightFoot = inRightFoot;
}

CjrlFoot * HumDynMultiBodyPrivate::rightFoot() const
{
  return m_RightFoot;
}

void HumDynMultiBodyPrivate::chest(CjrlJoint *inChest)
{
  m_ChestJoint = inChest;
}

CjrlJoint * HumDynMultiBodyPrivate::chest() const
{
  return m_ChestJoint;
}

/***************************************************/
/* End of the implementation                       */
/***************************************************/
