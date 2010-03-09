#include "heightMap.h"
#include "heightMapStripe.h"
#include "imageTool.h"

#include "assert.h"
#include "Vec3"
#include <iostream>
#include <cmath>

HeightMap::HeightMap(const std::string& sName)
{
	init(sName,50,10,10,10);
}

HeightMap::HeightMap(const std::string& sName, unsigned int iDiscretization,unsigned int iWidth, unsigned int iHeight, unsigned int iZmax)
{
	init(sName,iDiscretization,iWidth,iHeight,iZmax);
}

HeightMap::~HeightMap()
{
	//std::vector<HeightMapStripe*>::iterator iter = m_pStripes.begin();
	//for (iter = m_pStripes.begin(); iter != m_pStripes.end(); ++iter)
	//	delete (*iter);
}

void HeightMap::init(const std::string& sName, unsigned int iDiscretization,unsigned int iWidth, unsigned int iHeight,unsigned int iZmax)
{	
	m_iHeight = iHeight;
	m_iWidth = iWidth;
	m_iZmax = iZmax;
	m_iDiscretization = iDiscretization;

	//Loading the image
	unsigned int iChannel;
	unsigned char* pData = NULL;
	pData = ImageTool::loadImage(sName,m_iWidthTex,m_iHeightTex,iChannel); 
	assert(pData);

	//Were are only interested into the Gray value;
	m_pData = new unsigned int[m_iWidthTex * m_iHeightTex];
	unsigned int j = 0;
	for (unsigned int i=0;i<m_iWidthTex * m_iHeightTex;++i)
	{
		//std::cout<<static_cast<unsigned int>(m_pData[i])<<std::endl;
		//convert char* into int
		m_pData[i] = static_cast<unsigned int>(pData[j]);
		j += iChannel;
	}

	float stepX = static_cast<float>(m_iWidth) / static_cast<float>(iDiscretization) ;
	float stepY = static_cast<float>(m_iHeight) / static_cast<float>(iDiscretization) ;
	
	//Index for Value
	float* vertexPosition;
	float* normal;
	float* texture;
	float* tangent;

	//HACK: Variables so that we need only one "for"
	unsigned int iForXCoord = 0;
	unsigned int iTex = 0;

	//Creating the stripes (one for each line)
	for (unsigned int j=0; j < iDiscretization ; ++j)
	{
		//* 2 (we are using TRIANGLE_STRIP)* 3 (X Y Z)
		vertexPosition = new float[(iDiscretization + 1)*2*3 ];
		normal = new float[(iDiscretization + 1)*2*3 ];
		texture = new float[(iDiscretization + 1)*2*2 ];
		tangent = new float[(iDiscretization + 1)*2*3 ];
		//Z coordinate
		unsigned int iXImage, iYImage;
		//Creating each square
		for (unsigned int i = 0; i<(iDiscretization+1) * 6; i+=6)
		{
			vertexPosition[i]	= iForXCoord * stepX;	// X1
			vertexPosition[i+1] = j * stepY;			// Y1

			//Z1
			iXImage = (vertexPosition[i]*m_iWidthTex / m_iWidth);
			if (iXImage >= m_iWidthTex)
					iXImage = m_iWidthTex-1;
			iYImage = (vertexPosition[i+1]*m_iHeightTex/m_iHeight);
			if (iYImage >= m_iHeightTex)
					iYImage = m_iHeightTex-1;
			vertexPosition[i+2] =  (m_pData[ iYImage * m_iWidthTex  + iXImage ] / 255.0) * m_iZmax;		


			vertexPosition[i+3]	= iForXCoord * stepX;	// X2
			vertexPosition[i+4] = (j+1) * stepY;		// Y2

			//Z2
			iXImage = (vertexPosition[i+3]*m_iWidthTex / m_iWidth);
			if (iXImage >= m_iWidthTex)
					iXImage = m_iWidthTex-1;
			iYImage = (vertexPosition[i+4]*m_iHeightTex / m_iHeight);
			if (iYImage >= m_iHeightTex)
					iYImage = m_iHeightTex-1;
			vertexPosition[i+5] = (m_pData[ iYImage * m_iWidthTex  + iXImage ] / 255.0) * m_iZmax;					

			normal[i]	= 0;	// X1
			normal[i+1] = 0;	// Y1
			normal[i+2] = -1;	// Z1

			normal[i+3]	= 0;	// X2
			normal[i+4] = 0;	// Y2
			normal[i+5] = -1;	// Z2

			++iForXCoord;

		}
		iForXCoord = 0;

		

		//For the texture coordinate (only 2 axes)
		for (unsigned int i = 0; i<(iDiscretization+1) * 4; i+=4)
		{
			texture[i]	= static_cast<float>(iForXCoord * stepX) / static_cast<float>(m_iWidth);	// X1
			texture[i+1] = static_cast<float>(j * stepY) / static_cast<float>(m_iHeight);			// Y1

			texture[i+2] = static_cast<float>(iForXCoord * stepX) / static_cast<float>(m_iWidth);	// X2
			texture[i+3] = static_cast<float>((j+1) * stepY) / static_cast<float>(m_iHeight);	// Y2
			++iForXCoord;
		}
		iForXCoord = 0;
		//Tangente:
		osg::Vec3 vTangent;
		for (unsigned int i = 0; i<(iDiscretization+1) * 3; i+=3)
		{
			osg::Vec3 v0(vertexPosition[i],vertexPosition[i+1],vertexPosition[i+2]);
			osg::Vec3 v1(vertexPosition[i+3],vertexPosition[i+4],vertexPosition[i+5]);
			osg::Vec3 v2(vertexPosition[i+6],vertexPosition[i+7],vertexPosition[i+8]);

			osg::Vec3 vect10 = v0-v1;
			osg::Vec3 vect12 = v2-v1;

			float deltaT10 = texture[iForXCoord+1] - texture[iForXCoord+3];
			float deltaT12 = texture[iForXCoord+5] - texture[iForXCoord+3];

			vTangent = (vect10 * deltaT12 ) - (vect12 * deltaT10 );
			vTangent.normalize();

			tangent[i] = vTangent.x();
			tangent[i+1] = vTangent.y();
			tangent[i+2] = vTangent.z();

			tangent[i+3] = vTangent.x();
			tangent[i+4] = vTangent.y();
			tangent[i+5] = vTangent.z();

			tangent[i+6] = vTangent.x();
			tangent[i+7] = vTangent.y();
			tangent[i+8] = vTangent.z();

			iForXCoord +=2; 
		}

		iForXCoord = 0;
		m_pStripes.push_back( new HeightMapStripe( ((iDiscretization +1)*2),vertexPosition ,normal,texture,tangent) );
	}

	//In order to Calculte the normal, we take the previous stripe and the next one
	for (unsigned int i = 1; i<( m_pStripes.size() - 1) ; ++i)
		m_pStripes[i]->computeNormal(m_pStripes[i-1] , m_pStripes[i+1]);


}

float HeightMap::getHeightAtPosition(const osg::Vec3f& vPosition)
{
	int posX = (int)floor(vPosition.x());
	int posZ = (int)floor(vPosition.z());

	if(posX < 0 || posZ < 0)
		return 0.0;

	const GLfloat * pArray =  m_pStripes[posZ]->getVertexArray();

	float y1 = pArray[posX * 6 + 2];
	/*float y2 = pArray[posX * 6 + 5];
	float y3 = pArray[posX * 6 + 8];
	float y4 = pArray[posX * 6 + 11];*/
	return y1;
	//return (y1 + y2 + y3 + y4) / 4.0;
}



