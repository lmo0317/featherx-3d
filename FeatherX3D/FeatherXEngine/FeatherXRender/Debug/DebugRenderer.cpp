#include "FeatherXEnginePrecompile.h"

DEBUG_RENDERER::DEBUG_RENDERER()
{
   m_DebugRenderVertex = NULL;
   m_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
}

DEBUG_RENDERER::~DEBUG_RENDERER()
{
}

// 렌더링
void DEBUG_RENDERER::RenderBuffer( const LPDIRECT3DDEVICE9 pd3dDevice, const DEBUG_RENDER_VERTEX *pVertex, const D3DPRIMITIVETYPE Type, const int VertexCount )
{
   // 현재 렌더스테이트 설정을 저장

   DWORD RStateLightingBK;
   pd3dDevice->GetRenderState( D3DRS_LIGHTING, &RStateLightingBK );
   // 라이팅 Disable
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

   DWORD RStateCullModeBK;
   pd3dDevice->GetRenderState( D3DRS_CULLMODE, &RStateCullModeBK );
   // 컬 모드 해제
   pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

   IDirect3DBaseTexture9* ppTexBK = NULL;
   pd3dDevice->GetTexture( 0, &ppTexBK );
   // 텍스처를 Disable
   pd3dDevice->SetTexture( 0, NULL );

   DWORD FVFBK;
   pd3dDevice->GetFVF( &FVFBK );
   pd3dDevice->SetFVF( m_FVF );

   // 렌더링
   pd3dDevice->DrawPrimitiveUP( Type, VertexCount , pVertex, sizeof(DEBUG_RENDER_VERTEX) );

   // 렌더스테이트 복구
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, RStateLightingBK );
   pd3dDevice->SetRenderState( D3DRS_CULLMODE, RStateCullModeBK );
   pd3dDevice->SetTexture( 0, ppTexBK );
   pd3dDevice->SetFVF( FVFBK );
}

// 버텍스 데이터를 PhysX로부터 얻는다
void DEBUG_RENDERER::RenderData( const LPDIRECT3DDEVICE9 pd3dDevice, const NxDebugRenderable* data )
{
   // 버텍스 좌표를 갱신하는 방식으로 이동시키고있으므로 월드행렬을 단위행렬로 한다.
   D3DXMATRIX matWorld;
   D3DXMatrixIdentity( &matWorld );
   pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

   FBDEL( m_DebugRenderVertex );

   if( data == NULL || pd3dDevice == NULL )
      goto ERROR_EXIT;

   DWORD dwCount;

   //*******************************************************************************************
   // 버텍스 그리기
   //*******************************************************************************************

   // 버텍스 카운트 얻어옴
   unsigned int NbPoints = data->getNbPoints();
   if(NbPoints)
   {
      // 버텍스 카운트 만큼 영역확보
      m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbPoints];
      if( m_DebugRenderVertex == NULL )
         goto ERROR_EXIT;

      // 버텍스 데이터 취득
      const NxDebugPoint* Points = data->getPoints();

      dwCount = 0;

      // 버텍스 정보를 Direct3D 렌더용 구조체로 카피
      while(NbPoints--)
      {
         m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Points->p.x, Points->p.y, Points->p.z );
         m_DebugRenderVertex[dwCount].color = (DWORD)Points->color;
         dwCount++;
         Points++;
      }

      //렌더링
      RenderBuffer( pd3dDevice, m_DebugRenderVertex, D3DPT_POINTLIST, data->getNbPoints() );

      FBDEL( m_DebugRenderVertex );
   }

   //*******************************************************************************************
   // 선 그리기
   //*******************************************************************************************

   // 선 카운트 얻어옴
   unsigned int NbLines = data->getNbLines();
   if(NbLines)
   {
      //선의 개수만큼 영역확보
      m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbLines * 2];
      if( m_DebugRenderVertex == NULL )
         goto ERROR_EXIT;

      // 선 데이터 얻어옴
      const NxDebugLine* Lines = data->getLines();

      dwCount = 0;

      //선 정보를 Direct3D 렌더용 구조체로 카피
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

      // 렌더링
      RenderBuffer( pd3dDevice, m_DebugRenderVertex, D3DPT_LINELIST, data->getNbLines() );

      FBDEL( m_DebugRenderVertex );
   }

   //*******************************************************************************************
   //△ 폴리곤 렌더링
   //*******************************************************************************************

   //△ 폴리곤 개수 얻어옴
   unsigned int NbTriangles = data->getNbTriangles();
   if(NbTriangles)
   {
      //△ 폴리곤 개수만큼 영역확보
      m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbTriangles * 3];
      if( m_DebugRenderVertex == NULL )
         goto ERROR_EXIT;

      //△폴리곤 데이터 얻어옴
      const NxDebugTriangle* Triangles = data->getTriangles();

      dwCount = 0;

      // △폴리곤의 정보를 Direct3D 렌더용 구조체로 카피
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

      //렌더링
      RenderBuffer( pd3dDevice, m_DebugRenderVertex, D3DPT_TRIANGLELIST, data->getNbTriangles() );

      FBDEL( m_DebugRenderVertex );
   }

   return;

ERROR_EXIT:

   return;
}