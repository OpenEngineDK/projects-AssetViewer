// Game Factory for the OERacer project.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// Serialization (must be first)
#include <fstream>
#include <Utils/Serialization.h>

// Class header
#include "GameFactory.h"

// OpenEngine library
#include <Display/Camera.h>

#include <Display/Frustum.h>
#include <Display/InterpolatedViewingVolume.h>
#include <Display/ViewingVolume.h>
#include <Display/SDLFrame.h>
#include <Devices/SDLInput.h>
#include <Logging/Logger.h>
#include <Renderers/IRenderNode.h>
#include <Renderers/RenderStateNode.h>
#include <Renderers/OpenGL/Renderer.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Renderers/OpenGL/TextureLoader.h>
#include <Resources/IModelResource.h>
#include <Resources/File.h>
#include <Resources/GLSLResource.h>
#include <Resources/TGAResource.h>
#include <Resources/OBJResource.h>
#include <Resources/DirectoryManager.h>
#include <Resources/ResourceManager.h>
#include <Scene/SceneNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/VertexArrayTransformer.h>
#include <Scene/DisplayListTransformer.h>
#include <Utils/Statistics.h>

#include <Scene/PointLightNode.h>
#include <Scene/DirectionalLightNode.h>

// from AccelerationStructures extension
#include <Scene/CollectedGeometryTransformer.h>
#include <Scene/QuadTransformer.h>
#include <Scene/BSPTransformer.h>
#include <Scene/ASDotVisitor.h>
//#include <Scene/ReflectionNode.h>
#include <Renderers/AcceleratedRenderingView.h>

// from FixedTimeStepPhysics
#include <Physics/FixedTimeStepPhysics.h>
#include <Physics/RigidBox.h>

// From extensions
#include <Resources/ColladaResource.h>

// Project files
#include "KeyboardHandler.h"
#include "RenderStateHandler.h"
#include <Utils/MoveHandler.h>
#include <Utils/QuitHandler.h>

// Additional namespaces (others are in the header).
using namespace OpenEngine::Devices;
using namespace OpenEngine::Renderers::OpenGL;
using namespace OpenEngine::Renderers;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Physics;

// composite rendering view. Uses RenderingView for drawing and
// AcceleratedRenderingView for clipping. 
class MyRenderingView : 
    public RenderingView,
    public AcceleratedRenderingView {
public:
    MyRenderingView(Viewport& viewport) :
        IRenderingView(viewport),
        RenderingView(viewport),
        AcceleratedRenderingView(viewport) {

    }
};

/**
 * Factory constructor.
 *
 * Initializes the different components so they are accessible when
 * the setup method is executed.
 */
