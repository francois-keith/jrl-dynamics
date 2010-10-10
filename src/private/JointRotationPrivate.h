/* \file This class implements the behavior of a revolute joint.

   Copyright (c) 2007, 
   @author Olivier Stasse, Florent Lamiraux, Francois Keith
   
   JRL-Japan, CNRS/AIST

   All rights reserved.

   Please refers to file License.txt for details on the license.   
*/

#ifndef JOINT_ROTATION_PRIVATE_H
#define JOINT_ROTATION_PRIVATE_H

#include <vector>

#include "MatrixAbstractLayer/MatrixAbstractLayer.h"
#include "robotDynamics/jrlJoint.h"
#include <JointPrivate.h>

using namespace std;

namespace dynamicsJRLJapan
{  
    class DynamicBodyPrivate;
    
    /*! \brief This class computes quantities such as position, velocity, acceleration, 
      force and torques specific to revolute joints and its associated body.
     */
    class JointRotationPrivate : public JointPrivate
    {
    public:
      /*! \brief Constructor when the initial position is not known.
	This evaluation will done after and by doing a normalization.
       */
      JointRotationPrivate();
      
      /*! \brief Constructor when the initial position is known.
       Do not do any normalization. */
      JointRotationPrivate(const matrix4d &inInitialPosition);

      /*! Default destructor */
      virtual ~JointRotationPrivate();

      /*! \brief Compute position and orientation for state vector given inDofVector. */
      bool updateTransformation(const vectorN & inDofVector);
 
	  

      /*! \brief Computes speed in joint and global reference frame. */
      bool updateVelocity(const vectorN& inRobotConfigVector,
			  const vectorN& inRobotSpeedVector);

      /*! \brief Computes acceleration in joint and global reference frame. */
      bool updateAcceleration(const vectorN& inRobotConfigVector,
			      const vectorN& inRobotSpeedVector,
			      const vectorN& inRobotAccelerationVector);
            
      /*! \brief Here the number of DOFs is one. */
	  unsigned int numberDof() const
	  { return 1; }
      /*virtual unsigned int numberDof() 
      {	
			m_nbDofs = 1;
			return m_nbDofs;
	  }*/

	      /*! \brief Returns the free modes of the  joint. 
	Currently this will return an empty matrix.
      */
      const virtual matrixNxP & pcalc(const vectorN & qi);

      /*! \brief Returns the derivative of the free modes of the  joint. 
	Currently this will return an empty matrix.
      */
      const virtual matrixNxP & pdcalc(const vectorN & qi);

    };

};

#endif /* JOINT_ROTATION_PRIVATE_*/
