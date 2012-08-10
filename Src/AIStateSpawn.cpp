/**
* @file AIStateSpawn.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created May 30, 2006
*
* This file contains the implementation of the CAIStateSpawn class.
*/

// Local Includes
#include "AIStateSpawn.h"
#include "Enemy.h"
#include "AIGroup.h"
#include "ParticleEmitter.h"

/**
* CAIStateSpawn::CAIStateSpawn
* @date Modified May 30, 2006
*/
CAIStateSpawn::CAIStateSpawn(void)
{
	setStateType(CAIState::AIS_SPAWN);
}

/**
* CAIStateSpawn::~CAIStateSpawn
* @date Modified May 30, 2006
*/
CAIStateSpawn::~CAIStateSpawn(void)
{
}

/**
* CAIStateSpawn::update
* @date Modified May 30, 2006
*/
void CAIStateSpawn::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	D3DXVECTOR3 vPos = poCharacter->getPosition();
	// if they are above ground activate them and then they wait for the rest of there group to finish spawning
	if (vPos.y > 0.0f)
	{
		// reset vertical position
		vPos.y = 0.0f;
		poCharacter->setPosition(vPos);

		((CEnemy*)poCharacter)->setAIState(NULL);

		// make them active for physics again
		poCharacter->activate();
		poCharacter->setVelocity(D3DXVECTOR3(0.0f,0.0f,0.0f));
		// go to my group and tell them to update
		poAIEntity->getGroup()->updateLeader();
	}
}

/**
* CAIStateSpawn::enter
* @date Modified May 30, 2006
*/
void CAIStateSpawn::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	D3DXVECTOR3 vPos = poCharacter->getPosition();
	SSphere sBV = poCharacter->getBV();

	// start the particle effect
	((CEnemy*)poCharacter)->getDirtEmitter()->setActorMatrix(poCharacter->getOrientation(), poCharacter->getPosition(), D3DXVECTOR3(1,1,1));
	((CEnemy*)poCharacter)->getDirtEmitter()->setPosition(D3DXVECTOR3(vPos.x, 0.0f, vPos.z));
	((CEnemy*)poCharacter)->getDirtEmitter()->play();

	// set them to be below the ground
	switch (poCharacter->getType())
	{
	case OBJ_ENEMY_ZOMBIECITIZEN:
		vPos.y -= 30.0f;
		sBV.centerPt.y -= 30.0f;
		break;
	case OBJ_ENEMY_GASEOUSCLAY:
		vPos.y -= 30.0f;
		sBV.centerPt.y -= 30.0f;
		break;
	case OBJ_ENEMY_ICECREAMMAN:
		vPos.y -= 30.0f;
		sBV.centerPt.y -= 30.0f;
		break;
	case OBJ_ENEMY_QUARTERBACK:
		vPos.y -= 30.0f;
		sBV.centerPt.y -= 30.0f;
		break;
	}
	poCharacter->setPosition(vPos);
	poCharacter->setBV(sBV);
	poCharacter->deactivate();

	poCharacter->setVelocity(D3DXVECTOR3(0.0f, 15.0f, 0.0f));
}

/**
* CAIStateSpawn::exit
* @date Modified May 30, 2006
*/
void CAIStateSpawn::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	((CEnemy*)poCharacter)->getDirtEmitter()->pause();
}