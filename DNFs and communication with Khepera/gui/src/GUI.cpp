#include "GUI.hpp"

DNFGUI::DNFGUI(Manager *pManager, RobotComm *pRobotComm, bool showCam)
{
    setWindowTitle("Neural Fields");
    mpCentralWidget = new QWidget(this);
    mpGrid = new QGridLayout(mpCentralWidget);
    setCentralWidget(mpCentralWidget);

    addManager(pManager);

    if(showCam)
        addCamera(pRobotComm);

    if(pRobotComm != nullptr)
        addRobotComm(pRobotComm);

    this->show();
}

DNFGUI::~DNFGUI()
{
    managerThread.quit();
    managerThread.wait();
    communicationThread.quit();
    communicationThread.wait();

    cout << "Bye." << endl;
}

void DNFGUI::closeEvent (QCloseEvent *event)
{
    emit(stop());
    event->accept();
}

void DNFGUI::addCamera(RobotComm *pRobotComm)
{
    QCam *pQCam = new QCam(pRobotComm);

    mpCameraMenu = menuBar()->addMenu("Camera");
    QAction *mpShowCam = new QAction("Show Camera", this);
    mpCameraMenu->addAction(mpShowCam);

    connect(mpShowCam, &QAction::triggered, pQCam, &QCam::showCam);

    QAction *mpWhiteBalance = new QAction("White Balance", this);
    mpCameraMenu->addAction(mpWhiteBalance);
    connect(mpWhiteBalance, &QAction::triggered, pRobotComm, &RobotComm::whiteBalance, Qt::DirectConnection);
}

void DNFGUI::addManager(Manager *pManager)
{
    // put the manager in his thread and connect signals
    pManager->moveToThread(&managerThread);
    connect(this, SIGNAL(run(int)), pManager, SLOT(run(int)));
    connect(this, SIGNAL(stop()), pManager, SLOT(stop()), Qt::DirectConnection);

    int row = 0;
    int col = 0;
    for(auto pField : pManager->mpFields)
    {
        QLabel *pNameLabel = new QLabel(this);
        pNameLabel->setText(pField->mFieldName);
        mpGrid->addWidget(pNameLabel, row, col);

        PlotField *pPlotField;
        if(pField->mDimensions == 1)
            pPlotField = new PlotField1D(pField, this);
        else
            pPlotField = new PlotFieldMat(pField, this);

        mpGrid->addWidget(pPlotField->mpPlot, row+1, col, Qt::AlignCenter);
        connect(pManager, SIGNAL(update()), pPlotField, SLOT(update()));

        row += 2;
        if(row == 6)
        {
            row = 0;
            col++;
        }
    }

    mpFieldsMenu = menuBar()->addMenu("Tune Fields");
    for(auto pField : pManager->mpFields)
    {
        QAction *mpFieldAction = new QAction(pField->mFieldName, this);
        mpFieldsMenu->addAction(mpFieldAction);

        if(pField->mDimensions > 0)
        {
            Parameters_NF * pParameters = new Parameters_NF((NeuralField*)pField);
            connect(mpFieldAction, &QAction::triggered, pParameters, &Parameters_NF::showWindow);
        }
        else if(pField->mDimensions == 0)
        {
            Parameters_NN * pParameters = new Parameters_NN((NeuralNode*)pField);
            connect(mpFieldAction, &QAction::triggered, pParameters, &Parameters_NF::showWindow);
        }
    }

    pManager->mHasGUI = true;
    managerThread.start();
    emit run(-1); // run infinite
}

void DNFGUI::addRobotComm(RobotComm *pRobotComm)
{
    pRobotComm->moveToThread(&communicationThread);
    connect(this, SIGNAL(communicate()), pRobotComm, SLOT(communicate()));
    communicationThread.start();
    emit communicate();
}
