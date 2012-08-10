/**
* @file AnimatedMesh.h
* @author Matt Rudder
* @date Created May 15, 2006
*
* Encapsulates the loading, updating and rendering of skinned meshes.
*/

#ifndef _ANIMATEDMESH_H_
#define _ANIMATEDMESH_H_

// Local includes
#include "Mesh.h"

// System includes
#include <d3dx9anim.h>

struct SMeshContainer;
struct SMeshFrame;

/**
* Encapsulates the loading, updating and rendering of skinned meshes.
*
* @author Matt Rudder
* @date Created May 15, 2006
* @date Modified May 15, 2006
*/
class CAnimatedMesh : public CMesh
{
protected:
	//! Beginning of the animation hierarchy
	LPD3DXFRAME                 m_pFrameRoot;
	//! Animation controller
	LPD3DXANIMATIONCONTROLLER   m_pAnimController;    
	//! Array of bone transforms.
	D3DXMATRIX*					m_pBoneMatrices;
	//! Size of m_pBoneMatrices array.
	UINT                        m_unNumBoneMax;
	//! First mesh container in the hierarchy.
	SMeshContainer*				m_pFirstMesh;

	//! Animation Vars
	float						m_fAnimSpeed;
	float						m_fCurrentTime;
	float						m_fLastEngineTime;
	DWORD						m_dwCurrentTrack;
	DWORD						m_dwNumAnimSets;
	DWORD						m_dwCurrentAnimSet;

	//! hold on to the last set animation name
	CString						m_oAnimationName;

public:
	CAnimatedMesh(void);
	virtual ~CAnimatedMesh(void);

	/**
	* Gets the last set animation name
	*
	* @date Created June 7, 2006
	*/
	CString getLastAnimationName(void)
	{
		return m_oAnimationName;
	}

	/**
	* Allows a resource to output information about itself for debugging purposes
	*
	* @date Created Apr 01, 2006
	* @return Description string
	*/
	virtual CString toString(void) const;

	/**
	* Called by the Resource Manager when a previously loaded resource is 
	* requested again.
	*
	* @date Created Mar 31, 2006
	* @return New instance of the object
	*/
	virtual CBaseResource* cloneResource(void);

	/**
	* Creates a CMesh instance and loads the associated mesh file.
	*
	* @date Created Mar 31, 2006
	* @param[in]	sFilename	Absolute file path to the mesh.
	* @return Pointer to the new CMesh instance.
	*/
	static CBaseResource* createMeshFromFile(const CString sFilename);

	/**
	* Traverses the hierarchy and sets the combined matrices.
	*
	* @date Created May 16, 2006
	* @param[in]	pDev	D3D Device.
	* @param[in]	pFrame	The current frame.
	* @param[in]	pParent	The parent frame matrix.
	*/
	void setupBones(LPDIRECT3DDEVICE9 pDev, SMeshFrame* pFrame, LPD3DXMATRIX pParent);

	//! Accessors
	inline DWORD getCurrentAnimationIndex(void) const { return m_dwCurrentAnimSet; }
	inline DWORD getNumAnimationSets(void) const { return m_dwNumAnimSets; }
	inline UINT getMaxBones(void) const { return m_unNumBoneMax; }
	virtual bool isSkinned(void)		{ return true;		}
	inline float getAnimationSpeed()	{ return m_fAnimSpeed; }
	inline void setAnimationSpeed(float fSpeed)	{ m_fAnimSpeed = fSpeed; }

	/**
	* Updates the frame hierarchy from the top
	*
	* @date Created May 16, 2006
	* @param[in]	mWorld		Current world matrix.
	* @param[in]	pFrame		The frame to update from; NULL for all
	*/
	void updateFrame(const D3DXMATRIX* mWorld, SMeshFrame* pFrame = NULL);

	/**
	* Set the currently active animation.
	*
	* @date Created May 16, 2006
	* @param[in]	unIndex		Index of the animation to switch to.
	*/
	void setAnimationSet(UINT unIndex);

	/**
	* Retrieves an animation set ID by name.
	*
	* @date Created May 17, 2006
	* @param[in]	szName	The animation's name.
	* @return The animation ID.
	*/
	UINT getAnimationSetByName(const char* szName);

	/**
	* Retrieves the length of an animation set in seconds.
	*
	* @date Created May 21, 2006
	* @param[in]	bScaled		Determines if the return value is scaled by the current animation speed. Defaulted to true.
	* @param[in]	szName		Allows the user to specify an animation to retrieve the length of. Returns the current animation by default.
	* @return The length of the animation.
	*/
	double getAnimationLength(bool bScaled = true, const char* szName = NULL);

	/**
	* Retrieves an animation set ID by name, and sets it as the active animation.
	*
	* @date Created May 17, 2006
	* @param[in]	szName	The animation's name.
	*/
	void setAnimationSetByName(const char* szName);

	/**
	* Renders the animated mesh.
	*
	* @date Created May 16, 2006
	* @param[in]	bTextured	Draw with texture
	* @param[in]	dwColor		additive color
	*/
	virtual void draw(bool bTextured = true, D3DCOLOR* pColor = NULL);

	/**
	* Retrieves a bone matrix by the name of the bone.
	*
	* @date Created May 17, 2006
	* @param[in]	szName			The name of the bone to retrieve.
	* @param[out]	mBoneTransform	The matrix to fill out.
	* @return true if the bone was found; false otherwise.
	*/
	bool getBoneByName(const char* szName, D3DXMATRIX& mBoneTransform);

protected:

	/**
	* Updates the bone hierarchy just previous to rendering.
	*
	* @date Created May 16, 2006
	* @param[in]	pFrame		The current mesh frame.
	* @param[in]	pParent		The parent hierarchy transform.
	*/
	void updateBones(SMeshFrame* pFrame, const D3DXMATRIX* pParent);

	/**
	* Draws a frame in the animation hierarchy, including all child frames and 
	* mesh containers.
	*
	* @date Created May 16, 2006
	* @param[in]	pDev	Shortcut to the current D3D device.
	* @param[in]	pFrame	The frame to render.
	*/
	void drawFrame(LPDIRECT3DDEVICE9 pDev, bool bTextured, SMeshFrame* pFrame) const;

	/**
	* Draws a mesh container in the animation hierarchy.
	*
	* @date Created May 16, 2006
	* @param[in]	pDev	Shortcut to the current D3D device.
	* @param[in]	pMesh	The mesh to render.
	* @param[in]	pFrame	The frame containing pMesh.
	*/
	void drawMesh(LPDIRECT3DDEVICE9 pDev, bool bTextured, SMeshContainer* pMesh, SMeshFrame* pFrame) const;
};

#endif // _ANIMATEDMESH_H_
