#include "grass.h"
#include "meshManager.h"
#include "objMesh.h"

grass::grass(int iType, int iTurn):
m_iType(iType),
m_iTurn(iTurn){


}



void grass::draw()const{

	glDisable(GL_CULL_FACE);

	glPushMatrix();
		MeshManager::getInstance().getMesh("grass.obj")->Draw();
	glPopMatrix();

	glEnable(GL_CULL_FACE);
}