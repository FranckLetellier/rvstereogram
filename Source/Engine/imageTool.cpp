

#include <iostream>
#undef _UNICODE
#include "il.h"

#include "imageTool.h"
#include "FBO.h"

bool ImageTool::m_bInit = false;
GLubyte* ImageTool::m_pTextureStereo = NULL;

GLubyte* ImageTool::loadImage(const std::string& sName, unsigned int& iWidthOut,unsigned int& iHeightOut, unsigned int& iChannelOut)
{
	std::cout<<"LOADING : " <<sName<<std::endl;
	/// we are using DevIL just as a img loader, just for the data, we'll handle the rest
	ILuint ImageName;
	
	///Create the name for the image
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	///Load the image and return a Bool whether it true or false
	if (!ilLoadImage(sName.c_str()))
	{
		//TODO: Better exit handling
		std::cout<<"ERROR : Couldn't load the image " <<sName<<std::endl;
		return NULL;
	}

	///Save the datas
	iWidthOut = ilGetInteger(IL_IMAGE_WIDTH);
	iHeightOut = ilGetInteger(IL_IMAGE_HEIGHT);
	iChannelOut = ilGetInteger(IL_IMAGE_BPP); // RGB? RGBA?

	if(iChannelOut==4)
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	ILubyte *data = ilGetData();
	//We need to retrieve the data out of the devIL Class
	GLubyte* img = new GLubyte[(size_t)(iWidthOut) * (size_t)(iHeightOut) * (size_t)(iChannelOut)];
	memcpy(img, data, (size_t)(iWidthOut) * (size_t)(iHeightOut) * (size_t)(iChannelOut));

    // Suppression de la texture
    ilBindImage(0);
    ilDeleteImages(1, &ImageName);
	
	return img;

}

void ImageTool::SIRDS(FBO* oFboOrigin, unsigned int iEyeDistance,GLuint	&iHandle)
{
	//FBO* resFBO = new FBO(oFboOrigin.getWidth(),oFboOrigin.getHeight(),E_FBO_2D);
	int width = oFboOrigin->getWidth();
	int height = oFboOrigin->getHeight();
	unsigned char* img = new unsigned char[3*width*height];
	//GLfloat* img = new GLfloat[3*width*height];
	///Create the array that will contain the FBO
	oFboOrigin->activateTexture();
		glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,img);
	oFboOrigin->desactivateTexture();
	
	unsigned int * T = new unsigned int[width];
	GLfloat z = 0;
	GLfloat s = 0;
	unsigned char * image = new unsigned char[3*(size_t)(width)*(size_t)(height)];

	for(unsigned int l = 0;  l<height ; l++){

		int left, right; 

		for(unsigned int i = 0; i<width ; i++){
			T[i] = i; //init
		}
		for(unsigned int i = 0 ; i<width ; i++){

			z = img[l*width*3+i*3]/255.0;

			float mu = 1.0/3.0;
			s = ((1.0-(z*mu))*iEyeDistance) / (2.0-(z*mu));

			

			if (s>=0){
				left = i-s/2;
				right = left+s;
				if(right<width && left>=0 ){
  
					unsigned int visible;
					int t = 1;
					float zt;
					do{ 
						zt = img[l*width*3+i*3]/255.0 + 2*(2 - mu*(img[l*width*3+i*3]/255.0))*t/(mu*iEyeDistance);
   						visible = (img[l*width*3+(i-t)*3]/255.0)<zt && (img[l*width*3+(i+t)*3]/255.0)<zt; // False if obscured  
						 t++;
					} 
					while( visible && zt < 1 ); 

					if( visible ){ 
						unsigned int ll;       

  						for( ll = T[left]; ll != left && ll != right ; ll = T[left]){
							   							
							if( ll < right ) 
         							left = ll;  
      							else { 
								left = right;
         							right = ll;
     							}
						}
						T[left] = right; 
					}
				}

			}
		}

		for(int j = width-1 ; j>=0 ; j--){
			if(T[j] == j){
				image[l*width*3+j*3] = (unsigned char) rand()%256;
				image[l*width*3+j*3+1] = (unsigned char) rand()%256;
				image[l*width*3+j*3+2] = (unsigned char) rand()%256;
			}	
			else{
				image[l*width*3+j*3] = image[l*width*3+T[j]*3];
				image[l*width*3+j*3+1] = image[l*width*3+T[j]*3+1];
				image[l*width*3+j*3+2] = image[l*width*3+T[j]*3+2];
			}
		}
	}


	glDeleteTextures(1, &iHandle);
	glGenTextures(1, &iHandle);

	//Active texture
	glBindTexture(GL_TEXTURE_2D,iHandle);

	//Create texture from file
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE , image );

	
	//desactive texture
	glBindTexture(GL_TEXTURE_2D,0);


	delete[] img;
	delete[] image;

	
}

