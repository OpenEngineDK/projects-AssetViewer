#include "MainGUI.h"

#include <Display/SceneGraphGUI.h>
#include <Display/SceneNodeGUI.h>

#include "AssetViewer/ui_MainGUI.h"

using namespace std;
using namespace OpenEngine::Display;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Scene;

MainGUI::MainGUI(string title, QtEnvironment& env, SimpleSetup& setup) {
    // install the qt-designer generated gui
    ui = new Ui::MainGUI();
    ui->setupUi(this);

    // set the main window title and status
    setWindowTitle(QString(title.c_str()));
    ui->statusbar->showMessage(QString("Loading..."));

    // add the gl widget to the left pane
    ui->topLayout->addWidget(env.GetGLWidget());

    // add the node and graph components to the right pane
    graphGui = new SceneGraphGUI(setup.GetScene());
    nodeGui  = new SceneNodeGUI();
    setup.GetEngine().InitializeEvent().Attach(*graphGui);
    graphGui->SelectionEvent().Attach(*nodeGui);
    graphGui->setMinimumWidth(150);
    ui->rightLayout->addWidget(nodeGui);
    ui->rightLayout->addWidget(graphGui);

    // Show the gui!
    show();
}

MainGUI::~MainGUI() {
    delete ui;
    delete graphGui;
    delete nodeGui;
}
