#include "MainWindowPrivate.h"
#include "mainwindow.h"
#include <QDebug>
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
    mainWinowPtr->setWindowIcon(QIcon(":/windowIcons/icons/windowIcon/figureWindow.svg"));
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
    actionOpen->setIcon(QIcon(":/icons/icons/open.png"));

    actionWindowCascade = new QAction(mainWinowPtr);
    actionWindowCascade->setObjectName(QStringLiteral("actionWindowCascade"));
    actionWindowCascade->setIcon(QIcon(":/icons/icons/windowCascade.png"));

    actionWindowTile = new QAction(mainWinowPtr);
    actionWindowTile->setObjectName(QStringLiteral("actionWindowTile"));
    actionWindowTile->setIcon(QIcon(":/icons/icons/windowTile.png"));

    actionWindowMode = new QAction(mainWinowPtr);
    actionWindowMode->setObjectName(QStringLiteral("actionWindowMode"));
    actionWindowMode->setCheckable(true);
    actionWindowMode->setIcon(QIcon(":/icons/icons/windowMode.png"));

    actionTabMode = new QAction(mainWinowPtr);
    actionTabMode->setObjectName(QStringLiteral("actionTabMode"));
    actionTabMode->setCheckable(true);
    actionTabMode->setIcon(QIcon(":/icons/icons/tabMode.png"));

    actionSave = new QAction(mainWinowPtr);
    actionSave->setObjectName(QStringLiteral("actionSave"));
    actionSave->setEnabled(true);
    actionSave->setIcon(QIcon(":/icons/icons/save.png"));

    actionQuit = new QAction(mainWinowPtr);
    actionQuit->setObjectName(QStringLiteral("actionQuite"));
    actionQuit->setIcon(QIcon(":/icons/icons/quit.svg"));

    actionDataFeatureDock = new QAction(mainWinowPtr);
    actionDataFeatureDock->setObjectName(QStringLiteral("actionDataFeatureDock"));
    actionDataFeatureDock->setIcon(QIcon(":/icons/icons/dataFeatureDock.png"));

    actionNewChart = new QAction(mainWinowPtr);
    actionNewChart->setObjectName(QStringLiteral("actionNewChart"));
    actionNewChart->setIcon(QIcon(":/icons/icons/newChart.png"));

    actionNewTrend = new QAction(mainWinowPtr);
    actionNewTrend->setObjectName(QStringLiteral("actionNewTrend"));
    actionNewTrend->setIcon(QIcon(":/icons/icons/newTrend.png"));

    actionClearProject = new QAction(mainWinowPtr);
    actionClearProject->setObjectName(QStringLiteral("actionClearProject"));
    actionClearProject->setIcon(QIcon(":/icons/icons/clearProject.png"));

    actionInRangDataRemove = new QAction(mainWinowPtr);
    actionInRangDataRemove->setObjectName(QStringLiteral("actionInRangDataRemove"));
    actionInRangDataRemove->setIcon(QIcon(":/icons/icons/inRangDataRemove.png"));

    actionOutRangDataRemove = new QAction(mainWinowPtr);
    actionOutRangDataRemove->setObjectName(QStringLiteral("actionOutRangDataRemove"));
    actionOutRangDataRemove->setIcon(QIcon(":/icons/icons/outRangDataRemove.png"));

    actionSubWindowListDock = new QAction(mainWinowPtr);
    actionSubWindowListDock->setObjectName(QStringLiteral("actionSubWindowListDock"));
    actionSubWindowListDock->setIcon(QIcon(":/icons/icons/subWindowListDock.png"));

    actionMessageInfoDock = new QAction(mainWinowPtr);
    actionMessageInfoDock->setObjectName(QStringLiteral("actionMessageInfoDock"));


    actionEnableChartCrossCursor = new QAction(mainWinowPtr);
    actionEnableChartCrossCursor->setObjectName(QStringLiteral("actionEnableChartPicker"));
    actionEnableChartCrossCursor->setIcon(QIcon(":/figureSet/icons/figureSet/crossCursor.svg"));

    actionEnableChartPanner = new QAction(mainWinowPtr);
    actionEnableChartPanner->setObjectName(QStringLiteral("actionEnableChartPanner"));
    actionEnableChartPanner->setIcon(QIcon(":/figureSet/icons/figureSet/pannerChart.png"));

    actionEnableChartZoom = new QAction(mainWinowPtr);
    actionEnableChartZoom->setObjectName(QStringLiteral("actionEnableChartZoom"));
    actionEnableChartZoom->setIcon(QIcon(":/figureSet/icons/figureSet/zoomChart.png"));

    actionYDataPicker = new QAction(mainWinowPtr);
    actionYDataPicker->setObjectName(QStringLiteral("actionYDataPicker"));
    actionYDataPicker->setIcon(QIcon(":/figureSet/icons/figureSet/yDataPicker.png"));

    actionXYDataPicker = new QAction(mainWinowPtr);
    actionXYDataPicker->setObjectName(QStringLiteral("actionXYDataPicker"));
    actionXYDataPicker->setIcon(QIcon(":/figureSet/icons/figureSet/xyDataPicker.png"));

    actionShowGrid = new QAction(mainWinowPtr);
    actionShowGrid->setObjectName(QStringLiteral("actionShowGrid"));
    actionShowGrid->setIcon(QIcon(":/figureSet/icons/figureSet/showGrid.png"));

    actionShowHGrid = new QAction(mainWinowPtr);
    actionShowHGrid->setObjectName(QStringLiteral("actionShowHGrid"));
    actionShowHGrid->setIcon(QIcon(":/figureSet/icons/figureSet/GridXOnly.png"));

    actionShowVGrid = new QAction(mainWinowPtr);
    actionShowVGrid->setObjectName(QStringLiteral("actionShowVGrid"));
    actionShowVGrid->setIcon(QIcon(":/figureSet/icons/figureSet/GridYOnly.png"));

    actionShowCrowdedHGrid = new QAction(mainWinowPtr);
    actionShowCrowdedHGrid->setObjectName(QStringLiteral("actionShowCrowdedHGrid"));
    actionShowCrowdedHGrid->setIcon(QIcon(":/figureSet/icons/figureSet/GridXMin.png"));

    actionShowCrowdedVGrid = new QAction(mainWinowPtr);
    actionShowCrowdedVGrid->setObjectName(QStringLiteral("actionShowCrowdedVGrid"));
    actionShowCrowdedVGrid->setIcon(QIcon(":/figureSet/icons/figureSet/GridYMin.png"));

    actionShowLegend = new QAction(mainWinowPtr);
    actionShowLegend->setObjectName(QStringLiteral("actionShowLegend"));
    actionShowLegend->setIcon(QIcon(":/figureSet/icons/figureSet/ShowLegend.png"));

    actionShowLegendPanel = new QAction(mainWinowPtr);
    actionShowLegendPanel->setObjectName(QStringLiteral("actionLegendPanel"));
    actionShowLegendPanel->setIcon(QIcon(":/figureSet/icons/figureSet/legendPanel.svg"));

    actionChartZoomReset = new QAction(mainWinowPtr);
    actionChartZoomReset->setObjectName(QStringLiteral("actionChartZoomReset"));
    actionChartZoomReset->setIcon(QIcon(":/figureSet/icons/figureSet/zoomChartReset.png"));

    actionPickCurveToData = new QAction(mainWinowPtr);
    actionPickCurveToData->setObjectName(QStringLiteral("actionPickCurveToData"));
    actionPickCurveToData->setIcon(QIcon(":/icons/icons/pickCurveToData.png"));

    actionAbout = new QAction(mainWinowPtr);
    actionAbout->setObjectName(QStringLiteral("actionAbout"));
    actionAbout->setIcon(QIcon(":/icons/icons/information.png"));

    actionUndo = new QAction(mainWinowPtr);
    actionUndo->setObjectName(QStringLiteral("actionRescind"));
    actionUndo->setIcon(QIcon(":/icons/icons/undo.png"));

    actionRedo = new QAction(mainWinowPtr);
    actionRedo->setObjectName(QStringLiteral("actionRedo"));
    actionRedo->setIcon(QIcon(":/icons/icons/redo.png"));

    actionValueManagerDock = new QAction(mainWinowPtr);
    actionValueManagerDock->setObjectName(QStringLiteral("actionValueManagerDock"));
    actionValueManagerDock->setIcon(QIcon(":/icons/icons/valueManagerDock.png"));

    actionFigureSetDock = new QAction(mainWinowPtr);
    actionFigureSetDock->setObjectName(QStringLiteral("actionValueManagerDock"));
    actionFigureSetDock->setIcon(QIcon(":/icons/icons/figureSet.png"));

    actionViewValueInCurrentTab = new QAction(mainWinowPtr);
    actionViewValueInCurrentTab->setObjectName(QStringLiteral("actionViewValueInCurrentTab"));
    actionViewValueInCurrentTab->setIcon(QIcon(":/icons/icons/viewValueInCurrentTab.png"));

    actionViewValueAppendInCurrentTab = new QAction(mainWinowPtr);
    actionViewValueAppendInCurrentTab->setObjectName(QStringLiteral("actionViewValueAppendInCurrentTab"));
    actionViewValueAppendInCurrentTab->setIcon(QIcon(":/icons/icons/viewValueInCurrentTab.png"));

    actionViewValueInNewTab = new QAction(mainWinowPtr);
    actionViewValueInNewTab->setObjectName(QStringLiteral("actionViewValueInNewTab"));
    actionViewValueInNewTab->setIcon(QIcon(":/icons/icons/viewValueInNewTab.png"));


    actionLayerOutDock = new QAction(mainWinowPtr);
    actionLayerOutDock->setObjectName(QStringLiteral("actionLayerOutDock"));
    actionLayerOutDock->setIcon(QIcon(":/icons/icons/layout.png"));

    actionRenameValue = new QAction(mainWinowPtr);
    actionRenameValue->setObjectName(QStringLiteral("actionRenameValue"));
    actionRenameValue->setIcon(QIcon(":/icons/icons/renameValue.png"));

    actionSetDefalutDockPos = new QAction(mainWinowPtr);
    actionSetDefalutDockPos->setObjectName(QStringLiteral("actionSetDefalutDockPos"));
    actionSetDefalutDockPos->setIcon(QIcon(":/icons/icons/setDefalutDockPos.png"));

    actionValueViewerDock = new QAction(mainWinowPtr);
    actionValueViewerDock->setObjectName(QStringLiteral("actionValueViewerDock"));
    actionValueViewerDock->setIcon(QIcon(":/icons/icons/valueViewerDock.png"));

    actionFigureViewer = new QAction(mainWinowPtr);
    actionFigureViewer->setObjectName(QStringLiteral("actionFigureViewer"));
    actionFigureViewer->setIcon(QIcon(":/icons/icons/figureViewer.png"));

    actionDeleteValue = new QAction(mainWinowPtr);
    actionDeleteValue->setObjectName(QStringLiteral("actionDeleteValue"));
    actionDeleteValue->setIcon(QIcon(":/icons/icons/deleteValue.png"));

    actionOpenProject = new QAction(mainWinowPtr);
    actionOpenProject->setObjectName(QStringLiteral("actionOpenProject"));
    actionOpenProject->setIcon(QIcon(":/icons/icons/openProject.png"));

    actionSaveAs = new QAction(mainWinowPtr);
    actionSaveAs->setObjectName(QStringLiteral("actionSaveAs"));
    actionSaveAs->setIcon(QIcon(":/icons/icons/saveAs.png"));

    actionProjectSetting = new QAction(mainWinowPtr);
    actionProjectSetting->setObjectName(QStringLiteral("actionProjectSetting"));
    actionProjectSetting->setIcon(QIcon(":/icons/icons/project.png"));

    actionSetZoomBase = new QAction(mainWinowPtr);
    actionSetZoomBase->setObjectName(QStringLiteral("actionZoomBase"));
    actionSetZoomBase->setIcon(QIcon(":/icons/icons/zoomBase.png"));

    actionZoomIn = new QAction(mainWinowPtr);
    actionZoomIn->setObjectName(QStringLiteral("actionZoomIn"));
    actionZoomIn->setIcon(QIcon(":/icons/icons/zoomIn.png"));

    actionZoomOut = new QAction(mainWinowPtr);
    actionZoomOut->setObjectName(QStringLiteral("actionZoomOut"));
    actionZoomOut->setIcon(QIcon(":/icons/icons/zoomOut.png"));

    actionZoomInBestView = new QAction(mainWinowPtr);
    actionZoomInBestView->setObjectName(QStringLiteral("actionZoomInBestView"));
    //actionZoomInBestView->setIcon(QIcon(":/icons/icons/zoomOut.png"));

    actionGroupSelection = new QActionGroup(mainWinowPtr);
    actionGroupSelection->setObjectName(QStringLiteral("actionGroupSelection"));
    actionGroupSelection->setExclusive(true);

    actionStartRectSelect = new QAction(mainWinowPtr);
    actionStartRectSelect->setIcon(QIcon(":/icons/icons/StartRectSelect.svg"));
    actionStartRectSelect->setObjectName(QStringLiteral("actionStartRectSelect"));
    actionStartRectSelect->setCheckable(true);
    actionGroupSelection->addAction(actionStartRectSelect);

    actionStartEllipseSelect = new QAction(mainWinowPtr);
    actionStartEllipseSelect->setIcon(QIcon(":/icons/icons/StartEllipseSelect.svg"));
    actionStartEllipseSelect->setObjectName(QStringLiteral("actionStartEllipseSelect"));
    actionStartEllipseSelect->setCheckable(true);
    actionGroupSelection->addAction(actionStartEllipseSelect);

    actionStartPolygonSelect = new QAction(mainWinowPtr);
    actionStartPolygonSelect->setIcon(QIcon(":/icons/icons/StartPolygonSelect.svg"));
    actionStartPolygonSelect->setObjectName(QStringLiteral("actionStartPolygonSelect"));
    actionStartPolygonSelect->setCheckable(true);
    actionGroupSelection->addAction(actionStartPolygonSelect);

    actionClearAllSelectiedRegion = new QAction(mainWinowPtr);
    actionClearAllSelectiedRegion->setObjectName(QStringLiteral("actionClearAllSelectiedRegion"));


    actionSingleSelection = new QAction(mainWinowPtr);
    actionSingleSelection->setObjectName(QStringLiteral("actionSingleSelection"));
    actionSingleSelection->setCheckable(true);
    actionSingleSelection->setChecked(true);
    actionSingleSelection->setMenuRole(QAction::TextHeuristicRole);
    actionSingleSelection->setIcon(QIcon(":/icons/icons/SingleSelection.png"));

    actionAdditionalSelection = new QAction(mainWinowPtr);
    actionAdditionalSelection->setObjectName(QStringLiteral("actionAdditionalSelection"));
    actionAdditionalSelection->setCheckable(true);
    actionAdditionalSelection->setIcon(QIcon(":/icons/icons/AdditionalSelection.png"));

    actionIntersectionSelection = new QAction(mainWinowPtr);
    actionIntersectionSelection->setObjectName(QStringLiteral("actionIntersectionSelection"));
    actionIntersectionSelection->setCheckable(true);
    actionIntersectionSelection->setIcon(QIcon(":/icons/icons/IntersectionSelection.png"));

    actionSubtractionSelection = new QAction(mainWinowPtr);
    actionSubtractionSelection->setObjectName(QStringLiteral("actionSubtractionSelection"));
    actionSubtractionSelection->setCheckable(true);
    actionSubtractionSelection->setIcon(QIcon(":/icons/icons/SubtractionSelection.png"));

    actionDrawScatterChart = new QAction(mainWinowPtr);
    actionDrawScatterChart->setObjectName(QStringLiteral("actionDrawScatterChart"));
    //actionDrawScatterChart->setIcon(QIcon(":/icons/icons/SubtractionSelection.png"));

    actionDrawBarChart = new QAction(mainWinowPtr);
    actionDrawBarChart->setObjectName(QStringLiteral("actionDrawBarChart"));
    //actionDrawBarChart->setIcon(QIcon(":/icons/icons/SubtractionSelection.png"));

    actionDrawHistogramChart = new QAction(mainWinowPtr);
    actionDrawHistogramChart->setObjectName(QStringLiteral("actionDrawHistogramChart"));

    actionDrawBoxChart = new QAction(mainWinowPtr);
    actionDrawBoxChart->setObjectName(QStringLiteral("actionDrawBoxChart"));
    //actionDrawBoxChart->setIcon(QIcon(":/icons/icons/SubtractionSelection.png"));

    actionSelectionRegionMove = new QAction(mainWinowPtr);
    actionSelectionRegionMove->setObjectName(QStringLiteral("actionSelectionRegionMove"));
    actionSelectionRegionMove->setCheckable(true);
    actionSelectionRegionMove->setIcon(QIcon(":/icons/icons/selectionRegionMove.png"));

    actionSelectionRegionDataMove = new QAction(mainWinowPtr);
    actionSelectionRegionDataMove->setObjectName(QStringLiteral("actionSelectionRegionDataMove"));
    actionSelectionRegionDataMove->setCheckable(true);
    actionSelectionRegionDataMove->setIcon(QIcon(":/icons/icons/selectionRegionDataMove.png"));

    actionChartEditor = new QActionGroup(mainWinowPtr);
    actionChartEditor->setExclusive(true);
    actionChartEditor->addAction(actionSelectionRegionMove);
    actionChartEditor->addAction(actionSelectionRegionDataMove);

    actionSelectSkin = new QAction(mainWinowPtr);
    actionSelectSkin->setObjectName(QStringLiteral("actionSelectSkin"));
    actionSelectSkin->setIcon(QIcon(":/icons/icons/skin.png"));
    actionGroupSkins = new QActionGroup(mainWinowPtr);
    actionGroupSkins->setObjectName(QStringLiteral("actionGroupSkins"));
    actionGroupSkins->setExclusive(true);


    menuBar = mainWinowPtr->ribbonBar();
    menuBar->setObjectName(QStringLiteral("menuBar"));

    menuFile = new SARibbonMenu(menuBar);
    menuFile->setObjectName(QStringLiteral("menuFile"));

    menuExport = new SARibbonMenu(menuFile);
    menuExport->setObjectName(QStringLiteral("menu_export"));
    menuExport->setIcon(QIcon(":/icons/icons/export.png"));

    menuImport = new SARibbonMenu(menuFile);
    menuImport->setObjectName(QStringLiteral("menu_import"));
    menuImport->setIcon(QIcon(":/icons/icons/import.png"));


    menuWindowsViewSet = new SARibbonMenu(menuBar);
    menuWindowsViewSet->setObjectName(QStringLiteral("menu_windowsViewSet"));
    QIcon icon46;
    icon46.addFile(QStringLiteral(":/icons/icons/windowViewSet.png"), QSize(), QIcon::Normal, QIcon::Off);
    menuWindowsViewSet->setIcon(icon46);

    menuAnalysis = new SARibbonMenu(menuBar);
    menuAnalysis->setObjectName(QStringLiteral("menuAnalysis"));
    menuAnalysis->setIcon(QIcon(":/icons/icons/allFunction.svg"));

    menuLineChart = new SARibbonMenu(menuBar);
    menuLineChart->setObjectName(QStringLiteral("menuLineChart"));
    menuLineChart->setIcon(QIcon(":/icons/icons/lineChart.svg"));

    menuScatterChart = new SARibbonMenu(menuBar);
    menuScatterChart->setObjectName(QStringLiteral("menuScatterChart"));
    menuScatterChart->setIcon(QIcon(":/icons/icons/scatterChart.svg"));

    menuBarChart = new SARibbonMenu(menuBar);
    menuBarChart->setObjectName(QStringLiteral("menuBarChart"));
    menuBarChart->setIcon(QIcon(":/icons/icons/barChart.svg"));

    menuHistogramChart = new SARibbonMenu(menuBar);
    menuHistogramChart->setObjectName(QStringLiteral("menuHistogramChart"));
    menuHistogramChart->setIcon(QIcon(":/icons/icons/histogramChart.svg"));

    menuBoxChart = new SARibbonMenu(menuBar);
    menuBoxChart->setObjectName(QStringLiteral("menuBoxChart"));
    menuBoxChart->setIcon(QIcon(":/icons/icons/boxChart.svg"));

    menuDataManager = new SARibbonMenu(menuBar);
    menuDataManager->setObjectName(QStringLiteral("menu_dataManager"));
    menuChartSet = new SARibbonMenu(menuBar);
    menuChartSet->setObjectName(QStringLiteral("menu_chartSet"));
    menuGrid = new SARibbonMenu(menuBar);
    menuGrid->setObjectName(QStringLiteral("menu_grid"));
    //menuGrid->setIcon(icon18);

    menuZoomSet = new SARibbonMenu(menuBar);
    menuZoomSet->setObjectName(QStringLiteral("menuZoomSet"));

    menuSkinList = new SARibbonMenu(menuBar);
    menuSkinList->setObjectName(QStringLiteral("menuSkinList"));

    menuRegionSelect = new SARibbonMenu(menuBar);
    menuRegionSelect->setObjectName(QStringLiteral("menuRegionSelect"));
    menuHelp = new SARibbonMenu(menuBar);
    menuHelp->setObjectName(QStringLiteral("menuHelp"));
    menuEdit = new SARibbonMenu(menuBar);
    menuEdit->setObjectName(QStringLiteral("menuEdit"));
    menuTool = new SARibbonMenu(menuBar);
    menuTool->setObjectName(QStringLiteral("menu_tool"));


