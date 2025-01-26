#include "..\..\Header\Renderer.h"

#include "Camera.h"
#include "UICamera.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer() : m_pCamera (nullptr), m_pUICamera(nullptr)
{
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eID, CGameObject * pGameObject)
{
	if (RENDER_END <= eID || nullptr == pGameObject)
		return;

	m_RenderGroup[eID].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	if (!m_pCamera && !m_pUICamera)
	{
		Render_Priority(pGraphicDev);
		Render_NonAlpha(pGraphicDev);
		Render_Alpha(pGraphicDev);
		Render_UI(pGraphicDev);
		Clear_RenderGroup();
	}
	else
	{
		m_pCamera->Set_View_Space();
		m_pCamera->Set_Projection_Space();

		Render_Priority(pGraphicDev);
		Render_NonAlpha(pGraphicDev);
		Render_Alpha(pGraphicDev);

		_matrix matWorld;
		
		//m_pUICamera->Set_World_Space();
		//m_pUICamera->Set_View_Space();
		//m_pUICamera->Set_Projection_Space();

		Render_UI(pGraphicDev);

		m_pCamera->Set_View_Space();
		m_pCamera->Set_Projection_Space();

		Clear_RenderGroup();
	}

}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_GameObject();

}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_GameObject();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//(RGB)A		 (R`G`B`)A`
	//(AR, AG, AB) + ((1 - A)R`, (1 - A)G`, (1 - A)B`)

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	m_RenderGroup[RENDER_ALPHA].sort([](CGameObject* pDst, CGameObject* pSrc)->bool 
	{
		return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
	});

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_GameObject();
}

void CRenderer::Render_Particle(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	//터지면 아래 오브젝트 렌더 전까지 렌더 버퍼로 옮기자
	pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, _ulong(0.f));

	pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, _ulong(0.f));
	pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, _ulong(0.f));
	pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, _ulong(0.f));

	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	for (auto& iter : m_RenderGroup[RENDER_PARTICLE])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Free()
{
	Clear_RenderGroup();
}