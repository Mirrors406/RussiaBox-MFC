
// RBoxView.cpp : CRBoxView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "RBox.h"
#endif

#include "RBoxDoc.h"
#include "RBoxView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRBoxView

IMPLEMENT_DYNCREATE(CRBoxView, CView)

BEGIN_MESSAGE_MAP(CRBoxView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_START, &CRBoxView::OnStart)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CRBoxView 构造/析构

CRBoxView::CRBoxView()
{
	// TODO:  在此处添加构造代码
	int iRow, iCol;
	
	for (iRow = 0; iRow < MAX_ROW; iRow++){
		for (iCol = 0; iCol < MAX_COL; iCol++){
			m_boxMap[iRow][iCol] = false;
		}
	}

	for (iRow = 0; iRow < MIN_ROW_COL; iRow++){
		for (iCol = 0; iCol < MIN_ROW_COL; iCol++){
			m_boxWill[iRow][iCol] = false;
			m_boxNow[iRow][iCol] = false;
		}
	}
	
	m_potNow.x = m_potNow.y = 0;
	m_bisEnd = false;
	m_iScore = 0;
	m_iSpeed = 400;

}

CRBoxView::~CRBoxView()
{
}

BOOL CRBoxView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CRBoxView 绘制

void CRBoxView::OnDraw(CDC* pDC)
{
	CRBoxDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
	CDC backDC;							//创建后屏缓冲区
	backDC.CreateCompatibleDC(pDC);	

	//设置界面背景
	CBitmap backbit;
	backbit.LoadBitmap(IDB_BACK);
	backDC.SelectObject(backbit);

	//绘制地图方块(包括了空白方块、掉落方块、堆积方块)
	DrawMap(&backDC);

	//绘制下一方块
	DrawWillBox(&backDC);

	//绘制界面文字提示
	DrawText(&backDC);

	pDC->StretchBlt(0, 0, 600, 600, &backDC, 0, 0, 600, 600, SRCCOPY);

}


// CRBoxView 打印

BOOL CRBoxView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CRBoxView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CRBoxView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CRBoxView 诊断

#ifdef _DEBUG
void CRBoxView::AssertValid() const
{
	CView::AssertValid();
}

void CRBoxView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRBoxDoc* CRBoxView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRBoxDoc)));
	return (CRBoxDoc*)m_pDocument;
}
#endif //_DEBUG


// CRBoxView 消息处理程序



// 用于绘制地图方块（空白方块、下落方块、堆积方块）
//
void CRBoxView::DrawMap(CDC * pDC)
{
	int iRow, iCol;
	CPen penMap;
	penMap.CreatePen(PS_SOLID, 1, RGB(130, 140, 140));
	pDC->SelectObject(penMap);
	
	for (iRow = 0; iRow < MAX_ROW; iRow++)
	{
		for (iCol = 0; iCol < MAX_COL; iCol++)
		{	
			if (m_boxMap[iRow][iCol] == false)	//绘制空白方块
			{		
				CBrush brushMap;
				brushMap.CreateSolidBrush(RGB(120, 130, 130));
				pDC->SelectObject(brushMap);
				pDC->Rectangle(iCol*BOX_SIZE, (iRow - 4)*BOX_SIZE, (iCol + 1)*BOX_SIZE, (iRow + 1 - 4)*BOX_SIZE);
			}
			if (m_boxMap[iRow][iCol] == true)	//绘制下落方块、堆积方块
			{	
				CBrush brushBox;
				brushBox.CreateSolidBrush(RGB(30, 35, 40));
				pDC->SelectObject(brushBox);
				pDC->Rectangle(iCol*BOX_SIZE, (iRow - 4)*BOX_SIZE, (iCol + 1)*BOX_SIZE, (iRow + 1 - 4)*BOX_SIZE);
			}
		}
	}
}



// 用于绘制预览框地图方块、下一方块
//
void CRBoxView::DrawWillBox(CDC * pDC)
{
	int iRow, iCol;
	CPen penMap;
	penMap.CreatePen(PS_SOLID, 1, RGB(130, 140, 140));
	pDC->SelectObject(penMap);

	for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
	{
		for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
		{	
			if (m_boxWill[iRow][iCol] == false)		// 绘制预览框空白方块
			{	
				CBrush brushMap;
				brushMap.CreateSolidBrush(RGB(120, 130, 130));
				pDC->SelectObject(brushMap);
				pDC->Rectangle((390 + iCol*BOX_SIZE), (60 + iRow*BOX_SIZE), (390 + (iCol + 1)*BOX_SIZE), (60 + (iRow + 1)*BOX_SIZE));
			}
			if (m_boxWill[iRow][iCol] == true)		// 绘制下一方块
			{	
				CBrush brushBox;
				brushBox.CreateSolidBrush(RGB(30, 35, 40));
				pDC->SelectObject(brushBox);
				pDC->Rectangle((390 + iCol*BOX_SIZE), (60 + iRow*BOX_SIZE), (390 + (iCol + 1)*BOX_SIZE), (60 + (iRow + 1)*BOX_SIZE));
			}
		}
	}
}



