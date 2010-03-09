#include "outdoorScene.h"

#include "plan.h"
#include "heightMap.h"
#include "cubeMapObject.h"
#include "FBO.h"
#include "abstractCamera.h"

#include "glh/glh_linear.h"

//Managers
#include "shaderManager.h"
#include "shader.h"
#include "textureManager.h"
#include "texture2D.h"
#include "meshManager.h"
#include "objMesh.h"

#include "Spline.hpp"

#include "grassField.h"


OutdoorScene::OutdoorScene(const std::string& sName, AbstractCamera* pCam):AbstractScene(sName,pCam)
{
	cubeMap = NULL;
	map = NULL;
	water = NULL;
	m_pWaterReflexionFBO = NULL;

	m_pGrass0 = NULL;
}

OutdoorScene::~OutdoorScene()
{
	delete cubeMap;
	delete map;
	delete water;
	delete m_pWaterReflexionFBO;
	delete m_pGrass0;

	delete[] m_fLightPosition;
}

bool OutdoorScene::init()
{

	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();

	//Spline
	cameraSpline = new Spline("../../data/Spline/outdoor_pos.xml",0.5);
	cameraAimSpline = new Spline("../../data/Spline/outdoor_aim.xml",0.5);

	//Specific Shader
	if (!(shaderManager.addShader("field", "../Shaders/field.vert","../Shaders/field.frag")))
		return false;
	if (!(shaderManager.addShader("water", "../Shaders/water.vert","../Shaders/water.frag")))
		return false;
	if (!(shaderManager.addShader("dayAndNight", "../Shaders/dayAndNight.vert","../Shaders/dayAndNight.frag")))
		return false;


	shaderManager.getShader("water")->Activate();
	shaderManager.getShader("water")->setUniformTexture("reflexion",0);
	shaderManager.getShader("water")->setUniformTexture("refraction",1);
	shaderManager.getShader("water")->setUniformTexture("perlin",2);
	shaderManager.getShader("water")->Desactivate();



	shaderManager.getShader("field")->Activate();
	shaderManager.getShader("field")->setUniformTexture("tex1",0);
	shaderManager.getShader("field")->setUniformTexture("tex2",1);
	shaderManager.getShader("field")->setUniformTexture("tex3",2);
	shaderManager.getShader("field")->setUniformTexture("spec",3);
	shaderManager.getShader("field")->setUniformTexture("spec_texture",4);
	shaderManager.getShader("field")->setUniformTexture("bump",5);
	shaderManager.getShader("field")->setUniformf("height_max",7.0);
	shaderManager.getShader("field")->Desactivate();


	shaderManager.getShader("dayAndNight")->Activate();
	shaderManager.getShader("dayAndNight")->setUniformTexture("day",0);
	shaderManager.getShader("dayAndNight")->setUniformTexture("night",1);
	shaderManager.getShader("dayAndNight")->Desactivate();



	//CubeMap
	cubeMap = new CubeMapObject("cube", "cubeNight");

	cubeMap->setTexture(E_CUBEMAP_NEG_X,"../../data/sky_rt.tga",0);
	cubeMap->setTexture(E_CUBEMAP_NEG_Y,"../../data/sky_dn.tga",0); 
	cubeMap->setTexture(E_CUBEMAP_NEG_Z,"../../data/sky_bk.tga",0); 
	cubeMap->setTexture(E_CUBEMAP_POS_X,"../../data/sky_lf.tga",0);
	cubeMap->setTexture(E_CUBEMAP_POS_Y,"../../data/sky_up.tga",0); 
	cubeMap->setTexture(E_CUBEMAP_POS_Z,"../../data/sky_ft.tga",0);

	cubeMap->setTexture(E_CUBEMAP_NEG_X,"../../data/sky_rt_night.tga",1);
	cubeMap->setTexture(E_CUBEMAP_NEG_Y,"../../data/sky_dn_night.tga",1); 
	cubeMap->setTexture(E_CUBEMAP_NEG_Z,"../../data/sky_bk_night.tga",1); 
	cubeMap->setTexture(E_CUBEMAP_POS_X,"../../data/sky_lf_night.tga",1);
	cubeMap->setTexture(E_CUBEMAP_POS_Y,"../../data/sky_up_night.tga",1); 
	cubeMap->setTexture(E_CUBEMAP_POS_Z,"../../data/sky_ft_night.tga",1);

	//HeightMap
	map = new HeightMap("../../data/test_heightmap.jpg",300,90,90,20.0);
	//water
	water = new Plan(1,400,400);

	//ground
	ground = new Plan(1,400,400);

	//Outdoor
	meshManager.loadMesh("temple.obj");
	meshManager.loadMesh("pier.obj");
	meshManager.loadMesh("arbre.obj");
	meshManager.loadMesh("grass.obj");
	meshManager.loadMesh("plane.obj");
	meshManager.loadMesh("collumns.obj");

	//Texture loading
	TextureManager & textureManager = TextureManager::getInstance();
	textureManager.getTexture2D("../../data/perlin_noise_water_DUDV.jpg");
	textureManager.getTexture2D("../../data/perlin_noise_water_NM.jpg");
	textureManager.getTexture2D("../../data/mult01.JPG");
	textureManager.getTexture2D("../../data/mult02.JPG");
	textureManager.getTexture2D("../../data/mult03.JPG");
	textureManager.getTexture2D("../../data/heightMap_specific_texture.jpg");
	textureManager.getTexture2D("../../data/moss_diffuse.jpg");
	textureManager.getTexture2D("../../data/moss_NM_height.png");
	textureManager.getTexture2D("../../data/sun.png");
	textureManager.getTexture2D("../../data/tree_diffuse.jpg");
	textureManager.getTexture2D("../../data/wall_temple_diffuse.jpg");
	textureManager.getTexture2D("../../data/wall_temple_NM.jpg");
	textureManager.getTexture2D("../../data/door_temple_diffuse.jpg");
	textureManager.getTexture2D("../../data/door_temple_normal.jpg");
	textureManager.getTexture2D("../../data/roof_temple_diffuse.jpg");
	textureManager.getTexture2D("../../data/roof_temple_normal.jpg");


	//Set the value of the light
	m_fLightPosition = new GLfloat[4];

	m_fLightPosition[0] = 0.0;
	m_fLightPosition[1] = 30.0;
	m_fLightPosition[2] = 0.0;
	m_fLightPosition[3] = 0.0;


	m_fSunDistance = 80.0;
	
	m_fWaterLevel = 1.5;


	m_pWaterReflexionFBO = new FBO(1024,768,E_FBO_2D);
	m_pWaterReflexionFBO->generateColorOnly();


	//m_pGrass0 = new GrassField(GRASS_CIRCLE,2.0,3.0,12.0);
	m_pGrass0 = new GrassField(GRASS_CIRCLE,2.0,3.0,25.0);
	m_fGrassAngle = 0.0;

	//CameraState
	m_iStartPosition = 0;
	m_iStartAim = 0 ;
	m_iStartControlPosition = 0;


	return true;
}

