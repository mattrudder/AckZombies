/**
* @file HUD.cpp
* @author Jordan Leckner
* @date Created April 12, 2006
*
* This file includes the definition of the HUD Menu class
* used for displaying a HUD menu and its buttons.
*/
#include "HUD.h"
#include "ObjectManager.h"
#include "Player.h"
#include "AIManager.h"

/**
* CHUDMenu::CHUDMenu
* @date Modified June 2, 2006
*/
CHUDMenu::CHUDMenu() : CMenu(0, 0, CMenuManager::getInstance().getWindowWidth(), CMenuManager::getInstance().getWindowHeight(),
				   "../Textures/Menu/HUD.png", 1024, 768), m_poObjectiveFont(0)
{
	m_nType = MT_HUD;

	int nWindowWidth = CMenuManager::getInstance().getWindowWidth();
	int nWindowHeight = CMenuManager::getInstance().getWindowHeight();

	m_poHUDBackground = new CMenuButton(0, 0, nWindowWidth, nWindowHeight, "../Textures/Menu/HUD.png", NULL, 1024, 768, false);

	// Health
	m_poPlayer1Health = new CTextButton(90, nWindowHeight - 52,
		128, 64, NULL, NULL, 0, 0, false, this, "Arial Black", 12, 18);
	m_poPlayer1Health->setFormat(DT_LEFT);
	// Weapon Ammo
	m_poPlayer1Ammo = new CTextButton(168, nWindowHeight - 52,
		192, 64, NULL, NULL, 0, 0, false, this, "Arial Black", 12, 18);
	m_poPlayer1Ammo->setFormat(DT_LEFT);
	// Grenade Ammo
	m_poPlayer1GrenadeAmmo = new CTextButton(112, nWindowHeight - 138,
		192, 64, NULL, NULL, 0, 0, false, this, "Arial Black", 12, 18);
	m_poPlayer1GrenadeAmmo->setFormat(DT_LEFT);

	// Health
	m_poPlayer2Health = new CTextButton(nWindowWidth - 130, nWindowHeight - 52,
		128, 64, NULL, NULL, 0, 0, false, this, "Arial Black", 12, 18);
	m_poPlayer2Health->setFormat(DT_LEFT);
	// Weapon Ammo
	m_poPlayer2Ammo = new CTextButton(nWindowWidth - 208, nWindowHeight - 52,
		192, 64, NULL, NULL, 0, 0, false, this, "Arial Black", 12, 18);
	m_poPlayer2Ammo->setFormat(DT_LEFT);
	// Grenade Ammo
	m_poPlayer2GrenadeAmmo = new CTextButton(nWindowWidth - 124, nWindowHeight - 138,
		192, 64, NULL, NULL, 0, 0, false, this, "Arial Black", 12, 18);
	m_poPlayer2GrenadeAmmo->setFormat(DT_LEFT);

	// Awesome meter
	m_poAwesomeMeter = new CMenuButton((nWindowWidth >> 1) - 163, nWindowHeight - 60, 0, 64, 
		"../Textures/Menu/AwesomeMeter.tga", NULL, 512, 64, false, this);

	m_nAwesomeMeterWidth = 0;
	m_nAwesomeMeterBuffer = 0;

	m_poAwesomeMeter->addToVertexDataUV(-1.0f, 0.0f, true);

	// Player
	m_poPlayerScore = new CTextButton(30, 24,
		192, 64, NULL, NULL, 0, 0, false, this, "Arial Black", 12, 18);
	m_poPlayerScore->setFormat(DT_LEFT);

	// load up the reticle
	m_poReticleP1 = new CMenuButton((nWindowWidth >> 1) - 64, nWindowHeight >> 1, 32, 32, "../Textures/target.tga", NULL, 32, 32, false);
	m_poReticleP2 = new CMenuButton((nWindowWidth >> 1) + 64, nWindowHeight >> 1, 32, 32, "../Textures/target2.tga", NULL, 32, 32, false);
	m_ReticleSize.x = 32.0f;
	m_ReticleSize.y = 32.0f;

	// get the objective font B
	m_poObjectiveFont = (CBitmapFont*)CResourceManager::getInstance().loadResource(RES_FONT, "HandelGothic.ini");

	m_pObjectiveArrow = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "arrow.x");

	m_poObjectiveTextBG = new CMenuButton(nWindowWidth - 512, 10, 512, 58, "../Textures/ObjectiveCover.tga",
		NULL, 512, 58, false);

	m_dwgrenadeColor = CGrenadeWeapon::getRandomColor();
}