// 绘制游戏界面文字提示
//
void CRBoxView::DrawText(CDC * pDC)
{

	//pDC->SetROP2(R2_NOTXORPEN);
	pDC->SetBkMode(TRANSPARENT);	//设置字体背景为透明

	pDC->SetTextColor(RGB(150, 150, 150)); 
	pDC->TextOut(410, 25, _T("下一个方块")); 

	CString strScore;
	strScore.Format(_T("分数：%d 分"), m_iScore);
	pDC->TextOut(405, 220, strScore);

	CString strSpeed;
	strSpeed.Format(_T("速度：%d ms"), m_iSpeed);
	pDC->TextOut(405, 250, strSpeed);

	CString strLevel;
	switch (m_iSpeed/100)
	{
	case 1:
		strLevel.Format(_T("难度：炼狱"));
		break;
	case 2:
		strLevel.Format(_T("难度：困难"));
		break;
	case 3:
		strLevel.Format(_T("难度：一般"));
		break;
	case 4:
		strLevel.Format(_T("难度：简单"));
		break;

	default:
		break;
	}
	pDC->TextOut(405, 280, strLevel);

	//Tips
	pDC->TextOut(390, 335, _T("Tips："));
	pDC->TextOut(400, 370, _T("开始游戏：空格"));
	pDC->TextOut(400, 400, _T("方块变换：↑"));
	pDC->TextOut(400, 430, _T("加速下落：↓"));
	pDC->TextOut(400, 460, _T("向左移动：←"));
	pDC->TextOut(400, 490, _T("向右移动：→"));

}



// 开始游戏
//
void CRBoxView::OnStart()
{
	// TODO:  在此添加命令处理程序代码
	//CRBoxView();

	int iRow, iCol;

	// 初始化地图方块
	for (iRow = 0; iRow < MAX_ROW; iRow++)
	{
		for (iCol = 0; iCol < MAX_COL; iCol++)
		{
			m_boxMap[iRow][iCol] = false;
		}
	}

	// 初始化掉落方块、下一方块
	for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
	{
		for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
		{
			m_boxWill[iRow][iCol] = false;
			m_boxNow[iRow][iCol] = false;
		}
	}
	m_iScore = 0;
	m_iSpeed = 400;
	m_bisEnd = false;
	m_bisGaming = true;	//游戏中

	boxCreate();	//产生两次下一方块，才能赋值第一个下落方块nowBox
	boxCreate();	//因为第一次赋值时，下一方块willBox是空的

	SetTimer(1, m_iSpeed, NULL);
	
}



// 创建方块 
//
void CRBoxView::boxCreate()		//赋值下落方块，产生下一方块
{
	int iRow, iCol;
	
	for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
	{
		for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
		{
			m_boxNow[iRow][iCol] = m_boxWill[iRow][iCol];
			m_boxWill[iRow][iCol] = false;
		}
	}

	srand((unsigned)time(0));
	int iType = rand() % 4;		//四种方块，其中L型有两种

	switch (iType)
	{
	case 0:
		m_boxWill[0][0] = 1;	// ■□□□
		m_boxWill[1][0] = 1;	// ■□□□
		m_boxWill[2][0] = 1;	// ■□□□	
		m_boxWill[3][0] = 1;	// ■□□□
		break;
	case 1:
		m_boxWill[0][0] = 1;	// ■■□□
		m_boxWill[0][1] = 1;	// ■□□□
		m_boxWill[1][0] = 1;	// ■□□□
		m_boxWill[2][0] = 1;	// □□□□
		break;
	case 2:
		m_boxWill[0][0] = 1;	// ■■□□
		m_boxWill[1][1] = 1;	// □■□□
		m_boxWill[0][1] = 1;	// □■□□
		m_boxWill[2][1] = 1;	// □□□□
		break;
	case 3:
		m_boxWill[0][0] = 1;	// ■■□□
		m_boxWill[0][1] = 1;	// ■■□□
		m_boxWill[1][0] = 1;	// □□□□
		m_boxWill[1][1] = 1;	// □□□□
		break;

	default:
		break;
	}

	//重置下落方块出现的位置
	m_potNow.x = 0;
	m_potNow.y = MAX_COL / 2;

}



