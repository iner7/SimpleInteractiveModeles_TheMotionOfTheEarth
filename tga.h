#ifndef RYAN_TGA_H
#define RYAN_TGA_H

#include <Windows.h>
#include <gl\GL.h>

class TGA {
	private:
		GLuint textureHandle;
	public:
		TGA(char* imagePath);
		GLuint getTextureHandle(void);
};

#endif