
// RBoxView.h : CRBoxView 类的接口
//

#pragma once

#define MAX_ROW 22	//设置行数为22
#define MAX_COL 12	//设置列数为12
#define BOX_SIZE 30	//设置方块大小为30
#define MIN_ROW_COL 4	//设置下一方块、下落方块在4行4列方块中

#define KEY_UP 0
#define KEY_DOWN 1
#define KEY_LEFT 2
#define KEY_RIGHT 3


class CRBoxView : public CView
{
protected: // 仅从序列化创建
	CRBoxView();
	DECLARE_DYNCREATE(CRBoxView)

// 特性
public:
	CRBoxDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CRBoxView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawMap(CDC * pDC);

private:
	bool m_boxMap[MAX_ROW][MAX_COL];	//地图方块
	bool m_boxNow[MIN_ROW_COL][MIN_ROW_COL];	//下落方块
	bool m_boxWill[MIN_ROW_COL][MIN_ROW_COL];	//下一方块

	CPoint m_potNow;	//下落方块的左上点坐标
	bool m_bisEnd;	//游戏结束标志
	int m_iScore;	//游戏得分
	int m_iSpeed;	//下落速度

	bool m_bisGaming; //是否在游戏中

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

#ifndef _DEBUG  // RBoxView.cpp 中的调试版本
inline CRBoxDoc* CRBoxView::GetDocument() const
   { return reinterpret_cast<CRBoxDoc*>(m_pDocument); }
#endif

