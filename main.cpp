// main
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

// OpenEngine stuff
#include <Meta/OpenGL.h>
#include <Meta/Config.h>
#include <Meta/OpenGL.h>
#include <Utils/SimpleSetup.h>
#include <Display/QtEnvironment.h>
#include <Resources/ResourceManager.h>
#include <Resources/DirectoryManager.h>
#include <Resources/ColladaResource.h>
// #include <Resources/SDLFont.h>
#include <Resources/CairoFont.h>
#include <string>

#include "MainGUI.h"

using namespace std;
using namespace OpenEngine;
using namespace Resources;

int main(int argc, char** argv) {
    string resources("projects/AssetViewer/data/");
    DirectoryManager::AppendPath(resources);
    // ResourceManager<IFontResource>::AddPlugin(new SDLFontPlugin());
    ResourceManager<IFontResource>::AddPlugin(new CairoFontPlugin());

    string title("OpenEngine Asset Viewer");

    // Use Qt as our window toolkit
    Display::QtEnvironment* env = new Display::QtEnvironment(false);

    // Create a simple-setup
    Utils::SimpleSetup* setup = new Utils::SimpleSetup(title, NULL, env);
    ResourceManager<IModelResource>::AddPlugin(new ColladaPlugin());

    // Create the GUI
    MainGUI* gui = new MainGUI(title, *env, *setup);

    // Start the engine.
    setup->GetEngine().Start();

    // Return when the engine stops.
    return EXIT_SUCCESS;
}