void ImageTool::SITS(FBO* oFboOrigin, unsigned int iEyeDistance,GLuint	&iHandle)
{
	if (!m_bInit){
		unsigned int h,w,channel;
		m_pTextureStereo = loadImage("../../Data/0015.jpg",h,w,channel);
		m_bInit = true;
	}

	int width = oFboOrigin->getWidth();
	int height = oFboOrigin->getHeight();
	unsigned char* img = new unsigned char[3*width*height];
	
	///Create the array that will contain the FBO
	oFboOrigin->activateTexture();
		glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,img);
	oFboOrigin->desactivateTexture();

	unsigned int * T = new unsigned int[width];
	GLfloat z = 0;
	GLfloat s = 0;
	unsigned char * image = new unsigned char[3*(size_t)(width)*(size_t)(height)];

for(unsigned int l = 0;  l<height ; l++){

		int left, right; 

		for(unsigned int i = 0; i<width ; i++){
			T[i] = i; //init
		}
		for(unsigned int i = 0 ; i<width ; i++){

			z = img[l*width*3+i*3]/255.0;

			float mu = 1.0/3.0;
			s = ((1.0-(z*mu))*iEyeDistance) / (2.0-(z*mu));

			

			if (s>=0){
				left = i-s/2;
				right = left+s;
				if(right<width && left>=0 ){
  
					unsigned int visible;
					int t = 1;
					float zt;
					do{ 
						zt = img[l*width*3+i*3]/255.0 + 2*(2 - mu*(img[l*width*3+i*3]/255.0))*t/(mu*iEyeDistance);
   						visible = (img[l*width*3+(i-t)*3]/255.0)<zt && (img[l*width*3+(i+t)*3]/255.0)<zt; // False if obscured  
						 t++;
					} 
					while( visible && zt < 1 ); 

					if( visible ){ 
						unsigned int ll;       

  						for( ll = T[left]; ll != left && ll != right ; ll = T[left]){
							   							
							if( ll < right ) 
         							left = ll;  
      							else { 
								left = right;
         							right = ll;
     							}
						}
						T[left] = right; 
					}
				}

			}
		}
		for(int i = width-1 ; i>=0 ; i--){
			if(T[i] == i){
				image[l*width*3+i*3] = m_pTextureStereo[l*width*3+i*3];
				image[l*width*3+i*3+1] = m_pTextureStereo[l*width*3+i*3+1];
				image[l*width*3+i*3+2] = m_pTextureStereo[l*width*3+i*3+2];
			}	
			else{
				image[l*width*3+i*3] = image[l*width*3+T[i]*3];
				image[l*width*3+i*3+1] = image[l*width*3+T[i]*3+1];
				image[l*width*3+i*3+2] = image[l*width*3+T[i]*3+2];
			}
		}
	}
	glDeleteTextures(1, &iHandle);
	glGenTextures(1, &iHandle);

	//Active texture
	glBindTexture(GL_TEXTURE_2D,iHandle);

	//Create texture from file
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE , image );

	
	//desactive texture
	glBindTexture(GL_TEXTURE_2D,0);

	delete[] img;
	delete[] image;
	
}