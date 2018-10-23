#include "ofMain.h"
#include "ofApp.h"
#include "ofVideoApp.h"
#include "ofAppGLFWWindow.h"
#include "ofxJsonSettings.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

//========================================================================
int main( ){

    Settings::get().load("settings.json");
    ofGLFWWindowSettings interfaceSettings;
//    settings.setGLVersion(4,3);
    interfaceSettings.setSize(Settings::getInt("interface_monitor_width"), Settings::getInt("interface_monitor_height"));
    interfaceSettings.monitor =Settings::getInt("interface_monitor_index");
    interfaceSettings.windowMode = Settings::getBool("windowed") ? OF_WINDOW :OF_FULLSCREEN;
    shared_ptr<ofAppBaseWindow> interfaceWindow = ofCreateWindow(interfaceSettings);

    ofGLFWWindowSettings videoSettings;
//    settings.setGLVersion(4,3);
    videoSettings.setSize(Settings::getInt("video_monitor_width"), Settings::getInt("video_monitor_height"));
    videoSettings.monitor =Settings::getInt("video_monitor_index");
    videoSettings.windowMode = Settings::getBool("windowed") ? OF_WINDOW :OF_FULLSCREEN;
    shared_ptr<ofAppBaseWindow> videoWindow = ofCreateWindow(videoSettings);

    shared_ptr<ofApp> interfaceApp (new ofApp());
    shared_ptr<ofVideoApp> videoApp (new ofVideoApp());
    auto t =std::dynamic_pointer_cast<ofAppBaseGLWindow> (videoWindow);

    glfwSetWindowFocusCallback( std::dynamic_pointer_cast<ofAppGLFWWindow> (t)->getGLFWWindow(), videoApp->window_focus_callback );

    ofRunApp(videoWindow, videoApp);
    ofRunApp(interfaceWindow, interfaceApp);
    ofRunMainLoop();

}
