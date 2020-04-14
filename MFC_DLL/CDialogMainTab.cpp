// CDialogMainTab.cpp: 实现文件
//
#include "stdafx.h"
#include "pch.h"
#include "MFC_DLL.h"
#include "CDialogMainTab.h"
#include "afxdialogex.h"
#include "global_Var.h"
#include "CAutoPlay.h"
#include "HookGameMainThread.h"

// CDialogMainTab 对话框

IMPLEMENT_DYNAMIC(CDialog_MainTab, CDialogEx)

CDialog_MainTab::CDialog_MainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MAINTAB, pParent)
{

}

CDialog_MainTab::~CDialog_MainTab()
{
}

void CDialog_MainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_ctl_TabMain);
	DDX_Control(pDX, IDC_BUTTON_STARTAUTOPLAY, m_ctl_BtnStartOrStop);
}


BEGIN_MESSAGE_MAP(CDialog_MainTab, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CDialog_MainTab::OnTcnSelchangeTabMain)
	ON_BN_CLICKED(IDC_BUTTON_APPLYSET, &CDialog_MainTab::OnBnClickedButtonApplyset)
	ON_BN_CLICKED(IDC_BUTTON_STARTAUTOPLAY, &CDialog_MainTab::OnBnClickedButtonStartautoplay)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_MainTab::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON8, &CDialog_MainTab::OnBnClickedButton8)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogMainTab 消息处理程序

#define ConfigFilePath "C:\\RXJHconfig.ini"
BOOL CDialog_MainTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//获取窗口客户区的位置到tabRect
	RECT tabRect;
	m_ctl_TabMain.GetClientRect(&tabRect);
	//调整tabRect范围,使其适合选项卡页窗口
	tabRect.top += 25;
	tabRect.left += 5;
	tabRect.bottom -= 5;
	tabRect.right -= 5;

	//创建窗口,利用构造函数初始化控件值
	m_ctl_TabMain.InsertItem(0, "挂机");
	m_Page_GuaJi.Create(IDD_PAGE_GUAJI);
	m_Page_GuaJi.SetParent(&m_ctl_TabMain);
	m_Page_GuaJi.ShowWindow(SW_SHOW);
	m_Page_GuaJi.MoveWindow(&tabRect);

	m_ctl_TabMain.InsertItem(1, "买卖");
	m_Page_BuySell.Create(IDD_PAGE_BUYSELL);
	m_Page_BuySell.SetParent(&m_ctl_TabMain);
	m_Page_BuySell.ShowWindow(SW_HIDE);
	m_Page_BuySell.MoveWindow(&tabRect);
	m_Page_BuySell.m_Page_GoodsManage.SetParent(&m_ctl_TabMain);
	m_Page_BuySell.m_Page_GoodsManage.ShowWindow(SW_HIDE);
	m_Page_BuySell.m_Page_GoodsManage.MoveWindow(&tabRect);

	//再读取用户配置,自动应用设置
	ReadConfigFromFile(ConfigFilePath);
	OnBnClickedButtonApplyset();

	//创建时钟
	SetTimer(ID_TIMER_QingGong, 3000, NULL);	//轻功时钟
	if (g_cAutoPlay.dwCheckHpMpSpeed == NULL) g_cAutoPlay.dwCheckHpMpSpeed = 1000;	//HP,MP保护时钟
	else if(g_cAutoPlay.dwCheckHpMpSpeed < 200)	g_cAutoPlay.dwCheckHpMpSpeed = 200;
	else if(g_cAutoPlay.dwCheckHpMpSpeed > 2000) g_cAutoPlay.dwCheckHpMpSpeed = 2000;
	SetTimer(ID_TIMER_HMProtect, g_cAutoPlay.dwCheckHpMpSpeed, NULL);
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



/********************************************************************************************/
/* Description	: 切换选项卡
/* Return		: 成功返回真，失败返回假
/* Time			: 2020/01/23 15:00
/* Remark		: 
/********************************************************************************************/
void CDialog_MainTab::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_ctl_TabMain.GetCurSel())
	{
	case 0:
		m_Page_GuaJi.ShowWindow(SW_SHOW);
		m_Page_BuySell.ShowWindow(SW_HIDE);
		m_Page_BuySell.m_Page_GoodsManage.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_Page_GuaJi.ShowWindow(SW_HIDE);
		m_Page_BuySell.ShowWindow(SW_SHOW);
		m_Page_BuySell.m_Page_GoodsManage.ShowWindow(SW_HIDE);
		break;
	}
	*pResult = 0;
}