// 方块移动
//
void CRBoxView::boxMove(int iKey)
{
	bool bisHit = false;//碰撞状态

	switch (iKey)
	{
	case KEY_UP:
		boxHitJudge(m_boxNow, KEY_UP, m_potNow);
		break; 

	case KEY_DOWN:
		bisHit = boxHitJudge(m_boxNow, KEY_DOWN, m_potNow);
		if (bisHit == true)		//若为下边界碰撞
		{	
			boxLineDel();			//判断消行
			boxCreate();		//产生新下落方块和下一方块
			m_bisEnd = gameOver();	//游戏是否结束
		}
		break;

	case KEY_LEFT:
		boxHitJudge(m_boxNow, KEY_LEFT, m_potNow);
		break;

	case KEY_RIGHT:
		boxHitJudge(m_boxNow, KEY_RIGHT, m_potNow);
		break;

	default:
		break;
	}
}



// 定时器
//
void CRBoxView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	boxMove(KEY_DOWN);	//方块自动下落
	
	if (m_bisEnd == true)	//游戏状态判断
	{		
		KillTimer(1);
		CString tips;
		tips.Format(_T("\t游戏结束！！！\n\t你的分数为：%d 分\n\t是否重新开始游戏 ？"), m_iScore);
		MessageBox(tips, _T("Tips"), MB_OKCANCEL);
	}

	InvalidateRect(0, NULL);

	CView::OnTimer(nIDEvent);
}



// 检测碰撞
//
bool CRBoxView::boxHitJudge(bool boxTmp[4][4], int iKey, CPoint & p)
{
	int iRow, iCol;

	//清空下落方块经过的路径
	for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
	{
		for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
		{
			if (boxTmp[iRow][iCol] == true)
			{
				m_boxMap[p.x + iRow][p.y + iCol] = false;	//把原下落方块擦除
			}
		}
	}

	//碰撞检测
	for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
	{
		for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
		{
			if (boxTmp[iRow][iCol] == true)
			{
				switch (iKey)
				{
				case KEY_UP:
					{
						bool bisRoll = boxRoll(boxTmp, p);
						if (bisRoll == true)	//可以旋转
							return false;		//返回未发生碰撞
					}break;

				case KEY_DOWN:
					{
						if ((p.x + iRow + 1) >= MAX_ROW)	//与底边界碰撞
							goto HIT_TRUE;
						if (m_boxMap[p.x + iRow + 1][p.y + iCol] == true)	//与地图上堆积的方块碰撞
							goto HIT_TRUE;
					}break;

				case KEY_LEFT:
					{
						if ((p.y + iCol - 1) < 0)	//与左边界碰撞
							goto HIT_TRUE;
						if (m_boxMap[p.x + iRow][p.y + iCol - 1])	//与地图上堆积的方块碰撞
							goto HIT_TRUE;
					}break;

				case KEY_RIGHT:
					{
						if ((p.y + iCol + 1) >= MAX_COL)	//与右边界碰撞
							goto HIT_TRUE;
						if (m_boxMap[p.x + iRow][p.y + iCol + 1])	//与地图上堆积的方块碰撞
							goto HIT_TRUE;
					}break;

				default:
					break;
				}
			}
		}
	}

	switch (iKey)
	{
	case KEY_UP:
		break;
	case KEY_DOWN:
		p.x++;
		break;
	case KEY_LEFT:
		p.y--;
		break;
	case KEY_RIGHT:
		p.y++;
		break;

	default:
		break;
	}

	//未发生碰撞，下落方块位置变动后更新到地图上
	for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
	{
		for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
		{
			if (boxTmp[iRow][iCol] == true)
			{
				m_boxMap[p.x + iRow][p.y + iCol] = true;
			}
		}
	}
	return false;	//返回未发生碰撞


//发生碰撞，下落方块位置不变的更新到地图上
HIT_TRUE:
	for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
	{
		for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
		{
			if (boxTmp[iRow][iCol] == true)
			{
				m_boxMap[p.x + iRow][p.y + iCol] = true;
			}
		}
	}
	return true;	//返回发生碰撞

}