void OutdoorScene::renderGrass()
{

	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	textureManager.getTexture2D("../../data/grass.tga")->activate();
	shaderManager.getShader("grass")->Activate();
	shaderManager.getShader("grass")->setUniformf("angle",m_fGrassAngle);
	shaderManager.getShader("grass")->setUniformi("sens",1);
	float posX, posZ;

	//1
	posX = 34.0;
	posZ = -52.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	posX = 36.0;
	posZ = -52.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//2
	posX = 34.0;
	posZ = -55.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//3
	posX = 37.0;
	posZ = -52.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//4
	posX = 42.0;
	posZ = -52.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//5
	posX = 40.0;
	posZ = -51.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//6
	posX = 43.0;
	posZ = -49.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//7
	posX = 45.0;
	posZ = -49.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();


	//8
	posX = 47.0;
	posZ = -45.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//9
	posX = 47.0;
	posZ = -48.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//10
	posX = 45.0;
	posZ = -44.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//11
	posX = 50.0;
	posZ = -44.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//12
	posX = 51.0;
	posZ = -42.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//13
	posX = 43.0;
	posZ = -39.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//14
	posX = 46.0;
	posZ = -41.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//15
	posX = 42.0;
	posZ = -39.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//16
	posX = 38.0;
	posZ = -39.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//17
	posX = 35.0;
	posZ = -40.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	//18
	posX = 32.0;
	posZ = -41.0;

	glPushMatrix();
		glTranslatef(posX,0.0,posZ);
		m_pGrass0->draw(map,posX,posZ);		
	glPopMatrix();

	shaderManager.getShader("grass")->Desactivate();

}


