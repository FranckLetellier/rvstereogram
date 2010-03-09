#include "grassField.h"
#include <math.h>

GrassField::GrassField(GrassType type, int dim , int spreadX , int spreadY){
	this->type = type;
	this->dim = dim;
	this->spreadX = spreadX;
	this->spreadY = spreadY;

	grassPoints = new std::vector<grassPoint>();
	herbe = new std::vector<grass *>();

	init();
}

GrassField::~GrassField(){

	delete grassPoints;
	delete herbe;

}

void GrassField::init(){

	
	std::cout << "init grass field" << std::endl;
	
	
	switch (type){
		case GRASS_CIRCLE:
			initCircle();
		break;
		
		case GRASS_SQUARE:
			initSquare();
		break;
		
		default:
		break;

	}

}

void GrassField::initCircle(){
		
	float randX;
	float randY;
	float randRotation;
	//srand(time(null));
	for(float r = dim; r>=0.0; r-=float(dim)/float(spreadX)){

		for(float alpha = 0.0; alpha<=360.0; alpha+=360.0/spreadY){

			grassPoint point;
			
			randX = (float)(rand() % 10)/10; 
			randY = (float)(rand() % 10)/10; 
			randRotation = (float)(rand() % 360); 

			point.x = r*cos(alpha)+randX;
			point.y = r*sin(alpha)+randY;
			point.rotation = randRotation;

			grassPoints->push_back(point);

			herbe->push_back(new grass(0,1));
		}
	}
}

void GrassField::initSquare(){
}

void GrassField::draw(HeightMap * const map, const float posX, const float posZ){

	if(grassPoints->empty()) return;
	
	grassPoint point;
	float y;
	float fWidth = map->getWidth();
	float fDiscretization = map->getDiscretization();
	osg::Vec3f pos;
	pos.y() = 0.0;

	for(unsigned int i=0; i<grassPoints->size();i++){			
		point = grassPoints->at(i);

		pos.x() = (point.x + posX + 1.0) / fWidth * fDiscretization;
		pos.z() = (point.y + posZ - 0.5) / fWidth * fDiscretization * -1;
		y = map->getHeightAtPosition(pos);

		glPushMatrix();
			glTranslatef(point.x,y+0.5,point.y);
			glRotatef(point.rotation,0.0,1.0,0.0);
			herbe->at(i)->draw();						
		glPopMatrix();
	}



}
