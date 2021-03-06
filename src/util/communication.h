#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#endif // COMMUNICATION_H
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxJsonSettings.h"



class CommunicationManager{
private:
    ofxOscSender videoSender;
    ofxOscSender controllerSender;

public:
    ofxOscReceiver receiver_python_controller;
    ofxOscReceiver receiver_controller;
    ofxOscReceiver receiver_playing;
    int outgoing_player_message_counter;
    int outgoing_controller_message_counter;
    int r_off;
    int b_off;
    int r_off_purp;

    const int BAD_INDEXES_VERY_RED  [2 ] = {5,12};
    const int BAD_INDEXES_PURP[10] = {4,6,9,11,14,15,20,21,22,23};

    CommunicationManager(){
        std::cout << "listening for osc messages on port " <<  Settings::getInt("CONTROL_RECEIVE_PORT") << " and "<< Settings::getInt("PLAYING_INFO_PORT")<<"\n";
        Settings::get().load("settings.json");
        r_off      =    Settings::getInt("r_off"); //red offset
        r_off_purp =    Settings::getInt("r_off_purp"); //red offset
        b_off      =    Settings::getInt("b_off"); //blue offset

        receiver_python_controller.setup( Settings::getInt("PYTHON_CONTROL_RECEIVE_PORT" ));
        receiver_controller.setup( Settings::getInt("CONTROL_RECEIVE_PORT") );
        receiver_playing.setup( Settings::getInt("PLAYING_INFO_PORT") );

        std::cout << "Sending on " << Settings::getInt("VIDEO_CONTROL_PORT") << "\n";
        videoSender.setup("localhost", Settings::getInt("VIDEO_CONTROL_PORT"));
        controllerSender.setup(Settings::getString("CONTROLLER_IP"), Settings::getInt("CONTROLLER_SEND_PORT"));
        outgoing_player_message_counter =0;
        outgoing_controller_message_counter =0;

    }

    void logMessageCount(){
        ofLogNotice()<<"Sent " <<outgoing_controller_message_counter<< " OSC messages to controller in the last second"<<endl;
        ofLogNotice()<<"Sent " <<outgoing_player_message_counter<< " OSC messages to player in the last second"<<endl;
        outgoing_controller_message_counter =0;
        outgoing_player_message_counter =0;
    }

    void publishVideos(vector<string> v1, bool log){
        outgoing_player_message_counter++;
        ofxOscMessage m;
        if (log) ofLogVerbose(ofToString(ofGetElapsedTimef(),3))<<"Sending batch of length: "<<v1.size() ;

        m.setAddress("/VIDEO_NAMES");
        m.addInt32Arg(v1.size());
        for (std::size_t i = 0; i < v1.size(); i++){
            m.addStringArg(v1[i]);

            if (log) ofLogVerbose(ofToString(ofGetElapsedTimef(),3))<<v1[i];
        }
        videoSender.sendMessage(m);
    }

    void publishVideoNow(string v1, bool log){
        outgoing_player_message_counter++;
        ofxOscMessage m;
        if (log) ofLogVerbose(ofToString(ofGetElapsedTimef(),3))<<"Sending "<< v1<<"to play now"<<endl ;
        m.setAddress("/PLAY_NOW");
        m.addStringArg(v1);
        videoSender.sendMessage(m);
    }


    void sendLightControl(int knob_index,int value){
        outgoing_controller_message_counter++;
        ofxOscMessage m;
        value = CLAMP(value, 0, 4095);
        m.setAddress("/RGB");
        m.addInt32Arg(knob_index);
//        int v= 4095;
//        int vr = v;
//        int vg = v;
//        int vb = v;
//        for (auto b1: BAD_INDEXES_VERY_RED){
//            if (b1 == knob_index){
//                vr = CLAMP(vr- r_off, 0,4095);
//            }
//        }
//        for (auto b2: BAD_INDEXES_PURP){
//            if (b2 == knob_index){
//                vb = CLAMP(vb- b_off, 0,4095);
//            }
//        }
//        m.addInt32Arg(vg-2000);
//        m.addInt32Arg(vg);
//        m.addInt32Arg(0);
        m.addInt32Arg(value);
        m.addInt32Arg(value);
        m.addInt32Arg(value);

        //SUPER IMPORTANT TO SET BUNDLE TO FALSE FOR ARDUINO
        ofLogVerbose(ofToString(ofGetElapsedTimef(),3)) <<"Sending brightness" <<value<< "to knob" << knob_index <<endl;
        controllerSender.sendMessage(m, false);
    }

    void publishSpeed(int speed){
        outgoing_player_message_counter++;
        ofxOscMessage m;
        ofLogVerbose(ofToString(ofGetElapsedTimef(),3))<<"Sending speed value" ;

        m.setAddress("/SET_SPEED");
        m.addInt32Arg(speed);
        videoSender.sendMessage(m);
    }
};