// 方块旋转
//
bool CRBoxView::boxRoll(bool boxTmp[4][4], CPoint & p)
{

	bool boxPre[MIN_ROW_COL][MIN_ROW_COL];		//暂存存放
	bool boxFin[MIN_ROW_COL][MIN_ROW_COL];		//完成旋转

	int iMinRow = MIN_ROW_COL;
	int iMinCol = MIN_ROW_COL;
	int iRow, iCol;

	bool bisRightOver = false;	//超过右边界
	int iRightOver = 0;		//用于记录超出有边界的列数

	if (p.x >= 4)	//下落方块完全进入地图界面
	{
		for (iRow = 0; iRow < MIN_ROW_COL; iRow++)				//■■□□						□□□□			
		{														//■□□□		逆时针旋转->	□□□□
			for (iCol = 0; iCol < MIN_ROW_COL; iCol++)			//■□□□						■□□□
			{													//□□□□						■■■□
				boxPre[iRow][iCol] = boxTmp[iCol][3 - iRow];	//将方块逆时针旋转90°给暂存数组
				boxFin[iRow][iCol] = false;
			}
		}

		for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
		{
			for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
			{
				if (boxPre[iRow][iCol] == true)
				{
					if (iRow < iMinRow)
						iMinRow = iRow;
					if (iCol < iMinCol)
						iMinCol = iCol;
				}
			}
		}
		for (iRow = iMinRow; iRow < MIN_ROW_COL; iRow++)						//□□□□					■□□□
		{																		//□□□□		移动->		■■■□
			for (iCol = iMinCol; iCol < MIN_ROW_COL; iCol++)					//■□□□					□□□□
			{																	//■■■□					□□□□
				boxFin[iRow - iMinRow][iCol - iMinCol] = boxPre[iRow][iCol];	//将暂存数组中的方块移至左上角 给完成数组
			}
		}

		for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
		{
			for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
			{
				if (boxFin[iRow][iCol] == false)
					continue;
				if ((p.x + iRow) >= MAX_ROW)	//碰撞地图下边界
					return false;
				if (m_boxMap[p.x + iRow][p.y + iCol] == true)	//碰撞到地图上原本存在的方块
					return false;
				if ((p.y + iCol) >= MAX_COL)	//超过右边界
				{
					bisRightOver = true;
					iRightOver = p.y + iCol + 1 - MAX_COL;	//记录超出右边界多少
				}
			}
		}
																				//	右边界
																				//    ||
		if (bisRightOver == true)	//超出右边界的情况的处理					//■■||□□   	  ■□||□□	  ■□□||□
		{																		//■□||□□  ->  ■■||■□  ->  ■■■||□
			p.y -= iRightOver;		//将方块向左移动使其不超出右边界			//■□||□□	  □□||□□	  □□□||□
		}																		//□□||□□	  □□||□□	  □□□||□

		for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
		{
			for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
			{
				boxTmp[iRow][iCol] = boxFin[iRow][iCol];
			}
		}

		return true;	//旋转成功

	}
	else
		return false;
}


// 消行
//
void CRBoxView::boxLineDel()
{
	int iRow, iCol;
	bool bisDel;	//是否可消行

	for (iRow = 0; iRow < MAX_ROW; iRow++)
	{
		bisDel = true;

		for (iCol = 0; iCol < MAX_COL; iCol++)
		{

			if (m_boxMap[iRow][iCol] == false)	//当前行存在空方块就不可消行
			{		
				bisDel = false;			
			}
		}

		if (bisDel == true)		//当前行可消
		{	
			for (int iRowTmp = iRow; iRowTmp > 0; iRowTmp--)
			{
				for (int iColTmp = 0; iColTmp < MAX_COL; iColTmp++)
				{
					//将当前行以上的方块都向下移动一行
					m_boxMap[iRowTmp][iColTmp] = m_boxMap[iRowTmp - 1][iColTmp];	
				}
			}
			for (int iFirst = 0; iFirst < MAX_COL; iFirst++)
			{
				m_boxMap[0][iFirst] = false;		//手动清空第一行
			}

			m_iScore +=100;		//消行记分

			//游戏难度增加
			switch (m_iScore/100)	
			{
			case 10:
				m_iSpeed = 300;
				break;
			case 20:
				m_iSpeed = 200;
				break;
			case 30:
				m_iSpeed = 100;

			default:
				break;
			}

			SetTimer(1, m_iSpeed, NULL);

		}
	}
}


// 游戏结束
//
bool CRBoxView::gameOver()
{
	int iCol;

	//有方块超过界面顶部游戏结束									
	for (iCol = 0; iCol < MAX_COL; iCol++)							
	{							
		if (m_boxMap[2][iCol] == true)
		{									
			return true;
		}
	}
	return false;

}


// 键盘响应函数
//
void CRBoxView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	switch (nChar)
	{
	case VK_UP:
		boxMove(KEY_UP);
		break;

	case VK_DOWN:
		boxMove(KEY_DOWN);
		break;

	case VK_LEFT:
		boxMove(KEY_LEFT);
		break;

	case VK_RIGHT:
		boxMove(KEY_RIGHT);
		break;

	case VK_SPACE:
		OnStart();
		break;

	default:
		break;
	}

	InvalidateRect(0, NULL);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