/**
* CHUDMenu::drawMenu
* @date Modified May 15, 2006
*/
void CHUDMenu::drawMenu(D3DXVECTOR2 *pMouseCursor)
{
	// Direct3d Device
	LPDIRECT3DDEVICE9 poDirect3DDevice = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();

	// get the current cull mode and save it to restore later
	static DWORD dwCullMode;
	poDirect3DDevice->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
	poDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	poDirect3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DXCOLOR white(1,1,1,1);
	D3DMATERIAL9 matDefault, matOld;
	matDefault.Ambient = white;
	matDefault.Diffuse = white;
	matDefault.Specular = white;
	matDefault.Emissive = white;
	matDefault.Power = 5.0f;
	poDirect3DDevice->GetMaterial(&matOld);
	poDirect3DDevice->SetMaterial(&matDefault);

	// dont test against the z buffer when drawing. we don't want collision with anything.
	poDirect3DDevice->SetRenderState(D3DRS_ZENABLE, false);

	// our list of players
	static CObjectManager::ObjectList lPlayerList;
	CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayerList);

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	// Player's Score
	unsigned int unPlayerScore = 0;

	// should we draw the reticle?
	bool bDrawReticle = false;

	static char szBuffer[128] = {0};
	CObjectManager::ObjectList::iterator iter = lPlayerList.begin();
	CPlayer *poPlayer1 = 0, *poPlayer2 = 0;
	if (iter != lPlayerList.end())
	{
		poPlayer1 = (CPlayer*)*iter;
		sprintf(szBuffer, "%d", poPlayer1->getHealth());
		m_poPlayer1Health->setText(szBuffer);
		if ((poPlayer1->getCurrentWeapon()->getType() & OBJ_WEAPON_PISTOL) == OBJ_WEAPON_PISTOL)
			sprintf(szBuffer, "INF");
		else
			sprintf(szBuffer, "%d", poPlayer1->getCurrentWeapon()->getAmmo());
		m_poPlayer1Ammo->setText(szBuffer);
		unPlayerScore = poPlayer1->getAmtMoney();

		if (poPlayer1->getInputType() == PIT_KEYBOARDMOUSE)
		{
			bDrawReticle = true;
			// move the reticle based on mouse input
			D3DXVECTOR2 mousePos = CMenuManager::getInstance().checkMouseInput(m_ReticleSize.x, m_ReticleSize.y);
			// invert the y
			mousePos.y = -mousePos.y - m_ReticleSize.y;
			m_poReticleP1->setVertexData(&mousePos);
		}
		else if (poPlayer1->getInputType() == PIT_1ST_XBOX360PAD || poPlayer1->getInputType() == PIT_2ND_XBOX360PAD)
		{
			D3DXVECTOR2 cursorPos;

			D3DXMATRIXA16 matView, matProj, matFinal;
			poDirect3DDevice->GetTransform(D3DTS_VIEW, &matView);
			poDirect3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
			matFinal = matView * matProj;
			D3DXVECTOR3 out = poPlayer1->getOrientation();// * 250.0f;
			D3DXVec3TransformCoord(&out, &out, &matFinal);

			cursorPos.x = out.x * 250.0f;
			cursorPos.y = out.z * 250.0f;
			cursorPos.x -= m_ReticleSize.x * 0.5f;
			cursorPos.y -= m_ReticleSize.y * 0.5f;
			m_poReticleP1->setVertexData(&cursorPos);
		}
	}
	++iter;
	// if no second player
	if (iter == lPlayerList.end())
	{
		m_poPlayer2Ammo->setText("Press Start");
		m_poPlayer2Health->setText("");
		m_poPlayer2GrenadeAmmo->setText("");
	}
	else // Second player
	{
		// get player 2's money
		unPlayerScore += ((CPlayer*)*iter)->getAmtMoney();
		poPlayer1->setAmtMoney(unPlayerScore);
		poPlayer2 = (CPlayer*)*iter;
		// Player 2
		sprintf(szBuffer, "%d", poPlayer2->getHealth());
		m_poPlayer2Health->setText(szBuffer);
		if ((poPlayer2->getCurrentWeapon()->getType() & OBJ_WEAPON_PISTOL) == OBJ_WEAPON_PISTOL)
			sprintf(szBuffer, "INF");
		else
			sprintf(szBuffer, "%d", poPlayer2->getCurrentWeapon()->getAmmo());
		m_poPlayer2Ammo->setText(szBuffer);
		poPlayer2->setAmtMoney(0);

		if (poPlayer2->getInputType() == PIT_KEYBOARDMOUSE)
		{
			bDrawReticle = true;
			// move the reticle based on mouse input
			D3DXVECTOR2 mousePos = CMenuManager::getInstance().checkMouseInput(m_ReticleSize.x, m_ReticleSize.y);
			// invert the y
			mousePos.y = -mousePos.y - m_ReticleSize.y;
			m_poReticleP1->setVertexData(&mousePos);
		}
		else if (poPlayer2->getInputType() == PIT_1ST_XBOX360PAD || poPlayer2->getInputType() == PIT_2ND_XBOX360PAD ||
			poPlayer2->getInputType() == PIT_1ST_GAMEPAD || poPlayer2->getInputType() == PIT_2ND_GAMEPAD)
		{
			D3DXVECTOR2 cursorPos;
			D3DXVECTOR3 orien = poPlayer2->getOrientation();
			D3DXVECTOR3 transformed;
			D3DXMATRIXA16 invView;

			poDirect3DDevice->GetTransform(D3DTS_VIEW, &invView);
			D3DXMatrixInverse(&invView, NULL, &invView);
			D3DXVec3TransformCoord(&transformed, &orien, &invView);

			cursorPos.x = -transformed.x * 250.0f;
			cursorPos.y = -transformed.z * 250.0f;
			cursorPos.x -= m_ReticleSize.x * 0.5f;
			cursorPos.y -= m_ReticleSize.y * 0.5f;
			m_poReticleP2->setVertexData(&cursorPos);
		}
	}

	sprintf(szBuffer, "%d", unPlayerScore);
	m_poPlayerScore->setText(szBuffer);

	// Draw the reticle first.
	if (bDrawReticle)
		m_poReticleP1->drawButton(NULL);
	// BROKEN:
	//if (poPlayer2)
	//	m_poReticleP2->drawButton(NULL);

	//// Draw the HUD last.
	//poDirect3DDevice->SetTexture(0, m_pTextureBackground);
	//// Finally, draw the triangles of the quad.
	//poDirect3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SMenuFVF));
	//poDirect3DDevice->SetFVF(SMenuFVF::FVF_Flags);
	//poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	//poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 3, 1);
	//// Clear out the texture
	//poDirect3DDevice->SetTexture(0, 0);
	m_poHUDBackground->drawButton(NULL);

	// only draw the buttons if the child menu's texture is valid.
	// this way we can save memory on the background.
	if (!m_poChildMenu || (m_poChildMenu && m_poChildMenu->getTextureBackground()))
	{
		// Go through all of the buttons for this menu and draw them in front
		// of this menu.
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		{
			// Draw this button and check for input
			if (m_Buttons[i]->drawButton(pMouseCursor))
			{
				// Mouse is hovering over the button
			}
		}
	}

	// put back z-testing
	poDirect3DDevice->SetRenderState(D3DRS_ZENABLE, true);


	D3DXMATRIXA16 mat, rotX, rotY, rotZ, old, scale;
	D3DXMatrixIdentity(&mat);
	static float fRot = 0;
	fRot += 0.01f;//CTimer::getInstance().getLastTime() * 0.001f;
	D3DXMatrixRotationY(&rotY, fRot);
	D3DXMatrixRotationX(&rotX, fRot);
	D3DXMatrixRotationZ(&rotZ, fRot);
	D3DXMatrixMultiply(&mat, &rotY, &rotX);
	D3DXMatrixMultiply(&mat, &mat, &rotZ);
	D3DXMatrixScaling(&scale, 2.5f, 2.5f, 2.5f);
	D3DXMatrixMultiply(&mat, &mat, &scale);
	mat._41 = -260;
	mat._42 = -270;

	mat._31 = -mat._31;
	mat._32 = -mat._32;
	mat._33 = -mat._33;
	mat._11 = -mat._11;
	mat._12 = -mat._12;
	mat._13 = -mat._13;

	// reset cull mode for drawing the weapons
	poDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	poDirect3DDevice->GetTransform(D3DTS_WORLD, &old);
	poDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	if (poPlayer1)
	{
		poDirect3DDevice->SetTransform(D3DTS_WORLD, &mat);
		m_poWeaponMesh = poPlayer1->getCurrentWeapon()->getMesh();
		m_poGrenadeMesh = poPlayer1->getCurrentGrenade()->getMesh();
		poDirect3DDevice->GetMaterial(&matOld);
		poDirect3DDevice->SetMaterial(&matDefault);
		if (m_poWeaponMesh)
			m_poWeaponMesh->draw();

		// redo for the grenade
		D3DXMatrixIdentity(&mat);
		if ((poPlayer1->getCurrentGrenade()->getType() & OBJ_WEAPON_BIGCLUCKINGBOMB) == OBJ_WEAPON_BIGCLUCKINGBOMB)
		{
			D3DXMatrixScaling(&scale, 3.0f, 3.0f, 3.0f);
		}
		else
		{
			D3DXMatrixScaling(&scale, 90.5f, 90.5f, 90.5f);
		}

		D3DXMatrixRotationY(&rotY, fRot);
		D3DXMatrixMultiply(&mat, &mat, &rotY);
		D3DXMatrixMultiply(&mat, &mat, &scale);

		mat._41 = -380;
		if ((poPlayer1->getCurrentGrenade()->getType() & OBJ_WEAPON_BIGCLUCKINGBOMB) == OBJ_WEAPON_BIGCLUCKINGBOMB)
			mat._42 = -200;
		else
			mat._42 = -270;

		mat._31 = -mat._31;
		mat._32 = -mat._32;
		mat._33 = -mat._33;
		mat._11 = -mat._11;
		mat._12 = -mat._12;
		mat._13 = -mat._13;

		poDirect3DDevice->SetTransform(D3DTS_WORLD, &mat);
		if (m_poGrenadeMesh)
		{
			if ((poPlayer1->getCurrentGrenade()->getType() & OBJ_WEAPON_BIGCLUCKINGBOMB) == OBJ_WEAPON_BIGCLUCKINGBOMB)
				m_poGrenadeMesh->draw();
			else
				m_poGrenadeMesh->draw(false, &m_dwgrenadeColor);
		}
		poDirect3DDevice->SetMaterial(&matOld);

		// draw the ammo for the grenade
		if ((poPlayer1->getCurrentGrenade()->getType() & OBJ_WEAPON_BIGCLUCKINGBOMB) == OBJ_WEAPON_BIGCLUCKINGBOMB)
			sprintf(szBuffer, "%d", poPlayer1->getCurrentGrenade()->getAmmoLeft());
		else
			sprintf(szBuffer, "%d", poPlayer1->getCurrentGrenade()->getNumGrenades());
		m_poPlayer1GrenadeAmmo->setText(szBuffer);
		m_poPlayer1GrenadeAmmo->drawButton(NULL);
	}
	if (poPlayer2)
	{
		D3DXMatrixIdentity(&mat);
		D3DXMatrixRotationY(&rotY, fRot);
		D3DXMatrixRotationX(&rotX, fRot);
		D3DXMatrixRotationZ(&rotZ, fRot);
		D3DXMatrixMultiply(&mat, &rotY, &rotX);
		D3DXMatrixMultiply(&mat, &mat, &rotZ);
		D3DXMatrixScaling(&scale, 2.5f, 2.5f, 2.5f);
		D3DXMatrixMultiply(&mat, &mat, &scale);

		mat._31 = -mat._31;
		mat._32 = -mat._32;
		mat._33 = -mat._33;
		mat._11 = -mat._11;
		mat._12 = -mat._12;
		mat._13 = -mat._13;

		mat._41 = 260;
		mat._42 = -270;
		poDirect3DDevice->SetTransform(D3DTS_WORLD, &mat);
		m_poWeaponMesh = poPlayer2->getCurrentWeapon()->getMesh();
		m_poGrenadeMesh = poPlayer2->getCurrentGrenade()->getMesh();
		poDirect3DDevice->GetMaterial(&matOld);
		poDirect3DDevice->SetMaterial(&matDefault);
		if (m_poWeaponMesh)
			m_poWeaponMesh->draw();

		// redo for the grenade
		D3DXMatrixIdentity(&mat);
		if ((poPlayer2->getCurrentGrenade()->getType() & OBJ_WEAPON_BIGCLUCKINGBOMB) == OBJ_WEAPON_BIGCLUCKINGBOMB)
		{
			D3DXMatrixScaling(&scale, 3.0f, 3.0f, 3.0f);
		}
		else
		{
			D3DXMatrixScaling(&scale, 90.5f, 90.5f, 90.5f);
		}

		D3DXMatrixRotationY(&rotY, fRot);
		D3DXMatrixMultiply(&mat, &mat, &rotY);
		D3DXMatrixMultiply(&mat, &mat, &scale);
		mat._41 = 380;
		if ((poPlayer2->getCurrentGrenade()->getType() & OBJ_WEAPON_BIGCLUCKINGBOMB) == OBJ_WEAPON_BIGCLUCKINGBOMB)
			mat._42 = -200;
		else
			mat._42 = -270;

		mat._31 = -mat._31;
		mat._32 = -mat._32;
		mat._33 = -mat._33;
		mat._11 = -mat._11;
		mat._12 = -mat._12;
		mat._13 = -mat._13;

		poDirect3DDevice->SetTransform(D3DTS_WORLD, &mat);
		if (m_poGrenadeMesh)
		{
			if ((poPlayer2->getCurrentGrenade()->getType() & OBJ_WEAPON_BIGCLUCKINGBOMB) == OBJ_WEAPON_BIGCLUCKINGBOMB)
				m_poGrenadeMesh->draw();
			else
				m_poGrenadeMesh->draw(false, &m_dwgrenadeColor);
		}
		poDirect3DDevice->SetMaterial(&matOld);

		// draw the ammo for the grenade
		if ((poPlayer2->getCurrentGrenade()->getType() & OBJ_WEAPON_BIGCLUCKINGBOMB) == OBJ_WEAPON_BIGCLUCKINGBOMB)
			sprintf(szBuffer, "%d", poPlayer2->getCurrentGrenade()->getAmmoLeft());
		else
			sprintf(szBuffer, "%d", poPlayer2->getCurrentGrenade()->getNumGrenades());
		m_poPlayer2GrenadeAmmo->setText(szBuffer);
		m_poPlayer2GrenadeAmmo->drawButton(NULL);
	}

	D3DXMatrixIdentity(&mat);
	// draw the arrow to point to the next objective
	D3DXVECTOR3 vDirection;
	CAIManager::getInstance().getDistanceToObjective(&vDirection);
	D3DXVECTOR3 vCameraAt = CCamera::getInstance().getCameraAt();
	vCameraAt.y = 0;
	D3DXVec3Normalize(NULL, &vCameraAt, &vCameraAt);

	float fDot = D3DXVec3Dot(&vCameraAt, &vDirection);
	float fZXDot = D3DXVec3Dot(&vDirection, &CCamera::getInstance().getCameraRight());
	D3DXMATRIX mRot;
	
	if (fZXDot < 0)
		D3DXMatrixRotationY(&mRot, (2.0f * D3DX_PI) +acosf(fDot));
	else
		D3DXMatrixRotationY(&mRot, -acosf(fDot));

	D3DXMatrixMultiply(&mat, &mat, &mRot);
	D3DXMatrixRotationY(&rotY, D3DX_PI * 0.5f);
	D3DXMatrixMultiply(&mat, &mat, &rotY);
	D3DXMatrixRotationX(&rotX, D3DX_PI * 0.5f);
	D3DXMatrixMultiply(&mat, &mat, &rotX);
	D3DXMatrixScaling(&scale, 20.5f, 20.5f, 20.5f);
	D3DXMatrixMultiply(&mat, &mat, &scale);
	mat._41 = -32;
	mat._42 = float((CMenuManager::getInstance().getWindowHeight() >> 1) - 30);

	// Revert for drawing correctly
	mat._31 = -mat._31;
	mat._32 = -mat._32;
	mat._33 = -mat._33;
	mat._11 = -mat._11;
	mat._12 = -mat._12;
	mat._13 = -mat._13;

	// draw the objective arrow.
	poDirect3DDevice->SetTransform(D3DTS_WORLD, &mat);
	poDirect3DDevice->GetMaterial(&matOld);
	poDirect3DDevice->SetMaterial(&matDefault);
	m_pObjectiveArrow->draw();
	poDirect3DDevice->SetMaterial(&matOld);


	poDirect3DDevice->SetTransform(D3DTS_WORLD, &old);

	// set to none for the objective text background.
	poDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	poDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// draw the objective fontness
	CObjectiveTrigger* poObjective = CAIManager::getInstance().getCurrentObjective();

	if (poObjective)
	{
		m_poObjectiveTextBG->drawButton(NULL);

		CString output(poObjective->m_strToolTip.GetBuffer());
		m_poObjectiveFont->drawString(output, ((CMenuManager::getInstance().getWindowWidth() >> 1)
			+ 18.0f), 22.0f);
	}

	poDirect3DDevice->SetRenderState(D3DRS_CULLMODE, dwCullMode);
	poDirect3DDevice->SetMaterial(&matOld);
}

