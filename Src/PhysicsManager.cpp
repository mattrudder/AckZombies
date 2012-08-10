/**
* @file PhysicsManger.cpp
* @author Tom Williams
* @date Created Mar 29, 2006
*
* Update and check collision between actors and world
*/

#include ".\physicsmanager.h"
#include "Math3D.h"
#include "Coin.h"

#define EPSILON 0.15f

/**
* CPhyscisManager::CPhysicsManager()
* @date Modified April 4, 2006
*/
CPhysicsManager::CPhysicsManager(void) : m_oCheckCol(checkCollision, this), m_oCheckFrustum(checkFrustumCol, this)
{
	// get the instance to the object manager
	m_poObjManager = CObjectManager::getInstancePtr();

	// set up the collison geometry
	m_poColGeom = CCollisionGeometry::getInstancePtr();

	// set the value of initial value of friction/gravity
	m_fFriction = INIT_FRICTION;
	m_fGravity = INIT_GRAVITY;
}

/**
* CPhyscisManager::~CPhysicsManager()
* @date Modified March 29, 2006
*/
CPhysicsManager::~CPhysicsManager(void)
{
	m_poColGeom->deleteTree(m_poColGeom->getRoot());
	CCollisionGeometry::deleteInstance();
}

/**
* CPhyscisManager::update()
* @date Modified April 24, 2006
*/
void CPhysicsManager::update(CActor* poActor, float fDT)
{
	// store the current actor's type
	EObjectType eType = poActor->getType();

	// use in collision offsets
	D3DXVECTOR3 offset, curOff, vec;

	// used to check for collisions
	std::vector<D3DXVECTOR3> points;
	std::vector<STriangle>	tris;

	// if the object is affected by gravity/friction, update the velocity then
	if((eType & OBJ_WORLD) == OBJ_WORLD || eType == OBJ_WEAPON_GRENADE || eType == OBJ_WEAPON_BIGCLUCKINGBOMB)
	{
		vec = poActor->getVelocity();

		// stop the object from falling through the ground
		if(vec.y != 0)
			vec.y -= m_fGravity*fDT;

		// prevent jiggling
		if(vec.x <= EPSILON && vec.x >= -EPSILON)
			vec.x = 0;
		if(vec.z <= EPSILON && vec.z >= -EPSILON)
			vec.z = 0;

		// set the velocity of the object
		poActor->setVelocity(vec);
	}

	// check for level collision
	if(m_poColGeom->checkSphereCollision(points, tris, m_poColGeom->getRoot(), poActor->getBV()))
	{
		// if there was level collision, make sure the character did not go through the level
		// get the vector offset
		offset = D3DXVECTOR3(0,0,0);
		for(size_t i = 0; i < points.size(); ++i)
		{
			vec = points[i] - poActor->getBV().centerPt;
			D3DXVec3Normalize(NULL, &vec, &vec);
			vec *= poActor->getBV().fRadius;
			vec += poActor->getBV().centerPt;

			// if it's the player, just move them back, but allow enemies to slide a bit
			if(eType != OBJ_ENEMY)
				curOff = points[i] - vec;
			else
				curOff = points[i] - D3DXVECTOR3(vec.x*1.001f, vec.y, vec.z*1.001f);

			// get the largest movement
			if(abs(curOff.x) > abs(offset.x))
				offset.x = curOff.x;
			if(abs(curOff.z) > abs(offset.z))
				offset.z = curOff.z;
		}			

		// reset the position of the character based off the offset
		poActor->setPosition(offset+poActor->getPosition());
		poActor->setBV(offset+poActor->getBV().centerPt, poActor->getBV().fRadius);
		poActor->collided(NULL);

		// if the object was one that requires vector reflection, then reflect the velocity
		if((eType & OBJ_WORLD) == OBJ_WORLD || eType == OBJ_WEAPON_GRENADE || eType == OBJ_WEAPON_BIGCLUCKINGBOMB)
		{
			// change the direction of this object's velocity
			vec = poActor->getVelocity();
			vec.y += INIT_IMPACT*fDT;

			// apply friction to the grenade
			vec.x -= vec.x*m_fFriction*fDT;
			vec.z -= vec.z*m_fFriction*fDT;

			// check the amount to offset by
			offset = D3DXVECTOR3(0,0,0);

			for(size_t j = 0; j < tris.size(); ++j)
			{
				// this is vector reflection for a flat ground
				if(tris[j].normal.y >= 0.98f || tris[j].normal.y <= -0.98f)
					offset += D3DXVECTOR3(vec.x, -vec.y, vec.z);
				// this is normal vector relection
				else
				{
					D3DXVec3Reflect(&curOff, &vec, &tris[j].normal);
					offset += curOff;
				}
			}

			vec = offset * (1.0f/(float)tris.size());

			// make sure the y does not go through
			if(vec.y < EPSILON)
				vec.y = 0;

			poActor->setVelocity(vec);
		}
	}
	// if the object is only moving along the x or z, but y is at zero, put gravity in place
	else if(poActor->getVelocity().y == 0.0f && ((eType & OBJ_WORLD) == OBJ_WORLD || eType == OBJ_WEAPON_GRENADE || eType == OBJ_WEAPON_BIGCLUCKINGBOMB))
	{
		vec.y = -m_fGravity*fDT;
		poActor->setVelocity(vec);
	}

	// set the current actor to this one
	m_poCurrentActor = poActor;

	// let the object manager iterate the actors and call the collision check
	m_poObjManager->iterateObjects(OBJ_ACTOR, &m_oCheckCol);
	if(eType == OBJ_PLAYER) m_poObjManager->iterateObjects(OBJ_PLAYER, &m_oCheckFrustum);
}

