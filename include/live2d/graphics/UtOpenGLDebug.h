/**
 *  UtOpenGLDebug.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __UT_OPENGL_DEBUG__
#define __UT_OPENGL_DEBUG__


#ifndef __SKIP_DOC__

//------------ LIVE2D NAMESPACE ------------
namespace live2d
{ 


	class UtOpenGLDebug
	{
	public:
		static void dumpState() ;
		static int checkError( const char* str ) ;

	};


} //------------ LIVE2D NAMESPACE ------------

#endif // __SKIP_DOC__

#endif // __UT_OPENGL_DEBUG__
