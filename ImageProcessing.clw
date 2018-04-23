; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainProcessDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ImageProcessing.h"
LastPage=0

ClassCount=15
Class1=CImageProcessingApp
Class2=CImageProcessingDoc
Class3=CImageProcessingView
Class4=CMainFrame

ResourceCount=24
Resource1=IDR_IMAGEPTYPE
Resource2=IDD_MAIN_PROCESS (Korean)
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDD_CHAP5
Class7=CChap2
Resource4=IDD_CHAP3
Class8=CThresholdingDlg
Resource5=IDR_CONTEXT
Class9=CChap3
Resource6=IDD_CHAP2
Resource7=IDD_APPLICATION
Class10=CChap4
Resource8=IDD_MAIN_PROCESS
Class11=CChap5
Resource9=IDD_CHAP4
Class12=CApplication
Resource10=IDD_SAVE_SCREEN_OPT
Class13=CMainProcessDlg
Resource11=IDD_ABOUTBOX
Resource12=IDD_CHAP2 (Korean)
Resource13=IDD_CHAP3 (Korean)
Resource14=IDD_CHAP4 (Korean)
Resource15=IDD_ABOUTBOX (Korean)
Resource16=IDD_THRESHOLDING_DLG (Korean)
Resource17=IDD_APPLICATION (Korean)
Resource18=IDR_MAINFRAME (Korean)
Resource19=IDD_CHAP5 (Korean)
Resource20=IDR_IMAGEPTYPE (Korean)
Resource21=IDR_CONTEXT (Korean)
Resource22=IDD_THRESHOLDING_DLG
Class14=CYuvOptionDlg
Class15=CSaveScreenOpt
Resource23=IDR_MAINFRAME
Resource24=IDD_YUV_OPT

[CLS:CImageProcessingApp]
Type=0
HeaderFile=ImageProcessing.h
ImplementationFile=ImageProcessing.cpp
Filter=N
LastObject=ID_FILE_OPEN
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CImageProcessingDoc]
Type=0
HeaderFile=ImageProcessingDoc.h
ImplementationFile=ImageProcessingDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CImageProcessingDoc

[CLS:CImageProcessingView]
Type=0
HeaderFile=ImageProcessingView.h
ImplementationFile=ImageProcessingView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=IDM_DIFFERENCE


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CMDIFrameWnd
VirtualFilter=fWC


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=ImageProcessing.cpp
ImplementationFile=ImageProcessing.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE_AS
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_IMAGEPTYPE]
Type=1
Class=CImageProcessingView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_OPEN_CUR
Command4=ID_FILE_CLOSE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=IDM_GRAY_DUPLICATE
Command16=IDM_COLOR_DUPLICATE
Command17=IDM_CHAP2
Command18=IDM_CHAP3
Command19=IDM_CHAP4
Command20=IDM_CHAP5
Command21=IDM_APPLICATION
Command22=ID_VIEW_TOOLBAR
Command23=ID_VIEW_STATUS_BAR
Command24=IDD_VIEW_MAIN_PROCESS
Command25=ID_WINDOW_NEW
Command26=ID_WINDOW_CASCADE
Command27=ID_WINDOW_TILE_HORZ
Command28=ID_WINDOW_ARRANGE
Command29=ID_APP_ABOUT
CommandCount=29

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_CHAP2]
Type=1
Class=CChap2
ControlCount=15
Control1=IDC_HISTOGRAM,button,1342242816
Control2=IDC_CANCEL,button,1342242816
Control3=IDC_REVERSE,button,1342242816
Control4=IDC_THRESHOLDING,button,1342242816
Control5=IDC_THRESHOLDING_DLG,button,1342242816
Control6=IDC_OPERATION,button,1342242816
Control7=IDC_THRESHOLD_VALUE,edit,1350631554
Control8=IDC_OPERATION_TYPE,combobox,1344340227
Control9=IDC_MOVE,button,1342242816
Control10=IDC_X0,edit,1350631554
Control11=IDC_Y0,edit,1350631554
Control12=IDC_SCALE_X2,button,1342242816
Control13=IDC_ROTATE,button,1342242816
Control14=IDC_DEG,edit,1350631554
Control15=IDC_DITHERING,button,1342242816

[CLS:CChap2]
Type=0
HeaderFile=Chap2.h
ImplementationFile=Chap2.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_OPERATION_TYPE

