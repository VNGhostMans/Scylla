#pragma once

#include <windows.h>
#include "resource.h"

// WTL
#include <atlbase.h>       // base ATL classes
#include <atlapp.h>        // base WTL classes
#include <atlwin.h>        // ATL GUI classes
#include <atlframe.h>      // WTL window frame helpers
#include <atlmisc.h>       // WTL utility classes
#include <atlcrack.h>      // WTL enhanced msg map macros
#include <atlctrls.h>      // WTL controls
#include <atlddx.h>        // WTL dialog data exchange
#include "multitree.h"
#include "hexedit.h"

#include "Logger.h"
#include "ProcessLister.h"
#include "IATSearch.h"
#include "PickDllGui.h"
#include "DumpMemoryGui.h"
#include "DumpSectionGui.h"
#include "ImportsHandling.h"
#include "FunctionExport.h"
#include "IATReferenceScan.h"

class MainGui : public CDialogImpl<MainGui>, public CWinDataExchange<MainGui>, public CDialogResize<MainGui>, public CMessageFilter
{
public:
	enum { IDD = IDD_DLG_MAIN };

	// Dialog Data eXchange, attaches/subclasses child controls to wrappers
	// DDX_CONTROL : subclass
	// DDX_CONTROL_HANDLE : attach
	BEGIN_DDX_MAP(MainGui)
		DDX_CONTROL(IDC_TREE_IMPORTS, TreeImportsSubclass) // needed for message reflection
		DDX_CONTROL(IDC_TREE_IMPORTS, TreeImports)
		DDX_CONTROL_HANDLE(IDC_CBO_PROCESSLIST, ComboProcessList)
		DDX_CONTROL_HANDLE(IDC_LIST_LOG, ListLog)
		DDX_CONTROL(IDC_EDIT_OEPADDRESS, EditOEPAddress)
		DDX_CONTROL(IDC_EDIT_IATADDRESS, EditIATAddress)
		DDX_CONTROL(IDC_EDIT_IATSIZE, EditIATSize)
	END_DDX_MAP()

	// Our message map
	// Messages are passed from top to bottom
	// The first handler that doesn't call SetMsgHandled(FALSE) aborts the chain
	// If none signals the message as handled, it will be passed to mixins (CHAIN_MSG_MAP)
	// or ultimately passed to DefWindowProc
	BEGIN_MSG_MAP_EX(MainGui)

		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_SIZE(OnSize)
		MSG_WM_CONTEXTMENU(OnContextMenu)
		MSG_WM_COMMAND(OnCommand)

		NOTIFY_HANDLER_EX(IDC_TREE_IMPORTS, NM_DBLCLK, OnTreeImportsDoubleClick)
		NOTIFY_HANDLER_EX(IDC_TREE_IMPORTS, TVN_KEYDOWN, OnTreeImportsKeyDown)

