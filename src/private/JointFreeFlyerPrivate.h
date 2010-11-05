/*
 * Copyright 2010, 
 *
 * Francois Keith
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
/*! Class to implement a Joint object.
*/

#ifndef JOINT_FREE_FLYER_PRIVATE_H
#define JOINT_FREE_FLYER_PRIVATE_H

#include <vector>

#include "JointPrivate.h"
#include "jrl/mal/matrixabstractlayer.hh"
#include "robotDynamics/jrlJoint.h"

using namespace std;

namespace dynamicsJRLJapan
{  
    class DynamicBodyPrivate;
    class JointFreeflyerPrivate : public JointPrivate
    {
    public:
      /*! \brief Constructor when the initial position is not known 
	This call for a renormalization of the object. */
      JointFreeflyerPrivate();
      
      /*! \brief Constructor by copy. */
      JointFreeflyerPrivate(const JointFreeflyerPrivate &a);

      /*! \brief Constructor when the initial position is known 
	Do not renormalize the object.
       */
      JointFreeflyerPrivate(const matrix4d &inInitialPosition);
      virtual ~JointFreeflyerPrivate();

      /*! 
	\brief Compute position and orientation for state vector 
	given inDofVector. 
	\param inDofVector: The current configuration of the robot.
	\return false is the number of dofs is not sufficient. */
      bool updateTransformation(const vectorN & inDofVector);


      /*! \brief Computes speed in joint and global reference frame. */
      bool updateVelocity(const vectorN& inRobotConfigVector,
			  const vectorN& inRobotSpeedVector);
      
      /*! \bri Computes acceleration in joint and global reference frame. */
      bool updateAcceleration(const vectorN & inRobotConfigVector,
			      const vectorN & inRobotSpeedVector,
			      const vectorN & inRobotAccelerationVector);


      /*! \brief Here the number of DOFs is 6. */
      unsigned int numberDof() const 
      { return 6;};


    private:
      /*! Store variables */
      vectorN m_dof6D;
      
    };

};

#endif /* JOINT_FREE_FLYER_PRIVATE*/