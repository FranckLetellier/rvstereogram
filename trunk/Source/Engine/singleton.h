/*************************************
*
*		ImacDemo Project
*		
*		Created :	02/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/


#ifndef __SINGLETON_H__
#define __SINGLETON_H__
/**
*	@name singleton.h
*	@brief Singleton template class
*	@author Franck Letellier
*	@date 02/11/09
*/

template <typename T> class Singleton //typename instead of class, since it can recieve anything
{
	protected:
		static T* s_pInstance; 
		
		/**
		*	Constructor - Destructor
		*/
		Singleton(){}
		virtual ~Singleton(){}

		/**
		*	TODO : Copy Constructor - "=" operator
		*/
		

	public:

		static inline T& getInstance()
		{
			if (s_pInstance == NULL)
			{
				s_pInstance = new T;
			}
			return *s_pInstance;
		}

		static inline void killInstance()
		{
			if (s_pInstance != NULL)
				delete s_pInstance;
			s_pInstance = NULL;
		}

};

/**
*	Set our Singleton to Zero
*/

template<typename T> T* Singleton<T>::s_pInstance = NULL;

/**
*	In order to create a singleton, need this :
*	class OnlyOne : public Singleton<OnlyOne>
 {
     // constructeurs/destructeur de OnlyOne accessibles au Singleton
     friend class Singleton<OnlyOne>; 
 };
 {from Wikipedia}
*/


#endif //__SINGLETON_H__