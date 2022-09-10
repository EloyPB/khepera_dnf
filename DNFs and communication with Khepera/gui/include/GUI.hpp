#ifndef GUI_HPP
#define GUI_HPP

#include "Manager.hpp"
#include "RobotComm.hpp"
#include "PlotField1D.hpp"
#include "PlotFieldMat.hpp"
#include "Parameters_NF.hpp"
#include "Parameters_NN.hpp"
#include "RobotListener_QCam.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QThread>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QCloseEvent>

class DNFGUI : public QMainWindow
{
    Q_OBJECT
    QThread managerThread, communicationThread;

public:
    explicit DNFGUI(Manager *pManager, RobotComm *pRobotComm = nullptr, bool showCam = false);
    void addManager(Manager *pManager);
    void addRobotComm(RobotComm *pRobotComm);
    void addCamera(RobotComm *pRobotComm);
    void closeEvent (QCloseEvent *event);
    ~DNFGUI();

public slots:


signals:
    void run(int tmax);
    void stop();
    void communicate();

private:
    QWidget *mpCentralWidget;
    QGridLayout *mpGrid;

    QMenu *mpFieldsMenu, *mpCameraMenu;
};

#endif //GUI_HPP
