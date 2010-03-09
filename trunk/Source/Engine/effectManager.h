/*************************************
*
*		ImacDemo Project
*		
*		Created :	18/12/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*					Helena Duchaussoy
*
**************************************/

#ifndef __EFFECTMANAGER_H__
#define __EFFECTMANAGER_H__

/**
*	@name effectManager.h
*	@brief Manager all our scenes
*	@author Franck Letellier
*	@date 18/12/09
*/

#include <string>
#include <map>
#include <list>

#include "singleton.h"


class AbstractEffect;

class EffectManager:public Singleton<EffectManager>
{
	friend class Singleton<EffectManager>;

	public:
		void addEffect(AbstractEffect* pEffect);
		void queueEffect(const std::string& name);
		bool isQueued(const std::string& name);
		void removeFromQueue(const std::string& name);
		void clear();
		void clearQueue();

		inline const std::list<std::string>& getQueue(){return m_mEffectsQueue;};
		AbstractEffect* getEffect(const std::string& name);

	private:
		std::map<std::string,AbstractEffect*> m_mEffects;
		std::list<std::string> m_mEffectsQueue;



		EffectManager();
		virtual ~EffectManager(){clear();};

};


#endif //__EFFECTMANAGER_H__