//=======start ribbon set=============================================================================================
    ribbonApplicationButton = qobject_cast<SARibbonApplicationButton*>(menuBar->applitionButton());
    ribbonRightTopBar = menuBar->activeTabBarRightButtonGroup();
    ribbonRightTopBar->addButton(actionAbout);
    //![1] Main Category Page
    mainRibbonCategory = menuBar->addCategoryPage(QStringLiteral("main"));
    mainRibbonCategory->setObjectName(QStringLiteral("mainRibbonCategory"));
    //File Pannel
    mainCategoryFilePannel = mainRibbonCategory->addPannel(QStringLiteral("File"));
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
    menuFile->addAction(actionQuit);
    ribbonButtonFileOpen->setMenu(menuFile);

    ribbonButtonOpen = mainCategoryFilePannel->addSmallAction(actionOpen);
    ribbonButtonSave = mainCategoryFilePannel->addSmallAction(actionSave);
    ribbonButtonClearProject = mainCategoryFilePannel->addSmallAction(actionClearProject);

    //Chart Pannel
    mainCategoryChartPannel = mainRibbonCategory->addPannel(QStringLiteral("Chart"));
    ribbonButtonNewChart = mainCategoryChartPannel->addLargeAction(actionNewChart);
    menuLineChart->addAction(actionNewTrend);
    ribbonButtonLineChart = mainCategoryChartPannel->addLargeMenu(menuLineChart);
    ribbonButtonScatterChart = mainCategoryChartPannel->addLargeMenu(menuScatterChart);
    ribbonButtonBarChart = mainCategoryChartPannel->addLargeMenu(menuBarChart);
    ribbonButtonHistogramChart = mainCategoryChartPannel->addLargeMenu(menuHistogramChart);
    ribbonButtonBoxChart = mainCategoryChartPannel->addLargeMenu(menuBoxChart);


    //! operate Category Page
    operateRibbonCategory = menuBar->addCategoryPage(QStringLiteral("Operate"));
    operateRibbonCategory->setObjectName(QStringLiteral("operateRibbonCategory"));
    //Selector Editor
    operateCategoryChartEditorPannel = operateRibbonCategory->addPannel(QStringLiteral("Select Editor"));
    operateCategoryChartEditorPannel->setObjectName(QStringLiteral("operateCategorySelectEditorPannel"));

    ribbonButtonStartSelection = operateCategoryChartEditorPannel->addLargeAction(actionStartRectSelect);
    menuSelection = new SARibbonMenu(Parent);
    menuSelection->addAction(actionStartRectSelect);
    menuSelection->addAction(actionStartEllipseSelect);
    menuSelection->addAction(actionStartPolygonSelect);
    menuSelection->addSeparator();
    menuSelection->addAction(actionClearAllSelectiedRegion);

    ribbonButtonStartSelection->setMenu(menuSelection);
    ribbonButtonStartSelection->setObjectName(QStringLiteral("ribbonButtonStartSelection"));
    ribbonButtonStartSelection->setPopupMode(QToolButton::MenuButtonPopup);


    ribbonButtonGroupSelectionMode = new SARibbonButtonGroupWidget(Parent);
    ribbonButtonSingleSelection = ribbonButtonGroupSelectionMode->addButton(actionSingleSelection);
    ribbonButtonAdditionalSelection = ribbonButtonGroupSelectionMode->addButton(actionAdditionalSelection);
    ribbonButtonSubtractionSelection = ribbonButtonGroupSelectionMode->addButton(actionSubtractionSelection);
    ribbonButtonIntersectionSelection = ribbonButtonGroupSelectionMode->addButton(actionIntersectionSelection);
    operateCategoryChartEditorPannel->addWidget(ribbonButtonGroupSelectionMode
                                                 ,0,3);

    SARibbonButtonGroupWidget* tmpButtonGroup = new SARibbonButtonGroupWidget(Parent);
    ribbonButtonSelectionRegionMove = tmpButtonGroup->addButton(actionSelectionRegionMove);
    operateCategoryChartEditorPannel->addWidget(tmpButtonGroup,3,3);

    operateCategoryChartEditorPannel->addSeparator();

    menuDataRemove = new SARibbonMenu(Parent);
    menuDataRemove->addAction(actionOutRangDataRemove);
    ribbonButtonInRangDataRemove = operateCategoryChartEditorPannel->addLargeAction(actionInRangDataRemove);
    ribbonButtonInRangDataRemove->setMenu(menuDataRemove);
    ribbonButtonInRangDataRemove->setPopupMode(QToolButton::MenuButtonPopup);
    ribbonButtonPickCurveToData = operateCategoryChartEditorPannel->addLargeAction(actionPickCurveToData);

    ribbonButtonSelectionRegionDataMove = operateCategoryChartEditorPannel->addLargeAction(actionSelectionRegionDataMove);

    //data view editor
    operateCategoryDataViewPannel = operateRibbonCategory->addPannel(QStringLiteral("Data View"));
    operateCategoryDataViewPannel->setObjectName(QStringLiteral("operateCategorySelectEditorPannel"));
    ribbonButtonChartCrossCursor = operateCategoryDataViewPannel->addLargeAction(actionEnableChartCrossCursor);
    ribbonButtonChartPanner = operateCategoryDataViewPannel->addLargeAction(actionEnableChartPanner);
    ribbonButtonChartZoom = operateCategoryDataViewPannel->addLargeActionMenu(actionEnableChartZoom,menuZoomSet);
    ribbonButtonXYDataPicker = operateCategoryDataViewPannel->addSmallAction(actionXYDataPicker);
    ribbonButtonYDataPicker = operateCategoryDataViewPannel->addSmallAction(actionYDataPicker);

    //legend pannel
    chartLegendCategoryWindowPannel = operateRibbonCategory->addPannel("legend");
    ribbonButtonShowLegend = chartLegendCategoryWindowPannel->addLargeAction(actionShowLegend);
    ribbonButtonShowLegendPanel = chartLegendCategoryWindowPannel->addLargeAction(actionShowLegendPanel);
    //Grid pannel
    chartGridCategoryWindowPannel = operateRibbonCategory->addPannel("Grid");
    ribbonButtonShowGrid = chartGridCategoryWindowPannel->addLargeAction(actionShowGrid);
    ribbonButtonShowHGrid = chartGridCategoryWindowPannel->addSmallAction(actionShowHGrid);
    ribbonButtonShowVGrid = chartGridCategoryWindowPannel->addSmallAction(actionShowVGrid);
    menuShowCrowdedGrid = new SARibbonMenu(menuBar);
    menuShowCrowdedGrid->setObjectName(QStringLiteral("menuShowCrowdedGrid"));
    menuShowCrowdedGrid->setIcon(QIcon(":/figureSet/icons/figureSet/GridXOnly.png"));
    menuShowCrowdedGrid->addAction(actionShowCrowdedHGrid);
    menuShowCrowdedGrid->addAction(actionShowCrowdedVGrid);
    ribbonButtonShowCrowdedGrid = chartGridCategoryWindowPannel->addSmallMenu(menuShowCrowdedGrid);
    ribbonButtonShowCrowdedGrid->setIcon(QIcon(":/figureSet/icons/figureSet/GridXOnly.png"));
    //! Analysis
    analysisRibbonCategory = menuBar->addCategoryPage(QStringLiteral("Analysis"));
    analysisRibbonCategory->setObjectName(QStringLiteral("analysisRibbonCategory"));
    //Analysis Pannel
    analysisCategoryFunctionPannel = analysisRibbonCategory->addPannel(QStringLiteral("Function"));
    ribbonButtonAllFuntion = analysisCategoryFunctionPannel->addLargeMenu(menuAnalysis);
    ribbonGalleryFuntions = analysisCategoryFunctionPannel->addGallery();
    //! View Category Page
    viewRibbonCategory = menuBar->addCategoryPage(QStringLiteral("View"));
    viewRibbonCategory->setObjectName(QStringLiteral("viewRibbonCategory"));
    //Window Pannel
    viewCategoryWindowPannel = viewRibbonCategory->addPannel(QStringLiteral("Window"));
    ribbonButtonAllDock = viewCategoryWindowPannel->addLargeToolButton(QStringLiteral("Dock\nView"),QIcon(":/icons/icons/AllDock.png"),QToolButton::InstantPopup);
    menuWindowsViewSet->addAction(actionFigureViewer);
    menuWindowsViewSet->addAction(actionFigureSetDock);
    menuWindowsViewSet->addAction(actionDataFeatureDock);
    menuWindowsViewSet->addAction(actionSubWindowListDock);
    menuWindowsViewSet->addAction(actionLayerOutDock);
    menuWindowsViewSet->addAction(actionValueManagerDock);
    menuWindowsViewSet->addAction(actionValueViewerDock);
    menuWindowsViewSet->addAction(actionMessageInfoDock);
    ribbonButtonAllDock->setMenu(menuWindowsViewSet);
    ribbonButtonFigureViewer = viewCategoryWindowPannel->addSmallAction(actionFigureViewer);
    ribbonButtonFigureSetDock = viewCategoryWindowPannel->addSmallAction(actionFigureSetDock);
    ribbonButtonDataFeatureDock = viewCategoryWindowPannel->addSmallAction(actionDataFeatureDock);
    ribbonButtonLayerOutDock = viewCategoryWindowPannel->addSmallAction(actionLayerOutDock);
    ribbonButtonValueViewerDock = viewCategoryWindowPannel->addSmallAction(actionValueViewerDock);
    ribbonButtonValueManagerDock = viewCategoryWindowPannel->addSmallAction(actionValueManagerDock);
    ribbonButtonSetDefalutDockPos = viewCategoryWindowPannel->addLargeAction(actionSetDefalutDockPos);
    //sub window mode
    windowModeCategoryWindowPannel = viewRibbonCategory->addPannel(QStringLiteral("Sub Window Mode"));
    ribbonButtonTabMode = windowModeCategoryWindowPannel->addLargeAction(actionTabMode);
    ribbonButtonWindowMode = windowModeCategoryWindowPannel->addLargeAction(actionWindowMode);
    ribbonButtonWindowCascade = windowModeCategoryWindowPannel->addMediumAction(actionWindowCascade);
    ribbonButtonWindowTile = windowModeCategoryWindowPannel->addMediumAction(actionWindowTile);

    menuBar->quickAccessBar()->setEnableShowIcon(false);
    menuBar->quickAccessBar()->addSeparator();
    menuBar->quickAccessBar()->addButton(actionSave);
    menuBar->quickAccessBar()->addSeparator();
    menuBar->quickAccessBar()->addButton(actionUndo);
    menuBar->quickAccessBar()->addButton(actionRedo);
    menuBar->quickAccessBar()->addSeparator();
    SARibbonToolButton* ribbonToobBtnSelectSkin = menuBar->quickAccessBar()->addButton(actionSelectSkin);
    ribbonToobBtnSelectSkin->setMenu(menuSkinList);
    ribbonToobBtnSelectSkin->setPopupMode(QToolButton::InstantPopup);
    menuBar->quickAccessBar()->addSeparator();
    menuBar->setRibbonStyle(SARibbonBar::WpsLiteStyle);
    //menuBar->repaint();