/**
* CHUDMenu::addToAwesomeMeter
* @date Modified May 26, 2006
*/
void CHUDMenu::addToAwesomeMeter(int nNum)
{
	if (nNum == 0)
	{
		m_poAwesomeMeter->addToVertexData(-m_nAwesomeMeterWidth, 0, true);
		m_poAwesomeMeter->addToVertexDataUV(-((float)m_nAwesomeMeterWidth / MAX_AWESOME), 0, true);
		m_nAwesomeMeterWidth = 0;
	}
	else
	{
		// Use a buffer to increment awesome meter so that it fills up slower.
		m_nAwesomeMeterBuffer += nNum;
		if (m_nAwesomeMeterBuffer >= MAX_AWESOME_BUFFER)
		{
			m_nAwesomeMeterBuffer = 0;
			if (m_nAwesomeMeterWidth + nNum >= MAX_AWESOME_TEX)
			{
				CObjectManager::ObjectList loPlayers;
				CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
				CObjectManager::ObjectList::iterator playerIter = loPlayers.begin();

				while(loPlayers.end() != playerIter)
				{
					if(!((CPlayer*)*playerIter)->isAwesome())
						((CPlayer*)*playerIter)->enterAwesomeTime();

					++playerIter;
				}

				return;
			}

			m_nAwesomeMeterWidth += nNum;
			m_poAwesomeMeter->addToVertexData(nNum, 0, true);
			m_poAwesomeMeter->addToVertexDataUV((float)nNum / MAX_AWESOME, 0, true);
		}
	}
}





