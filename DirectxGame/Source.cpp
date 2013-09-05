
// 初期化サンプルプログラム

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>


TCHAR gName[100] = _T("初期化サンプルプログラム");


LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam){
   if(mes == WM_DESTROY) {PostQuitMessage(0); return 0;}
   return DefWindowProc(hWnd, mes, wParam, lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
   // アプリケーションの初期化
   MSG msg; HWND hWnd;
   WNDCLASSEX wcex ={sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, NULL,
                                    (HBRUSH)(COLOR_WINDOW+1), NULL, (TCHAR*)gName, NULL};
   if(!RegisterClassEx(&wcex))
      return 0;

   int w = 640, h = 480;
   RECT clientRect = {0, 0, w, h};
   ::AdjustWindowRect( &clientRect, WS_OVERLAPPEDWINDOW, FALSE );

   if(!(hWnd = CreateWindow(gName, gName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
           clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
                                    NULL, NULL, hInstance, NULL)))
      return 0;

   // Direct3Dの初期化
   LPDIRECT3D9 g_pD3D;
   LPDIRECT3DDEVICE9 g_pD3DDev;
   if( !(g_pD3D = Direct3DCreate9( D3D_SDK_VERSION )) ) return 0;

   D3DPRESENT_PARAMETERS d3dpp = {w,h,D3DFMT_UNKNOWN,0,D3DMULTISAMPLE_NONE,0,
                                 D3DSWAPEFFECT_DISCARD,NULL,TRUE,TRUE,D3DFMT_D24S8,0,0}; 

   if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev ) ) )
   if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev ) ) )
   if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev ) ) )
   if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev ) ) )
   {
      g_pD3D->Release();
      return 0;
   }

   ShowWindow(hWnd, nCmdShow);


   // フォントの生成
   int fontsize = 24;
   D3DXFONT_DESC lf = {fontsize, 0, 0, 1, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
                        PROOF_QUALITY, FIXED_PITCH | FF_MODERN, _T("ＭＳ ゴシック")};

   // ID3DXFontコンポーネント生成
   LPD3DXFONT pD3DFont;
   if(FAILED(D3DXCreateFontIndirect(g_pD3DDev, &lf, &pD3DFont))){
      g_pD3DDev->Release();  g_pD3D->Release();
      return 0;
   }

   // メッセージ ループ
   do{
      if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
                  TranslateMessage( &msg );
                  DispatchMessage( &msg );
          }
      else{
         // Direct3Dの処理
         g_pD3DDev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
         g_pD3DDev->BeginScene();

         // 描画
             RECT r={0,0,0,0};
         pD3DFont->DrawText(NULL, _T("Hello World !"), -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, 0xffffffff);

         g_pD3DDev->EndScene();
         g_pD3DDev->Present( NULL, NULL, NULL, NULL );
      }
   }while(msg.message != WM_QUIT);

   pD3DFont->Release();
   g_pD3DDev->Release();
   g_pD3D->Release();

   return 0;
}