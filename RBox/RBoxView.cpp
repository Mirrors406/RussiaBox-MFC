
// RBoxView.cpp : CRBoxView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_START, &CRBoxView::OnStart)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CRBoxView ����/����

CRBoxView::CRBoxView()
{
	// TODO:  �ڴ˴���ӹ������
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
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CRBoxView ����

void CRBoxView::OnDraw(CDC* pDC)
{
	CRBoxDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
	CDC backDC;							//��������������
	backDC.CreateCompatibleDC(pDC);	

	//���ý��汳��
	CBitmap backbit;
	backbit.LoadBitmap(IDB_BACK);
	backDC.SelectObject(backbit);

	//���Ƶ�ͼ����(�����˿հ׷��顢���䷽�顢�ѻ�����)
	DrawMap(&backDC);

	//������һ����
	DrawWillBox(&backDC);

	//���ƽ���������ʾ
	DrawText(&backDC);

	pDC->StretchBlt(0, 0, 600, 600, &backDC, 0, 0, 600, 600, SRCCOPY);

}


// CRBoxView ��ӡ

BOOL CRBoxView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CRBoxView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CRBoxView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CRBoxView ���

#ifdef _DEBUG
void CRBoxView::AssertValid() const
{
	CView::AssertValid();
}

void CRBoxView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRBoxDoc* CRBoxView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRBoxDoc)));
	return (CRBoxDoc*)m_pDocument;
}
#endif //_DEBUG


// CRBoxView ��Ϣ�������



// ���ڻ��Ƶ�ͼ���飨�հ׷��顢���䷽�顢�ѻ����飩
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
			if (m_boxMap[iRow][iCol] == false)	//���ƿհ׷���
			{		
				CBrush brushMap;
				brushMap.CreateSolidBrush(RGB(120, 130, 130));
				pDC->SelectObject(brushMap);
				pDC->Rectangle(iCol*BOX_SIZE, (iRow - 4)*BOX_SIZE, (iCol + 1)*BOX_SIZE, (iRow + 1 - 4)*BOX_SIZE);
			}
			if (m_boxMap[iRow][iCol] == true)	//�������䷽�顢�ѻ�����
			{	
				CBrush brushBox;
				brushBox.CreateSolidBrush(RGB(30, 35, 40));
				pDC->SelectObject(brushBox);
				pDC->Rectangle(iCol*BOX_SIZE, (iRow - 4)*BOX_SIZE, (iCol + 1)*BOX_SIZE, (iRow + 1 - 4)*BOX_SIZE);
			}
		}
	}
}



// ���ڻ���Ԥ�����ͼ���顢��һ����
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
			if (m_boxWill[iRow][iCol] == false)		// ����Ԥ����հ׷���
			{	
				CBrush brushMap;
				brushMap.CreateSolidBrush(RGB(120, 130, 130));
				pDC->SelectObject(brushMap);
				pDC->Rectangle((390 + iCol*BOX_SIZE), (60 + iRow*BOX_SIZE), (390 + (iCol + 1)*BOX_SIZE), (60 + (iRow + 1)*BOX_SIZE));
			}
			if (m_boxWill[iRow][iCol] == true)		// ������һ����
			{	
				CBrush brushBox;
				brushBox.CreateSolidBrush(RGB(30, 35, 40));
				pDC->SelectObject(brushBox);
				pDC->Rectangle((390 + iCol*BOX_SIZE), (60 + iRow*BOX_SIZE), (390 + (iCol + 1)*BOX_SIZE), (60 + (iRow + 1)*BOX_SIZE));
			}
		}
	}
}



// ������Ϸ����������ʾ
//
void CRBoxView::DrawText(CDC * pDC)
{

	//pDC->SetROP2(R2_NOTXORPEN);
	pDC->SetBkMode(TRANSPARENT);	//�������屳��Ϊ͸��

	pDC->SetTextColor(RGB(150, 150, 150)); 
	pDC->TextOut(410, 25, _T("��һ������")); 

	CString strScore;
	strScore.Format(_T("������%d ��"), m_iScore);
	pDC->TextOut(405, 220, strScore);

	CString strSpeed;
	strSpeed.Format(_T("�ٶȣ�%d ms"), m_iSpeed);
	pDC->TextOut(405, 250, strSpeed);

	CString strLevel;
	switch (m_iSpeed/100)
	{
	case 1:
		strLevel.Format(_T("�Ѷȣ�����"));
		break;
	case 2:
		strLevel.Format(_T("�Ѷȣ�����"));
		break;
	case 3:
		strLevel.Format(_T("�Ѷȣ�һ��"));
		break;
	case 4:
		strLevel.Format(_T("�Ѷȣ���"));
		break;

	default:
		break;
	}
	pDC->TextOut(405, 280, strLevel);

	//Tips
	pDC->TextOut(390, 335, _T("Tips��"));
	pDC->TextOut(400, 370, _T("��ʼ��Ϸ���ո�"));
	pDC->TextOut(400, 400, _T("����任����"));
	pDC->TextOut(400, 430, _T("�������䣺��"));
	pDC->TextOut(400, 460, _T("�����ƶ�����"));
	pDC->TextOut(400, 490, _T("�����ƶ�����"));

}



