#include "MainGUI.h"

#include <Display/SceneGraphGUI.h>
#include <Display/SceneNodeGUI.h>

#include <Math/Vector.h>

#include "RenderingView.h"

#include "AssetViewer/ui_MainGUI.h"

using namespace std;
using namespace OpenEngine::Display;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Math;

MainGUI::MainGUI(string title, QtEnvironment& env, SimpleSetup& setup) {
    // install the qt-designer generated gui
    ui = new Ui::MainGUI();
    ui->setupUi(this);

    // set the main window title and status
    setWindowTitle(QString(title.c_str()));
    ui->statusbar->showMessage(QString("Loading..."));

    // add the gl widget to the left pane
    ui->topLayout->addWidget(env.GetGLWidget());
    setup.GetRenderer().SetBackgroundColor(Vector<4,float>(.5,.5,.5,1));
    Viewport*      vp = new Viewport(setup.GetFrame());
    RenderingView* rv = new RenderingView(*vp);
    vp->SetViewingVolume(setup.GetCamera());
    setup.GetRenderer().PreProcessEvent().Attach(*rv);

    // set the initial camera position
    Vector<3,float> position(50,40,50);
    Vector<3,float> lookat(0,0,0);
    setup.GetCamera()->SetPosition(position);
    setup.GetCamera()->LookAt(lookat);

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
