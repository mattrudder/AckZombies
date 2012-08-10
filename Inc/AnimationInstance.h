#ifndef _ANIMATION_INSTANCE_H_
#define _ANIMATION_INSTANCE_H_

//! Includes
#include "Animation.h"
#include "Globals.h"
#include "AnimatedMesh.h"

class CAnimationInstance
{
	friend class CAnimationManager;
	friend class CAnimatedMesh;
	friend class CRenderSystem;
public:
	CAnimationInstance( CAnimatedMesh * pAnimatedMesh);
	virtual ~CAnimationInstance();
	
	LPD3DXANIMATIONCONTROLLER GetAnimationController(void);
	HRESULT InitializeInstance( LPD3DXANIMATIONCONTROLLER pAnimationController, double dCurrentTime);
	HRESULT ResetTime (void);
	void UpdateAnimation( /*D3DXMATRIXA16 * pWorldMatrix,*/ double dElapsedTime);
	void SetWorldMatrix( const D3DXMATRIX * pWorldMatrix);
	void SetShader(CShaderVariant& oParam);
	D3DXMATRIX GetWorldMatrix(void);
	CAnimatedMesh * GetAnimatedMeshPointer(void);
	void CleanUp(void);
	void Draw(void);

	//! Animation Set Indexes
	DWORD m_dwAnimIndexIdle;
	DWORD m_dwAnimIndexWalk;
	DWORD m_dwAnimIndexJog;
	//! Current Animation Track
	DWORD m_dwCurrentTrack;

	//! Animation Swapping Functions
	void SetAnimationIdle(void);
	void SetAnimationWalk(void);
	void SetAnimationRun(void);

	//! Timer Information
	double m_dTimePrev;							//! Time Value before Recent Update.
	double m_dTimeCurrent;						//! Current Global Time Value.

	//! Get Animation Indecies
	DWORD GetAnimationIndex(const char * szAnimationName);

private:
	CAnimatedMesh * m_pAnimatedMesh;
	LPD3DXANIMATIONCONTROLLER m_pAnimationController;
	D3DXFRAME_ANIM * m_pFrameRoot;
	D3DXMATRIX m_WorldMatrix;

	void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pTransformMatrix);
	void DrawFrames( D3DXFRAME_ANIM * pFrame);
	void DrawMeshFrame( D3DXFRAME_ANIM * pFrame);
	
};

#endif //_ANIMATION_INSTANCE_H_