/********************************************************************************************/
/* Description	: 点击按钮“应用设置”
/* Return		: 
/* Time			: 2020/01/23 14:59
/* Remark		: 应用设置到挂机类成员变量
/********************************************************************************************/
void CDialog_MainTab::OnBnClickedButtonApplyset()
{
	m_Page_GuaJi.UpdateData(TRUE);	//控件 -> 变量
	m_Page_BuySell.UpdateData(TRUE);//控件 -> 变量
	//先清空容器
	g_cAutoPlay.vct_SupplyList.clear();
	g_cAutoPlay.vct_GoodsMngList.clear();

	//-----------"挂机"选项卡---------//
	//打怪相关
	g_cAutoPlay.IsAutoBeatMonster = m_Page_GuaJi.m_chk_IsAutoBeatMonster;
	g_cAutoPlay.IsRangeLimit = m_Page_GuaJi.m_chk_IsRangeLimit;
	g_cAutoPlay.RangeOfHitMonster = m_Page_GuaJi.m_edt_hitMonsterRange;
	g_cAutoPlay.GuaJiPosX = m_Page_GuaJi.m_edt_LimitPosX;
	g_cAutoPlay.GuaJiPosY = m_Page_GuaJi.m_edt_LimitPosY;
	g_cAutoPlay.IsUseSkill = m_Page_GuaJi.m_chk_IsUseSkill;
	g_cAutoPlay.IsPickupGoods = m_Page_GuaJi.m_chk_IsPickupGoods;
	strcpy_s(g_cAutoPlay.szSkillName, m_Page_GuaJi.m_cmb_szSkillName);
	msg_DropSkillToF1F10(g_cAutoPlay.szSkillName);	//点击完"应用设置"就把技能放到快捷栏
	
	//保护相关
	g_cAutoPlay.IsAutoUseHpGoods = m_Page_GuaJi.m_chk_IsAutoUseHpGoods;
	g_cAutoPlay.IsAutoUseMpGoods = m_Page_GuaJi.m_chk_IsAutoUseMpGoods;
	g_cAutoPlay.dwHpPercent = m_Page_GuaJi.m_edt_dwHpPercent;
	g_cAutoPlay.dwMpPercent = m_Page_GuaJi.m_edt_dwMpPercent;
	g_cAutoPlay.dwCheckHpMpSpeed = m_Page_GuaJi.m_edt_CheckHpMpSpeed;
	strcpy_s(g_cAutoPlay.szHpGoodsName, m_Page_GuaJi.m_cmb_szHpGoodsName);
	strcpy_s(g_cAutoPlay.szMpGoodsName, m_Page_GuaJi.m_cmb_szMpGoodsName);

	//轻功相关
	g_cAutoPlay.IsAutoUseQingGong = m_Page_GuaJi.m_chk_IsAutoUseQingGong;
	strcpy_s(g_cAutoPlay.szQingGongName, m_Page_GuaJi.m_cmb_szQingGongName);

	//画面相关
	HookRefreshScreen(m_Page_GuaJi.m_chk_IsDisableRefresh);

	//-----------"买卖"选项卡---------//
	//药品相关
	g_cAutoPlay.IsAutoBuyHpGoods = m_Page_BuySell.m_chk_IsAutoBuyHpGoods;
	g_cAutoPlay.IsAutoBuyMpGoods = m_Page_BuySell.m_chk_IsAutoBuyMpGoods;
	g_cAutoPlay.dwMinNum_HpGoods = m_Page_BuySell.m_edt_dwMinNum_HpGoods;
	g_cAutoPlay.dwMaxNum_HpGoods = m_Page_BuySell.m_edt_dwMaxNum_HpGoods;
	g_cAutoPlay.dwMinNum_MpGoods = m_Page_BuySell.m_edt_dwMinNum_MpGoods;
	g_cAutoPlay.dwMaxNum_MpGoods = m_Page_BuySell.m_edt_dwMaxNum_MpGoods;

	char szLineText[80];
	int i = 0, j = 0;
	//补给物品列表
	TSupplyData tBuyData;
	for (i=0;i < m_Page_BuySell.m_lst_ctl_SupplyGoods.GetCount();i++)
	{
		//逐行获取列表框文本
		m_Page_BuySell.m_lst_ctl_SupplyGoods.GetText(i, szLineText);

		//拆分出"去哪买"
		strcpy_s(tBuyData.szAddr, szLineText);
		ReplaceChar(tBuyData.szAddr, ',', '\0');

		//拆分出"买什么"
		strcpy_s(tBuyData.szGoodsName, szLineText + 9);	// "购买药符," 共9字节
		ReplaceChar(tBuyData.szGoodsName, ':', '\0');

		//拆分出"买多少"
		char strTmp[30];	//接收购买数量的临时字符串
		for (j = 0; j < 60; j++)	if (szLineText[j] == ':')	break;
		strcpy_s(strTmp, szLineText + j + 1);
		tBuyData.dwNum = atoi(strTmp);

		//加入到vct_SupplyList
		g_cAutoPlay.vct_SupplyList.push_back(tBuyData);
	}
	//处理物品列表
	CListCtrl* pLstC = &(m_Page_BuySell.m_Page_GoodsManage.m_lstC_GoodsTable);	//为避免每次都要写这么长,故设置一个指针
	TGoodsManage tGoodsMng;
	for (i=0;i<pLstC->GetItemCount();i++)
	{
		//获取物品名
		pLstC->GetItemText(i, 0, szLineText, sizeof(szLineText));	//第i行第0列
		strcpy_s(tGoodsMng.szGoodsName, szLineText);
		tGoodsMng.dwGoodsManageFlag = Goods_Manage_No;	//默认为不处理

		//获取存仓库状态
		pLstC->GetItemText(i, 1, szLineText, sizeof(szLineText));
		if (strcmp(szLineText,"√") == 0)	tGoodsMng.dwGoodsManageFlag |= Goods_Manage_Depot;

		//获取卖商店状态
		pLstC->GetItemText(i, 2, szLineText, sizeof(szLineText));
		if (strcmp(szLineText, "√") == 0)	tGoodsMng.dwGoodsManageFlag |= Goods_Manage_Shop;

		//获取不拾取状态
		pLstC->GetItemText(i, 3, szLineText, sizeof(szLineText));
		if (strcmp(szLineText, "√") == 0)	tGoodsMng.dwGoodsManageFlag |= Goods_Manage_NoPick;

		//加入到vct_SupplyList
		g_cAutoPlay.vct_GoodsMngList.push_back(tGoodsMng);
	}
}


