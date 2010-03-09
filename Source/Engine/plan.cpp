#include "plan.h"
#include "heightMapStripe.h"

#include <iostream>
#include <cmath>

Plan::Plan()
{
	//init(1,1,1);
}

Plan::Plan(unsigned int iDiscretization,unsigned int iWidth, unsigned int iHeight)
{
	init(iDiscretization,iWidth,iHeight);
}

Plan::~Plan()
{
	std::vector<HeightMapStripe*>::iterator iter = m_pStripes.begin();
	for (iter = m_pStripes.begin(); iter != m_pStripes.end(); ++iter)
		delete (*iter);
}

void Plan::init(unsigned int iDiscretization,unsigned int iWidth, unsigned int iHeight)
{	
	m_iHeight = iHeight;
	m_iWidth = iWidth;

	float stepX = static_cast<float>(m_iWidth) / static_cast<float>(iDiscretization) ;
	float stepY = static_cast<float>(m_iHeight) / static_cast<float>(iDiscretization) ;
	
	//Index for Value
	float* vertexPosition;
	float* normal;
	float* texture;

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

		
		//Creating each square
		for (unsigned int i = 0; i<(iDiscretization+1) * 6; i+=6)
		{
			vertexPosition[i]	= iForXCoord * stepX;	// X1
			vertexPosition[i+1] = j * stepY;			// Y1
			vertexPosition[i+2] =  0;		


			vertexPosition[i+3]	= iForXCoord * stepX;	// X2
			vertexPosition[i+4] = (j+1) * stepY;		// Y2
			vertexPosition[i+5] = 0;					

			normal[i]	= 0;	// X1
			normal[i+1] = 0;	// Y1
			normal[i+2] = 1;	// Z1

			normal[i+3]	= 0;	// X2
			normal[i+4] = 0;	// Y2
			normal[i+5] = 1;	// Z2

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
		m_pStripes.push_back( new HeightMapStripe( ((iDiscretization +1)*2),vertexPosition ,normal,texture,NULL) );
	}
}

void Plan::draw()
{
	for (std::vector<HeightMapStripe*>::iterator iter = m_pStripes.begin(); iter != m_pStripes.end(); ++iter)
		(*iter)->draw();
}

