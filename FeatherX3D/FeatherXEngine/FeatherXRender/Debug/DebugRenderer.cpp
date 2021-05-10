#include "FeatherXEnginePrecompile.h"

DEBUG_RENDERER::DEBUG_RENDERER()
{
   m_DebugRenderVertex = NULL;
   m_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
}

DEBUG_RENDERER::~DEBUG_RENDERER()
{
}

// ������
void DEBUG_RENDERER::RenderBuffer( const LPDIRECT3DDEVICE9 pd3dDevice, const DEBUG_RENDER_VERTEX *pVertex, const D3DPRIMITIVETYPE Type, const int VertexCount )
{
   // ���� ����������Ʈ ������ ����

   DWORD RStateLightingBK;
   pd3dDevice->GetRenderState( D3DRS_LIGHTING, &RStateLightingBK );
   // ������ Disable
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

   DWORD RStateCullModeBK;
   pd3dDevice->GetRenderState( D3DRS_CULLMODE, &RStateCullModeBK );
   // �� ��� ����
   pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

   IDirect3DBaseTexture9* ppTexBK = NULL;
   pd3dDevice->GetTexture( 0, &ppTexBK );
   // �ؽ�ó�� Disable
   pd3dDevice->SetTexture( 0, NULL );

   DWORD FVFBK;
   pd3dDevice->GetFVF( &FVFBK );
   pd3dDevice->SetFVF( m_FVF );

   // ������
   pd3dDevice->DrawPrimitiveUP( Type, VertexCount , pVertex, sizeof(DEBUG_RENDER_VERTEX) );

   // ����������Ʈ ����
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, RStateLightingBK );
   pd3dDevice->SetRenderState( D3DRS_CULLMODE, RStateCullModeBK );
   pd3dDevice->SetTexture( 0, ppTexBK );
   pd3dDevice->SetFVF( FVFBK );
}

// ���ؽ� �����͸� PhysX�κ��� ��´�
void DEBUG_RENDERER::RenderData( const LPDIRECT3DDEVICE9 pd3dDevice, const NxDebugRenderable* data )
{
   // ���ؽ� ��ǥ�� �����ϴ� ������� �̵���Ű�������Ƿ� ��������� ������ķ� �Ѵ�.
   D3DXMATRIX matWorld;
   D3DXMatrixIdentity( &matWorld );
   pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

   FBDEL( m_DebugRenderVertex );

   if( data == NULL || pd3dDevice == NULL )
      goto ERROR_EXIT;

   DWORD dwCount;

   //*******************************************************************************************
   // ���ؽ� �׸���
   //*******************************************************************************************

   // ���ؽ� ī��Ʈ ����
   unsigned int NbPoints = data->getNbPoints();
   if(NbPoints)
   {
      // ���ؽ� ī��Ʈ ��ŭ ����Ȯ��
      m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbPoints];
      if( m_DebugRenderVertex == NULL )
         goto ERROR_EXIT;

      // ���ؽ� ������ ���
      const NxDebugPoint* Points = data->getPoints();

      dwCount = 0;

      // ���ؽ� ������ Direct3D ������ ����ü�� ī��
      while(NbPoints--)
      {
         m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Points->p.x, Points->p.y, Points->p.z );
         m_DebugRenderVertex[dwCount].color = (DWORD)Points->color;
         dwCount++;
         Points++;
      }

      //������
      RenderBuffer( pd3dDevice, m_DebugRenderVertex, D3DPT_POINTLIST, data->getNbPoints() );

      FBDEL( m_DebugRenderVertex );
   }

   //*******************************************************************************************
   // �� �׸���
   //*******************************************************************************************

   // �� ī��Ʈ ����
   unsigned int NbLines = data->getNbLines();
   if(NbLines)
   {
      //���� ������ŭ ����Ȯ��
      m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbLines * 2];
      if( m_DebugRenderVertex == NULL )
         goto ERROR_EXIT;

      // �� ������ ����
      const NxDebugLine* Lines = data->getLines();

      dwCount = 0;

      //�� ������ Direct3D ������ ����ü�� ī��
      while(NbLines--)
      {
         m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Lines->p0.x, Lines->p0.y, Lines->p0.z );
         m_DebugRenderVertex[dwCount].color = (DWORD)Lines->color;
         dwCount++;

         m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Lines->p1.x, Lines->p1.y, Lines->p1.z );
         m_DebugRenderVertex[dwCount].color = (DWORD)Lines->color;
         dwCount++;

         Lines++;
      }

      // ������
      RenderBuffer( pd3dDevice, m_DebugRenderVertex, D3DPT_LINELIST, data->getNbLines() );

      FBDEL( m_DebugRenderVertex );
   }

   //*******************************************************************************************
   //�� ������ ������
   //*******************************************************************************************

   //�� ������ ���� ����
   unsigned int NbTriangles = data->getNbTriangles();
   if(NbTriangles)
   {
      //�� ������ ������ŭ ����Ȯ��
      m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbTriangles * 3];
      if( m_DebugRenderVertex == NULL )
         goto ERROR_EXIT;

      //�������� ������ ����
      const NxDebugTriangle* Triangles = data->getTriangles();

      dwCount = 0;

      // ���������� ������ Direct3D ������ ����ü�� ī��
      while(NbTriangles--)
      {
         m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Triangles->p0.x, Triangles->p0.y, Triangles->p0.z );
         m_DebugRenderVertex[dwCount].color = (DWORD)Triangles->color;
         dwCount++;

         m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Triangles->p1.x, Triangles->p1.y, Triangles->p1.z );
         m_DebugRenderVertex[dwCount].color = (DWORD)Triangles->color;
         dwCount++;

         m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Triangles->p2.x, Triangles->p2.y, Triangles->p2.z );
         m_DebugRenderVertex[dwCount].color = (DWORD)Triangles->color;
         dwCount++;

         Triangles++;
      }

      //������
      RenderBuffer( pd3dDevice, m_DebugRenderVertex, D3DPT_TRIANGLELIST, data->getNbTriangles() );

      FBDEL( m_DebugRenderVertex );
   }

   return;

ERROR_EXIT:

   return;
}