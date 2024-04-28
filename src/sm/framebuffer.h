//
// Created by Nate Spielman on 4/28/2024.
//
#ifndef SHADERMAKER_FRAMEBUFFER_H
#define SHADERMAKER_FRAMEBUFFER_H

namespace sm {
	struct Framebuffer {
		unsigned int fbo;
		unsigned int colorBuffer[8];
		unsigned int depthBuffer;
		unsigned int width;
		unsigned int height;
	};
	Framebuffer createFramebuffer(unsigned int width, unsigned int height, int colorFormat);
}
#endif //SHADERMAKER_FRAMEBUFFER_H