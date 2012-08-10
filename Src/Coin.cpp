/**
* @file Coin.cpp
* @author Tom Williams
* @date Modified April 24, 2006
*
* Money objects within the world
*/
#include ".\coin.h"
#include "player.h"
#include "objectmanager.h"
#include "limits.h"

#define MAX_PLAYER_GRAV_DIST 2500.0f

CCoin::CCoin(EObjectType eType)
{
	m_eType = eType;
	m_unValue = COPPER_VALUE;
	m_oTimeSpan.setInterval(10.0f);
	m_fGravDist = FLT_MAX;
	m_fAngle = 0.0f;
	m_poPlayer = NULL;
}

/**
* CCoin::collided()
* @date Modified April 23, 2006
*/
void CCoin::collided(CActor *poActor)
{
	if(!poActor)
		return;

	// only update if there's collision with a player
	if(poActor->getType() == OBJ_PLAYER && !isDeleted())
		CObjectManager::getInstance().removeObject((CObject*)this);
}

/**
* CCoin::collided()
* @date Modified April 23, 2006
*/
void CCoin::setValue(ECoinTypes eValue)
{
	m_unValue = eValue;

	// set the mesh based on the value of the coin
	switch(eValue)
	{
	case COPPER_VALUE:
		setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "bronzecoin.x"));
		setScale(D3DXVECTOR3(0.15f,0.15f,0.15f));
		break;
	case SILVER_VALUE:
		setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "silvercoin.x"));
		setScale(D3DXVECTOR3(0.15f,0.15f,0.15f));
		break;
	case GOLD_VALUE:
		setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "goldcoin.x"));
		setScale(D3DXVECTOR3(0.15f,0.15f,0.15f));
		break;
	}
}

/**
* CCoin::update()
* @date Modified May 3, 2006
*/
void CCoin::update()
{
	// gold money doesn't gravitate
	if (m_unValue != GOLD_VALUE)
	{
		if (!gravitate())
			return;
	}

	// spin the coin
	rotateActorY(CTimer::getInstance().getFrameTime()*0.75f);

	if(!m_oTimeSpan.tick())
	{
		CActor::update();
		return;
	}

	// kill the coin if it's lifespan elapses
	CObjectManager::getInstance().removeObject((CObject*)this);
}

/**
* CCoin::init
* @date Modified May 15, 2006
*/
void CCoin::init(void)
{
	CActor::init();

	m_oTimeSpan.setInterval(10.0f);
	m_fGravDist = FLT_MAX;
	m_fAngle = 0.0f;
	m_poPlayer = NULL;
}

/**
* CCoin::gravitate
* @date Modified May 15, 2006
*/
bool CCoin::gravitate(void)
{
	// if it isn't active it needs to try and gravitate to the player
	if (!isActive())
	{
		if (m_fGravDist < FLT_MAX)
		{	
			if (m_poPlayer == NULL)
			{
				// kill the coin if it's trying to go to a player that doesn't exist
				CObjectManager::getInstance().removeObject((CObject*)this);
				return false;
			}

			// store the elapsed time
			float fElapsedTime;
			// update at 60fps
			if(!m_oTimer.tick(&fElapsedTime))
				return false;

			D3DXVECTOR3 vPlayerPos = ((CPlayer*)(m_poPlayer))->getPickupSphere().centerPt;

			setPosition(D3DXVECTOR3(

				vPlayerPos.x + (m_fGravDist * cos(m_fAngle)),
				vPlayerPos.y,
				vPlayerPos.z + (m_fGravDist * sin(m_fAngle))));


			(m_fAngle < 0.0f) ? m_fAngle -= 5.0f * fElapsedTime : m_fAngle += 5.0f * fElapsedTime;

			setBV(getPosition(), getBV().fRadius);
			m_fGravDist -= 50.0f * fElapsedTime;
			return false;
		}
		else
		{
			// all this see if the closest player is close enough for his gravity to effect this coin
			float fDist = FLT_MAX, fShortest = FLT_MAX;
			CObjectManager::ObjectList loPlayers;
			CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
			CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();
			while (oPlayerIter != loPlayers.end())
			{
				fDist = computeDistanceSquared(((CActor*)(*oPlayerIter))->getPosition(), getPosition());

				if (fDist > MAX_PLAYER_GRAV_DIST)
				{
					oPlayerIter++;
					continue;
				}

				if (fDist < fShortest)
				{
					fShortest = fDist;
					m_poPlayer = ((CActor*)(*oPlayerIter));
				}
				oPlayerIter++;
			}

			if (m_poPlayer == NULL)
				return false;

			// set the values for circling
			m_fGravDist = fastSqrtf(fShortest);
			// get the initial angle
			D3DXVECTOR3 vPlayerToCoin;
			D3DXVec3Subtract(&vPlayerToCoin, &getPosition(), &m_poPlayer->getPosition());
			D3DXVec3Normalize(NULL, &vPlayerToCoin, &vPlayerToCoin);

			// calculate the initial angle
			m_fAngle = D3DXVec3Dot(&vPlayerToCoin, &D3DXVECTOR3(1.0f,0.0f,0.0f));
			m_fAngle = acos(m_fAngle);

			// positive or negative
			if (D3DXVec3Dot(&vPlayerToCoin, &D3DXVECTOR3(0.0f,0.0f,1.0f)) < 0.0f)
				m_fAngle *= -1.0f;
		}
		return false;
	}

	return true;
}