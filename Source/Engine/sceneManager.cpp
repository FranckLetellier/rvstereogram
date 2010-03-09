#include "sceneManager.h"
#include "abstractScene.h"
#include "abstractCamera.h"

#include <assert.h>
#include <iostream>

SceneManager::SceneManager()
{
	m_pCurrentScene = NULL;
}


void SceneManager::addScene(AbstractScene* pScene)
{
	assert(pScene);
	if (m_mScenes.find(pScene->getName()) != m_mScenes.end() )
		std::cerr<<"ERROR : Scene is already added!"<<std::endl;
	else
	{
		m_mScenes[pScene->getName()] = pScene;
	}

}

void SceneManager::clear()
{
	for (std::map<std::string , AbstractScene*>::iterator iter = m_mScenes.begin() ; iter != m_mScenes.end() ; ++iter)
	{
		delete (*iter).second;
	}
	m_mScenes.clear();

	m_pCurrentScene = NULL;
}


void SceneManager::update()
{
	assert(m_pCurrentScene);
	m_pCurrentScene->update();
}


void SceneManager::setCurrentScene(const std::string& sName)
{
	std::map<std::string,AbstractScene*>::iterator it = m_mScenes.find(sName);
	if (it == m_mScenes.end())
		std::cerr<<"ERROR : No Scene found"<<std::endl;
	else
	{
		m_pCurrentScene = (*it).second;
	}
}

void SceneManager::display()
{
	assert(m_pCurrentScene);
	m_pCurrentScene->display();

}

void SceneManager::handleKeyUp(unsigned char c, int x, int y)
{
	assert(m_pCurrentScene);
	m_pCurrentScene->handleKeyUp(c,x,y);

}

void SceneManager::handleKeyDown(unsigned char c, int x, int y)
{
	assert(m_pCurrentScene);
	m_pCurrentScene->handleKeyDown(c,x,y);

}

void SceneManager::reset()
{
	assert(m_pCurrentScene);
	m_pCurrentScene->reset();

}
void SceneManager::setCamera(AbstractCamera* pCam)
{
	assert(m_pCurrentScene);
	m_pCurrentScene->setCamera(pCam);
}
	
bool SceneManager::isFinished()
{
	assert(m_pCurrentScene);
	return m_pCurrentScene->isFinished();

}

const std::string& SceneManager::getNextScene()
{
	assert(m_pCurrentScene);
	return m_pCurrentScene->getSuccessorName();
}

AbstractScene* SceneManager::getCurrentScene()
{
	return m_pCurrentScene;
}