void OutdoorScene::renderWater()
{
	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	shaderManager.getShader("water")->Activate();

	glActiveTexture(GL_TEXTURE0);
	m_pWaterReflexionFBO->activateTexture();

	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/perlin_noise_water_DUDV.jpg")->activate();
	glActiveTexture(GL_TEXTURE2);
	textureManager.getTexture2D("../../data/perlin_noise_water_NM.jpg")->activate();

		glPushMatrix();
			glRotatef(90,-1,0,0);
			int iTx = water->getWidth()/2;
			int iTy = water->getHeight()/2;
			glTranslatef(-iTx ,-iTy ,m_fWaterLevel);
			water->draw();			
		glPopMatrix();
	
	glActiveTexture(GL_TEXTURE0);
	m_pWaterReflexionFBO->desactivateTexture();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/perlin_noise_water_DUDV.jpg")->desactivate();
	glActiveTexture(GL_TEXTURE2);
	textureManager.getTexture2D("../../data/perlin_noise_water_NM.jpg")->desactivate();
	glActiveTexture(GL_TEXTURE0);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	shaderManager.getShader("water")->Desactivate();
}


void OutdoorScene::renderEnvironment(bool m_bWater)
{

	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	glPushMatrix();
		if (m_bWater)
		{
			m_pWaterReflexionFBO->activate();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//Inverse the lookat for the water
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glScalef(-1.0, 1.0, 1.0);
			m_pCam->invertLookAt(m_fWaterLevel);
			
			
		}

		//CubeMap
		glColor3f(1,1,1);
		
		m_fCubemapMix = m_fCubemapMix > 1.0 ? 1.0 : m_fCubemapMix;
		m_fCubemapMix = m_fCubemapMix < 0.0 ? 0.0 : m_fCubemapMix;

		glPushMatrix();
		
		glTranslatef(0.0,-8.0,0.0);
	
			cubeMap->display(m_fCubemapMix);
			cubeMap->desactivate();
		
		glPopMatrix();

		//We don't need all the faces
		if (!m_bWater)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);


		//LightParameter
		
		glPushMatrix();
		glRotatef(75.0,0.0,1.0,0.0);
		glRotatef(m_fSunAngle,1.0,0.0,0.0);
		glLightfv(GL_LIGHT0, GL_POSITION,m_fLightPosition);
		glPopMatrix();


		glLightfv(GL_LIGHT0, GL_SPECULAR, m_fBlackColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, m_fWhiteColor);
		glLightfv(GL_LIGHT0, GL_AMBIENT, m_fWhiteColor);

		glMaterialfv(GL_FRONT, GL_SPECULAR,m_fWhiteColor);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fWhiteColor);
		glMaterialfv(GL_FRONT, GL_AMBIENT,m_fDarkGreyColor);
		glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);
		


		////// Clipping //////
		//Prevents reflexion of what's under the water level

		glh::matrix4f P,M;
		
		if(m_bWater){
		glPushMatrix();
		glRotatef(90,1.0,0.0,0.0);
		glTranslatef(0.0,0.0,-m_fWaterLevel);
		GLfloat p[16];

		glGetFloatv(GL_PROJECTION_MATRIX, p);
		P = glh::matrix4f(p);

		GLfloat m[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, m);
		M = glh::matrix4f(m);
       
		glh::matrix4f invtrans_MVP = (P * M).inverse().transpose();

		glh::vec4f oplane(0,0,-1,0);
		glh::vec4f cplane;
		invtrans_MVP.mult_matrix_vec(oplane, cplane);

		cplane /= abs(cplane[2]); // normalize such that depth is not scaled
		cplane[3] -= 1;

		if(cplane[2] < 0)
			cplane *= -1;

		glh::matrix4f suffix;
		suffix.set_row(2, cplane);
		glh::matrix4f newP = suffix * P;
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(newP.m);
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		}
		////// end clipping //////

		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
		

		//-----------------HeightMap
		shaderManager.getShader("field")->Activate();
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/mult01.JPG")->activate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/mult02.JPG")->activate();
		glActiveTexture(GL_TEXTURE2);
		textureManager.getTexture2D("../../data/mult03.JPG")->activate();
		glActiveTexture(GL_TEXTURE3);
		textureManager.getTexture2D("../../data/heightMap_specific_texture.jpg")->activate();
		glActiveTexture(GL_TEXTURE4);
		textureManager.getTexture2D("../../data/moss_diffuse.jpg")->activate();
		//glActiveTexture(GL_TEXTURE5);
		//textureManager.getTexture2D("../../data/terrain_detail_NM.tga")->activate();

		glPushMatrix();
			glRotatef(90,-1,0,0);
			glTranslatef(-1,0,0.5);
			if (!m_bWater) glCullFace(GL_FRONT); // FRONT because the height map is Backward
			else glCullFace(GL_BACK);
			map->draw();
		glPopMatrix();

		if (!m_bWater)	//if we are not rendering the water FBO, we are drawing the floor
		{
			glPushMatrix();
				glRotatef(90,-1,0,0);
				int iTx = ground->getWidth()/2;
				int iTy = ground->getHeight()/2;
				glTranslatef(-iTx,-iTy,0.67);
				ground->draw();			
			glPopMatrix();
		}
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/mult01.JPG")->desactivate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/mult02.JPG")->desactivate();
		glActiveTexture(GL_TEXTURE2);
		textureManager.getTexture2D("../../data/mult03.JPG")->desactivate();
		glActiveTexture(GL_TEXTURE3);
		textureManager.getTexture2D("../../data/heightMap_specific_texture.jpg")->desactivate();
		glActiveTexture(GL_TEXTURE4);
		textureManager.getTexture2D("../../data/moss_diffuse.jpg")->desactivate();
		//glActiveTexture(GL_TEXTURE5);
		//textureManager.getTexture2D("../../data/terrain_detail_NM.tga")->desactivate();
		glActiveTexture(GL_TEXTURE0);

		shaderManager.getShader("field")->Desactivate();
		//-----------------End_HeightMap
		
		glLightfv(GL_LIGHT0, GL_SPECULAR, m_fWhiteColor);
		glMaterialfv(GL_FRONT, GL_SPECULAR,m_fWhiteColor);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fWhiteColor);
		glMaterialfv(GL_FRONT, GL_AMBIENT,m_fDarkGreyColor);

		glMaterialf( GL_FRONT, GL_SHININESS, 70.0f);

		//-----------------Houses
		shaderManager.getShader("light")->Activate();
		shaderManager.getShader("light")->setUniformi("useTexture",1);
		shaderManager.getShader("light")->setUniformi("useBump",1);
		shaderManager.getShader("light")->setUniformf("fSize",15.0);
		shaderManager.getShader("light")->setUniformi("useShadowMap",0);

		glCullFace(GL_BACK);

		glPushMatrix();
			glTranslatef(39.0,4.4,-47);	
			glRotatef(270.0,0,1,0);
			drawTemple();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(44.0,4.65,-45.5);	
			glRotatef(195.0,0,1,0);
			drawTemple();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(46.0,4.65,-41);	
			glRotatef(150.0,0,1,0);
			drawTemple();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(40.5,4.35,-36.0);	
			glRotatef(60.0,0,1,0);
			drawTemple();
		glPopMatrix();


		//-----------------End_house
		
		

		glMaterialf( GL_FRONT, GL_SHININESS, 5.0f);
		//-----------------Pier
		glPushMatrix();
			glTranslatef(29.0,1.3,-45.0);
			glRotatef(-110,0,1,0);
			glScalef(0.08,0.08,0.08);


			shaderManager.getShader("light")->setUniformf("fSize",10.0);
			shaderManager.getShader("light")->setUniformi("useBump",1);
			shaderManager.getShader("light")->setUniformf("heightFactor",0.01);

			glActiveTexture(GL_TEXTURE0);
			textureManager.getTexture2D("../../data/moss_diffuse.jpg")->activate();
			glActiveTexture(GL_TEXTURE1);
			textureManager.getTexture2D("../../data/moss_NM_height.png")->activate();

			meshManager.getMesh("pier.obj")->Draw(0);



			shaderManager.getShader("light")->setUniformf("fSize",2.0);
			glActiveTexture(GL_TEXTURE0);
			textureManager.getTexture2D("../../data/wall_temple_diffuse.jpg")->activate();
			glActiveTexture(GL_TEXTURE1);
			textureManager.getTexture2D("../../data/wall_temple_NM.jpg")->activate();

			meshManager.getMesh("pier.obj")->Draw(1);
			meshManager.getMesh("pier.obj")->Draw(2);

			glActiveTexture(GL_TEXTURE0);
			textureManager.getTexture2D("../../data/wall_temple_diffuse.jpg")->desactivate();
			glActiveTexture(GL_TEXTURE1);
			textureManager.getTexture2D("../../data/wall_temple_NM.jpg")->desactivate();
			glActiveTexture(GL_TEXTURE0);
			
		glPopMatrix();
		//-----------------End pier


		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/stonefloor_diffuse.jpg")->activate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/stonefloor_normal.jpg")->activate();
		glActiveTexture(GL_TEXTURE0);
		
		glPushMatrix();
			glTranslatef(38.3,4.3,-48.0);
			glRotatef(90.0,0,1,0);
			glScalef(0.01,0.01,0.01);
			meshManager.getMesh("collumns.obj")->Draw();
		glPopMatrix();

		glLightfv(GL_LIGHT0, GL_SPECULAR, m_fBlackColor);
		shaderManager.getShader("light")->setUniformf("fSize",4.0);
		shaderManager.getShader("light")->setUniformi("useBump",0);
		shaderManager.getShader("light")->setUniformi("tree",1);

		//-----------------Tree
		glPushMatrix();
			glTranslatef(40.0,4.2,-55.0);
			drawTree();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(50.0,4.5,-45.0);
			glRotatef(30.0,0,1,0);
			drawTree();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(30.5,3.2,-40.0);
			glRotatef(165.0,0,1,0);
			drawTree();
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(45.3,4.5,-37.0);
			glRotatef(270.0,0,1,0);
			glScalef(1.1,1.1,1.1);
			drawTree();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(37.3,4.1,-52.0);
			glRotatef(90.0,0,1,0);
			glScalef(0.8,0.8,0.8);
			drawTree();
		glPopMatrix();
		//-----------------EndTree

		glLightfv(GL_LIGHT0, GL_SPECULAR, m_fWhiteColor);
		shaderManager.getShader("light")->setUniformi("tree",0);
		shaderManager.getShader("light")->setUniformf("fSize",1.0);
		shaderManager.getShader("light")->Desactivate();
		
		

		//-----------------Sun
		textureManager.getTexture2D("../../data/sun.png")->activate();
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glPushMatrix();			
			glTranslatef(m_pCam->getPos()[0],m_pCam->getPos()[1],m_pCam->getPos()[2]);
			glRotatef(75.0,0.0,1.0,0.0);
			glRotatef(m_fSunAngle,1.0,0.0,0.0);
			glTranslatef(0.0,m_fSunDistance,0.0);
			glRotatef(90,1.0,0.0,0.0);
			glScalef(3.0,3.0,3.0);
			glColor3f(1.0,1.0,1.0);
			meshManager.getMesh("plane.obj")->Draw();
		glPopMatrix();
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		textureManager.getTexture2D("../../data/sun.png")->desactivate();
		//-----------------End_Sun

		
		
		////// stop clipping //////
		if (m_bWater)
		{
			m_pWaterReflexionFBO->desactivate();
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(P.m);
			glMatrixMode(GL_MODELVIEW);
			
		}



	glPopMatrix();
	glDisable(GL_CULL_FACE);



}