/********************************************************************************************/
/* Description	: 点击按钮“开始挂机”
/* Return		: 
/* Time			: 2020/01/23 15:54
/* Remark		: 
/********************************************************************************************/
void CDialog_MainTab::OnBnClickedButtonStartautoplay()
{
	CString str;
	m_ctl_BtnStartOrStop.GetWindowTextA(str);
	if (str == "开始挂机")
	{
		m_ctl_BtnStartOrStop.SetWindowTextA("停止挂机");
		g_cAutoPlay.StartAutoPlay();
	}
	else
	{
		m_ctl_BtnStartOrStop.SetWindowTextA("开始挂机");
		g_cAutoPlay.StopAutoPlay();
	}
	
}

/********************************************************************************************/
/* Description	: 点击按钮"保存配置"
/* Return		: 
/* Time			: 2020/02/21 16:04
/* Remark		: 
/********************************************************************************************/
void CDialog_MainTab::OnBnClickedButton1()
{
	SaveConfigToFile(ConfigFilePath);
}

/********************************************************************************************/
/* Description	: 点击按钮"载入配置"
/* Return		: 
/* Time			: 2020/02/25 10:01
/* Remark		: 
/********************************************************************************************/
void CDialog_MainTab::OnBnClickedButton8()
{
	ReadConfigFromFile(ConfigFilePath);
}