[DLG:IDD_THRESHOLDING_DLG]
Type=1
Class=CThresholdingDlg
ControlCount=3
Control1=IDC_CANCEL,button,1342242816
Control2=IDC_THRESHOLD,msctls_trackbar32,1342242840
Control3=IDC_THRESHOLD_VALUE,static,1342308353

[CLS:CThresholdingDlg]
Type=0
HeaderFile=ThresholdingDlg.h
ImplementationFile=ThresholdingDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_CANCEL

[DLG:IDD_CHAP3]
Type=1
Class=CChap3
ControlCount=19
Control1=IDC_HISTOGRAM_EQ,button,1342242816
Control2=IDC_CANCEL,button,1342242816
Control3=IDC_MEAN,button,1342242816
Control4=IDC_WINDOW_SIZE,edit,1350631554
Control5=IDC_STATIC,static,1342308864
Control6=IDC_MEDIAN,button,1342242816
Control7=IDC_SOBEL,button,1342242816
Control8=IDC_LAPLACIAN,button,1342242816
Control9=IDC_LAPLACIAN_SHARP,button,1342242816
Control10=IDC_DFT,button,1342242816
Control11=IDC_FFT,button,1342242816
Control12=IDC_DFT_ROW_COL,button,1342242816
Control13=IDC_FILTERING,button,1342242816
Control14=IDC_FILTER_TYPE,combobox,1344340227
Control15=IDC_FILTER_TYPE2,combobox,1344340227
Control16=IDC_N,edit,1350631554
Control17=IDC_STATIC,static,1342308864
Control18=IDC_DC,edit,1350631554
Control19=IDC_STATIC,static,1342308864

[CLS:CChap3]
Type=0
HeaderFile=Chap3.h
ImplementationFile=Chap3.cpp
BaseClass=CDialog
Filter=D
LastObject=CChap3
VirtualFilter=dWC

[MNU:IDR_CONTEXT]
Type=1
Class=CImageProcessingView
Command1=IDM_SCALE_X2
Command2=IDM_SCALE_HALF
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=IDM_SEL_REGION
Command7=IDM_SEL_REGION_ZOOM
Command8=IDM_SUM
Command9=IDM_SUBTRACT
Command10=IDM_DIFFERENCE
CommandCount=10

[DLG:IDD_CHAP4]
Type=1
Class=CChap4
ControlCount=5
Control1=IDC_CANCEL,button,1342242816
Control2=IDC_DCT,button,1342242816
Control3=IDC_BLOCK_SIZE,edit,1350631554
Control4=IDC_STATIC,static,1342308864
Control5=IDC_COMPRESSION,button,1342242816

[CLS:CChap4]
Type=0
HeaderFile=Chap4.h
ImplementationFile=Chap4.cpp
BaseClass=CDialog
Filter=D
LastObject=CChap4
VirtualFilter=dWC

[DLG:IDD_CHAP5]
Type=1
Class=CChap5
ControlCount=23
Control1=IDC_CANCEL,button,1342242816
Control2=IDC_THRESHOLDING_PTILE,button,1342242816
Control3=IDC_PERCENT,edit,1350631554
Control4=IDC_STATIC,static,1342308864
Control5=IDC_UP_DOWN,combobox,1344340227
Control6=IDC_THRESHOLDING_VALLEY,button,1342242816
Control7=IDC_THRESHOLDING_ITERATE,button,1342242816
Control8=IDC_HOUGH,button,1342242816
Control9=IDC_TURTLE,button,1342242816
Control10=IDC_K_COSINE,button,1342242816
Control11=IDC_K,edit,1350631554
Control12=IDC_STATIC,static,1342308864
Control13=IDC_DILATION,button,1342242816
Control14=IDC_BIN,button,1342242819
Control15=IDC_ELE_SIZE,edit,1350631554
Control16=IDC_SHAPE,combobox,1344340227
Control17=IDC_EROSION,button,1342242816
Control18=IDC_CLOSE,button,1342242816
Control19=IDC_OPEN,button,1342242816
Control20=IDC_LABELING,button,1342242816
Control21=IDC_THINNING,button,1342242816
Control22=IDC_LABELING_MOMENT,button,1342242816
Control23=IDC_MEAN_SHIFT,button,1342242816

[CLS:CChap5]
Type=0
HeaderFile=Chap5.h
ImplementationFile=Chap5.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BIN
VirtualFilter=dWC

