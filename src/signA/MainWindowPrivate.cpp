#include "MainWindowPrivate.h"
#include "mainwindow.h"

MainWindowPrivate::MainWindowPrivate(MainWindow* p)
    :Parent(p)
{

}

void MainWindowPrivate::init()
{
    setupUi(this->Parent);
}



void MainWindowPrivate::setupUi(MainWindow *mainWinowPtr)
{
    if (mainWinowPtr->objectName().isEmpty())
        mainWinowPtr->setObjectName(QStringLiteral("MainWindow"));

    mainWinowPtr->resize(1219, 689);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(mainWinowPtr->sizePolicy().hasHeightForWidth());
    mainWinowPtr->setSizePolicy(sizePolicy);
    mainWinowPtr->setAcceptDrops(true);
    mainWinowPtr->setAutoFillBackground(false);
    //mainWinowPtr->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    actionOpen = new QAction(mainWinowPtr);
    actionOpen->setObjectName(QStringLiteral("actionOpen"));
    QIcon icon;
    icon.addFile(QStringLiteral(":/icons/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionOpen->setIcon(icon);
    actionWindowCascade = new QAction(mainWinowPtr);
    actionWindowCascade->setObjectName(QStringLiteral("actionWindowCascade"));
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/icons/icons/windowCascade.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionWindowCascade->setIcon(icon1);
    actionWindowTile = new QAction(mainWinowPtr);
    actionWindowTile->setObjectName(QStringLiteral("actionWindowTile"));
    QIcon icon2;
    icon2.addFile(QStringLiteral(":/icons/icons/windowTile.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionWindowTile->setIcon(icon2);
    actionWindowMode = new QAction(mainWinowPtr);
    actionWindowMode->setObjectName(QStringLiteral("actionWindowMode"));
    actionWindowMode->setCheckable(true);
    actionWindowMode->setChecked(true);
    QIcon icon3;
    icon3.addFile(QStringLiteral(":/icons/icons/windowMode.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionWindowMode->setIcon(icon3);
    actionTabMode = new QAction(mainWinowPtr);
    actionTabMode->setObjectName(QStringLiteral("actionTabMode"));
    actionTabMode->setCheckable(true);
    QIcon icon4;
    icon4.addFile(QStringLiteral(":/icons/icons/tabMode.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionTabMode->setIcon(icon4);
    actionSave = new QAction(mainWinowPtr);
    actionSave->setObjectName(QStringLiteral("actionSave"));
    actionSave->setEnabled(true);
    QIcon icon5;
    icon5.addFile(QStringLiteral(":/icons/icons/save.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSave->setIcon(icon5);
    actionQuite = new QAction(mainWinowPtr);
    actionQuite->setObjectName(QStringLiteral("actionQuite"));
    QIcon icon6;
    icon6.addFile(QStringLiteral(":/icons/icons/quit.svg"), QSize(), QIcon::Normal, QIcon::Off);
    actionQuite->setIcon(icon6);
    actionDataFeatureDock = new QAction(mainWinowPtr);
    actionDataFeatureDock->setObjectName(QStringLiteral("actionDataFeatureDock"));
    QIcon icon7;
    icon7.addFile(QStringLiteral(":/icons/icons/dataFeatureDock.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionDataFeatureDock->setIcon(icon7);
    actionNewChart = new QAction(mainWinowPtr);
    actionNewChart->setObjectName(QStringLiteral("actionNewChart"));
    QIcon icon8;
    icon8.addFile(QStringLiteral(":/icons/icons/newChart.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNewChart->setIcon(icon8);
    actionNewTrend = new QAction(mainWinowPtr);
    actionNewTrend->setObjectName(QStringLiteral("actionNewTrend"));
    QIcon icon9;
    icon9.addFile(QStringLiteral(":/icons/icons/newTrend.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNewTrend->setIcon(icon9);
    actionClearProject = new QAction(mainWinowPtr);
    actionClearProject->setObjectName(QStringLiteral("actionClearProject"));
    QIcon icon10;
    icon10.addFile(QStringLiteral(":/icons/icons/clearProject.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionClearProject->setIcon(icon10);
    actionInRangDataRemove = new QAction(mainWinowPtr);
    actionInRangDataRemove->setObjectName(QStringLiteral("actionInRangDataRemove"));
    QIcon icon11;
    icon11.addFile(QStringLiteral(":/icons/icons/inRangDataRemove.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionInRangDataRemove->setIcon(icon11);
    actionOutRangDataRemove = new QAction(mainWinowPtr);
    actionOutRangDataRemove->setObjectName(QStringLiteral("actionOutRangDataRemove"));
    QIcon icon12;
    icon12.addFile(QStringLiteral(":/icons/icons/outRangDataRemove.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionOutRangDataRemove->setIcon(icon12);
    actionExportReport = new QAction(mainWinowPtr);
    actionExportReport->setObjectName(QStringLiteral("actionExportReport"));
    actionSubWindowListDock = new QAction(mainWinowPtr);
    actionSubWindowListDock->setObjectName(QStringLiteral("actionSubWindowListDock"));
    QIcon icon13;
    icon13.addFile(QStringLiteral(":/icons/icons/subWindowListDock.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSubWindowListDock->setIcon(icon13);
    actionEnableChartPicker = new QAction(mainWinowPtr);
    actionEnableChartPicker->setObjectName(QStringLiteral("actionEnableChartPicker"));
    QIcon icon14;
    icon14.addFile(QStringLiteral(":/figureSet/icons/figureSet/crossCursor.svg"), QSize(), QIcon::Normal, QIcon::Off);
    actionEnableChartPicker->setIcon(icon14);
    actionEnableChartPanner = new QAction(mainWinowPtr);
    actionEnableChartPanner->setObjectName(QStringLiteral("actionEnableChartPanner"));
    QIcon icon15;
    icon15.addFile(QStringLiteral(":/figureSet/icons/figureSet/pannerChart.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionEnableChartPanner->setIcon(icon15);
    actionEnableChartZoom = new QAction(mainWinowPtr);
    actionEnableChartZoom->setObjectName(QStringLiteral("actionEnableChartZoom"));
    QIcon icon16;
    icon16.addFile(QStringLiteral(":/figureSet/icons/figureSet/zoomChart.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionEnableChartZoom->setIcon(icon16);
    actionYDataPicker = new QAction(mainWinowPtr);
    actionYDataPicker->setObjectName(QStringLiteral("actionYDataPicker"));
    QIcon icon17;
    icon17.addFile(QStringLiteral(":/figureSet/icons/figureSet/yDataPicker.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionYDataPicker->setIcon(icon17);
    actionShowGrid = new QAction(mainWinowPtr);
    actionShowGrid->setObjectName(QStringLiteral("actionShowGrid"));
    QIcon icon18;
    icon18.addFile(QStringLiteral(":/figureSet/icons/figureSet/showGrid.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionShowGrid->setIcon(icon18);
    actionShowHGrid = new QAction(mainWinowPtr);
    actionShowHGrid->setObjectName(QStringLiteral("actionShowHGrid"));
    QIcon icon19;
    icon19.addFile(QStringLiteral(":/figureSet/icons/figureSet/GridXOnly.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionShowHGrid->setIcon(icon19);
    actionShowVGrid = new QAction(mainWinowPtr);
    actionShowVGrid->setObjectName(QStringLiteral("actionShowVGrid"));
    QIcon icon20;
    icon20.addFile(QStringLiteral(":/figureSet/icons/figureSet/GridYOnly.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionShowVGrid->setIcon(icon20);
    actionShowCrowdedHGrid = new QAction(mainWinowPtr);
    actionShowCrowdedHGrid->setObjectName(QStringLiteral("actionShowCrowdedHGrid"));
    QIcon icon21;
    icon21.addFile(QStringLiteral(":/figureSet/icons/figureSet/GridXMin.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionShowCrowdedHGrid->setIcon(icon21);
    actionShowCrowdedVGrid = new QAction(mainWinowPtr);
    actionShowCrowdedVGrid->setObjectName(QStringLiteral("actionShowCrowdedVGrid"));
    QIcon icon22;
    icon22.addFile(QStringLiteral(":/figureSet/icons/figureSet/GridYMin.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionShowCrowdedVGrid->setIcon(icon22);
    actionShowLegend = new QAction(mainWinowPtr);
    actionShowLegend->setObjectName(QStringLiteral("actionShowLegend"));
    QIcon icon23;
    icon23.addFile(QStringLiteral(":/figureSet/icons/figureSet/ShowLegend.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionShowLegend->setIcon(icon23);
    actionLegendPanel = new QAction(mainWinowPtr);
    actionLegendPanel->setObjectName(QStringLiteral("actionLegendPanel"));
    QIcon icon24;
    icon24.addFile(QStringLiteral(":/figureSet/icons/figureSet/legendPanel.svg"), QSize(), QIcon::Normal, QIcon::Off);
    actionLegendPanel->setIcon(icon24);
    actionChartSet = new QAction(mainWinowPtr);
    actionChartSet->setObjectName(QStringLiteral("actionChartSet"));
    QIcon icon25;
    icon25.addFile(QStringLiteral(":/figureSet/icons/figureSet/chartSet.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionChartSet->setIcon(icon25);
    actionChartZoomReset = new QAction(mainWinowPtr);
    actionChartZoomReset->setObjectName(QStringLiteral("actionChartZoomReset"));
    QIcon icon26;
    icon26.addFile(QStringLiteral(":/figureSet/icons/figureSet/zoomChartReset.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionChartZoomReset->setIcon(icon26);
    actionPickCurveToData = new QAction(mainWinowPtr);
    actionPickCurveToData->setObjectName(QStringLiteral("actionPickCurveToData"));
    QIcon icon27;
    icon27.addFile(QStringLiteral(":/icons/icons/pickCurveToData.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionPickCurveToData->setIcon(icon27);
    actionAbout = new QAction(mainWinowPtr);
    actionAbout->setObjectName(QStringLiteral("actionAbout"));
    QIcon icon28;
    icon28.addFile(QStringLiteral(":/icons/icons/information.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionAbout->setIcon(icon28);
    actionRescind = new QAction(mainWinowPtr);
    actionRescind->setObjectName(QStringLiteral("actionRescind"));
    QIcon icon29;
    icon29.addFile(QStringLiteral(":/icons/icons/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionRescind->setIcon(icon29);
    actionRedo = new QAction(mainWinowPtr);
    actionRedo->setObjectName(QStringLiteral("actionRedo"));
    QIcon icon30;
    icon30.addFile(QStringLiteral(":/icons/icons/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionRedo->setIcon(icon30);
    actionValueManagerDock = new QAction(mainWinowPtr);
    actionValueManagerDock->setObjectName(QStringLiteral("actionValueManagerDock"));
    actionValueManagerDock->setCheckable(false);
    actionValueManagerDock->setChecked(false);
    QIcon icon31;
    icon31.addFile(QStringLiteral(":/icons/icons/valueManagerDock.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionValueManagerDock->setIcon(icon31);
    actionViewValueInCurrentTab = new QAction(mainWinowPtr);
    actionViewValueInCurrentTab->setObjectName(QStringLiteral("actionViewValueInCurrentTab"));
    QIcon icon32;
    icon32.addFile(QStringLiteral(":/icons/icons/viewValueInCurrentTab.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionViewValueInCurrentTab->setIcon(icon32);
    actionViewValueInNewTab = new QAction(mainWinowPtr);
    actionViewValueInNewTab->setObjectName(QStringLiteral("actionViewValueInNewTab"));
    QIcon icon33;
    icon33.addFile(QStringLiteral(":/icons/icons/viewValueInNewTab.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionViewValueInNewTab->setIcon(icon33);
    actionLayerOutDock = new QAction(mainWinowPtr);
    actionLayerOutDock->setObjectName(QStringLiteral("actionLayerOutDock"));
    QIcon icon34;
    icon34.addFile(QStringLiteral(":/icons/icons/layout.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionLayerOutDock->setIcon(icon34);
    actionRenameValue = new QAction(mainWinowPtr);
    actionRenameValue->setObjectName(QStringLiteral("actionRenameValue"));
    QIcon icon35;
    icon35.addFile(QStringLiteral(":/icons/icons/renameValue.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionRenameValue->setIcon(icon35);
    actionSetDefalutDockPos = new QAction(mainWinowPtr);
    actionSetDefalutDockPos->setObjectName(QStringLiteral("actionSetDefalutDockPos"));
    QIcon icon36;
    icon36.addFile(QStringLiteral(":/icons/icons/setDefalutDockPos.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSetDefalutDockPos->setIcon(icon36);
    actionValueViewerDock = new QAction(mainWinowPtr);
    actionValueViewerDock->setObjectName(QStringLiteral("actionValueViewerDock"));
    QIcon icon37;
    icon37.addFile(QStringLiteral(":/icons/icons/valueViewerDock.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionValueViewerDock->setIcon(icon37);
    actionFigureViewer = new QAction(mainWinowPtr);
    actionFigureViewer->setObjectName(QStringLiteral("actionFigureViewer"));
    QIcon icon38;
    icon38.addFile(QStringLiteral(":/icons/icons/figureViewer.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionFigureViewer->setIcon(icon38);
    actionXYDataPicker = new QAction(mainWinowPtr);
    actionXYDataPicker->setObjectName(QStringLiteral("actionXYDataPicker"));
    QIcon icon39;
    icon39.addFile(QStringLiteral(":/figureSet/icons/figureSet/xyDataPicker.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionXYDataPicker->setIcon(icon39);
    actionDeleteValue = new QAction(mainWinowPtr);
    actionDeleteValue->setObjectName(QStringLiteral("actionDeleteValue"));
    QIcon icon40;
    icon40.addFile(QStringLiteral(":/icons/icons/deleteValue.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionDeleteValue->setIcon(icon40);
    actionOpenProject = new QAction(mainWinowPtr);
    actionOpenProject->setObjectName(QStringLiteral("actionOpenProject"));
    QIcon icon41;
    icon41.addFile(QStringLiteral(":/icons/icons/openProject.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionOpenProject->setIcon(icon41);
    actionSaveAs = new QAction(mainWinowPtr);
    actionSaveAs->setObjectName(QStringLiteral("actionSaveAs"));
    QIcon icon42;
    icon42.addFile(QStringLiteral(":/icons/icons/saveAs.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSaveAs->setIcon(icon42);
    actionProjectSetting = new QAction(mainWinowPtr);
    actionProjectSetting->setObjectName(QStringLiteral("actionProjectSetting"));
    QIcon icon43;
    icon43.addFile(QStringLiteral(":/icons/icons/project.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionProjectSetting->setIcon(icon43);
    actionZoomBase = new QAction(mainWinowPtr);
    actionZoomBase->setObjectName(QStringLiteral("actionZoomBase"));
    actionZoomIn = new QAction(mainWinowPtr);
    actionZoomIn->setObjectName(QStringLiteral("actionZoomIn"));
    actionZoomOut = new QAction(mainWinowPtr);
    actionZoomOut->setObjectName(QStringLiteral("actionZoomOut"));
    actionStartRectSelect = new QAction(mainWinowPtr);
    actionStartRectSelect->setObjectName(QStringLiteral("actionStartRectSelect"));
    actionStartRectSelect->setCheckable(true);
    actionStartEllipseSelect = new QAction(mainWinowPtr);
    actionStartEllipseSelect->setObjectName(QStringLiteral("actionStartEllipseSelect"));
    actionStartEllipseSelect->setCheckable(true);
    actionStartPolygonSelect = new QAction(mainWinowPtr);
    actionStartPolygonSelect->setObjectName(QStringLiteral("actionStartPolygonSelect"));
    actionStartPolygonSelect->setCheckable(true);
    actionClearAllSelectiedRegion = new QAction(mainWinowPtr);
    actionClearAllSelectiedRegion->setObjectName(QStringLiteral("actionClearAllSelectiedRegion"));
    actionClearAllSelectiedRegion->setCheckable(true);
    actionSingleSelection = new QAction(mainWinowPtr);
    actionSingleSelection->setObjectName(QStringLiteral("actionSingleSelection"));
    actionSingleSelection->setCheckable(true);
    actionSingleSelection->setChecked(true);
    actionSingleSelection->setMenuRole(QAction::TextHeuristicRole);
    actionAdditionalSelection = new QAction(mainWinowPtr);
    actionAdditionalSelection->setObjectName(QStringLiteral("actionAdditionalSelection"));
    actionAdditionalSelection->setCheckable(true);
    actionIntersectionSelection = new QAction(mainWinowPtr);
    actionIntersectionSelection->setObjectName(QStringLiteral("actionIntersectionSelection"));
    actionIntersectionSelection->setCheckable(true);
    actionSubtractionSelection = new QAction(mainWinowPtr);
    actionSubtractionSelection->setObjectName(QStringLiteral("actionSubtractionSelection"));
    actionSubtractionSelection->setCheckable(true);
    centralWidget = new QWidget(mainWinowPtr);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    centralWidget->setMaximumSize(QSize(0, 0));
    mainWinowPtr->setCentralWidget(centralWidget);

    menuBar = mainWinowPtr->ribbonBar();
    menuBar->setObjectName(QStringLiteral("menuBar"));

    menuFile = new SARibbonMenu(menuBar);
    menuFile->setObjectName(QStringLiteral("menuFile"));

    menuExport = new SARibbonMenu(menuFile);
    menuExport->setObjectName(QStringLiteral("menu_export"));
    QIcon icon44;
    icon44.addFile(QStringLiteral(":/icons/icons/export.png"), QSize(), QIcon::Normal, QIcon::Off);
    menuExport->setIcon(icon44);

    menuImport = new SARibbonMenu(menuFile);
    menuImport->setObjectName(QStringLiteral("menu_import"));
    QIcon icon45;
    icon45.addFile(QStringLiteral(":/icons/icons/import.png"), QSize(), QIcon::Normal, QIcon::Off);
    menuImport->setIcon(icon45);


    menuWindowsViewSet = new SARibbonMenu(menuBar);
    menuWindowsViewSet->setObjectName(QStringLiteral("menu_windowsViewSet"));
    QIcon icon46;
    icon46.addFile(QStringLiteral(":/icons/icons/windowViewSet.png"), QSize(), QIcon::Normal, QIcon::Off);
    menuWindowsViewSet->setIcon(icon46);

    menuAnalysis = new SARibbonMenu(menuBar);
    menuAnalysis->setObjectName(QStringLiteral("menu_Analysis"));
    menuLineChart = new SARibbonMenu(menuBar);
    menuLineChart->setObjectName(QStringLiteral("menuLineChart"));
    menuData = new SARibbonMenu(menuBar);
    menuData->setObjectName(QStringLiteral("menuData"));
    menuChartDataManager = new SARibbonMenu(menuData);
    menuChartDataManager->setObjectName(QStringLiteral("menu_chartDataManager"));
    QIcon icon47;
    icon47.addFile(QStringLiteral(":/icons/icons/chartDataManager.png"), QSize(), QIcon::Normal, QIcon::Off);
    menuChartDataManager->setIcon(icon47);
    menuDataManager = new SARibbonMenu(menuData);
    menuDataManager->setObjectName(QStringLiteral("menu_dataManager"));
    menuChartSet = new SARibbonMenu(menuBar);
    menuChartSet->setObjectName(QStringLiteral("menu_chartSet"));
    menuGrid = new SARibbonMenu(menuChartSet);
    menuGrid->setObjectName(QStringLiteral("menu_grid"));
    menuGrid->setIcon(icon18);
    menuLegend = new SARibbonMenu(menuChartSet);
    menuLegend->setObjectName(QStringLiteral("menu_legend"));
    menuLegend->setIcon(icon23);
    menuZoomSet = new SARibbonMenu(menuChartSet);
    menuZoomSet->setObjectName(QStringLiteral("menuZoomSet"));
    menuZoomSet->setIcon(icon16);
    menuDataPickMenu = new SARibbonMenu(menuChartSet);
    menuDataPickMenu->setObjectName(QStringLiteral("menu_dataPickMenu"));
    menuRegionSelect = new SARibbonMenu(menuChartSet);
    menuRegionSelect->setObjectName(QStringLiteral("menuRegionSelect"));
    menuHelp = new SARibbonMenu(menuBar);
    menuHelp->setObjectName(QStringLiteral("menuHelp"));
    menuEdit = new SARibbonMenu(menuBar);
    menuEdit->setObjectName(QStringLiteral("menuEdit"));
    menuTool = new SARibbonMenu(menuBar);
    menuTool->setObjectName(QStringLiteral("menu_tool"));




    //![1] Main Category Page
    mainRibbonCategory = menuBar->addCategoryPage(QStringLiteral("main"));
    mainRibbonCategory->setObjectName(QStringLiteral("mainRibbonCategory"));
    //File Pannel
    mainCategoryFilePannel = mainRibbonCategory->addPannel(QStringLiteral("mainCategoryFilePannel"));
    mainCategoryFilePannel->setObjectName(QStringLiteral("mainCategoryFilePannel"));

    ribbonButtonFileOpen = mainCategoryFilePannel->addLargeAction(actionOpenProject);
    ribbonButtonFileOpen->setObjectName(QStringLiteral("ribbonButtonFileOpen"));
    ribbonButtonFileOpen->setPopupMode(QToolButton::MenuButtonPopup);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionOpenProject);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionClearProject);
    menuFile->addAction(menuExport->menuAction());
    menuFile->addAction(menuImport->menuAction());
    menuFile->addSeparator();
    menuFile->addAction(actionExportReport);
    menuFile->addAction(actionQuite);
    ribbonButtonFileOpen->setMenu(menuFile);

    ribbonButtonOpen = mainCategoryFilePannel->addSmallAction(actionOpen);
    ribbonButtonSave = mainCategoryFilePannel->addSmallAction(actionSave);
    ribbonButtonClearProject = mainCategoryFilePannel->addSmallAction(actionClearProject);

    //Chart Pannel
    mainCategoryChartPannel = mainRibbonCategory->addPannel(QStringLiteral("mainCategoryChartPannel"));
    ribbonButtonNewChart = mainCategoryChartPannel->addLargeAction(actionNewChart);
    menuLineChart->addAction(actionNewTrend);
    ribbonButtonLineChart = mainCategoryChartPannel->addLargeMenu(menuLineChart);
    //! operate Category Page
    operateRibbonCategory = menuBar->addCategoryPage(QStringLiteral("Operate"));
    operateRibbonCategory->setObjectName(QStringLiteral("operateRibbonCategory"));
    //Selector Editor
    operateCategorySelectEditorPannel = operateRibbonCategory->addPannel(QStringLiteral("Select Editor"));
    operateCategorySelectEditorPannel->setObjectName(QStringLiteral("operateCategorySelectEditorPannel"));

    //! View Category Page
    viewRibbonCategory = menuBar->addCategoryPage(QStringLiteral("View"));
    viewRibbonCategory->setObjectName(QStringLiteral("viewRibbonCategory"));
    //Window Pannel
    viewCategoryWindowPannel = viewRibbonCategory->addPannel(QStringLiteral("Window"));
    ribbonButtonFigureViewer = viewCategoryWindowPannel->addLargeAction(actionFigureViewer);
    ribbonButtonDataFeatureDock = viewCategoryWindowPannel->addLargeAction(actionDataFeatureDock);
    ribbonButtonLayerOutDock = viewCategoryWindowPannel->addSmallAction(actionLayerOutDock);
    ribbonButtonValueViewerDock = viewCategoryWindowPannel->addSmallAction(actionValueViewerDock);
    ribbonButtonValueManagerDock = viewCategoryWindowPannel->addSmallAction(actionValueManagerDock);
    ribbonButtonAllDock = viewCategoryWindowPannel->addLargeToolButton(QStringLiteral("Dock View"),QIcon(),QToolButton::InstantPopup);
    menuWindowsViewSet->addAction(actionFigureViewer);
    menuWindowsViewSet->addAction(actionDataFeatureDock);
    menuWindowsViewSet->addAction(actionSubWindowListDock);
    menuWindowsViewSet->addAction(actionLayerOutDock);
    menuWindowsViewSet->addAction(actionValueManagerDock);
    menuWindowsViewSet->addAction(actionValueViewerDock);
    ribbonButtonAllDock->setMenu(menuWindowsViewSet);
    viewCategoryWindowPannel->addSeparator();
    ribbonButtonSetDefalutDockPos = viewCategoryWindowPannel->addLargeAction(actionSetDefalutDockPos);
    //sub window mode
    windowModeCategoryWindowPannel = viewRibbonCategory->addPannel(QStringLiteral("Sub Window Mode"));
    ribbonButtonTabMode = windowModeCategoryWindowPannel->addLargeAction(actionTabMode);
    ribbonButtonWindowMode = windowModeCategoryWindowPannel->addLargeAction(actionWindowMode);
    ribbonButtonWindowCascade = windowModeCategoryWindowPannel->addMediumAction(actionWindowCascade);
    ribbonButtonWindowTile = windowModeCategoryWindowPannel->addMediumAction(actionWindowTile);





    statusBar = new QStatusBar(mainWinowPtr);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    mainWinowPtr->setStatusBar(statusBar);
    dockWidget_DataFeature = new QDockWidget(mainWinowPtr);
    dockWidget_DataFeature->setObjectName(QStringLiteral("dockWidget_DataFeature"));
    dockWidget_DataFeature->setFloating(false);
    dockWidget_DataFeature->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dockWidgetContents_2 = new QWidget();
    dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
    verticalLayout_2 = new QVBoxLayout(dockWidgetContents_2);
    verticalLayout_2->setSpacing(0);
    verticalLayout_2->setContentsMargins(11, 11, 11, 11);
    verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);
    dataFeatureWidget = new SADataFeatureWidget(dockWidgetContents_2);
    dataFeatureWidget->setObjectName(QStringLiteral("dataFeatureWidget"));

    verticalLayout_2->addWidget(dataFeatureWidget);

    dockWidget_DataFeature->setWidget(dockWidgetContents_2);
    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_DataFeature);
    toolBar_chart = new QToolBar(mainWinowPtr);
    toolBar_chart->setObjectName(QStringLiteral("toolBar_chart"));
    toolBar_chart->setIconSize(QSize(24, 24));
    toolBar_chart->setToolButtonStyle(Qt::ToolButtonIconOnly);
    mainWinowPtr->addToolBar(Qt::TopToolBarArea, toolBar_chart);
    toolBar_chartSet = new QToolBar(mainWinowPtr);
    toolBar_chartSet->setObjectName(QStringLiteral("toolBar_chartSet"));
    toolBar_chartSet->setToolButtonStyle(Qt::ToolButtonIconOnly);
    mainWinowPtr->addToolBar(Qt::TopToolBarArea, toolBar_chartSet);
    toolBar_plot = new QToolBar(mainWinowPtr);
    toolBar_plot->setObjectName(QStringLiteral("toolBar_plot"));
    toolBar_plot->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    mainWinowPtr->addToolBar(Qt::TopToolBarArea, toolBar_plot);
    dockWidget_windowList = new QDockWidget(mainWinowPtr);
    dockWidget_windowList->setObjectName(QStringLiteral("dockWidget_windowList"));
    dockWidgetContents_4 = new QWidget();
    dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
    verticalLayout_6 = new QVBoxLayout(dockWidgetContents_4);
    verticalLayout_6->setSpacing(1);
    verticalLayout_6->setContentsMargins(11, 11, 11, 11);
    verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
    verticalLayout_6->setContentsMargins(1, 1, 1, 1);
    listView_window = new QListView(dockWidgetContents_4);
    listView_window->setObjectName(QStringLiteral("listView_window"));

    verticalLayout_6->addWidget(listView_window);

    dockWidget_windowList->setWidget(dockWidgetContents_4);
    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_windowList);
    dockWidget_valueManage = new QDockWidget(mainWinowPtr);
    dockWidget_valueManage->setObjectName(QStringLiteral("dockWidget_valueManage"));
    dockWidget_valueManage->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
    dockWidgetContents_3 = new QWidget();
    dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
    verticalLayout_3 = new QVBoxLayout(dockWidgetContents_3);
    verticalLayout_3->setSpacing(1);
    verticalLayout_3->setContentsMargins(11, 11, 11, 11);
    verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
    verticalLayout_3->setContentsMargins(1, 1, 1, 1);
    treeView_valueManager = new SAValueManagerTreeView(dockWidgetContents_3);
    treeView_valueManager->setObjectName(QStringLiteral("treeView_valueManager"));
    treeView_valueManager->setContextMenuPolicy(Qt::CustomContextMenu);
    treeView_valueManager->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView_valueManager->setDragEnabled(true);
    treeView_valueManager->setDragDropOverwriteMode(true);
    treeView_valueManager->setDragDropMode(QAbstractItemView::DragDrop);
    treeView_valueManager->setAlternatingRowColors(true);
    treeView_valueManager->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView_valueManager->setWordWrap(false);

    verticalLayout_3->addWidget(treeView_valueManager);

    dockWidget_valueManage->setWidget(dockWidgetContents_3);
    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_valueManage);
    dockWidget_plotLayer = new QDockWidget(mainWinowPtr);
    dockWidget_plotLayer->setObjectName(QStringLiteral("dockWidget_plotLayer"));
    dockWidgetContents_5 = new QWidget();
    dockWidgetContents_5->setObjectName(QStringLiteral("dockWidgetContents_5"));
    verticalLayout_5 = new QVBoxLayout(dockWidgetContents_5);
    verticalLayout_5->setSpacing(1);
    verticalLayout_5->setContentsMargins(11, 11, 11, 11);
    verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
    verticalLayout_5->setContentsMargins(1, 1, 1, 1);
    tableView_layer = new QTableView(dockWidgetContents_5);
    tableView_layer->setObjectName(QStringLiteral("tableView_layer"));
    tableView_layer->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView_layer->setSelectionBehavior(QAbstractItemView::SelectRows);

    verticalLayout_5->addWidget(tableView_layer);

    dockWidget_plotLayer->setWidget(dockWidgetContents_5);
    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_plotLayer);
    dockWidget_chartDataViewer = new QDockWidget(mainWinowPtr);
    dockWidget_chartDataViewer->setObjectName(QStringLiteral("dockWidget_chartDataViewer"));
    dockWidgetContents_chartDataViewer = new QWidget();
    dockWidgetContents_chartDataViewer->setObjectName(QStringLiteral("dockWidgetContents_chartDataViewer"));
    verticalLayout_7 = new QVBoxLayout(dockWidgetContents_chartDataViewer);
    verticalLayout_7->setSpacing(0);
    verticalLayout_7->setContentsMargins(11, 11, 11, 11);
    verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
    verticalLayout_7->setContentsMargins(0, 0, 0, 0);
    splitter_chartDataViewer = new QSplitter(dockWidgetContents_chartDataViewer);
    splitter_chartDataViewer->setObjectName(QStringLiteral("splitter_chartDataViewer"));
    splitter_chartDataViewer->setOrientation(Qt::Horizontal);
    treeView_curPlotItem = new QTreeView(splitter_chartDataViewer);
    treeView_curPlotItem->setObjectName(QStringLiteral("treeView_curPlotItem"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(treeView_curPlotItem->sizePolicy().hasHeightForWidth());
    treeView_curPlotItem->setSizePolicy(sizePolicy1);
    treeView_curPlotItem->setMinimumSize(QSize(30, 0));
    treeView_curPlotItem->setSelectionMode(QAbstractItemView::ExtendedSelection);
    splitter_chartDataViewer->addWidget(treeView_curPlotItem);
    tableView_curSelItemDatas = new QTableView(splitter_chartDataViewer);
    tableView_curSelItemDatas->setObjectName(QStringLiteral("tableView_curSelItemDatas"));
    tableView_curSelItemDatas->setMinimumSize(QSize(50, 0));
    splitter_chartDataViewer->addWidget(tableView_curSelItemDatas);

    verticalLayout_7->addWidget(splitter_chartDataViewer);

    dockWidget_chartDataViewer->setWidget(dockWidgetContents_chartDataViewer);
    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_chartDataViewer);
    dockWidget_main = new QDockWidget(mainWinowPtr);
    dockWidget_main->setObjectName(QStringLiteral("dockWidget_main"));
    QIcon icon48;
    icon48.addFile(QStringLiteral(":/windowIcons/icons/windowIcon/figureWindow.svg"), QSize(), QIcon::Normal, QIcon::Off);
    dockWidget_main->setWindowIcon(icon48);
    dockWidget_main->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dockWidget_main->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockWidgetContents_7 = new QWidget();
    dockWidgetContents_7->setObjectName(QStringLiteral("dockWidgetContents_7"));
    verticalLayout_4 = new QVBoxLayout(dockWidgetContents_7);
    verticalLayout_4->setSpacing(0);
    verticalLayout_4->setContentsMargins(11, 11, 11, 11);
    verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
    verticalLayout_4->setContentsMargins(0, 0, 0, 0);
    mdiArea = new QMdiArea(dockWidgetContents_7);
    mdiArea->setObjectName(QStringLiteral("mdiArea"));
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setDocumentMode(false);
    mdiArea->setTabsClosable(true);
    mdiArea->setTabsMovable(true);
    mdiArea->setTabShape(QTabWidget::Triangular);

    verticalLayout_4->addWidget(mdiArea);

    dockWidget_main->setWidget(dockWidgetContents_7);
    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget_main);
    dockWidget_valueViewer = new QDockWidget(mainWinowPtr);
    dockWidget_valueViewer->setObjectName(QStringLiteral("dockWidget_valueViewer"));
    dockWidgetContents_6 = new QWidget();
    dockWidgetContents_6->setObjectName(QStringLiteral("dockWidgetContents_6"));
    verticalLayout_8 = new QVBoxLayout(dockWidgetContents_6);
    verticalLayout_8->setSpacing(0);
    verticalLayout_8->setContentsMargins(11, 11, 11, 11);
    verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
    verticalLayout_8->setContentsMargins(0, 0, 0, 0);
    tabWidget_valueViewer = new SATabValueViewerWidget(dockWidgetContents_6);
    tabWidget_valueViewer->setObjectName(QStringLiteral("tabWidget_valueViewer"));

    verticalLayout_8->addWidget(tabWidget_valueViewer);

    dockWidget_valueViewer->setWidget(dockWidgetContents_6);
    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_valueViewer);
    dockWidget_message = new QDockWidget(mainWinowPtr);
    dockWidget_message->setObjectName(QStringLiteral("dockWidget_message"));
    dockWidgetContents_8 = new QWidget();
    dockWidgetContents_8->setObjectName(QStringLiteral("dockWidgetContents_8"));
    verticalLayout_9 = new QVBoxLayout(dockWidgetContents_8);
    verticalLayout_9->setSpacing(0);
    verticalLayout_9->setContentsMargins(11, 11, 11, 11);
    verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
    verticalLayout_9->setContentsMargins(0, 0, 0, 0);
    saMessageWidget = new SAMessageWidget(dockWidgetContents_8);
    saMessageWidget->setObjectName(QStringLiteral("saMessageWidget"));

    verticalLayout_9->addWidget(saMessageWidget);

    dockWidget_message->setWidget(dockWidgetContents_8);
    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_message);
    dockWidget_plotSet = new QDockWidget(mainWinowPtr);
    dockWidget_plotSet->setObjectName(QStringLiteral("dockWidget_plotSet"));
    dockWidgetContents = new QWidget();
    dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
    verticalLayout = new QVBoxLayout(dockWidgetContents);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    figureSetWidget = new SAFiugreSetWidget(dockWidgetContents);
    figureSetWidget->setObjectName(QStringLiteral("figureSetWidget"));

    verticalLayout->addWidget(figureSetWidget);

    dockWidget_plotSet->setWidget(dockWidgetContents);
    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_plotSet);
    toolBarChartTools = new QToolBar(mainWinowPtr);
    toolBarChartTools->setObjectName(QStringLiteral("toolBarChartTools"));
    mainWinowPtr->addToolBar(Qt::TopToolBarArea, toolBarChartTools);

//    menuBar->addAction(menuFile->menuAction());
//    menuBar->addAction(menuEdit->menuAction());
//    menuBar->addAction(menu_chart->menuAction());
//    menuBar->addAction(menu_Analysis->menuAction());
//    menuBar->addAction(menuData->menuAction());
//    menuBar->addAction(menu_window->menuAction());
//    menuBar->addAction(menu_chartSet->menuAction());
//    menuBar->addAction(menuHelp->menuAction());
//    menuBar->addAction(menu_tool->menuAction());






    menuData->addAction(menuChartDataManager->menuAction());
    menuData->addAction(menuDataManager->menuAction());
    menuChartDataManager->addAction(actionInRangDataRemove);
    menuChartDataManager->addAction(actionOutRangDataRemove);
    menuChartDataManager->addAction(actionPickCurveToData);
    menuDataManager->addAction(actionViewValueInCurrentTab);
    menuDataManager->addAction(actionViewValueInNewTab);
    menuDataManager->addAction(actionDeleteValue);
    menuDataManager->addAction(actionRenameValue);
    menuChartSet->addAction(actionEnableChartPicker);
    menuChartSet->addAction(actionEnableChartPanner);
    menuChartSet->addAction(menuGrid->menuAction());
    menuChartSet->addAction(menuLegend->menuAction());
    menuChartSet->addAction(menuZoomSet->menuAction());
    menuChartSet->addAction(actionChartSet);
    menuChartSet->addAction(menuDataPickMenu->menuAction());
    menuChartSet->addAction(menuRegionSelect->menuAction());
    menuGrid->addAction(actionShowGrid);
    menuGrid->addSeparator();
    menuGrid->addAction(actionShowHGrid);
    menuGrid->addAction(actionShowVGrid);
    menuGrid->addSeparator();
    menuGrid->addAction(actionShowCrowdedHGrid);
    menuGrid->addAction(actionShowCrowdedVGrid);
    menuLegend->addAction(actionShowLegend);
    menuLegend->addAction(actionLegendPanel);
    menuZoomSet->addAction(actionEnableChartZoom);
    menuZoomSet->addAction(actionChartZoomReset);
    menuZoomSet->addAction(actionZoomBase);
    menuZoomSet->addAction(actionZoomIn);
    menuZoomSet->addAction(actionZoomOut);
    menuDataPickMenu->addAction(actionYDataPicker);
    menuDataPickMenu->addAction(actionXYDataPicker);
    menuRegionSelect->addAction(actionStartRectSelect);
    menuRegionSelect->addAction(actionStartEllipseSelect);
    menuRegionSelect->addAction(actionStartPolygonSelect);
    menuRegionSelect->addAction(actionClearAllSelectiedRegion);
    menuRegionSelect->addSeparator();
    menuRegionSelect->addAction(actionSingleSelection);
    menuRegionSelect->addAction(actionAdditionalSelection);
    menuRegionSelect->addAction(actionSubtractionSelection);
    menuRegionSelect->addAction(actionIntersectionSelection);
    menuHelp->addAction(actionAbout);
    menuEdit->addAction(actionRescind);
    menuEdit->addAction(actionRedo);
    menuTool->addAction(actionProjectSetting);

    toolBar_chart->addAction(actionPickCurveToData);
    toolBar_chart->addAction(actionInRangDataRemove);
    toolBar_chart->addAction(actionOutRangDataRemove);
    toolBar_chartSet->addAction(actionEnableChartPicker);
    toolBar_chartSet->addAction(actionEnableChartPanner);
    toolBar_chartSet->addAction(actionEnableChartZoom);
    toolBar_chartSet->addAction(actionXYDataPicker);
    toolBar_chartSet->addAction(actionShowGrid);
    toolBar_chartSet->addAction(actionShowLegend);
    toolBar_chartSet->addAction(actionLegendPanel);
    toolBar_chartSet->addAction(actionChartSet);
    toolBar_plot->addAction(actionNewChart);
    toolBar_plot->addAction(actionNewTrend);
    toolBar_plot->addSeparator();

    retranslateUi(mainWinowPtr);
    QObject::connect(actionQuite, SIGNAL(triggered()), mainWinowPtr, SLOT(close()));

    tabWidget_valueViewer->setCurrentIndex(-1);


    QMetaObject::connectSlotsByName(mainWinowPtr);
}


void MainWindowPrivate::retranslateUi(QMainWindow *mainWinowPtr)
{
    mainWinowPtr->setWindowTitle(QApplication::translate("MainWindow", "signA-\344\277\241\345\217\267\345\244\204\347\220\206", 0));
    actionOpen->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266", 0));
#ifndef QT_NO_TOOLTIP
    actionOpen->setToolTip(QApplication::translate("MainWindow", "\346\211\223\345\274\200\346\225\260\346\215\256\346\272\220", 0));
#endif // QT_NO_TOOLTIP
    actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
    actionWindowCascade->setText(QApplication::translate("MainWindow", "\345\261\202\345\217\240\345\270\203\345\261\200", 0));
    actionWindowTile->setText(QApplication::translate("MainWindow", "\345\271\263\351\223\272\345\270\203\345\261\200", 0));
    actionWindowMode->setText(QApplication::translate("MainWindow", "\347\252\227\345\217\243\346\250\241\345\274\217", 0));
    actionTabMode->setText(QApplication::translate("MainWindow", "\346\240\207\347\255\276\346\250\241\345\274\217", 0));
    actionSave->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", 0));
    actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
    actionQuite->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", 0));
    actionDataFeatureDock->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\347\211\271\346\200\247", 0));
    actionNewChart->setText(QApplication::translate("MainWindow", "\346\226\260\345\233\276\344\276\213", 0));
#ifndef QT_NO_TOOLTIP
    actionNewChart->setToolTip(QApplication::translate("MainWindow", "\346\226\260\345\233\276\344\276\213-\346\240\271\346\215\256\347\224\250\346\210\267\345\256\232\344\271\211x,y\350\275\264\347\232\204\346\225\260\346\215\256\347\273\230\345\210\266\345\233\276\345\275\242", 0));
#endif // QT_NO_TOOLTIP
    actionNewTrend->setText(QApplication::translate("MainWindow", "\350\266\213\345\212\277\345\233\276", 0));
#ifndef QT_NO_TOOLTIP
    actionNewTrend->setToolTip(QApplication::translate("MainWindow", "\346\214\211\346\225\260\346\215\256\345\272\217\345\210\227\351\241\272\345\272\217\347\273\230\345\210\266\350\266\213\345\212\277", 0));
#endif // QT_NO_TOOLTIP
    actionClearProject->setText(QApplication::translate("MainWindow", "\346\270\205\351\231\244\346\226\271\346\241\210", 0));
    actionInRangDataRemove->setText(QApplication::translate("MainWindow", "\347\247\273\351\231\244\351\200\211\345\256\232\350\214\203\345\233\264\346\225\260\346\215\256", 0));
    actionOutRangDataRemove->setText(QApplication::translate("MainWindow", "\347\247\273\351\231\244\350\214\203\345\233\264\345\244\226\346\225\260\346\215\256", 0));
    actionExportReport->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220\346\212\245\350\241\250", 0));
    actionSubWindowListDock->setText(QApplication::translate("MainWindow", "\345\255\220\347\252\227\345\217\243\345\210\227\350\241\250", 0));
    actionEnableChartPicker->setText(QApplication::translate("MainWindow", "\345\215\201\345\255\227\345\205\211\346\240\207", 0));
    actionEnableChartPanner->setText(QApplication::translate("MainWindow", "\347\247\273\345\212\250", 0));
    actionEnableChartZoom->setText(QApplication::translate("MainWindow", "\350\214\203\345\233\264\347\274\251\346\224\276", 0));
    actionEnableChartZoom->setIconText(QApplication::translate("MainWindow", "\347\274\251\346\224\276", 0));
    actionYDataPicker->setText(QApplication::translate("MainWindow", "Y\345\200\274\346\230\276\347\244\272", 0));
    actionYDataPicker->setIconText(QApplication::translate("MainWindow", "\346\225\260\346\215\256", 0));
#ifndef QT_NO_TOOLTIP
    actionYDataPicker->setToolTip(QApplication::translate("MainWindow", "\345\234\250\345\233\276\350\241\250\344\270\212\346\230\276\347\244\272\351\274\240\346\240\207\347\273\217\350\277\207\346\233\262\347\272\277\347\232\204\346\225\260\346\215\256", 0));
#endif // QT_NO_TOOLTIP
    actionShowGrid->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\347\275\221\346\240\274", 0));
    actionShowGrid->setIconText(QApplication::translate("MainWindow", "\347\275\221\346\240\274", 0));
    actionShowHGrid->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\346\260\264\345\271\263\347\275\221\346\240\274", 0));
    actionShowHGrid->setIconText(QApplication::translate("MainWindow", "\346\260\264\345\271\263\347\275\221\346\240\274", 0));
#ifndef QT_NO_TOOLTIP
    actionShowHGrid->setToolTip(QApplication::translate("MainWindow", "\346\230\276\347\244\272/\351\232\220\350\227\217 \346\260\264\345\271\263\347\275\221\346\240\274\347\272\277", 0));
#endif // QT_NO_TOOLTIP
    actionShowVGrid->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\345\236\202\347\233\264\347\275\221\346\240\274", 0));
    actionShowVGrid->setIconText(QApplication::translate("MainWindow", "\345\236\202\347\233\264\347\275\221\346\240\274", 0));
#ifndef QT_NO_TOOLTIP
    actionShowVGrid->setToolTip(QApplication::translate("MainWindow", "\346\230\276\347\244\272/\351\232\220\350\227\217\345\236\202\347\233\264\347\275\221\346\240\274", 0));
#endif // QT_NO_TOOLTIP
    actionShowCrowdedHGrid->setText(QApplication::translate("MainWindow", "\345\257\206\351\233\206\346\260\264\345\271\263\347\275\221\346\240\274", 0));
#ifndef QT_NO_TOOLTIP
    actionShowCrowdedHGrid->setToolTip(QApplication::translate("MainWindow", "\346\230\276\347\244\272/\351\232\220\350\227\217 \345\257\206\351\233\206\346\260\264\345\271\263\347\275\221\346\240\274", 0));
#endif // QT_NO_TOOLTIP
    actionShowCrowdedVGrid->setText(QApplication::translate("MainWindow", "\345\257\206\351\233\206\345\236\202\347\233\264\347\275\221\346\240\274", 0));
    actionShowLegend->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272/\351\232\220\350\227\217 \345\233\276\344\276\213", 0));
    actionShowLegend->setIconText(QApplication::translate("MainWindow", "\345\233\276\344\276\213", 0));
#ifndef QT_NO_TOOLTIP
    actionShowLegend->setToolTip(QApplication::translate("MainWindow", "\346\230\276\347\244\272/\351\232\220\350\227\217 \345\233\276\344\276\213", 0));
#endif // QT_NO_TOOLTIP
    actionLegendPanel->setText(QApplication::translate("MainWindow", "\345\233\276\344\276\213\351\200\211\346\213\251\345\231\250", 0));
    actionLegendPanel->setIconText(QApplication::translate("MainWindow", "\345\233\276\344\276\213\351\200\211\346\213\251", 0));
    actionChartSet->setText(QApplication::translate("MainWindow", "\345\233\276\350\241\250\350\256\276\347\275\256", 0));
    actionChartZoomReset->setText(QApplication::translate("MainWindow", "\346\234\200\344\274\230\350\247\206\345\233\276", 0));
    actionPickCurveToData->setText(QApplication::translate("MainWindow", "\344\273\216\346\233\262\347\272\277\346\217\220\345\217\226\346\225\260\346\215\256", 0));
    actionAbout->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216", 0));
    actionRescind->setText(QApplication::translate("MainWindow", "\346\222\244\351\224\200", 0));
    actionRescind->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0));
    actionRedo->setText(QApplication::translate("MainWindow", "\351\207\215\345\201\232", 0));
    actionRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", 0));
    actionValueManagerDock->setText(QApplication::translate("MainWindow", "\345\217\230\351\207\217\347\256\241\347\220\206\347\252\227\345\217\243", 0));
    actionViewValueInCurrentTab->setText(QApplication::translate("MainWindow", "\345\234\250\345\275\223\345\211\215\346\240\207\347\255\276\351\241\265\346\265\217\350\247\210", 0));
    actionViewValueInNewTab->setText(QApplication::translate("MainWindow", "\345\234\250\346\226\260\346\240\207\347\255\276\351\241\265\346\265\217\350\247\210", 0));
    actionLayerOutDock->setText(QApplication::translate("MainWindow", "\345\233\276\345\261\202\347\256\241\347\220\206", 0));
    actionRenameValue->setText(QApplication::translate("MainWindow", "\351\207\215\345\221\275\345\220\215", 0));
#ifndef QT_NO_TOOLTIP
    actionRenameValue->setToolTip(QApplication::translate("MainWindow", "\351\207\215\345\221\275\345\220\215\345\217\230\351\207\217", 0));
#endif // QT_NO_TOOLTIP
    actionSetDefalutDockPos->setText(QApplication::translate("MainWindow", "\346\201\242\345\244\215\351\273\230\350\256\244\345\270\203\345\261\200", 0));
    actionValueViewerDock->setText(QApplication::translate("MainWindow", "\345\217\230\351\207\217\351\242\204\350\247\210", 0));
    actionFigureViewer->setText(QApplication::translate("MainWindow", "\345\233\276\345\275\242\347\225\214\351\235\242", 0));
    actionXYDataPicker->setText(QApplication::translate("MainWindow", "XY\345\200\274\346\215\225\350\216\267", 0));
    actionDeleteValue->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244\345\217\230\351\207\217", 0));
#ifndef QT_NO_TOOLTIP
    actionDeleteValue->setToolTip(QApplication::translate("MainWindow", "\345\210\240\351\231\244\351\200\211\344\270\255\347\232\204\345\217\230\351\207\217\357\274\214\351\234\200\345\234\250\345\217\230\351\207\217\347\256\241\347\220\206\347\233\256\345\275\225\344\270\255\351\200\211\344\270\255\351\234\200\350\246\201\345\210\240\351\231\244\347\232\204\345\217\230\351\207\217\357\274\214\345\271\266\350\277\233\350\241\214\346\223\215\344\275\234", 0));
#endif // QT_NO_TOOLTIP
    actionOpenProject->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\351\241\271\347\233\256", 0));
    actionSaveAs->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272", 0));
    actionProjectSetting->setText(QApplication::translate("MainWindow", "Project Setting", 0));
    actionZoomBase->setText(QApplication::translate("MainWindow", "ZoomBase", 0));
    actionZoomIn->setText(QApplication::translate("MainWindow", "ZoomIn", 0));
    actionZoomIn->setShortcut(QApplication::translate("MainWindow", "Ctrl+=", 0));
    actionZoomOut->setText(QApplication::translate("MainWindow", "ZoomOut", 0));
    actionZoomOut->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0));
    actionStartRectSelect->setText(QApplication::translate("MainWindow", "rectSelect", 0));
    actionStartEllipseSelect->setText(QApplication::translate("MainWindow", "ellipseSelect", 0));
    actionStartPolygonSelect->setText(QApplication::translate("MainWindow", "polygonSelect", 0));
    actionClearAllSelectiedRegion->setText(QApplication::translate("MainWindow", "clearAllSelect", 0));
    actionSingleSelection->setText(QApplication::translate("MainWindow", "singleSelection", 0));
    actionAdditionalSelection->setText(QApplication::translate("MainWindow", "additionalSelection", 0));
    actionIntersectionSelection->setText(QApplication::translate("MainWindow", "intersectionSelection", 0));
    actionSubtractionSelection->setText(QApplication::translate("MainWindow", "subtractionSelection", 0));
    menuFile->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", 0));
    menuExport->setTitle(QApplication::translate("MainWindow", "\345\257\274\345\207\272", 0));
    menuImport->setTitle(QApplication::translate("MainWindow", "\345\257\274\345\205\245", 0));
    menuWindow->setTitle(QApplication::translate("MainWindow", "\347\252\227\345\217\243", 0));
    menuWindowsViewSet->setTitle(QApplication::translate("MainWindow", "\347\252\227\345\217\243", 0));
    menuAnalysis->setTitle(QApplication::translate("MainWindow", "\345\210\206\346\236\220", 0));
    menuLineChart->setTitle(QApplication::translate("MainWindow", "\345\233\276\350\241\250", 0));
    menuData->setTitle(QApplication::translate("MainWindow", "\346\225\260\346\215\256", 0));
    menuChartDataManager->setTitle(QApplication::translate("MainWindow", "\345\233\276\347\272\277\346\225\260\346\215\256\346\223\215\344\275\234", 0));
    menuDataManager->setTitle(QApplication::translate("MainWindow", "\345\217\230\351\207\217\346\223\215\344\275\234", 0));
    menuChartSet->setTitle(QApplication::translate("MainWindow", "\345\233\276\350\241\250\346\223\215\344\275\234", 0));
    menuGrid->setTitle(QApplication::translate("MainWindow", "\347\275\221\346\240\274\350\256\276\347\275\256", 0));
    menuLegend->setTitle(QApplication::translate("MainWindow", "\345\233\276\344\276\213", 0));
    menuZoomSet->setTitle(QApplication::translate("MainWindow", "\347\274\251\346\224\276\346\223\215\344\275\234", 0));
    menuDataPickMenu->setTitle(QApplication::translate("MainWindow", "\346\225\260\346\215\256\346\230\276\347\244\272", 0));
    menuRegionSelect->setTitle(QApplication::translate("MainWindow", "\351\200\211\346\213\251", 0));
    menuHelp->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", 0));
    menuEdit->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\276\221", 0));
    menuTool->setTitle(QApplication::translate("MainWindow", "Tool", 0));
    dockWidget_DataFeature->setWindowTitle(QApplication::translate("MainWindow", "\346\225\260\346\215\256\347\211\271\346\200\247", 0));
    toolBar_chart->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    toolBar_chartSet->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    toolBar_plot->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    dockWidget_windowList->setWindowTitle(QApplication::translate("MainWindow", "\345\255\220\347\252\227\345\217\243\345\210\227\350\241\250", 0));
    dockWidget_valueManage->setWindowTitle(QApplication::translate("MainWindow", "\345\217\230\351\207\217\347\256\241\347\220\206", 0));
    dockWidget_plotLayer->setWindowTitle(QApplication::translate("MainWindow", "\345\233\276\345\261\202", 0));
    dockWidget_chartDataViewer->setWindowTitle(QApplication::translate("MainWindow", "\347\273\230\345\233\276\346\225\260\346\215\256\351\242\204\350\247\210", 0));
#ifndef QT_NO_TOOLTIP
    dockWidget_main->setToolTip(QString());
#endif // QT_NO_TOOLTIP
    dockWidget_main->setWindowTitle(QApplication::translate("MainWindow", "\344\270\273\350\247\206\345\233\276", 0));
    dockWidget_valueViewer->setWindowTitle(QApplication::translate("MainWindow", "\345\217\230\351\207\217\346\225\260\346\215\256\351\242\204\350\247\210", 0));
    dockWidget_message->setWindowTitle(QApplication::translate("MainWindow", "\344\277\241\346\201\257\347\252\227\345\217\243", 0));
    dockWidget_plotSet->setWindowTitle(QApplication::translate("MainWindow", "plot config", 0));
    toolBarChartTools->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));

    mainRibbonCategory->setWindowTitle(QApplication::translate("MainWindow", "Main", 0));
    mainCategoryFilePannel->setWindowTitle(QApplication::translate("MainWindow", "File", 0));
    viewRibbonCategory->setWindowTitle(QApplication::translate("MainWindow", "View", 0));
    viewCategoryWindowPannel->setWindowTitle(QApplication::translate("MainWindow", "Window", 0));
    windowModeCategoryWindowPannel->setWindowTitle(QApplication::translate("MainWindow", "Sub Window Mode", 0));
    operateCategorySelectEditorPannel->setWindowTitle(QApplication::translate("MainWindow", "Select Editor", 0));
}