//=======end ribbon set=======================================================================================

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
    figureLayoutWidget = new SAFigureLayoutWidget(dockWidget_plotLayer);
    figureLayoutWidget->setObjectName(QStringLiteral("SAFigureLayoutWidget"));
    dockWidget_plotLayer->setWidget(figureLayoutWidget);

    mainWinowPtr->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_plotLayer);
    dockWidget_chartDataViewer = new QDockWidget(mainWinowPtr);
    dockWidget_chartDataViewer->setObjectName(QStringLiteral("dockWidget_chartDataViewer"));

    chartDatasViewWidget = new SAChartDatasViewWidget();
    chartDatasViewWidget->setObjectName(QStringLiteral("chartDatasViewWidget"));
    dockWidget_chartDataViewer->setWidget(chartDatasViewWidget);

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










    menuDataManager->addAction(actionViewValueInCurrentTab);
    menuDataManager->addAction(actionViewValueAppendInCurrentTab);
    menuDataManager->addAction(actionViewValueInNewTab);
    menuDataManager->addSeparator();
    menuDataManager->addAction(actionDeleteValue);
    menuDataManager->addAction(actionRenameValue);

    menuChartSet->addAction(actionEnableChartCrossCursor);
    menuChartSet->addAction(actionEnableChartPanner);
    menuChartSet->addAction(menuGrid->menuAction());
    menuChartSet->addAction(menuZoomSet->menuAction());
    menuChartSet->addAction(menuRegionSelect->menuAction());

    menuGrid->addAction(actionShowGrid);
    menuGrid->addSeparator();
    menuGrid->addAction(actionShowHGrid);
    menuGrid->addAction(actionShowVGrid);
    menuGrid->addSeparator();
    menuGrid->addAction(actionShowCrowdedHGrid);
    menuGrid->addAction(actionShowCrowdedVGrid);

    menuZoomSet->addAction(actionEnableChartZoom);
    menuZoomSet->addSeparator();
    menuZoomSet->addAction(actionZoomIn);
    menuZoomSet->addAction(actionZoomOut);
    menuZoomSet->addAction(actionChartZoomReset);
    menuZoomSet->addAction(actionZoomInBestView);
    menuZoomSet->addSeparator();
    menuZoomSet->addAction(actionSetZoomBase);

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

    menuEdit->addAction(actionUndo);
    menuEdit->addAction(actionRedo);

    menuTool->addAction(actionProjectSetting);

    menuScatterChart->addAction(actionDrawScatterChart);

    menuBarChart->addAction(actionDrawBarChart);

    menuHistogramChart->addAction(actionDrawHistogramChart);

    menuBoxChart->addAction(actionDrawBoxChart);
