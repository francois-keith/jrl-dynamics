/*
 * Copyright 2010,
 *
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
#include "Debug.h"

#include "JointAnchorPrivate.h"
#include "DynamicBodyPrivate.h"


using namespace dynamicsJRLJapan;

JointAnchorPrivate::JointAnchorPrivate(const MAL_S4x4_MATRIX_TYPE(double) &inInitialPosition)
{
  type(JointPrivate::FIX_JOINT);
  m_inGlobalFrame = true;
  m_globalPoseAtConstruction = inInitialPosition;

  ODEBUG2("anchor: inInitialPosition" << inInitialPosition);
  MAL_S3_VECTOR(laxis, double);

  MAL_S3_VECTOR_ACCESS(laxis,0) = 1.0;
  MAL_S3_VECTOR_ACCESS(laxis,1) = 0.0;
  MAL_S3_VECTOR_ACCESS(laxis,2) = 0.0;

  axis(laxis);
  setnumberDof(0);
}

JointAnchorPrivate::~JointAnchorPrivate()
{
}

bool JointAnchorPrivate::updateVelocity(const vectorN&,
					const vectorN&)
{
  return true;
}

bool JointAnchorPrivate::updateAcceleration(const vectorN&,
					    const vectorN&,
					    const vectorN&)
{
  return true;
}

const matrixNxP & JointAnchorPrivate::pcalc(const vectorN & )
{
  MAL_MATRIX_RESIZE(m_phi,6,1);
  MAL_MATRIX_FILL(m_phi,0);
  return m_phi;
}

const matrixNxP & JointAnchorPrivate::pdcalc(const vectorN & )
{
  MAL_MATRIX_RESIZE(m_dotphi,6,1);
  MAL_MATRIX_FILL(m_dotphi,0);
  return m_dotphi;
}
