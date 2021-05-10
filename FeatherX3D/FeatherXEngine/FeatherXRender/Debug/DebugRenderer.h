#pragma once

class DEBUG_RENDERER
{
private:
	// DebugRenderer �� ���ؽ� ����
	typedef struct _DEBUG_RENDER_VERTEX
	{
		D3DXVECTOR3 vPos;
		DWORD color;
	}DEBUG_RENDER_VERTEX;

	DWORD m_FVF;
	DEBUG_RENDER_VERTEX* m_DebugRenderVertex;

public:
	DEBUG_RENDERER();
	~DEBUG_RENDERER();

	// ���ؽ� �����͸� PhysX�κ��� ���´�
	void RenderData( const LPDIRECT3DDEVICE9 pd3dDevice, const NxDebugRenderable* data );

private:
	// ������
	void RenderBuffer( const LPDIRECT3DDEVICE9 pd3dDevice, const DEBUG_RENDER_VERTEX *pVertex, const D3DPRIMITIVETYPE Type, const int VertexCount );
};