//    toolBar_chart->addAction(actionPickCurveToData);
//    toolBar_chart->addAction(actionInRangDataRemove);
//    toolBar_chart->addAction(actionOutRangDataRemove);
//    toolBar_chartSet->addAction(actionEnableChartPicker);
//    toolBar_chartSet->addAction(actionEnableChartPanner);
//    toolBar_chartSet->addAction(actionEnableChartZoom);
//    toolBar_chartSet->addAction(actionXYDataPicker);
//    toolBar_chartSet->addAction(actionShowGrid);
//    toolBar_chartSet->addAction(actionShowLegend);
//    toolBar_chartSet->addAction(actionLegendPanel);
//    toolBar_chartSet->addAction(actionChartSet);
//    toolBar_plot->addAction(actionNewChart);
//    toolBar_plot->addAction(actionNewTrend);
//    toolBar_plot->addSeparator();





    retranslateUi(mainWinowPtr);
    QObject::connect(actionQuit, SIGNAL(triggered()), mainWinowPtr, SLOT(close()));

    tabWidget_valueViewer->setCurrentIndex(-1);


    QMetaObject::connectSlotsByName(mainWinowPtr);
}


void MainWindowPrivate::retranslateUi(MainWindow *mainWinowPtr)
{
    mainWinowPtr->setWindowTitle(QApplication::translate("MainWindow", "SA - Signal Analysis", 0));
    actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
#ifndef QT_NO_TOOLTIP
    actionOpen->setToolTip(QApplication::translate("MainWindow", "Open File", 0));
#endif // QT_NO_TOOLTIP
    actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
    actionWindowCascade->setText(QApplication::translate("MainWindow", "Cascade", 0));
    actionWindowTile->setText(QApplication::translate("MainWindow", "Tile", 0));
    actionWindowMode->setText(QApplication::translate("MainWindow", "Window\nMode", 0));
    actionTabMode->setText(QApplication::translate("MainWindow", "Tab \n Mode", 0));
    actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
    actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
    actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
    actionDataFeatureDock->setText(QApplication::translate("MainWindow", "Data Feature", 0));
    actionNewChart->setText(QApplication::translate("MainWindow", "New\nChart", 0));
#ifndef QT_NO_TOOLTIP
    actionNewChart->setToolTip(QApplication::translate("MainWindow", "New Chart with dialog", 0));
#endif // QT_NO_TOOLTIP
    actionNewTrend->setText(QApplication::translate("MainWindow", "Trend Chart", 0));
#ifndef QT_NO_TOOLTIP
    actionNewTrend->setToolTip(QApplication::translate("MainWindow", "Plot Trend With User Select Data Value", 0));
#endif // QT_NO_TOOLTIP
    actionClearProject->setText(QApplication::translate("MainWindow", "Clear", 0));
    actionInRangDataRemove->setText(QApplication::translate("MainWindow", "remove\nin", 0));
    actionOutRangDataRemove->setText(QApplication::translate("MainWindow", "remove\nout", 0));

    actionSubWindowListDock->setText(QApplication::translate("MainWindow", "SubWindow\nList", 0));
    actionEnableChartCrossCursor->setText(QApplication::translate("MainWindow", "Cross", 0));
    actionEnableChartPanner->setText(QApplication::translate("MainWindow", "Panner", 0));
    actionEnableChartZoom->setText(QApplication::translate("MainWindow", "Zoom", 0));
    actionEnableChartZoom->setIconText(QApplication::translate("MainWindow", "Zoom", 0));
    actionYDataPicker->setText(QApplication::translate("MainWindow", "Y Picker", 0));
    actionYDataPicker->setIconText(QApplication::translate("MainWindow", "Y Picker", 0));
    actionXYDataPicker->setText(QApplication::translate("MainWindow", "XY Picker", 0));
#ifndef QT_NO_TOOLTIP
    actionYDataPicker->setToolTip(QApplication::translate("MainWindow", "Pick Y Data in Chart", 0));
#endif // QT_NO_TOOLTIP
    actionShowGrid->setText(QApplication::translate("MainWindow", "Grid", 0));
    actionShowGrid->setIconText(QApplication::translate("MainWindow", "Grid", 0));
    actionShowHGrid->setText(QApplication::translate("MainWindow", "H Grid", 0));
    actionShowHGrid->setIconText(QApplication::translate("MainWindow", "H Grid", 0));
#ifndef QT_NO_TOOLTIP
    actionShowHGrid->setToolTip(QApplication::translate("MainWindow", "Horizontal Grid", 0));
#endif // QT_NO_TOOLTIP
    actionShowVGrid->setText(QApplication::translate("MainWindow", "V Grid", 0));
    actionShowVGrid->setIconText(QApplication::translate("MainWindow", "V Grid", 0));
#ifndef QT_NO_TOOLTIP
    actionShowVGrid->setToolTip(QApplication::translate("MainWindow", "Vertical Grid", 0));
#endif // QT_NO_TOOLTIP
    actionShowCrowdedHGrid->setText(QApplication::translate("MainWindow", "Crowded H Grid", 0));
#ifndef QT_NO_TOOLTIP
    actionShowCrowdedHGrid->setToolTip(QApplication::translate("MainWindow", "Crowded\nH Grid", 0));
#endif // QT_NO_TOOLTIP
    actionShowCrowdedVGrid->setText(QApplication::translate("MainWindow", "Crowded\nV Grid", 0));
    actionShowLegend->setText(QApplication::translate("MainWindow", "Legend", 0));
    actionShowLegend->setIconText(QApplication::translate("MainWindow", "Legend", 0));
#ifndef QT_NO_TOOLTIP
    actionShowLegend->setToolTip(QApplication::translate("MainWindow", "Show/Hide Legend", 0));
#endif // QT_NO_TOOLTIP
    actionShowLegendPanel->setText(QApplication::translate("MainWindow", "Legend\nPanel", 0));
    actionShowLegendPanel->setIconText(QApplication::translate("MainWindow", "Legend\nPanel", 0));
    actionChartZoomReset->setText(QApplication::translate("MainWindow", "Zoom Reset", 0));
    actionPickCurveToData->setText(QApplication::translate("MainWindow", "Pick\nData", 0));
    actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
    actionUndo->setText(QApplication::translate("MainWindow", "Rescind", 0));
    actionUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0));
    actionRedo->setText(QApplication::translate("MainWindow", "Redo", 0));
    actionRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", 0));
    actionValueManagerDock->setText(QApplication::translate("MainWindow", "Value Manager", 0));
    actionViewValueInCurrentTab->setText(QApplication::translate("MainWindow", "View Value In\nCurrent Tab", 0));
    actionViewValueAppendInCurrentTab->setText(QApplication::translate("MainWindow", "View Value Append\nIn Current Tab", 0));
    actionViewValueInNewTab->setText(QApplication::translate("MainWindow", "View Value In\nNew Tab", 0));
    actionLayerOutDock->setText(QApplication::translate("MainWindow", "Layerout", 0));
    actionRenameValue->setText(QApplication::translate("MainWindow", "Rename Value", 0));