/**
* CPhysicsManager::checkCollision()
* @date Modified May 26, 2006
*/
void CPhysicsManager::checkCollision(CObject* poObj)
{
	// if the objects are one and the same, don't bother checking
	// if the other object is inactive, don't bother checking
	if(poObj == m_poCurrentActor || !poObj->isActive())
		return;

	// set in collision offsets
	D3DXVECTOR3 offset, vec;

	// hold onto the pointer to the actor and its type
	CActor *poActor = (CActor*)poObj;
	EObjectType eType = m_poCurrentActor->getType();
	EObjectType eOther = poActor->getType();

	// push the objects off each other, and do vector reflection on the object if necessary
	// find the vector b/w the points, the length of the radius
	// if the other actor is a vending machine, deagle bullet, or qb, push the current actor.
	// if the current actor is a bullet, make sure it doesn't push characters.
	if(eOther == OBJ_WEAPONPROJ_50AWE || eOther == OBJ_VENDINGMACHINE || eOther == OBJ_ENEMY_QUARTERBACK || 
		eOther == OBJ_OBJECTIVE_BARRIER ||
		((eType & OBJ_WEAPONPROJ) == OBJ_WEAPONPROJ && (eOther & OBJ_CHARACTER) == OBJ_CHARACTER && eType != OBJ_WEAPONPROJ_50AWE))
	{
		// check for collision b/w this object and another
		if(checkCol_SphereSphere(&offset, m_poCurrentActor->getBV(), poActor->getBV()))
		{
			// if there was collision, call the collided functions on the objects
			m_poCurrentActor->collided(poActor);
			poActor->collided(m_poCurrentActor);
			offset.y = 0.0f;

			m_poCurrentActor->setPosition(offset+m_poCurrentActor->getPosition());
			m_poCurrentActor->setBV(offset+m_poCurrentActor->getBV().centerPt, m_poCurrentActor->getBV().fRadius);

			// if the object was one that requires vector reflection, then reflect the velocity
			if((eType & OBJ_WORLD) == OBJ_WORLD || eType == OBJ_WEAPON_GRENADE)
			{
				vec = m_poCurrentActor->getBV().centerPt - poActor->getBV().centerPt;
				D3DXVec3Normalize(NULL, &vec, &vec);
				// change the direction of this object's velocity
				D3DXVec3Reflect(&vec, &(m_poCurrentActor->getVelocity()), &vec);
				m_poCurrentActor->setVelocity(vec);
			}
		}
	}
	else
	{
		// check for collision b/w this object and another
		if(checkCol_SphereSphere(&offset, poActor->getBV(), m_poCurrentActor->getBV()))
		{
			// if there was collision, call the collided functions on the objects
			m_poCurrentActor->collided(poActor);
			poActor->collided(m_poCurrentActor);
			offset.y = 0.0f;

			poActor->setPosition(offset+poActor->getPosition());
			poActor->setBV(offset+poActor->getBV().centerPt, poActor->getBV().fRadius);

			// if the object was one that requires vector reflection, then reflect the velocity
			if((eOther & OBJ_WORLD) == OBJ_WORLD || eOther == OBJ_WEAPON_GRENADE)
			{
				vec = poActor->getBV().centerPt - m_poCurrentActor->getBV().centerPt;
				D3DXVec3Normalize(NULL, &vec, &vec);
				// change the direction of this object's velocity
				D3DXVec3Reflect(&vec, &(poActor->getVelocity()), &vec);
				poActor->setVelocity(vec);
			}
			// if the object was one that requires vector reflection, then reflect the velocity
			if((eType & OBJ_WORLD) == OBJ_WORLD || eType == OBJ_WEAPON_GRENADE)
			{
				vec = m_poCurrentActor->getBV().centerPt - poActor->getBV().centerPt;
				D3DXVec3Normalize(NULL, &vec, &vec);
				// change the direction of this object's velocity
				D3DXVec3Reflect(&vec, &(m_poCurrentActor->getVelocity()), &vec);
				m_poCurrentActor->setVelocity(vec);
			}
		}
	}

}

