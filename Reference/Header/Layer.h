#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CComponent*		Get_Component(COMPONENTID eID, wstring pObjTag, wstring pComponentTag);
	CGameObject*	Get_GameObject(wstring pObjTag);
	multimap<wstring, CGameObject*>* Get_Map() { return &m_mapObject; }
public:
	HRESULT		Add_GameObject(wstring pObjTag, CGameObject* pGameObject);

public:
	HRESULT		Ready_Layer();
	_int		Update_Layer(const _float& fTimeDelta);
	void		LateUpdate_Layer();

private:
	multimap<wstring, CGameObject*>		m_mapObject;

public:
	static		CLayer*		Create();

private:
	virtual		void		Free();

};

END