void OutdoorScene::preRender()
{
	//Render for the water (TRUE / FALSE)
	cubeMap->update(m_pCam,true,m_fWaterLevel);
	renderEnvironment(true);
}

void OutdoorScene::render()
{

	//Clean
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	m_pCam->lookAt();


	if (m_bDisplaySpline)
		displaySpline();
				
	cubeMap->update(m_pCam,false,m_fWaterLevel);
	//Now we can finally use a Final FBO
	renderEnvironment(false);
	renderWater();
	renderGrass();

}


		
void OutdoorScene::update()
{
	AbstractScene::update();
	///CubeMap Operation
	cubeMap->update(m_pCam,false,m_fWaterLevel);

	m_fGrassAngle += 0.01;

}

bool OutdoorScene::isFinished()
{
	switch(m_pCam->getCurrentControlPoint())
	{
		/// Arrived at the first house
		case 8 :
			m_iStartPosition = m_pCam->getCurrentPositionPoint();
			m_iStartAim = m_pCam->getCurrentAimPoint();
			m_iStartControlPosition = m_pCam->getCurrentControlPoint();
			m_sNextSceneName = "parallax";
			return true;

		/// Arrived at the second house
		case 11 :
			m_iStartPosition = m_pCam->getCurrentPositionPoint();
			m_iStartAim =m_pCam->getCurrentAimPoint();
			m_iStartControlPosition = m_pCam->getCurrentControlPoint();
			m_sNextSceneName = "shadows";
			return true;

		/// Arrived at the third house
		case 16 :
			m_iStartPosition = m_pCam->getCurrentPositionPoint();
			m_iStartAim = m_pCam->getCurrentAimPoint();
			m_iStartControlPosition = m_pCam->getCurrentControlPoint();
			m_sNextSceneName = "toon";
			return true;

		/// Arrived at the fourth house
		case 19 :
			m_iStartPosition = m_pCam->getCurrentPositionPoint();
			m_iStartAim = m_pCam->getCurrentAimPoint();
			m_iStartControlPosition = m_pCam->getCurrentControlPoint();
			m_sNextSceneName = "scifi";
			return true;

		default : 
			return false;
	}
	return false;
		
}