[DLG:IDD_APPLICATION]
Type=1
Class=CApplication
ControlCount=15
Control1=IDC_CANCEL,button,1342242816
Control2=IDC_VEHICLE_RUN,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_FOLDER_NAME,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_FOLDER_SEL,button,1342242816
Control7=IDC_AVI_RUN,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_FILE_NAME,edit,1350631552
Control10=IDC_STATIC,static,1342308864
Control11=IDC_FILE_SEL,button,1342242816
Control12=IDC_MOTION_RUN,button,1342242816
Control13=IDC_STATIC,button,1342177287
Control14=IDC_BPN_EX_RUN,button,1342242816
Control15=IDC_STATIC,button,1342177287

[CLS:CApplication]
Type=0
HeaderFile=Application.h
ImplementationFile=Application.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CApplication

[DLG:IDD_MAIN_PROCESS]
Type=1
Class=CMainProcessDlg
ControlCount=57
Control1=IDC_STATIC,button,1342177287
Control2=IDC_SOURCE_TYPE0,button,1342308361
Control3=IDC_SOURCE_TYPE1,button,1342177289
Control4=IDC_SOURCE_TYPE2,button,1342177289
Control5=IDC_SUB_FOLDER,button,1342373891
Control6=IDC_SAVE_SCREEN,button,1342242819
Control7=IDC_SEQ_TYPE,combobox,1344340227
Control8=IDC_SEL_SOURCE,button,1342242816
Control9=IDC_DISP_SOURCE,edit,1350633600
Control10=IDC_RUN,button,1342242816
Control11=IDC_PAUSE,button,1342242816
Control12=IDC_TOTAL_FRAME,static,1342308865
Control13=IDC_STATIC,static,1342308864
Control14=IDC_START,edit,1350631554
Control15=IDC_STATIC,static,1342308864
Control16=IDC_END,edit,1350631554
Control17=IDC_STEP,button,1342242819
Control18=IDC_SLEEP,msctls_trackbar32,1342242840
Control19=IDC_DISP_SLEEP,static,1342308866
Control20=IDC_FILE_LIST,SysListView32,1350631437
Control21=IDC_ZOOM_SCALE0,button,1342308361
Control22=IDC_ZOOM_SCALE1,button,1342177289
Control23=IDC_ZOOM_SCALE2,button,1342177289
Control24=IDC_ZOOM_SCALE3,button,1342177289
Control25=IDC_MASK,edit,1350762626
Control26=IDC_SEARCH,edit,1350631554
Control27=IDC_GROW,edit,1350631554
Control28=IDC_ME_GAP,edit,1350631554
Control29=IDC_RUN_TYPE0,button,1342308361
Control30=IDC_RUN_TYPE1,button,1342177289
Control31=IDC_RUN_TYPE2,button,1342177289
Control32=IDC_VIEW_PSNR,button,1342373891
Control33=IDC_STATIC,static,1342308864
Control34=IDC_USE_COLOR_TYPE0,button,1342308361
Control35=IDC_USE_COLOR_TYPE1,button,1342177289
Control36=IDC_STATIC,static,1342308864
Control37=IDC_PSNR_TYPE0,button,1342308361
Control38=IDC_PSNR_TYPE1,button,1342177289
Control39=IDC_PSNR_TYPE2,button,1342177289
Control40=IDC_STATIC,button,1342308359
Control41=IDC_GRAB,button,1342242816
Control42=IDC_SEL_FOLDER2,button,1342242816
Control43=IDC_GW,edit,1350631554
Control44=IDC_GH,edit,1350631554
Control45=IDC_STATIC,static,1342308864
Control46=IDC_STATIC,static,1342308864
Control47=IDC_VS,button,1342242816
Control48=IDC_CAM_ON,button,1342242816
Control49=IDC_TYPE,button,1342308361
Control50=IDC_TYPE2,button,1342177289
Control51=IDC_RESET_G_NUM,button,1342242816
Control52=IDC_GAP,edit,1350631554
Control53=IDC_STATIC,static,1342308864
Control54=IDC_LIST,listbox,1352728833
Control55=IDC_DISP_FOLDER2,edit,1350633600
Control56=IDC_STATIC,button,1342177287
Control57=IDC_THRE_DIFF,edit,1350631554

[CLS:CMainProcessDlg]
Type=0
HeaderFile=mainprocessdlg.h
ImplementationFile=mainprocessdlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_THRE_DIFF

[MNU:IDR_CONTEXT (Korean)]
Type=1
Class=?
Command1=IDM_SCALE_X2
Command2=IDM_SCALE_HALF
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
CommandCount=5

