/**
* @file ParticleManager.cpp
* @author Matt Rudder
* @date Modified Jun 01, 2006
*
* Handles the creation and update of particles, regardless of their emitter.
*/

// Local includes
#include "ParticleManager.h"

// Engine includes
#include "ParticleEffect.h"

D3DVERTEXELEMENT9 CParticleManager::m_pVertElem[] = 
{
	{ 0, 0,		D3DDECLTYPE_FLOAT3, 	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,		0 },
	{ 0, 12, 	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,			0 },
	{ 0, 16, 	D3DDECLTYPE_FLOAT2, 	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,		0 },
	D3DDECL_END()
};
const DWORD CParticleManager::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

CParticleManager::CParticleManager(void) : m_oVertexCache(1024, sizeof(SParticleVert), 1024, m_pVertElem)
{
	m_lInactiveParticles.reserve(100000);
	while(m_lInactiveParticles.size() < 100000)
		m_lInactiveParticles.push_back(new SParticle);
}

CParticleManager::CParticleManager(const CParticleManager&) : m_oVertexCache(1024, sizeof(SParticleVert), 1024, m_pVertElem)
{
}

CParticleManager::~CParticleManager(void)
{
	// Cleanup effects
	vector<CParticleEffect*>::iterator itParticle = m_lLoadedEffects.begin();
	while(itParticle != m_lLoadedEffects.end())
	{
		delete *itParticle;
		++itParticle;
	}
	m_lLoadedEffects.clear();

	ParticleList::iterator iter = m_lInactiveParticles.begin();
	while(iter != m_lInactiveParticles.end())
	{
		delete *iter;
		++iter;
	}
	m_lInactiveParticles.clear();	

	CParticleManager::ParticleMap::iterator itActive = m_mActiveParticles.begin();
	while(itActive != m_mActiveParticles.end())
	{
		iter = itActive->second.begin();
		while(iter != itActive->second.end())
		{
			delete *iter;
			++iter;
		}
		itActive->second.clear();
		++itActive;
	}
	m_mActiveParticles.clear();
}

/**
* CParticleManager::addParticle
* @date Modified Jun 01, 2006
*/
void CParticleManager::addParticle(D3DXMATRIX* mOffset, CParticleEffect* pEffect, CParticleSystem* pSys)
{
	// Get new particle.
	SParticle* pNew = NULL;
	if(m_lInactiveParticles.empty())
		pNew = new SParticle;
	else
	{
		pNew = m_lInactiveParticles.back();
		m_lInactiveParticles.pop_back();
	}

	// Reset particle
	memset(pNew, 0, sizeof(SParticle));
	pNew->System = pSys;
	pEffect->initParticle(pNew, mOffset);

	// Save particle
	ParticleMap::iterator itEffect = m_mActiveParticles.find(pEffect);
	m_mActiveParticles[pEffect].push_back(pNew);
}

/**
* Updates all the particles currently managed
*
* @date Created Jun 01, 2006
* @param[in]	fTime	Time passed since the last update.
*/
void CParticleManager::update(float fTime)
{
	// Iterate all active particles and update them.
	ParticleMap::iterator itParticleList = m_mActiveParticles.begin();
	while(itParticleList != m_mActiveParticles.end())
	{
		ParticleList::iterator itParticle = itParticleList->second.begin();
		while(itParticle != itParticleList->second.end())
		{
			if(!itParticleList->first->updateParticle(fTime, *itParticle))
			{
				(*itParticle)->System->removeParticle(itParticleList->first);
				m_lInactiveParticles.push_back(*itParticle);
				itParticle = itParticleList->second.erase(itParticle);
			}
			else
				++itParticle;
		}
		++itParticleList;
	}
}

