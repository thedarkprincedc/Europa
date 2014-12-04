HRESULT CMyD3DApplication::CreateD3DXTextMesh( LPD3DXMESH* ppMesh, 
                                               TCHAR* pstrFont, DWORD dwSize,
                                               BOOL bBold, BOOL bItalic )
{
    HRESULT hr;
    LPD3DXMESH pMeshNew = NULL;
    HDC hdc = CreateCompatibleDC( NULL );
    INT nHeight = -MulDiv( 60, GetDeviceCaps(hdc, LOGPIXELSY), 72 );
    HFONT hFont;
    HFONT hFontOld;

    hFont = CreateFont(nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, pstrFont);
    
    hFontOld = (HFONT)SelectObject(hdc, hFont); 

    hr = D3DXCreateText(m_pd3dDevice, hdc, _T("This is calling D3DXCreateText"), 
                        0.001f, 0.4f, &pMeshNew, NULL, NULL);

    SelectObject(hdc, hFontOld);
    DeleteObject( hFont );
    DeleteDC( hdc );

    if( SUCCEEDED( hr ) )
        *ppMesh = pMeshNew;

    return hr;
} 



if( m_pMesh3DText != NULL )
        {
            ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
            mtrl.Diffuse.r = mtrl.Ambient.r = 0.0f;
            mtrl.Diffuse.g = mtrl.Ambient.g = 0.0f;
            mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
            mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
            m_pd3dDevice->SetMaterial( &mtrl );
            m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObj2 );
            m_pMesh3DText->DrawSubset(0);
        }