		COMMAND_HANDLER_EX(IDC_CBO_PROCESSLIST, CBN_DROPDOWN, OnProcessListDrop)
		COMMAND_HANDLER_EX(IDC_CBO_PROCESSLIST, CBN_SELENDOK, OnProcessListSelected)
		COMMAND_ID_HANDLER_EX(IDC_BTN_PICKDLL, OnPickDLL)
		COMMAND_ID_HANDLER_EX(IDC_BTN_OPTIONS, OnOptions)
		COMMAND_ID_HANDLER_EX(IDC_BTN_DUMP, OnDump)
		COMMAND_ID_HANDLER_EX(IDC_BTN_FIXDUMP, OnFixDump)
		COMMAND_ID_HANDLER_EX(IDC_BTN_PEREBUILD, OnPERebuild)
		COMMAND_ID_HANDLER_EX(IDC_BTN_IATAUTOSEARCH, OnIATAutoSearch)
		COMMAND_ID_HANDLER_EX(IDC_BTN_GETIMPORTS, OnGetImports)
		COMMAND_ID_HANDLER_EX(IDC_BTN_INVALIDIMPORTS, OnInvalidImports)
		COMMAND_ID_HANDLER_EX(IDC_BTN_SUSPECTIMPORTS, OnSuspectImports)
		COMMAND_ID_HANDLER_EX(IDC_BTN_CLEARIMPORTS, OnClearImports)
		COMMAND_ID_HANDLER_EX(ID_FILE_DUMP, OnDump)
		COMMAND_ID_HANDLER_EX(ID_FILE_DUMPMEMORY, OnDumpMemory)
		COMMAND_ID_HANDLER_EX(ID_FILE_DUMPSECTION, OnDumpSection)
		COMMAND_ID_HANDLER_EX(ID_FILE_PEREBUILD, OnPERebuild)
		COMMAND_ID_HANDLER_EX(ID_FILE_FIXDUMP, OnFixDump)
		COMMAND_ID_HANDLER_EX(ID_FILE_EXIT, OnExit)
		COMMAND_ID_HANDLER_EX(ID_IMPORTS_SHOWINVALID, OnInvalidImports)
		COMMAND_ID_HANDLER_EX(ID_IMPORTS_SHOWSUSPECT, OnSuspectImports)
		COMMAND_ID_HANDLER_EX(ID_IMPORTS_INVALIDATESELECTED, OnInvalidateSelected)
		COMMAND_ID_HANDLER_EX(ID_IMPORTS_CUTSELECTED, OnCutSelected)
		COMMAND_ID_HANDLER_EX(ID_IMPORTS_CLEARIMPORTS, OnClearImports)
		COMMAND_ID_HANDLER_EX(ID_IMPORTS_SAVETREE, OnSaveTree)
		COMMAND_ID_HANDLER_EX(ID_IMPORTS_LOADTREE, OnLoadTree)
		COMMAND_ID_HANDLER_EX(ID_TRACE_AUTOTRACE, OnAutotrace)
		COMMAND_ID_HANDLER_EX(ID_MISC_DLLINJECTION, OnDLLInject)
		COMMAND_ID_HANDLER_EX(ID_MISC_DISASSEMBLER, OnDisassembler)
		COMMAND_ID_HANDLER_EX(ID_MISC_OPTIONS, OnOptions)
		COMMAND_ID_HANDLER_EX(ID_HELP_ABOUT, OnAbout)
		COMMAND_ID_HANDLER_EX(ID_HELP_DONATE, OnDonate)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnExit)

		REFLECT_NOTIFY_ID(IDC_TREE_IMPORTS) // pass WM_NOTIFY to child control
		CHAIN_MSG_MAP(CDialogResize<MainGui>)

		// Message map for subclassed treeview
		// CContainedWindow forwards all messages to this map
	ALT_MSG_MAP(IDC_TREE_IMPORTS)

		MSG_WM_GETDLGCODE(OnTreeImportsSubclassGetDlgCode)
		MSG_WM_CHAR(OnTreeImportsSubclassChar)

	END_MSG_MAP()

	// Dialog resize 'table'
	// States if child controls move or resize or center in a specific direction
	// when the parent dialog is resized
	BEGIN_DLGRESIZE_MAP(MainGui)
		DLGRESIZE_CONTROL(IDC_GROUP_ATTACH,    DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_CBO_PROCESSLIST, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BTN_PICKDLL,     DLSZ_MOVE_X)

		DLGRESIZE_CONTROL(IDC_GROUP_IMPORTS, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_TREE_IMPORTS,  DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_INVALIDIMPORTS, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_SUSPECTIMPORTS, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_CLEARIMPORTS,   DLSZ_MOVE_X | DLSZ_MOVE_Y)

		DLGRESIZE_CONTROL(IDC_GROUP_IATINFO,     DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC_OEPADDRESS, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC_IATADDRESS, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC_IATSIZE,    DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_OEPADDRESS,   DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_IATADDRESS,   DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_IATSIZE,      DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_IATAUTOSEARCH, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_GETIMPORTS,    DLSZ_MOVE_Y)

		DLGRESIZE_CONTROL(IDC_GROUP_ACTIONS, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_AUTOTRACE, DLSZ_MOVE_Y)

		DLGRESIZE_CONTROL(IDC_GROUP_DUMP,    DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_DUMP,      DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_PEREBUILD, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_FIXDUMP,   DLSZ_MOVE_Y)

		DLGRESIZE_CONTROL(IDC_GROUP_LOG, DLSZ_MOVE_Y | DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_LIST_LOG,  DLSZ_MOVE_Y | DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

	MainGui();

	//void addTextToOutputLog(const WCHAR * text);

	//CWindow getLogListboxHandle() const { return ListLog; }

protected:

	// Variables

	
	WCHAR stringBuffer[600];

	ImportsHandling importsHandling;
	//ProcessAccessHelp processAccessHelp;
	ApiReader apiReader;
	IATReferenceScan iatReferenceScan;

	Process * selectedProcess;
	bool isProcessSuspended;

	// File selection filters

	static const WCHAR filterExe[];
	static const WCHAR filterDll[];
	static const WCHAR filterExeDll[];
	static const WCHAR filterTxt[];
	static const WCHAR filterXml[];
	static const WCHAR filterMem[];

	// Controls

	CMultiSelectTreeViewCtrl TreeImports;
	CComboBox ComboProcessList;
	CHexEdit<DWORD_PTR> EditOEPAddress;
	CHexEdit<DWORD_PTR> EditIATAddress;
	CHexEdit<DWORD> EditIATSize;
	CListBox ListLog;
	CStatusBarCtrl StatusBar;

	enum StatusParts {
		PART_COUNT = 0,
		PART_INVALID,
		PART_IMAGEBASE,
		PART_MODULE
	};

	CContainedWindow TreeImportsSubclass;

	// Handles

	CIcon hIcon;
	CMenu hMenuImports;
	CMenu hMenuLog;
	CAccelerator accelerators;
	CIcon hIconCheck;
	CIcon hIconWarning;
	CIcon hIconError;

	static const int MenuImportsOffsetTrace = 2;
	static const int MenuImportsTraceOffsetScylla = 2;
	static const int MenuImportsTraceOffsetImpRec = 4;

	static const int PLUGIN_MENU_BASE_ID = 0x10;
protected:

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Message handlers

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void OnDestroy();
	void OnSize(UINT nType, CSize size);
	void OnContextMenu(CWindow wnd, CPoint point);
	void OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl);

	// WM_NOTIFY handlers

	LRESULT OnTreeImportsDoubleClick(const NMHDR* pnmh);
	LRESULT OnTreeImportsKeyDown(const NMHDR* pnmh);

	// Forwarded messages from subclassed treeview
	UINT OnTreeImportsSubclassGetDlgCode(const MSG * lpMsg);
	void OnTreeImportsSubclassChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	// WM_COMMAND handlers

	void OnProcessListDrop(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnProcessListSelected(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnPickDLL(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnOptions(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDump(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDumpMemory(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDumpSection(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnFixDump(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPERebuild(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDLLInject(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDisassembler(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnIATAutoSearch(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnGetImports(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnInvalidImports(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnSuspectImports(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnClearImports(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnInvalidateSelected(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnCutSelected(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnSaveTree(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnLoadTree(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnAutotrace(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnExit(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnAbout(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDonate(UINT uNotifyCode, int nID, CWindow wndCtl);

	// GUI functions

	bool showFileDialog(WCHAR * selectedFile, bool save, const WCHAR * defFileName, const WCHAR * filter = NULL, const WCHAR * defExtension = NULL, const WCHAR * directory = NULL);

	void setupStatusBar();
	void updateStatusBar();
	void fillProcessListComboBox(CComboBox& hCombo);
	void setIconAndDialogCaption();
	void enableDialogControls(BOOL value);

	CTreeItem findTreeItem(CPoint pt, bool screenCoordinates);

	// Actions

	void pickDllActionHandler();
	void pickApiActionHandler(CTreeItem item);
	void processSelectedActionHandler(int index);
	void showInvalidImportsActionHandler();
	void showSuspectImportsActionHandler();
	void deleteSelectedImportsActionHandler();
	void invalidateSelectedImportsActionHandler();
	void loadTreeActionHandler();
	void saveTreeActionHandler();
	void iatAutosearchActionHandler();
	void getImportsActionHandler();
	void dumpActionHandler();
	void dumpMemoryActionHandler();
	void dumpSectionActionHandler();
	void peRebuildActionHandler();
	void startDisassemblerGui(CTreeItem selectedTreeNode);
	void dumpFixActionHandler();
	void showAboutDialog();
	void showDonateDialog();
	void dllInjectActionHandler();
	void disassemblerActionHandler();
	void optionsActionHandler();
	void clearImportsActionHandler();
	void pluginActionHandler(int menuItem);

	// Popup menu functions

	void SetupImportsMenuItems(CTreeItem item);
	void appendPluginListToMenu(CMenuHandle hMenuTrackPopup);
	void DisplayContextMenuImports(CWindow, CPoint);
	void DisplayContextMenuLog(CWindow, CPoint);

	// Log

	void clearOutputLog();
	bool saveLogToFile(const WCHAR * file);

	// Misc

	bool getCurrentModulePath(WCHAR * buffer, size_t bufferSize);
	void checkSuspendProcess();
	void setDialogIATAddressAndSize( DWORD_PTR addressIAT, DWORD sizeIAT );
	void InitDllStartWithPreSelect( PGUI_DLL_PARAMETER guiParam );
	bool isIATOutsidePeImage( DWORD_PTR addressIAT );
    bool getCurrentDefaultDumpFilename( WCHAR * buffer, size_t bufferSize );
};