__forceinline void CParticleManager::drawParticle(SParticle* pParticle, D3DXVECTOR3& vRight, D3DXVECTOR3& vUp, D3DXVECTOR3& vAt)
{
	if(pParticle->Size <= 0.000001f)
		return;

	SParticleVert verts[6];
	memset(verts, 0, sizeof(SParticleVert) * 6);
	D3DXMATRIX mTrans, mRot, mCombine;

	D3DXMatrixTranslation(&mTrans, pParticle->Position.x, pParticle->Position.y, pParticle->Position.z);
	D3DXMatrixRotationAxis(&mRot, &vAt, pParticle->Rotation);
	D3DXMatrixMultiply(&mCombine, &mRot, &mTrans);

	// Calculate vertex position.
	verts[0].Position = (-vRight + vUp) * pParticle->Size;
	verts[1].Position = ( vRight - vUp) * pParticle->Size;
	verts[2].Position = (-vRight - vUp) * pParticle->Size;
	verts[4].Position = ( vRight + vUp) * pParticle->Size;

	D3DXVec3TransformCoord(&verts[0].Position, &verts[0].Position, &mCombine);
	verts[3].Position = verts[0].Position;
	D3DXVec3TransformCoord(&verts[1].Position, &verts[1].Position, &mCombine);
	verts[5].Position = verts[1].Position;
	D3DXVec3TransformCoord(&verts[2].Position, &verts[2].Position, &mCombine);
	D3DXVec3TransformCoord(&verts[4].Position, &verts[4].Position, &mCombine);

	// Texture coords
	verts[0].Tu = 0.0f; verts[0].Tv = 0.0f;
	verts[1].Tu = 1.0f; verts[1].Tv = 1.0f;
	verts[2].Tu = 0.0f; verts[2].Tv = 1.0f;
	verts[3].Tu = 0.0f; verts[3].Tv = 0.0f;
	verts[4].Tu = 1.0f; verts[4].Tv = 0.0f;
	verts[5].Tu = 1.0f; verts[5].Tv = 1.0f;

	// Colors
	verts[0].Color = pParticle->Color;
	verts[1].Color = pParticle->Color;
	verts[2].Color = pParticle->Color;
	verts[3].Color = pParticle->Color;
	verts[4].Color = pParticle->Color;
	verts[5].Color = pParticle->Color;

	m_oVertexCache.insert(verts, 6, NULL, 6);
}

/**
* Renders all currently active particles.
*
* @date Created Jun 01, 2006
*/
void CParticleManager::draw(void)
{
	if(m_mActiveParticles.empty())
		return;

	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();

	// Get billboard transform
	D3DXMATRIX mView;
	pDev->GetTransform(D3DTS_VIEW, &mView);
	D3DXMatrixTranspose(&mView, &mView);
	D3DXVECTOR3 vRight(mView._11, mView._12, mView._13),
				vUp(mView._21, mView._22, mView._23),
				vAt(mView._31, mView._32, mView._33);

	// Setup global particle render states
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDev->SetFVF(FVF);

	ParticleMap::iterator itParticleList = m_mActiveParticles.begin();
	ParticleList::iterator itParticle;
	CParticleEffect* pEffect = NULL;
	while(itParticleList != m_mActiveParticles.end())
	{
		// Check for per-effect initialization.
		if(pEffect != (CParticleEffect*)itParticleList->first)
		{
			// Cleanup states from last effect.
			if(pEffect)
			{
				m_oVertexCache.flush();
				switch(pEffect->m_eBlendMode)
				{
				case CParticleEffect::OP_REPLACE:
				case CParticleEffect::OP_ADDITIVE:
					pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
					pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					break;
				case CParticleEffect::OP_MULTIPLY:
					pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
					break;
				}
			}

			// Change states for next effect.
			pEffect = itParticleList->first;
				pDev->SetTexture(0, pEffect->m_pTexture->getD3DTexture());
			switch(pEffect->m_eBlendMode)
			{
			case CParticleEffect::OP_REPLACE:
				pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				break;
			case CParticleEffect::OP_ADDITIVE:
				pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
				pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				break;
			case CParticleEffect::OP_MULTIPLY:
				pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
				break;
			}

			m_oVertexCache.begin();
		}

		itParticle = itParticleList->second.begin();
		while(itParticle != itParticleList->second.end())
		{
			// Draw particles
			drawParticle(*itParticle, vRight, vUp, vAt);
			++itParticle;
		}
		++itParticleList;
	}

	// Cleanup states from final effect.
	if(pEffect)
	{
		m_oVertexCache.flush();
		pDev->SetTexture(0, NULL);
		switch(pEffect->m_eBlendMode)
		{
		case CParticleEffect::OP_REPLACE:
		case CParticleEffect::OP_ADDITIVE:
			pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			break;
		case CParticleEffect::OP_MULTIPLY:
			pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			break;
		}
	}

	// Revert global particle states
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

/**
* CParticleManager::addEffect
* @date Modified Jun 01, 2006
*/
void CParticleManager::addEffect(CParticleEffect* pEffect)
{
	m_mActiveParticles[pEffect].reserve(pEffect->getMaxParticles() * 6);
	m_lLoadedEffects.push_back(pEffect);
}
