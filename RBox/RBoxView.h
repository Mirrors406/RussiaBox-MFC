
// RBoxView.h : CRBoxView ��Ľӿ�
//

#pragma once

#define MAX_ROW 22	//��������Ϊ22
#define MAX_COL 12	//��������Ϊ12
#define BOX_SIZE 30	//���÷����СΪ30
#define MIN_ROW_COL 4	//������һ���顢���䷽����4��4�з�����

#define KEY_UP 0
#define KEY_DOWN 1
#define KEY_LEFT 2
#define KEY_RIGHT 3


class CRBoxView : public CView
{
protected: // �������л�����
	CRBoxView();
	DECLARE_DYNCREATE(CRBoxView)

// ����
public:
	CRBoxDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CRBoxView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawMap(CDC * pDC);

private:
	bool m_boxMap[MAX_ROW][MAX_COL];	//��ͼ����
	bool m_boxNow[MIN_ROW_COL][MIN_ROW_COL];	//���䷽��
	bool m_boxWill[MIN_ROW_COL][MIN_ROW_COL];	//��һ����

	CPoint m_potNow;	//���䷽������ϵ�����
	bool m_bisEnd;	//��Ϸ������־
	int m_iScore;	//��Ϸ�÷�
	int m_iSpeed;	//�����ٶ�

	bool m_bisGaming; //�Ƿ�����Ϸ��

public:
	void DrawWillBox(CDC * pDC);
	void DrawText(CDC * pDC);
	afx_msg void OnStart();
	void boxCreate();
	void boxMove(int iKey);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool boxHitJudge(bool boxTmp[4][4], int iKey, CPoint & p);
	bool boxRoll(bool boxTmp[4][4], CPoint & p);
	void boxLineDel();
	bool gameOver();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // RBoxView.cpp �еĵ��԰汾
inline CRBoxDoc* CRBoxView::GetDocument() const
   { return reinterpret_cast<CRBoxDoc*>(m_pDocument); }
#endif

