/* @doc Object used to parse a VRML file describing a robot.

   Copyright (c) 2005-2009, 

   @author : 
   Olivier Stasse
   
   JRL-Japan, CNRS/AIST

   All rights reserved.
   
   Please refers to file License.txt for details on the license.

*/

/*! System includes */
#include <fstream>
#include <sstream>
#include <map>
#include <string.h>

#include "SpiritVRMLGrammar.hpp"
#include "SpiritVRMLActions.hpp"

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_actor.hpp>
#include <boost/spirit/include/classic_dynamic.hpp>
#include <boost/spirit/include/phoenix1.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_functor_parser.hpp>

#include <dynamicsJRLJapan/dynamicsJRLJapanFactory.h>

//#include "DynamicMultiBodyCopy.h"

namespace dynamicsJRLJapan 
{
  namespace VRMLReader
  {
    
    int ParseVRMLFile(MultiBody *aMB, 
		      std::string aFileName,
		      vector<BodyGeometricalData> &aListOfURLs)
    {
      if (aFileName == std::string("")) {
	std::cerr << "SpiritVRMLReader: Filename is empty." << std::endl;
	return 0;
      }
      ifstream aif;

      SkipGrammar aSkipGrammar;
      SpiritOpenHRP< Actions > aSpiritOpenHRP;
      
      aif.open(aFileName.c_str(),ifstream::in|ifstream::binary);
      if (!aif.is_open())
	{
	  cout << "Unable to open file "<< aFileName<< endl;
	  return 0;
	}
      else {
	/*	if (aSpiritOpenHRP.getVerbose()>10)
	  {
	    cout << "Succeeded in opening " << aFileName <<endl;
	    } */
      }
    
      aif.open(aFileName.c_str(),ifstream::in|ifstream::binary);

      typedef multi_pass<istreambuf_iterator<char> > multi_pass_iterator_t;
      typedef istream::char_type char_t;
      
      multi_pass_iterator_t
        in_begin(make_multi_pass(istreambuf_iterator<char_t>(aif))),
        in_end(make_multi_pass(istreambuf_iterator<char_t>()));
      
      typedef position_iterator<multi_pass_iterator_t> iterator_t;
      
      iterator_t first(in_begin, in_end, aFileName), last;
      
      parse(first,last,aSpiritOpenHRP,aSkipGrammar);
      aif.close();

      // Iterate over the included files if there is some. 
      vector<BodyGeometricalData*> aLOU = aSpiritOpenHRP.actions.m_DataForParsing.m_ListOfURLs;
      vector<BodyGeometricalData*>::iterator it_BGD; 

      it_BGD = aLOU.begin();

      string Path;
      unsigned int npos = aFileName.find_last_of('/');
      Path = aFileName.substr(0,npos+1);

      cout << "Path: " << Path 
	   << " Size of m_ListOfURLs: " 
	   << aSpiritOpenHRP.actions.m_DataForParsing.m_ListOfURLs.size()
	   <<endl;
      unsigned int i=0;
      while (it_BGD!= aLOU.end())
	{
	  i++;
	  const vector<string > URLs = (*it_BGD)->getURLs();
	  cout << " i: " << i << " URLs.size():" << URLs.size() << endl;
	  for(unsigned int j=0;j<URLs.size();j++)
	    {
	      string GeomFileName = Path + URLs[j];
	      aif.open(GeomFileName.c_str(),ifstream::in|ifstream::binary);

	      if (!aif.is_open())
		{
		  cerr<<" Unable to open :" << GeomFileName << endl;
		}
	      else
		{
		  cerr<< "Open :" << GeomFileName << endl;
		  multi_pass_iterator_t
		    lin_begin(make_multi_pass(istreambuf_iterator<char_t>(aif))),
		    lin_end(make_multi_pass(istreambuf_iterator<char_t>()));
		  
		  iterator_t lfirst(lin_begin, lin_end, URLs[j]), llast;
		  
		  parse(lfirst,llast,aSpiritOpenHRP,aSkipGrammar);
		}
	      aif.close();
	    }
	  it_BGD++;
	}
      
      *aMB = aSpiritOpenHRP.actions.m_DataForParsing.m_MultiBody;
      
      return 1;

    };
  };
};