#ifndef QT_NO_TOOLTIP
    actionRenameValue->setToolTip(QApplication::translate("MainWindow", "Rename Value", 0));
#endif // QT_NO_TOOLTIP
    actionSetDefalutDockPos->setText(QApplication::translate("MainWindow", "Defalut\nView", 0));
    actionValueViewerDock->setText(QApplication::translate("MainWindow", "Value View", 0));
    actionFigureViewer->setText(QApplication::translate("MainWindow", "Figure View", 0));
    actionMessageInfoDock->setText(QApplication::translate("MainWindow", "Message View", 0));
    actionDeleteValue->setText(QApplication::translate("MainWindow", "Delete Value", 0));
#ifndef QT_NO_TOOLTIP
    actionDeleteValue->setToolTip(QApplication::translate("MainWindow", "Delete Value", 0));
    actionSelectSkin->setToolTip(QApplication::translate("MainWindow", "select skin", 0));
#endif // QT_NO_TOOLTIP
    actionOpenProject->setText(QApplication::translate("MainWindow", "Open\nProject", 0));
    actionSaveAs->setText(QApplication::translate("MainWindow", "Save As", 0));
    actionProjectSetting->setText(QApplication::translate("MainWindow", "Project Setting", 0));
    actionSetZoomBase->setText(QApplication::translate("MainWindow", "Set Zoom Base", 0));
    actionZoomIn->setText(QApplication::translate("MainWindow", "Zoom In", 0));
    actionZoomIn->setShortcut(QApplication::translate("MainWindow", "Ctrl+=", 0));
    actionZoomOut->setText(QApplication::translate("MainWindow", "Zoom Out", 0));
    actionZoomOut->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0));
    actionZoomInBestView->setText(QApplication::translate("MainWindow", "Best View", 0));
    actionStartRectSelect->setText(QApplication::translate("MainWindow", "Rect\nSelect", 0));
    actionStartEllipseSelect->setText(QApplication::translate("MainWindow", "Ellipse\nSelect", 0));
    actionStartPolygonSelect->setText(QApplication::translate("MainWindow", "Polygon\nSelect", 0));
    actionClearAllSelectiedRegion->setText(QApplication::translate("MainWindow", "Clear\nSelect", 0));
    actionSingleSelection->setText(QApplication::translate("MainWindow", "New Select", 0));
    actionAdditionalSelection->setText(QApplication::translate("MainWindow", "Add Select", 0));
    actionIntersectionSelection->setText(QApplication::translate("MainWindow", "Int Select", 0));
    actionSubtractionSelection->setText(QApplication::translate("MainWindow", "Sub Select", 0));
    actionDrawScatterChart->setText(QApplication::translate("MainWindow", "Scatter", 0));
    actionDrawBarChart->setText(QApplication::translate("MainWindow", "Bar", 0));
    actionDrawHistogramChart->setText(QApplication::translate("MainWindow", "Histogram", 0));
    actionDrawBoxChart->setText(QApplication::translate("MainWindow", "Box", 0));
    actionSelectionRegionMove->setText(QApplication::translate("MainWindow", "Transform", 0));
    actionSelectionRegionDataMove->setText(QApplication::translate("MainWindow", "Move\nDatas", 0));
    actionFigureSetDock->setText(QApplication::translate("MainWindow", "Figure Setting", 0));
    menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    menuExport->setTitle(QApplication::translate("MainWindow", "Export", 0));
    menuImport->setTitle(QApplication::translate("MainWindow", "Import", 0));
    menuWindowsViewSet->setTitle(QApplication::translate("MainWindow", "Windows\nView", 0));
    menuAnalysis->setTitle(QApplication::translate("MainWindow", "Analysis", 0));
    menuDataManager->setTitle(QApplication::translate("MainWindow", "Data Manager", 0));
    menuChartSet->setTitle(QApplication::translate("MainWindow", "Chart Set", 0));
    menuGrid->setTitle(QApplication::translate("MainWindow", "Grid", 0));
    menuZoomSet->setTitle(QApplication::translate("MainWindow", "Zoom Set", 0));

    menuRegionSelect->setTitle(QApplication::translate("MainWindow", "Select", 0));
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
    menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
    menuTool->setTitle(QApplication::translate("MainWindow", "Tool", 0));
    menuLineChart->setTitle(QApplication::translate("MainWindow", "Line", 0));
    menuScatterChart->setTitle(QApplication::translate("MainWindow", "Scatter", 0));
    menuBarChart->setTitle(QApplication::translate("MainWindow", "Bar", 0));
    menuBoxChart->setTitle(QApplication::translate("MainWindow", "Box", 0));
    menuHistogramChart->setTitle(QApplication::translate("MainWindow", "Histogram", 0));
    dockWidget_DataFeature->setWindowTitle(QApplication::translate("MainWindow", "Data Feature", 0));
