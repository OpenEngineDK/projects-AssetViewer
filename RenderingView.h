#include <Renderers/OpenGL/RenderingView.h>
//#include <Scene/RenderStateNode.h>
//#include <Display/IViewingVolume.h>
//#include "GridNode.h"

class GridNode;
namespace OpenEngine {
    namespace Scene {
        class ISceneNode;
        class RenderStateNode;
    }
}

using namespace OpenEngine;

class RenderingView : public Renderers::OpenGL::RenderingView {
    Scene::RenderStateNode* state;
    Scene::ISceneNode*      scene;
    GridNode*               grid;

public:
    RenderingView(Display::Viewport& vp);
    void Handle(Renderers::RenderingEventArg arg);
};
