#include "..\..\Header\Calculator.h"

#include "Export_Utility.h"

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator()
{
	return S_OK;
}

_float CCalculator::Compute_HeightOnTerrain(const _vec3 * pPos,
											const _vec3 * pTerrainVtxPos,
											const _ulong & dwCntX, 
											const _ulong & dwCntZ,
											const _ulong& dwVtxItv)
{

	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	_float	fRatioX = _float(pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float	fRatioZ = _float(pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;

	D3DXPLANE	Plane;

	// 오른쪽 위
	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + dwCntX + 1],
			&pTerrainVtxPos[dwIndex + 1]);
	}
	 // 왼쪽 아래 
	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}

	// Y = (-AX -CZ - D) / B


	return  (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;
}

_int CCalculator::Picking_OnTerrain(HWND hWnd,
									vector<Engine::CMesh*> _MeshGroupCom,
									CTransform * pTerrainTransCom)
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 윈도우 좌표계 -> 투영

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3		vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;

	vMousePos.z = 0.f;

	// 투영 -> 뷰스페이스
	_matrix	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matWorld;
	pTerrainTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	for (int index=0; index< (_MeshGroupCom).size();++index)
	{
		//(*_MeshGroupCom)[index]->Get_VtxPos();
		  vector<_vec3> pMeshVtxPos =((_MeshGroupCom)[index]->Get_VtxPos());	
		  vector<INDEX32> pMeshVtxIdc= ((_MeshGroupCom)[index]->Get_VtxIdc());	

		_ulong	dwVtxIdx[3]{};
		_float	fU(0.f), fV(0.f), fDist(0.f);

		for (_ulong i = 0; i < (pMeshVtxIdc).size(); ++i)
		{
				dwVtxIdx[0] = (pMeshVtxIdc)[i]._0;
				dwVtxIdx[1] = (pMeshVtxIdc)[i]._1;
				dwVtxIdx[2] = (pMeshVtxIdc)[i]._2;

				if (D3DXIntersectTri(&(pMeshVtxPos)[dwVtxIdx[1]],
					&(pMeshVtxPos)[dwVtxIdx[0]],	
					&(pMeshVtxPos)[dwVtxIdx[2]],
					&vRayPos, &vRayDir,
					&fU, &fV, &fDist))
				{
					return index;
				}
		}
	}

	return -1;
}


_vec3 CCalculator::Picking_OnObj(HWND hWnd,
	vector<Engine::CMesh*> _MeshGroupCom,
	CTransform* pTerrainTransCom)
{

	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 뷰포트 -> 투영

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3		vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;

	vMousePos.z = 0.f;

	// 투영 -> 뷰스페이스
	_matrix	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matWorld;
	pTerrainTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);


	_ulong	dwVtxIdx[3]{};
	_ulong	faceVtxIdx[3]{};
	DWORD DFace(0.f);
	_float	fU(0.f), fV(0.f), fDist(0.f);
	BOOL Hit;
	for (auto& iter: _MeshGroupCom)
	{
		D3DXIntersect(iter->Get_Mesh(), &vRayPos, &vRayDir, &Hit, &DFace,&fU,&fV, &fDist,NULL,NULL);
		if (Hit)
		{
			_vec3 b0 =iter->Get_VtxPos()[iter->Get_VtxIdc()[DFace]._0];
			_vec3 b1 =iter->Get_VtxPos()[iter->Get_VtxIdc()[DFace]._1];
			_vec3 b2 =iter->Get_VtxPos()[iter->Get_VtxIdc()[DFace]._2];

			//V1 + U(V2 - V1) + V(V3 - V1)
			//V2 + U(V0 - V2) + V()
			_vec3 a = _vec3(
				b0.x + fU * (b1.x -b0.x) + fV * (b2.x - b0.x),
				2.f,
				b0.z + fU * (b1.z - b0.z) +  fV * (b2.z - b0.z));
			
				return a;	
		}
	}



	return _vec3(0.f, 0.f, 0.f);
}

CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*		pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Calculator Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

CComponent * CCalculator::Clone(void)
{
	return new CCalculator(*this);
}

void CCalculator::Free()
{
	CComponent::Free();
}
