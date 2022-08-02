
// Test3View.h: интерфейс класса CTest3View
//

#pragma once
#include <cstring>



class CTest3View : public CView
{
protected: // создать только из сериализации
	CTest3View() noexcept;
	DECLARE_DYNCREATE(CTest3View)



// Атрибуты
public:
	CTest3Doc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CTest3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CFile* PickFile; //файл картинке
	HBITMAP LodePicture(CFile* pFile, int cx, int cy, CBrush *pBrush, CDC* pCDC);
// Созданные функции схемы сообщений
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};



#ifndef _DEBUG  // версия отладки в Test3View.cpp
inline CTest3Doc* CTest3View::GetDocument() const
   { return reinterpret_cast<CTest3Doc*>(m_pDocument); }
#endif