// ��ʼ��Ϸ
//
void CRBoxView::OnStart()
{
	// TODO:  �ڴ���������������
	//CRBoxView();

	int iRow, iCol;

	// ��ʼ����ͼ����
	for (iRow = 0; iRow < MAX_ROW; iRow++)
	{
		for (iCol = 0; iCol < MAX_COL; iCol++)
		{
			m_boxMap[iRow][iCol] = false;
		}
	}

	// ��ʼ�����䷽�顢��һ����
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
	m_bisGaming = true;	//��Ϸ��

	boxCreate();	//����������һ���飬���ܸ�ֵ��һ�����䷽��nowBox
	boxCreate();	//��Ϊ��һ�θ�ֵʱ����һ����willBox�ǿյ�

	SetTimer(1, m_iSpeed, NULL);
	
}



// �������� 
//
void CRBoxView::boxCreate()		//��ֵ���䷽�飬������һ����
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
	int iType = rand() % 4;		//���ַ��飬����L��������

	switch (iType)
	{
	case 0:
		m_boxWill[0][0] = 1;	// ��������
		m_boxWill[1][0] = 1;	// ��������
		m_boxWill[2][0] = 1;	// ��������	
		m_boxWill[3][0] = 1;	// ��������
		break;
	case 1:
		m_boxWill[0][0] = 1;	// ��������
		m_boxWill[0][1] = 1;	// ��������
		m_boxWill[1][0] = 1;	// ��������
		m_boxWill[2][0] = 1;	// ��������
		break;
	case 2:
		m_boxWill[0][0] = 1;	// ��������
		m_boxWill[1][1] = 1;	// ��������
		m_boxWill[0][1] = 1;	// ��������
		m_boxWill[2][1] = 1;	// ��������
		break;
	case 3:
		m_boxWill[0][0] = 1;	// ��������
		m_boxWill[0][1] = 1;	// ��������
		m_boxWill[1][0] = 1;	// ��������
		m_boxWill[1][1] = 1;	// ��������
		break;

	default:
		break;
	}

	//�������䷽����ֵ�λ��
	m_potNow.x = 0;
	m_potNow.y = MAX_COL / 2;

}



