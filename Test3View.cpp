
// Test3View.cpp: реализация класса CTest3View
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Test3.h"
#endif

#include "Test3Doc.h"
#include "Test3View.h"
#include "image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CTest3View





IMPLEMENT_DYNCREATE(CTest3View, CView)

BEGIN_MESSAGE_MAP(CTest3View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CTest3View

CTest3View::CTest3View() noexcept
{
	// TODO: добавьте код создания
	PickFile = new CFile();
}



CTest3View::~CTest3View()
{
}

BOOL CTest3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CTest3View

void CTest3View::OnDraw(CDC* pDC)
{
	CTest3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	PickFile->Open(L"image1.bmp", CFile::modeRead | CFile::shareDenyWrite);
}

HBITMAP CTest3View::LodePicture(CFile* pFile, int cx, int cy, CBrush* pBrush, CDC* pCDC)
{
	ASSERT(pFile != NULL);
	ASSERT(pFile->m_hFile != 0);

	CArchive ar(pFile, CArchive::load | CArchive::bNoFlushOnDelete);

	CArchiveStream ars(&ar);

	LPPICTURE lpPicture = NULL;

	HRESULT hr = OleLoadPicture((LPSTREAM)&ars, 0, FALSE, IID_IPicture, (void**)&lpPicture);

	((LPSTREAM)&ars)->Release();

	ar.Close();

	HBITMAP hbm = NULL;

	if (SUCCEEDED(hr) && (lpPicture != NULL)) 
	{
		HDC hDC = ::GetWindowDC(NULL);

		if (pCDC != NULL)
			hDC = pCDC->GetSafeHdc();
		else
			hDC = ::GetWindowDC(NULL);

		if (hDC != NULL) {
			CSize szScr = NULL;

			if (SUCCEEDED(lpPicture->get_Width((OLE_XSIZE_HIMETRIC*)&szScr.cx))) {
				if (SUCCEEDED(lpPicture->get_Height((OLE_XSIZE_HIMETRIC*)&szScr.cy))){
					CDC dc;
					CSize szDest(szScr);

					dc.Attach(hDC);
					dc.HIMETRICtoDP(&szDest);
					dc.Detach();

					if (cx > 0)
						szDest.cx = cx;
					if (cy > 0)
						szDest.cy = cy;

					HDC hDCMem = ::CreateCompatibleDC(hDC);
					if (hDCMem != NULL) {
						hbm = ::CreateCompatibleBitmap(hDC, szDest.cx, szDest.cy);

						if (hbm != NULL) {
							HBITMAP hbmold = (HBITMAP) ::SelectObject(hDCMem, hbm);
							::FillRect(hDCMem, CRect(CPoint(0, 0), szDest),
								(pBrush != NULL) ?
								(HBRUSH)pBrush->GetSafeHandle() :
								::GetSysColorBrush(COLOR_WINDOW));
							HRESULT hre = lpPicture->Render(hDCMem, 0, 0, szDest.cx, szDest.cy, 0, szDest.cy - 1, szDest.cx, -szDest.cy, NULL);
							::SelectObject(hDCMem, hbmold);

							if (FAILED(hre)) {
								::DeleteObject(hbm);
								hbm = NULL;
							}
						}

						::DeleteObject(hDCMem);
					}
				}
			}

			if (pCDC == NULL)
				::ReleaseDC(NULL, hDC);
		}

		lpPicture->Release();
	}

	return hbm;
}

void CTest3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTest3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CTest3View

#ifdef _DEBUG
void CTest3View::AssertValid() const
{
	CView::AssertValid();
}

void CTest3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTest3Doc* CTest3View::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest3Doc)));
	return (CTest3Doc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CTest3View
