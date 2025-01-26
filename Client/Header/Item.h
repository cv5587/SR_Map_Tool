#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCalculator;

END

class CItem : public Engine::CGameObject
{
private:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev, ITEMTYPE eItemType, _int iNum);
	explicit CItem(const CItem& rhs);
	virtual ~CItem();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	void Get_Item();

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;
	ITEMTYPE m_eItemtype;
	_int	 m_iNum;

public:
	static CItem* Create(LPDIRECT3DDEVICE9	pGraphicDev, ITEMTYPE eItemType, _int iNum);

private:
	virtual void Free() override;
};