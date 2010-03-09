/*************************************
*
*		ImacDemo Project
*		
*		Created :	10/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __ABSTRACTEFFECT_H__
#define __ABSTRACTEFFECT_H__

#include <string>

class AbstractEffect
{
	public:
	
		AbstractEffect(const std::string& sName);
		virtual ~AbstractEffect() {};

		virtual void preProcess(unsigned int windowWidth, unsigned int windowHeight) = 0;
		virtual void process(unsigned int windowWidth, unsigned int windowHeight) = 0;

		inline const std::string& getName(){return m_sName;};

	private:

		std::string		m_sName;


};


#endif //__ABSTRACTEFFECT_H__