void OutdoorScene::handleKeyUp(unsigned char c, int x, int y)
{
	AbstractScene::handleKeyUp(c,x,y);
	

}

void OutdoorScene::handleKeyDown(unsigned char c, int x, int y){

	switch(c){ 
		case 'h' :
			m_fSunAngle += 1.0;
			m_fCubemapMix = exp(abs(m_fSunAngle/90.0)+0.1)-2;
			
		break;
		case 'j' :
			m_fSunAngle -= 1.0;
			m_fCubemapMix = exp(abs(m_fSunAngle/90.0)+0.1)-2;
		break;

	}
}



void OutdoorScene::reset()
{

	AbstractScene::reset();
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT,m_fDarkGreyColor);
	
	m_fSunAngle = -70.0;
	m_fCubemapMix = exp(abs(m_fSunAngle/90.0)+0.1)-2;
	m_pCam->setSpeed(0.09);
	
	if (m_iStartPosition != 0 && m_iStartAim != 0 && m_iStartControlPosition != 0)
	{
		m_pCam->setStart(m_iStartPosition, m_iStartAim,m_iStartControlPosition);
		m_iStartPosition = 0;
		m_iStartAim = 0;
		m_iStartControlPosition = 0;
	}

}

void OutdoorScene::drawTemple()
{

	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	glScalef(0.02,0.02,0.02);

	glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);
	
	//The big part
	shaderManager.getShader("light")->setUniformf("fSize",11.0);
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/wall_temple_diffuse.jpg")->activate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/wall_temple_NM.jpg")->activate();
	
	meshManager.getMesh("temple.obj")->Draw(0);
	
	//The door
	shaderManager.getShader("light")->setUniformf("fSize",1.0);
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/door_temple_diffuse.jpg")->activate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/door_temple_normal.jpg")->activate();
	
	meshManager.getMesh("temple.obj")->Draw(1);

	//The roof
	shaderManager.getShader("light")->setUniformf("fSize",3.5);
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/roof_temple_diffuse.jpg")->activate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/roof_temple_normal.jpg")->activate();

	meshManager.getMesh("temple.obj")->Draw(2);

	
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/roof_temple_diffuse.jpg")->desactivate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/roof_temple_normal.jpg")->desactivate();
	glActiveTexture(GL_TEXTURE0);


}

void OutdoorScene::drawTree()
{
	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	glScalef(0.02,0.02,0.02);		
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/tree_diffuse.jpg")->activate();
	meshManager.getMesh("arbre.obj")->Draw();	//Trunc

	textureManager.getTexture2D("../../data/tree_diffuse.jpg")->desactivate();

}