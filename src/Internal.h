﻿
#pragma once

#define LN_INTERNAL_ACCESS				public
#define LN_CONSTRUCT_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public

#define NOMINMAX
#include <LuminoCore.h>
#include "../include/Lumino/Graphics/Common.h"	// TODO: Internal.h には置きたくない

#define GLEW_STATIC
#include "../external/GL/glew.h"
//#include <GL/gl.h>
#include "../external/GL/glext.h"

#if defined(LN_OS_WIN32)
#include <Objbase.h>
//#include <GL/wglew.h>
#include "../external/GL/wglext.h"
#elif defined(LN_OS_X11)
#include <GL/glx.h>
#include "../external/GL/glxext.h"
#endif

#include "Graphics/Device/OpenGL/LNGL.h"

// glGetError() でエラーチェック (各 gl～ の後で必ず呼ばないと正しいエラーが取れないので注意)
#define LN_CHECK_GLERROR()		{ GLenum lnglerr = glGetError(); LN_THROW(lnglerr == GL_NO_ERROR , OpenGLException, lnglerr); } 