GameFactory::GameFactory() {
    // Create a frame and viewport.
    this->frame = new SDLFrame(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

    // Main viewport
    viewport = new Viewport(*frame);

    // Create a renderer.
    this->renderer = new Renderer();
    renderer->InitializeEvent().Attach(*(new TextureLoader()));
    // Add a rendering view to the renderer
    MyRenderingView* rv = new MyRenderingView(*viewport);
    DisplayListTransformer* dlt = new DisplayListTransformer(rv);
    
    renderer->ProcessEvent().Attach(*rv);  // space leak
    renderer->InitializeEvent().Attach(*dlt);
    renderer->InitializeEvent().Attach(*(new TextureLoader()));
}

/**
 * Setup handler.
 *
 * This is the main setup method of the game factory. Here you can add
 * any non-standard modules and perform other setup tasks prior to the
 * engine start up.
 *
 * @param engine The game engine instance.
 */
bool GameFactory::SetupEngine(IEngine& engine) {
    
    engine.InitializeEvent().Attach(*this->frame);
    engine.ProcessEvent().Attach(*this->frame);
    engine.DeinitializeEvent().Attach(*this->frame);

    engine.InitializeEvent().Attach(*this->renderer);
    engine.ProcessEvent().Attach(*this->renderer);
    engine.DeinitializeEvent().Attach(*this->renderer);
  
    // Add mouse and keyboard module here
    SDLInput* input = new SDLInput();
    // Bind to the engine for processing time
    engine.InitializeEvent().Attach(*input);
    engine.ProcessEvent().Attach(*input);
    engine.DeinitializeEvent().Attach(*input);


    // Add Statistics module
    engine.ProcessEvent().Attach(*(new OpenEngine::Utils::Statistics(1000)));

    // Create a root scene node
    SceneNode* scene = new SceneNode();

    // Supply the scene to the renderer
    this->renderer->SetSceneRoot(scene);

    // Add RenderStateNode to change rendering features at runtime
    RenderStateNode* rNode = new RenderStateNode();
    rNode->AddOptions(RenderStateNode::RENDER_TEXTURES);
    rNode->AddOptions(RenderStateNode::RENDER_SHADERS);
    scene->AddNode(rNode);
    
    // Bind keys for changing rendering state
    RenderStateHandler* renderStateHandler = new RenderStateHandler(rNode);
    input->KeyEvent().Attach(*renderStateHandler);

    // Bind the quit handler
    QuitHandler* quit_h = new QuitHandler(engine);
    input->KeyEvent().Attach(*quit_h);

    // Bind the camera to the viewport
    Camera* camera = new Camera(*(new ViewingVolume));
    camera->SetPosition(Vector<3,float>(5,3,-30));
    camera->LookAt(Vector<3,float>(0,0,0));

    //FollowCamera* camera = new FollowCamera( *(new InterpolatedViewingVolume( *(new ViewingVolume()) )));
    Frustum* frustum = new Frustum(*camera, 0.001, 5000);
    viewport->SetViewingVolume(frustum);

    // Register movement handler to be able to move the camera
    MoveHandler* move = new MoveHandler(*camera, *input);
    input->KeyEvent().Attach(*move);
    engine.InitializeEvent().Attach(*move);
    engine.ProcessEvent().Attach(*move);
    engine.DeinitializeEvent().Attach(*move);
    
    // set the resources directory
    string resources = "projects/AssetViewer/data/";
    DirectoryManager::AppendPath(resources);
    logger.info << "Resource directory: " << resources << logger.end;

    // Load the resource plug-ins
    ResourceManager<IModelResource>::AddPlugin(new ColladaPlugin());
    ResourceManager<IModelResource>::AddPlugin(new OBJPlugin());   
    ResourceManager<ITextureResource>::AddPlugin(new TGAPlugin());
    ResourceManager<IShaderResource>::AddPlugin(new GLSLPlugin());

    // Keyboard bindings to the rigid box and camera
    KeyboardHandler* keyHandler = new KeyboardHandler(camera,NULL,NULL,engine);
    input->KeyEvent().Attach(*keyHandler);

    // Load the model
    IModelResourcePtr mod_res = ResourceManager<IModelResource>::Create("FutureTank/model.obj");
    mod_res->Load();
    ISceneNode* mod_node = mod_res->GetSceneNode();
    mod_res->Unload();

    IModelResourcePtr podium_res = ResourceManager<IModelResource>::Create("Podium001.dae");
    podium_res->Load();
    ISceneNode* podium_node = podium_res->GetSceneNode();
    podium_res->Unload();

    TransformationNode* mod_tran = new TransformationNode();    
    mod_tran->AddNode(mod_node); 

    TransformationNode* podium_tran = new TransformationNode();    
    podium_tran->AddNode(podium_node); 
    
    // Add reflection effect
    //ReflectionNode* reflect = new ReflectionNode(mod_tran, podium_tran, 0);
    //rNode->AddNode(reflect);
    //reflect->AddNode(mod_tran);
    //reflect->AddNode(podium_tran);

    rNode->AddNode(podium_tran);
    rNode->AddNode(mod_tran);

    ofstream out("scenegraph.dot", ios::out); // create output file
    ASDotVisitor dot;                        // create dot visitor
    dot.Write(*scene, &out);           // build and write the graph
    out.close();                       // close your file

    // Use vertex arrays
    VertexArrayTransformer vaT;
    vaT.Transform(*scene);

    /* @@@todo
    CairoSurfaceResourcePtr sr = 
        CairoSurfaceResourcePtr(new CairoSurfaceResource(CairoSurfaceResource::CreateCairoSurface(1000,100)));

    TextSurface *ts = new TextSurface(*sr, string("Hmm"));

    LayerNode *ln = new LayerNode(1024, 768); 
    Layer layer(0,0);
    //Layer babeLayer(100,100);
  
    layerStat = new LayerStatistics(1000, ts);
    engine.AddModule(*layerStat);
    //babeLayer.texr = ResourceManager<ITextureResource>::Create("hud.tga");
    layer.texr = sr;
    ln->AddLayer(layer); // = sr; //ResourceManager<ITextureResource>::Create("hud.tga");
    //ln->AddLayer(*layerStat);
    //ln->AddLayer(babeLayer);
    scene->AddNode(ln);
*/
    // Return true to signal success.
    return true;
}

// Other factory methods. The returned objects are all created in the
// factory constructor.
IFrame*      GameFactory::GetFrame()    { return this->frame;    }
IRenderer*   GameFactory::GetRenderer() { return this->renderer; }