//    toolBar_chart->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
//    toolBar_chartSet->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
//    toolBar_plot->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    dockWidget_windowList->setWindowTitle(QApplication::translate("MainWindow", "Window List", 0));
    dockWidget_valueManage->setWindowTitle(QApplication::translate("MainWindow", "Value Manage", 0));
    dockWidget_plotLayer->setWindowTitle(QApplication::translate("MainWindow", "Layout", 0));
    dockWidget_chartDataViewer->setWindowTitle(QApplication::translate("MainWindow", "Chart Data", 0));
#ifndef QT_NO_TOOLTIP
    dockWidget_main->setToolTip(QString());
#endif // QT_NO_TOOLTIP
    dockWidget_main->setWindowTitle(QApplication::translate("MainWindow", "Main", 0));
    dockWidget_valueViewer->setWindowTitle(QApplication::translate("MainWindow", "value View", 0));
    dockWidget_message->setWindowTitle(QApplication::translate("MainWindow", "Message", 0));
    dockWidget_plotSet->setWindowTitle(QApplication::translate("MainWindow", "plot config", 0));
//    toolBarChartTools->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));

    mainRibbonCategory->setWindowTitle(QApplication::translate("MainWindow", "Main", 0));
    viewRibbonCategory->setWindowTitle(QApplication::translate("MainWindow", "View", 0));
    operateRibbonCategory->setWindowTitle(QApplication::translate("MainWindow", "Operate", 0));
    analysisRibbonCategory->setWindowTitle(QApplication::translate("MainWindow", "Analysis", 0));

    mainCategoryFilePannel->setWindowTitle(QApplication::translate("MainWindow", "File", 0));
    mainCategoryChartPannel->setWindowTitle(QApplication::translate("MainWindow", "Chart", 0));
    viewCategoryWindowPannel->setWindowTitle(QApplication::translate("MainWindow", "Window", 0));
    windowModeCategoryWindowPannel->setWindowTitle(QApplication::translate("MainWindow", "Sub Window Mode", 0));
    operateCategoryChartEditorPannel->setWindowTitle(QApplication::translate("MainWindow", "Chart Editor", 0));
    ribbonButtonLineChart->setText(menuLineChart->title());
    ribbonButtonBarChart->setText(menuBarChart->title());
    ribbonButtonBoxChart->setText(menuBoxChart->title());
    ribbonButtonHistogramChart->setText(menuHistogramChart->title());
    ribbonButtonScatterChart->setText(menuScatterChart->title());
    ribbonButtonAllDock->setText(menuWindowsViewSet->title());
    ribbonButtonAllFuntion->setText(menuAnalysis->title());

    operateCategoryDataViewPannel->setWindowTitle(QApplication::translate("MainWindow", "Data View", 0));
    chartLegendCategoryWindowPannel->setWindowTitle(QApplication::translate("MainWindow", "Legend", 0));
    chartGridCategoryWindowPannel->setWindowTitle(QApplication::translate("MainWindow", "Grid", 0));
    ribbonButtonShowCrowdedGrid->setText(QApplication::translate("MainWindow", "Crowded", 0));

    ribbonApplicationButton->setText(QApplication::translate("MainWindow", "SA", 0));

}