[MNU:IDR_MAINFRAME (Korean)]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_IMAGEPTYPE (Korean)]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_OPEN_CUR
Command4=ID_FILE_CLOSE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=IDM_GRAY_DUPLICATE
Command16=IDM_COLOR_DUPLICATE
Command17=IDM_CHAP2
Command18=IDM_CHAP3
Command19=IDM_CHAP4
Command20=IDM_CHAP5
Command21=IDM_APPLICATION
Command22=ID_VIEW_TOOLBAR
Command23=ID_VIEW_STATUS_BAR
Command24=IDD_VIEW_MAIN_PROCESS
Command25=ID_WINDOW_NEW
Command26=ID_WINDOW_CASCADE
Command27=ID_WINDOW_TILE_HORZ
Command28=ID_WINDOW_ARRANGE
Command29=ID_APP_ABOUT
CommandCount=29

[DLG:IDD_CHAP3 (Korean)]
Type=1
Class=CChap3
ControlCount=19
Control1=IDC_HISTOGRAM_EQ,button,1342242816
Control2=IDC_CANCEL,button,1342242816
Control3=IDC_MEAN,button,1342242816
Control4=IDC_WINDOW_SIZE,edit,1350631554
Control5=IDC_STATIC,static,1342308864
Control6=IDC_MEDIAN,button,1342242816
Control7=IDC_SOBEL,button,1342242816
Control8=IDC_LAPLACIAN,button,1342242816
Control9=IDC_LAPLACIAN_SHARP,button,1342242816
Control10=IDC_DFT,button,1342242816
Control11=IDC_FFT,button,1342242816
Control12=IDC_DFT_ROW_COL,button,1342242816
Control13=IDC_FILTERING,button,1342242816
Control14=IDC_FILTER_TYPE,combobox,1344340227
Control15=IDC_FILTER_TYPE2,combobox,1344340227
Control16=IDC_N,edit,1350631554
Control17=IDC_STATIC,static,1342308864
Control18=IDC_DC,edit,1350631554
Control19=IDC_STATIC,static,1342308864

[DLG:IDD_CHAP4 (Korean)]
Type=1
Class=CChap4
ControlCount=5
Control1=IDC_CANCEL,button,1342242816
Control2=IDC_DCT,button,1342242816
Control3=IDC_BLOCK_SIZE,edit,1350631554
Control4=IDC_STATIC,static,1342308864
Control5=IDC_COMPRESSION,button,1342242816

[DLG:IDD_THRESHOLDING_DLG (Korean)]
Type=1
Class=CThresholdingDlg
ControlCount=3
Control1=IDC_CANCEL,button,1342242816
Control2=IDC_THRESHOLD,msctls_trackbar32,1342242840
Control3=IDC_THRESHOLD_VALUE,static,1342308353

[DLG:IDD_APPLICATION (Korean)]
Type=1
Class=CApplication
ControlCount=15
Control1=IDC_CANCEL,button,1342242816
Control2=IDC_VEHICLE_RUN,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_FOLDER_NAME,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_FOLDER_SEL,button,1342242816
Control7=IDC_AVI_RUN,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_FILE_NAME,edit,1350631552
Control10=IDC_STATIC,static,1342308864
Control11=IDC_FILE_SEL,button,1342242816
Control12=IDC_MOTION_RUN,button,1342242816
Control13=IDC_STATIC,button,1342177287
Control14=IDC_BPN_EX_RUN,button,1342242816
Control15=IDC_STATIC,button,1342177287

[DLG:IDD_CHAP2 (Korean)]
Type=1
Class=CChap2
ControlCount=15
Control1=IDC_HISTOGRAM,button,1342242816
Control2=IDC_CANCEL,button,1342242816
Control3=IDC_REVERSE,button,1342242816
Control4=IDC_THRESHOLDING,button,1342242816
Control5=IDC_THRESHOLDING_DLG,button,1342242816
Control6=IDC_OPERATION,button,1342242816
Control7=IDC_THRESHOLD_VALUE,edit,1350631554
Control8=IDC_OPERATION_TYPE,combobox,1344340227
Control9=IDC_MOVE,button,1342242816
Control10=IDC_X0,edit,1350631554
Control11=IDC_Y0,edit,1350631554
Control12=IDC_SCALE_X2,button,1342242816
Control13=IDC_ROTATE,button,1342242816
Control14=IDC_DEG,edit,1350631554
Control15=IDC_DITHERING,button,1342242816