/********************************************************************************************/
/* Description	: 保存所有页面设置到文件
/* Return		: 
/* Time			: 2020/02/24 20:18
/* Remark		: 
/********************************************************************************************/
void CDialog_MainTab::SaveConfigToFile(char* szConfigFile)
{
	//更新控件变量
	m_Page_GuaJi.UpdateData(TRUE);	//控件 -> 变量
	m_Page_BuySell.UpdateData(TRUE);	//控件 -> 变量

	CString strCfg, strTmp;

	//-----------"挂机"选项卡---------//
	strCfg += "[挂机]\n";
	//打怪相关
	strTmp.Format("m_Page_GuaJi.m_chk_IsAutoBeatMonster=%d\n", m_Page_GuaJi.m_chk_IsAutoBeatMonster);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_chk_IsRangeLimit=%d\n", m_Page_GuaJi.m_chk_IsRangeLimit);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_edt_hitMonsterRange=%d\n", m_Page_GuaJi.m_edt_hitMonsterRange);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_edt_LimitPosX=%d\n", m_Page_GuaJi.m_edt_LimitPosX);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_edt_LimitPosY=%d\n", m_Page_GuaJi.m_edt_LimitPosY);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_chk_IsUseSkill=%d\n", m_Page_GuaJi.m_chk_IsUseSkill);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_chk_IsPickupGoods=%d\n", m_Page_GuaJi.m_chk_IsPickupGoods);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_cmb_szSkillName=%s\n", m_Page_GuaJi.m_cmb_szSkillName);
	strCfg += strTmp;
	//保护相关
	strTmp.Format("m_Page_GuaJi.m_chk_IsAutoUseHpGoods=%d\n", m_Page_GuaJi.m_chk_IsAutoUseHpGoods);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_chk_IsAutoUseMpGoods=%d\n", m_Page_GuaJi.m_chk_IsAutoUseMpGoods);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_edt_dwHpPercent=%d\n", m_Page_GuaJi.m_edt_dwHpPercent);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_edt_dwMpPercent=%d\n", m_Page_GuaJi.m_edt_dwMpPercent);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_cmb_szHpGoodsName=%s\n", m_Page_GuaJi.m_cmb_szHpGoodsName);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_cmb_szMpGoodsName=%s\n", m_Page_GuaJi.m_cmb_szMpGoodsName);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_edt_CheckHpMpSpeed=%d\n", m_Page_GuaJi.m_edt_CheckHpMpSpeed);
	strCfg += strTmp;
	//轻功相关
	strTmp.Format("m_Page_GuaJi.m_chk_IsAutoUseQingGong=%d\n", m_Page_GuaJi.m_chk_IsAutoUseQingGong);
	strCfg += strTmp;
	strTmp.Format("m_Page_GuaJi.m_cmb_szQingGongName=%s\n", m_Page_GuaJi.m_cmb_szQingGongName);
	strCfg += strTmp;
	//画面相关
	strTmp.Format("m_Page_GuaJi.m_chk_IsDisableRefresh=%d\n", m_Page_GuaJi.m_chk_IsDisableRefresh);
	strCfg += strTmp;
	//-----------"买卖"选项卡---------//
	strCfg += "[买卖]\n";
	//药品相关
	strTmp.Format("m_Page_BuySell.m_chk_IsAutoBuyHpGoods=%d\n", m_Page_BuySell.m_chk_IsAutoBuyHpGoods);
	strCfg += strTmp;
	strTmp.Format("m_Page_BuySell.m_chk_IsAutoBuyMpGoods=%d\n", m_Page_BuySell.m_chk_IsAutoBuyMpGoods);
	strCfg += strTmp;
	strTmp.Format("m_Page_BuySell.m_edt_dwMinNum_HpGoods=%d\n", m_Page_BuySell.m_edt_dwMinNum_HpGoods);
	strCfg += strTmp;
	strTmp.Format("m_Page_BuySell.m_edt_dwMaxNum_HpGoods=%d\n", m_Page_BuySell.m_edt_dwMaxNum_HpGoods);
	strCfg += strTmp;
	strTmp.Format("m_Page_BuySell.m_edt_dwMinNum_MpGoods=%d\n", m_Page_BuySell.m_edt_dwMinNum_MpGoods);
	strCfg += strTmp;
	strTmp.Format("m_Page_BuySell.m_edt_dwMaxNum_MpGoods=%d\n", m_Page_BuySell.m_edt_dwMaxNum_MpGoods);
	strCfg += strTmp;
	//补给列表框
	strCfg += "m_Page_BuySell.m_lst_ctl_SupplyGoods{\n";
	for (int i=0; i<m_Page_BuySell.m_lst_ctl_SupplyGoods.GetCount(); i++)
	{
		m_Page_BuySell.m_lst_ctl_SupplyGoods.GetText(i, strTmp);
		strCfg += strTmp + "\n";
	}
	strCfg += "m_Page_BuySell.m_lst_ctl_SupplyGoods}\n\n";
	//-----------"买卖"选项卡(物品管理页)---------//
	//物品列表控件
	strCfg += "m_Page_BuySell.m_Page_GoodsManage.m_lstC_GoodsTable{\n";
	CListCtrl* pListC = &m_Page_BuySell.m_Page_GoodsManage.m_lstC_GoodsTable;
	for (int y=0; y < pListC->GetItemCount(); y++)
	{	//行操作
		strTmp = pListC->GetItemText(y, 0)+"|";
		for (int x=1; x<=3; x++)
		{	//列操作
			if (pListC->GetItemText(y, x).IsEmpty())	strTmp += "0|";
			else    strTmp += "1|";
		}
		strTmp += "\n";	//处理完一行数据添加换行
		strCfg += strTmp;
	}
	strCfg += "m_Page_BuySell.m_Page_GoodsManage.m_lstC_GoodsTable}\n";

	//写入到指定目录文件
	FILE* pFile;
	fopen_s(&pFile, szConfigFile, "w");
	fputs(strCfg,pFile);
	fclose(pFile);
}

