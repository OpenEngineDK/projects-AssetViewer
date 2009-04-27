
#include "RenderingView.h"

#include <Scene/SceneNode.h>
#include <Scene/RenderStateNode.h>
#include <Display/IViewingVolume.h>
#include "GridNode.h"

#include <Logging/Logger.h>

using namespace OpenEngine::Scene;
using namespace OpenEngine::Display;
using namespace OpenEngine::Renderers;
using namespace OpenEngine::Logging;

RenderingView::RenderingView(Viewport& vp)
    : IRenderingView(vp)
    , OpenGL::RenderingView(vp) {
    scene = new SceneNode();
    grid  = new GridNode(50, 10, 0);
    grid->SetFadeColor(0.4);
    grid->SetSolidRepeat(3);
    scene->AddNode(grid);

    state = new RenderStateNode();
    state->EnableOption(RenderStateNode::TEXTURE);
    state->EnableOption(RenderStateNode::SHADER);
    state->EnableOption(RenderStateNode::BACKFACE);
    state->EnableOption(RenderStateNode::DEPTH_TEST);
    state->DisableOption(RenderStateNode::LIGHTING); //@todo
    state->DisableOption(RenderStateNode::WIREFRAME);
}

void RenderingView::Handle(RenderingEventArg arg) {
    IViewingVolume* volume = viewport.GetViewingVolume();
    // If no viewing volume is set for the viewport ignore it.
    if (volume != NULL) {
        volume->SignalRendering(arg.approx);
        
        // Set viewport size
        Vector<4,int> d = viewport.GetDimension();
        glViewport((GLsizei)d[0], (GLsizei)d[1], (GLsizei)d[2], (GLsizei)d[3]);
        CHECK_FOR_GL_ERROR();
        
        // apply the volume
        arg.renderer.ApplyViewingVolume(*volume);
    }
    CHECK_FOR_GL_ERROR();
    ApplyRenderState(state);
    Render(&arg.renderer, scene);
}