[DLG:IDD_CHAP5 (Korean)]
Type=1
Class=CChap5
ControlCount=23
Control1=IDC_CANCEL,button,1342242816
Control2=IDC_THRESHOLDING_PTILE,button,1342242816
Control3=IDC_PERCENT,edit,1350631554
Control4=IDC_STATIC,static,1342308864
Control5=IDC_UP_DOWN,combobox,1344340227
Control6=IDC_THRESHOLDING_VALLEY,button,1342242816
Control7=IDC_THRESHOLDING_ITERATE,button,1342242816
Control8=IDC_HOUGH,button,1342242816
Control9=IDC_TURTLE,button,1342242816
Control10=IDC_K_COSINE,button,1342242816
Control11=IDC_K,edit,1350631554
Control12=IDC_STATIC,static,1342308864
Control13=IDC_DILATION,button,1342242816
Control14=IDC_BIN,button,1342242819
Control15=IDC_ELE_SIZE,edit,1350631554
Control16=IDC_SHAPE,combobox,1344340227
Control17=IDC_EROSION,button,1342242816
Control18=IDC_CLOSE,button,1342242816
Control19=IDC_OPEN,button,1342242816
Control20=IDC_LABELING,button,1342242816
Control21=IDC_THINNING,button,1342242816
Control22=IDC_LABELING_MOMENT,button,1342242816
Control23=IDC_MEAN_SHIFT,button,1342242816

[DLG:IDD_MAIN_PROCESS (Korean)]
Type=1
Class=CMainProcessDlg
ControlCount=31
Control1=IDC_SEL_FOLDER,button,1342242816
Control2=IDC_FILE_LIST,SysListView32,1350631437
Control3=IDC_RUN,button,1342242816
Control4=IDC_START,edit,1350631554
Control5=IDC_END,edit,1350631554
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342308864
Control8=IDC_PAUSE,button,1342242816
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,button,1342177287
Control11=IDC_GRAB,button,1342242816
Control12=IDC_SEL_FOLDER2,button,1342242816
Control13=IDC_DISP_FOLDER2,static,1342308864
Control14=IDC_GW,edit,1350631554
Control15=IDC_GH,edit,1350631554
Control16=IDC_STATIC,static,1342308864
Control17=IDC_STATIC,static,1342308864
Control18=IDC_VS,button,1342242816
Control19=IDC_CAM_ON,button,1342242816
Control20=IDC_TYPE,button,1342308361
Control21=IDC_TYPE2,button,1342177289
Control22=IDC_RESET_G_NUM,button,1342242816
Control23=IDC_GAP,edit,1350631554
Control24=IDC_STATIC,static,1342308864
Control25=IDC_SLEEP,msctls_trackbar32,1342242840
Control26=IDC_DISP_SLEEP,static,1342308866
Control27=IDC_LIST,listbox,1352728833
Control28=IDC_SEL_AVI,button,1342242816
Control29=IDC_DISP_AVI,edit,1350633600
Control30=IDC_DISP_FOLDER,edit,1350633600
Control31=IDC_AVI_RUN,button,1342242816

[TB:IDR_MAINFRAME (Korean)]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE_AS
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME (Korean)]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (Korean)]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CYuvOptionDlg]
Type=0
HeaderFile=YuvOptionDlg.h
ImplementationFile=YuvOptionDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_SAVE_SCREEN_OPT]
Type=1
Class=CSaveScreenOpt
ControlCount=6
Control1=IDCANCEL,button,1342242816
Control2=IDC_W,edit,1350631554
Control3=IDC_STATIC,static,1342308864
Control4=IDC_H,edit,1350631554
Control5=IDC_STATIC,static,1342308864
Control6=IDOK,button,1342242816

[CLS:CSaveScreenOpt]
Type=0
HeaderFile=SaveScreenOpt.h
ImplementationFile=SaveScreenOpt.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSaveScreenOpt

[DLG:IDD_YUV_OPT]
Type=1
Class=?
ControlCount=12
Control1=IDC_STATIC,static,1342308864
Control2=IDC_W,edit,1350631554
Control3=IDC_STATIC,static,1342308864
Control4=IDC_H,edit,1350631554
Control5=IDC_SCALE_FORMAT0,button,1342308361
Control6=IDC_SCALE_FORMAT1,button,1342177289
Control7=IDC_SCALE_FORMAT2,button,1342177289
Control8=IDC_SCALE_FORMAT3,button,1342177289
Control9=IDC_YUV_TYPE0,button,1342308361
Control10=IDC_YUV_TYPE1,button,1476395017
Control11=IDOK,button,1342373888
Control12=IDCANCEL,button,1342242816

