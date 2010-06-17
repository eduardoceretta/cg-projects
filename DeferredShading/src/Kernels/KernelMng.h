#ifndef KernelMng_H
#define KernelMng_H

#include "Kernels/KernelBase.h"
#include "Kernels/KernelGenerateRay.h"
#include "Kernels/KernelTraverse.h"
#include "Kernels/KernelGeometry.h"
#include "Kernels/KernelShade.h"
#include "Kernels/KernelMngStates.h"
#include "SpacePartition/UniformGrid.h"
#include "GraphBasis/FrameBufferObject.h"
#include "Objects/Scene.h"
#include "GraphBasis/Vector3.h"

class KernelMng {

public:
	KernelMng(int width, int height,Scene* scene, float nearPlaneWidth, float nearPlaneHeight);
	~KernelMng();

	void step(KernelMngState stateToStop, Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane);
	GLuint getTextureColorId();
	void renderKernelOutput(KernelMngState stateToRender, int outputNum);

private:
	KernelMngState oracle();
	void update(KernelMngState stateToStop);
	void render(Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane);
	int countActiveRays();

	KernelGenerateRay* m_kernelGenerateRay;
	KernelTraverse* m_kernelTraverse;
	KernelGeometry* m_kernelIntersect;
	KernelShade* m_kernelShade;
	KernelMngState m_currentState;
	UniformGrid* m_uniformGrid;
	GLuint m_textureRayPosition;
	GLuint m_textureRayDirection;
	FrameBufferObject m_fbo;

};


#endif