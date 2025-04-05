#include "tglfw.h"
#include "glew.h"
#include "wglew.h"
#include "sys_gl.h"
#include "log.h"

//========================================================================
// Setup and OpenGl Ortho 2D Window, Depreciated OGL 1.0-2.0 
//========================================================================
void set_ortho_view(int width, int height)
{
	glViewport(0, 0, width, height);             // Set Up An Ortho View	 
	glMatrixMode(GL_PROJECTION);			  // Select Projection
	glLoadIdentity();						  // Reset The Matrix
	glOrtho(0, width, 0, height, -1, 1);	  // Select Ortho 2D Mode DirectX style(640x480)
	glMatrixMode(GL_MODELVIEW);				  // Select Modelview Matrix
	glLoadIdentity();						  // Reset The Matrix
}


//========================================================================
// Resize OpenGL Window
//========================================================================
void gl_resize(int width, int height)
{
	if (height == 0)                // Prevent A Divide By Zero By
		height = 1;                    // Making Height Equal One
	SCREEN_W = width;             // Screen Width
	SCREEN_H = height;            // Screen Height
	set_ortho_view(width, height);
}


//========================================================================
// Enable Opengl Code,  depreciated 2.0 only
//========================================================================
int EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;

	// get the device context (DC)
	*hDC = GetDC(hWnd);

	// set the pixel format for the DC
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat(*hDC, &pfd);
	SetPixelFormat(*hDC, format, &pfd);

	// create and enable the render context (RC)
	*hRC = wglCreateContext(*hDC);
	wglMakeCurrent(*hDC, *hRC);

	//Enable GLEW
	GLenum error = glewInit(); // Enable GLEW
	if (error != GLEW_OK) // If GLEW fails
	{
		wrlog("Glew Init Failed");	return false;
	}

	wrlog("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	return true;
}

//========================================================================
// Enable Opengl 3/4 Code.
//========================================================================
int EnableOpenGL3(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	*hDC = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
	pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
	pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
	pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
	pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

	int nPixelFormat = ChoosePixelFormat(*hDC, &pfd); // Check if our PFD is valid and get a pixel format back
	if (nPixelFormat == 0) // If it fails
		return false;

	int bResult = SetPixelFormat(*hDC, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD
	if (!bResult) // If it fails
		return false;

	HGLRC tempOpenGLContext = wglCreateContext(*hDC); // Create an OpenGL 2.1 context for our device context
	wglMakeCurrent(*hDC, tempOpenGLContext); // Make the OpenGL 2.1 context current and active

	glewExperimental = true;
	GLenum error = glewInit(); // Enable GLEW
	if (error != GLEW_OK) // If GLEW fails
	{
		wrlog("Glew Init Failed");	return false;
	}

	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // Set the MAJOR version of OpenGL to 3
		WGL_CONTEXT_MINOR_VERSION_ARB, 2, // Set the MINOR version of OpenGL to 2
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB , //Set the compatibility
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1) { // If the OpenGL 3.x context creation extension is available
		*hRC = wglCreateContextAttribsARB(*hDC, NULL, attributes); // Create and OpenGL 3.x context based on the given attributes
		wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
		wglDeleteContext(tempOpenGLContext); // Delete the temporary OpenGL 2.1 context
		wglMakeCurrent(*hDC, *hRC); // Make our OpenGL 3.0 context current
	}
	else
	{
		*hRC = tempOpenGLContext; // If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context
	}

	int glVersion[2] = { -1, -1 }; // Set some default values for the version
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using

	wrlog("Gl version %x:%x", glVersion[0], glVersion[1]);
	wrlog("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (GLEW_EXT_framebuffer_multisample)
	{
		//Example of checking extensions
	}

	return true; // We have successfully created a context, return true
};


//========================================================================
// Disable and release the OpenGL Context
//========================================================================
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}


//========================================================================
// Enable / Disable vSync
//========================================================================
void win_set_vsync(bool enable)
{
	if (enable == TRUE)
	{
		wglSwapIntervalEXT(1);
	}
	else
	{
		wglSwapIntervalEXT(0);
	}
}