/********************************************************************************************/
/* Description	: 从文件读取所有页面设置
/* Return		: 
/* Time			: 2020/02/24 20:57
/* Remark		: 
/********************************************************************************************/
void CDialog_MainTab::ReadConfigFromFile(char* szConfigFile)
{
	char szLineData[200];	//接收每行数据的缓冲区
	fstream fs;
	fs.open(szConfigFile, ios::in);	//in读取文件,out写入文件
	if (fs.is_open() == FALSE)	return;	//判断配置文件是否存在
	while (TRUE)
	{
		fs.getline(szLineData, sizeof(szLineData));
		if (fs.eof()) break;
		if(szLineData == NULL) continue;
		ApplyConfigToCtrl(&fs,szLineData);
	}
	m_Page_GuaJi.UpdateData(FALSE);
	m_Page_BuySell.UpdateData(FALSE);
}

/********************************************************************************************/
/* Description	: 从一行数据中读取出控件变量名与值
/* Return		: 
/* Time			: 2020/02/24 22:06
/* Remark		: 
/********************************************************************************************/
void CDialog_MainTab::ApplyConfigToCtrl(fstream* fs, char* szLineData)
{
	//令 szCtrl_Name 和 szCtrl_Val 这两个指针都指向 szLineData
	char* szCtrl_Name = szLineData;
	char* szCtrl_Val = szLineData;

	szCtrl_Val = strchr(szLineData, '=');	//获取从=开始的字符串
	if (szCtrl_Val != NULL)
	{
		szCtrl_Val[0] = '\0';	//把=替换为字符串结束符,分割字符串
		szCtrl_Val += 1;	//获取从'\0'后一个开始的字符串指针
	}
	
	//-----------"挂机"选项卡---------//
	//打怪相关
	if (strcmp(szCtrl_Name,"m_Page_GuaJi.m_chk_IsAutoBeatMonster") == 0)
	{
		m_Page_GuaJi.m_chk_IsAutoBeatMonster = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_chk_IsRangeLimit") == 0)
	{
		m_Page_GuaJi.m_chk_IsRangeLimit = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_edt_hitMonsterRange") == 0)
	{
		m_Page_GuaJi.m_edt_hitMonsterRange = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_edt_LimitPosX") == 0)
	{
		m_Page_GuaJi.m_edt_LimitPosX = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_edt_LimitPosY") == 0)
	{
		m_Page_GuaJi.m_edt_LimitPosY = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_chk_IsUseSkill") == 0)
	{
		m_Page_GuaJi.m_chk_IsUseSkill = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_chk_IsPickupGoods") == 0)
	{
		m_Page_GuaJi.m_chk_IsPickupGoods = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_chk_IsAutoUseQingGong") == 0)
	{
		m_Page_GuaJi.m_chk_IsAutoUseQingGong = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_chk_IsDisableRefresh") == 0)
	{
		m_Page_GuaJi.m_chk_IsDisableRefresh = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_cmb_szSkillName") == 0)
	{
		m_Page_GuaJi.m_cmb_szSkillName = szCtrl_Val;		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_cmb_szQingGongName") == 0)
	{
		m_Page_GuaJi.m_cmb_szQingGongName = szCtrl_Val;		return;
	}
	//保护相关
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_chk_IsAutoUseHpGoods") == 0)
	{
		m_Page_GuaJi.m_chk_IsAutoUseHpGoods = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_chk_IsAutoUseMpGoods") == 0)
	{
		m_Page_GuaJi.m_chk_IsAutoUseMpGoods = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_edt_dwHpPercent") == 0)
	{
		m_Page_GuaJi.m_edt_dwHpPercent = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_edt_dwMpPercent") == 0)
	{
		m_Page_GuaJi.m_edt_dwMpPercent = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_cmb_szHpGoodsName") == 0)
	{
		m_Page_GuaJi.m_cmb_szHpGoodsName = szCtrl_Val;		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_cmb_szMpGoodsName") == 0)
	{
		m_Page_GuaJi.m_cmb_szMpGoodsName = szCtrl_Val;		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_GuaJi.m_edt_CheckHpMpSpeed") == 0)
	{
		m_Page_GuaJi.m_edt_CheckHpMpSpeed = atoi(szCtrl_Val);		return;
	}

	//-----------"买卖"选项卡---------//
	//药品相关
	if (strcmp(szCtrl_Name, "m_Page_BuySell.m_chk_IsAutoBuyHpGoods") == 0)
	{
		m_Page_BuySell.m_chk_IsAutoBuyHpGoods = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_BuySell.m_chk_IsAutoBuyMpGoods") == 0)
	{
		m_Page_BuySell.m_chk_IsAutoBuyMpGoods = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_BuySell.m_edt_dwMinNum_HpGoods") == 0)
	{
		m_Page_BuySell.m_edt_dwMinNum_HpGoods = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_BuySell.m_edt_dwMaxNum_HpGoods") == 0)
	{
		m_Page_BuySell.m_edt_dwMaxNum_HpGoods = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_BuySell.m_edt_dwMinNum_MpGoods") == 0)
	{
		m_Page_BuySell.m_edt_dwMinNum_MpGoods = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name, "m_Page_BuySell.m_edt_dwMaxNum_MpGoods") == 0)
	{
		m_Page_BuySell.m_edt_dwMaxNum_MpGoods = atoi(szCtrl_Val);		return;
	}
	if (strcmp(szCtrl_Name,"m_Page_BuySell.m_lst_ctl_SupplyGoods{") == 0)
	{
		m_Page_BuySell.m_lst_ctl_SupplyGoods.ResetContent();	//清空列表框
		char szLineBuf[200];
		while (TRUE)
		{
			fs->getline(szLineBuf, sizeof(szLineBuf));
			if (strcmp(szLineBuf,"m_Page_BuySell.m_lst_ctl_SupplyGoods}")==0)	break;
			m_Page_BuySell.m_lst_ctl_SupplyGoods.AddString(szLineBuf);
		}
	}
	if (strcmp(szCtrl_Name, "m_Page_BuySell.m_Page_GoodsManage.m_lstC_GoodsTable{") == 0)
	{
		CListCtrl* pListC = &m_Page_BuySell.m_Page_GoodsManage.m_lstC_GoodsTable;
		pListC->DeleteAllItems();	//清空列表控件
		char szLineBuf[200];
		int y = 0; //行数
		while (TRUE)
		{
			fs->getline(szLineBuf, sizeof(szLineBuf));
			if (strcmp(szLineBuf, "m_Page_BuySell.m_Page_GoodsManage.m_lstC_GoodsTable}") == 0)	
				break;
			char* szItem = strchr(szLineBuf,'|');
			szItem[0] = '\0';
			szItem += 1;	//指向第一个项数据
			pListC->InsertItem(y, szLineBuf);	//插到最后
			for (int x=1; x<=3; x++)
			{
				if (szItem[0] == '1')		pListC->SetItemText(y, x, "√");
				szItem += 2;
			}
			y++;
		}
	}
}

/********************************************************************************************/
/* Description	: 替换指定字串中的某一个字符
/* Return		: 
/* Time			: 2020/02/21 16:45
/* Remark		: 只替换一次
/********************************************************************************************/
void ReplaceChar(char* Text, char ToRpc, char RpcTo)
{
	for (DWORD i=0;i < strlen(Text);i++)
	{
		if (Text[i] == ToRpc) 
		{
			Text[i] = RpcTo;	
			break;
		}
	}
}




//时钟事件处理
void CDialog_MainTab::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case ID_TIMER_QingGong:
		if (g_cAutoPlay.IsAutoUseQingGong)	
			msg_UseSkillByName(g_cAutoPlay.szQingGongName);
	
	case ID_TIMER_HMProtect:
		if (g_cAutoPlay.IsAutoUseHpGoods || g_cAutoPlay.IsAutoUseMpGoods)
			g_cAutoPlay.LowHpMpProtect();

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
