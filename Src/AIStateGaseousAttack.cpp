/**
* @file AIStateGaseousAttack.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CAIStateGaseousAttack class
*/

#include "AIStateGaseousAttack.h"
#include "Gaseous.h"

/**
* CAIStateGaseousAttack::CAIStateGaseousAttack
* @date Modified April 7, 2006
*/
CAIStateGaseousAttack::CAIStateGaseousAttack(void)
{
	setStateType(CAIState::AIS_GASEOUSATTACK);
}

/**
* CAIStateGaseousAttack::~CAIStateGaseousAttack
* @date Modified April 7, 2006
*/
CAIStateGaseousAttack::~CAIStateGaseousAttack(void) {}

/**
* CAIStateGaseousAttack::update
* @date Modified April 7, 2006
*/
void CAIStateGaseousAttack::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	// do timing garbage here if there is an animation to explode this guy
	((CGaseous*)poCharacter)->doDamage();
}

/**
* CAIStateGaseousAttack::enter
* @date Modified April 13, 2006
*/
void CAIStateGaseousAttack::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}

/**
* CAIStateGaseousAttack::exit
* @date Modified April 13, 2006
*/
void CAIStateGaseousAttack::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}