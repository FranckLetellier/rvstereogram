#include "effectManager.h"
#include "abstractEffect.h"

#include <assert.h>
#include <iostream>

EffectManager::EffectManager()
{

}


void EffectManager::addEffect(AbstractEffect* pEffect)
{
	assert(pEffect);
	if (m_mEffects.find(pEffect->getName()) != m_mEffects.end() )
		std::cerr<<"ERROR : Effect is already added!"<<std::endl;
	else
	{
		m_mEffects[pEffect->getName()] = pEffect;
	}

}

void EffectManager::queueEffect(const std::string& name)
{
	std::map<std::string,AbstractEffect*>::iterator iter = m_mEffects.find(name);
	if (iter == m_mEffects.end())
	{
		std::cerr << "WARNING : cannot Find effect :  " << name << std::endl;
		return;
	}

	m_mEffectsQueue.push_back(name); 

}

void EffectManager::removeFromQueue(const std::string& name)
{
	std::list<std::string>::iterator iter = m_mEffectsQueue.begin();
	while(iter != m_mEffectsQueue.end()){
		if(*iter == name){
			m_mEffectsQueue.erase(iter);
			return;
		}
		++iter;
	}
}

void EffectManager::clear()
{
	for (std::map<std::string , AbstractEffect*>::iterator iter = m_mEffects.begin() ; iter != m_mEffects.end() ; ++iter)
	{
		delete (*iter).second;
	}
	m_mEffects.clear();

}

void EffectManager::clearQueue()
{
	m_mEffectsQueue.clear();
}


AbstractEffect*	EffectManager::getEffect(const std::string& name)
{
	std::map<std::string,AbstractEffect*>::iterator iter = m_mEffects.find(name);
	if (iter == m_mEffects.end())
	{
		std::cerr << "WARNING : cannot Find effect :  " << name << std::endl;
		return NULL;
	}
	return (*iter).second;
}

bool EffectManager::isQueued(const std::string& name)
{
	
	std::list<std::string>::iterator iter = m_mEffectsQueue.begin();

	while(iter != m_mEffectsQueue.end()){
		if(*iter == name)
			return true;

		++iter;
	}
	return false;
}