/*
* CPhysicsManager::checkFrustumCol()
* @date Modified May 26, 2006
*/
void CPhysicsManager::checkFrustumCol(CObject *poObj)
{
	// if the current actor is this player, don't bother checking with it
	if(m_poCurrentActor == poObj)
		return;

	CPlayer *pPlayer = (CPlayer*)poObj;
	D3DXPLANE plane;
	D3DXVECTOR3 PtOfCol, vec, offset;
	int nValue = 0;

	// test outer sphere collision
	SSphere testSphere = pPlayer->getBV();
	testSphere.fRadius *= 2.0f;

	// get the view frustum
	CFrustum oViewFrustum = CRenderSystem::getInstance().getViewFrustum();

	if(checkCol_PlaneSphere(&PtOfCol, testSphere, oViewFrustum.getClipPlane(CFrustum::CLIP_TOP)) <= 0 ||
		checkCol_PlaneSphere(&PtOfCol, testSphere, oViewFrustum.getClipPlane(CFrustum::CLIP_BOTTOM)) <= 0 ||
		checkCol_PlaneSphere(&PtOfCol, testSphere, oViewFrustum.getClipPlane(CFrustum::CLIP_NEAR)) <= 0 ||
		checkCol_PlaneSphere(&PtOfCol, testSphere, oViewFrustum.getClipPlane(CFrustum::CLIP_FAR)) <= 0)
	{
		// set that the outer sphere is colliding
		nValue += 1;
		PtOfCol = pPlayer->getBV().centerPt;

		// don't let the player leave the view frustum
		while(checkCol_PlaneSphere(&PtOfCol, pPlayer->getBV(), plane = oViewFrustum.getClipPlane(CFrustum::CLIP_TOP)) <= 0 ||
			checkCol_PlaneSphere(&PtOfCol, pPlayer->getBV(), plane = oViewFrustum.getClipPlane(CFrustum::CLIP_BOTTOM)) <= 0 ||
			checkCol_PlaneSphere(&PtOfCol, pPlayer->getBV(), plane = oViewFrustum.getClipPlane(CFrustum::CLIP_NEAR)) <= 0 ||
			checkCol_PlaneSphere(&PtOfCol, pPlayer->getBV(), plane = oViewFrustum.getClipPlane(CFrustum::CLIP_FAR)) <= 0)
		{
			// if the player ran into the view frustum, offset the player from moving
			offset = pPlayer->getBV().centerPt - PtOfCol;

			if(D3DXVec3Dot(&offset, &D3DXVECTOR3(plane.a,plane.b,plane.c)) < 0)
				offset = -offset;

			float fLength;
			D3DXVec3Normalize(&fLength, &offset, &offset);
			fLength += pPlayer->getBV().fRadius;

			offset *= fLength;
			offset.y = 0.0f;

			// set it yo
			pPlayer->setPosition(pPlayer->getPosition()+offset);
			pPlayer->setBV(pPlayer->getBV().centerPt+offset, pPlayer->getBV().fRadius);

			// update the camera
			PtOfCol = pPlayer->getBV().centerPt;
			CCamera::getInstance().updateCameraMP(pPlayer->getPosition(), m_poCurrentActor->getPosition());
		}
	}

	pPlayer = (CPlayer*)m_poCurrentActor;

	// test outer sphere collision
	testSphere = pPlayer->getBV();
	testSphere.fRadius *= 2.0f;

	if(checkCol_PlaneSphere(&PtOfCol, testSphere, oViewFrustum.getClipPlane(CFrustum::CLIP_TOP)) <= 0 ||
		checkCol_PlaneSphere(&PtOfCol, testSphere, oViewFrustum.getClipPlane(CFrustum::CLIP_BOTTOM)) <= 0 ||
		checkCol_PlaneSphere(&PtOfCol, testSphere, oViewFrustum.getClipPlane(CFrustum::CLIP_NEAR)) <= 0 ||
		checkCol_PlaneSphere(&PtOfCol, testSphere, oViewFrustum.getClipPlane(CFrustum::CLIP_FAR)) <= 0)
	{
		// set that the outer sphere is colliding
		nValue += 1;
		PtOfCol = pPlayer->getBV().centerPt;

		// don't let the player leave the view frustum
		while(checkCol_PlaneSphere(&PtOfCol, pPlayer->getBV(), plane = oViewFrustum.getClipPlane(CFrustum::CLIP_TOP)) <= 0 ||
			checkCol_PlaneSphere(&PtOfCol, pPlayer->getBV(), plane = oViewFrustum.getClipPlane(CFrustum::CLIP_BOTTOM)) <= 0 ||
			checkCol_PlaneSphere(&PtOfCol, pPlayer->getBV(), plane = oViewFrustum.getClipPlane(CFrustum::CLIP_NEAR)) <= 0 ||
			checkCol_PlaneSphere(&PtOfCol, pPlayer->getBV(), plane = oViewFrustum.getClipPlane(CFrustum::CLIP_FAR)) <= 0)
		{
			// if the player ran into the view frustum, offset the player from moving
			offset = pPlayer->getBV().centerPt - PtOfCol;

			if(D3DXVec3Dot(&offset, &D3DXVECTOR3(plane.a,plane.b,plane.c)) < 0)
				offset = -offset;

			float fLength;
			D3DXVec3Normalize(&fLength, &offset, &offset);
			fLength += pPlayer->getBV().fRadius;

			offset *= fLength;
			offset.y = 0.0f;

			// set it yo
			pPlayer->setPosition(pPlayer->getPosition()+offset);
			pPlayer->setBV(pPlayer->getBV().centerPt+offset, pPlayer->getBV().fRadius);

			// update the camera
			PtOfCol = pPlayer->getBV().centerPt;
			CCamera::getInstance().updateCameraMP(((CActor*)poObj)->getPosition(), m_poCurrentActor->getPosition());
		}
	}

	// if both outer spheres are colliding, don't update the multiplayer camera
	CCamera::getInstance().setToUpdate(nValue < 1);
}

/*
* CPhysicsManager::initCollisionGeometry
* @date Modified May 8, 2006
*/
void CPhysicsManager::initCollisionGeometry(const char *filename)
{
	m_poColGeom->initTree(filename, 5);
}