// �����ƶ�
//
void CRBoxView::boxMove(int iKey)
{
	bool bisHit = false;//��ײ״̬

	switch (iKey)
	{
	case KEY_UP:
		boxHitJudge(m_boxNow, KEY_UP, m_potNow);
		break; 

	case KEY_DOWN:
		bisHit = boxHitJudge(m_boxNow, KEY_DOWN, m_potNow);
		if (bisHit == true)		//��Ϊ�±߽���ײ
		{	
			boxLineDel();			//�ж�����
			boxCreate();		//���������䷽�����һ����
			m_bisEnd = gameOver();	//��Ϸ�Ƿ����
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



// ��ʱ��
//
void CRBoxView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	boxMove(KEY_DOWN);	//�����Զ�����
	
	if (m_bisEnd == true)	//��Ϸ״̬�ж�
	{		
		KillTimer(1);
		CString tips;
		tips.Format(_T("\t��Ϸ����������\n\t��ķ���Ϊ��%d ��\n\t�Ƿ����¿�ʼ��Ϸ ��"), m_iScore);
		MessageBox(tips, _T("Tips"), MB_OKCANCEL);
	}

	InvalidateRect(0, NULL);

	CView::OnTimer(nIDEvent);
}



// �����ײ
//
bool CRBoxView::boxHitJudge(bool boxTmp[4][4], int iKey, CPoint & p)
{
	int iRow, iCol;

	//������䷽�龭����·��
	for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
	{
		for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
		{
			if (boxTmp[iRow][iCol] == true)
			{
				m_boxMap[p.x + iRow][p.y + iCol] = false;	//��ԭ���䷽�����
			}
		}
	}

	//��ײ���
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
						if (bisRoll == true)	//������ת
							return false;		//����δ������ײ
					}break;

				case KEY_DOWN:
					{
						if ((p.x + iRow + 1) >= MAX_ROW)	//��ױ߽���ײ
							goto HIT_TRUE;
						if (m_boxMap[p.x + iRow + 1][p.y + iCol] == true)	//���ͼ�϶ѻ��ķ�����ײ
							goto HIT_TRUE;
					}break;

				case KEY_LEFT:
					{
						if ((p.y + iCol - 1) < 0)	//����߽���ײ
							goto HIT_TRUE;
						if (m_boxMap[p.x + iRow][p.y + iCol - 1])	//���ͼ�϶ѻ��ķ�����ײ
							goto HIT_TRUE;
					}break;

				case KEY_RIGHT:
					{
						if ((p.y + iCol + 1) >= MAX_COL)	//���ұ߽���ײ
							goto HIT_TRUE;
						if (m_boxMap[p.x + iRow][p.y + iCol + 1])	//���ͼ�϶ѻ��ķ�����ײ
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

	//δ������ײ�����䷽��λ�ñ䶯����µ���ͼ��
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
	return false;	//����δ������ײ


//������ײ�����䷽��λ�ò���ĸ��µ���ͼ��
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
	return true;	//���ط�����ײ

}


// ������ת
//
bool CRBoxView::boxRoll(bool boxTmp[4][4], CPoint & p)
{

	bool boxPre[MIN_ROW_COL][MIN_ROW_COL];		//�ݴ���
	bool boxFin[MIN_ROW_COL][MIN_ROW_COL];		//�����ת

	int iMinRow = MIN_ROW_COL;
	int iMinCol = MIN_ROW_COL;
	int iRow, iCol;

	bool bisRightOver = false;	//�����ұ߽�
	int iRightOver = 0;		//���ڼ�¼�����б߽������

	if (p.x >= 4)	//���䷽����ȫ�����ͼ����
	{
		for (iRow = 0; iRow < MIN_ROW_COL; iRow++)				//��������						��������			
		{														//��������		��ʱ����ת->	��������
			for (iCol = 0; iCol < MIN_ROW_COL; iCol++)			//��������						��������
			{													//��������						��������
				boxPre[iRow][iCol] = boxTmp[iCol][3 - iRow];	//��������ʱ����ת90����ݴ�����
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
		for (iRow = iMinRow; iRow < MIN_ROW_COL; iRow++)						//��������					��������
		{																		//��������		�ƶ�->		��������
			for (iCol = iMinCol; iCol < MIN_ROW_COL; iCol++)					//��������					��������
			{																	//��������					��������
				boxFin[iRow - iMinRow][iCol - iMinCol] = boxPre[iRow][iCol];	//���ݴ������еķ����������Ͻ� ���������
			}
		}

		for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
		{
			for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
			{
				if (boxFin[iRow][iCol] == false)
					continue;
				if ((p.x + iRow) >= MAX_ROW)	//��ײ��ͼ�±߽�
					return false;
				if (m_boxMap[p.x + iRow][p.y + iCol] == true)	//��ײ����ͼ��ԭ�����ڵķ���
					return false;
				if ((p.y + iCol) >= MAX_COL)	//�����ұ߽�
				{
					bisRightOver = true;
					iRightOver = p.y + iCol + 1 - MAX_COL;	//��¼�����ұ߽����
				}
			}
		}
																				//	�ұ߽�
																				//    ||
		if (bisRightOver == true)	//�����ұ߽������Ĵ���					//����||����   	  ����||����	  ������||��
		{																		//����||����  ->  ����||����  ->  ������||��
			p.y -= iRightOver;		//�����������ƶ�ʹ�䲻�����ұ߽�			//����||����	  ����||����	  ������||��
		}																		//����||����	  ����||����	  ������||��

		for (iRow = 0; iRow < MIN_ROW_COL; iRow++)
		{
			for (iCol = 0; iCol < MIN_ROW_COL; iCol++)
			{
				boxTmp[iRow][iCol] = boxFin[iRow][iCol];
			}
		}

		return true;	//��ת�ɹ�

	}
	else
		return false;
}


// ����
//
void CRBoxView::boxLineDel()
{
	int iRow, iCol;
	bool bisDel;	//�Ƿ������

	for (iRow = 0; iRow < MAX_ROW; iRow++)
	{
		bisDel = true;

		for (iCol = 0; iCol < MAX_COL; iCol++)
		{

			if (m_boxMap[iRow][iCol] == false)	//��ǰ�д��ڿշ���Ͳ�������
			{		
				bisDel = false;			
			}
		}

		if (bisDel == true)		//��ǰ�п���
		{	
			for (int iRowTmp = iRow; iRowTmp > 0; iRowTmp--)
			{
				for (int iColTmp = 0; iColTmp < MAX_COL; iColTmp++)
				{
					//����ǰ�����ϵķ��鶼�����ƶ�һ��
					m_boxMap[iRowTmp][iColTmp] = m_boxMap[iRowTmp - 1][iColTmp];	
				}
			}
			for (int iFirst = 0; iFirst < MAX_COL; iFirst++)
			{
				m_boxMap[0][iFirst] = false;		//�ֶ���յ�һ��
			}

			m_iScore +=100;		//���мǷ�

			//��Ϸ�Ѷ�����
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


// ��Ϸ����
//
bool CRBoxView::gameOver()
{
	int iCol;

	//�з��鳬�����涥����Ϸ����									
	for (iCol = 0; iCol < MAX_COL; iCol++)							
	{							
		if (m_boxMap[2][iCol] == true)
		{									
			return true;
		}
	}
	return false;

}


// ������Ӧ����
//
void CRBoxView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
