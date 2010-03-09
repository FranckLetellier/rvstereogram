//---------------------------------------------------------------------
//
// PROJECT ....
// Copyright (C) 
//
// All rights reserved
//
//---------------------------------------------------------------------

///  XML parser for splines's points files
///\author Helena DUCHAUSSOY, Fabien KAPALA, Franck LETELLIER, Baptiste MALAGA

#include "ReadXMLSpline.hpp"


std::vector<osg::Vec3f> loadXMLFile(const char * name){
	TiXmlDocument * document = new TiXmlDocument(name);
	int element_counter = 1; ///< counter used to report xml reading errors
	
	if(!document->LoadFile()){
  		std::cerr << "[ReadXML] Loading Error : " << document->ErrorDesc() << std::endl;
	}
	
	std::vector<osg::Vec3f> result;
	float x,y,z;
	int reading_result;
	
	TiXmlElement * xmlPoint = document->FirstChildElement();
	
	while (xmlPoint){
		
		reading_result = xmlPoint->QueryFloatAttribute("x",&x);
		if(reading_result == TIXML_WRONG_TYPE){
			std::cerr << "[ReadXML] Reading error at element n°"<< element_counter <<": attribute x is not a float" << std::endl;
		}
		else if(reading_result == TIXML_NO_ATTRIBUTE ){
			std::cerr << "[ReadXML] Reading error at element n°"<< element_counter <<": attribute x doesn't exist" << std::endl;
		}
		
		reading_result = xmlPoint->QueryFloatAttribute("y",&y);
		if(reading_result == TIXML_WRONG_TYPE){
			std::cerr << "[ReadXML] Reading error at element n°"<< element_counter <<": attribute y is not a float" << std::endl;
		}
		else if(reading_result == TIXML_NO_ATTRIBUTE ){
			std::cerr << "[ReadXML] Reading error at element n°"<< element_counter <<": attribute y doesn't exist" << std::endl;
		}
		
		reading_result = xmlPoint->QueryFloatAttribute("z",&z);
		if(reading_result == TIXML_WRONG_TYPE){
			std::cerr << "[ReadXML] Reading error at element n°"<< element_counter <<": attribute z is not a float" << std::endl;
		}
		else if(reading_result == TIXML_NO_ATTRIBUTE ){
			std::cerr << "[ReadXML] Reading error at element n°"<< element_counter <<": attribute z doesn't exist" << std::endl;
		}
		
		osg::Vec3f vec3Point(x,y,z);// = new std::vector<float>();
		/*vec3Point.push_back(x);
		vec3Point.push_back(y);
		vec3Point.push_back(z);*/
		result.push_back(vec3Point);
		
		xmlPoint = xmlPoint->NextSiblingElement();
		element_counter++;
	}
	return result;
}


