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

#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

/**
*	@name sceneManager.h
*	@brief Manager all our scenes
*	@author Franck Letellier
*	@date 18/12/09
*/

#include <string>
#include <map>

#include "singleton.h"


class AbstractScene;
class AbstractCamera;

class SceneManager:public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;

	public:
		void addScene(AbstractScene* pScene);
		void setCurrentScene(const std::string& sName);

		void clear();

		//Update and display the current scene
		void update();
		void display();
		void handleKeyUp(unsigned char c, int x, int y);
		void handleKeyDown(unsigned char c, int x, int y);
		void reset();
		void setCamera(AbstractCamera* pCam);

		bool isFinished();
		const std::string& getNextScene();
	
	private:
		std::map<std::string,AbstractScene*> m_mScenes;
		AbstractScene* m_pCurrentScene;

		AbstractScene* getCurrentScene();

		SceneManager();
		virtual ~SceneManager(){clear();};

};


#endif //__SCENEMANAGER_H__