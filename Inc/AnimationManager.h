#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

#ifndef NULL
	#define NULL 0
#endif

#include "AnimationInstance.h"
#include "AnimatedMesh.h"

#include <vector>
using std::vector;

class CAnimationManager
{
public:
	CAnimationManager();
	~CAnimationManager();
		
	static CAnimationManager * GetInstance(void);
	static CAnimationManager * m_poInstance;

	HRESULT UpdateAnimationInstances( /*D3DXMATRIXA16 * pWorldMatrix,*/ double dElapsedTime, D3DXMATRIX * pViewMatrix);
	CAnimatedMesh * GetAnimatedMesh( unsigned int uiAnimatedMeshIndex);
	CAnimatedMesh * CreateAnimatedMesh(void);
	unsigned int  GetAnimatedMeshIndex( CAnimatedMesh * pAnimatedMesh);

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX GetViewMatrix(void);

private:
	vector<CAnimatedMesh *>		m_vpAnimatedMeshes;
	

};

#endif //_ANIMATION_MANAGER_H_
