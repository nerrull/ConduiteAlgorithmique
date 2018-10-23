#ifndef VIDEODATABASELOADER_H
#define VIDEODATABASELOADER_H

#include "ofMain.h"
#include "ofxHDF5.h"

#define STRING_LENGTH 40

class VideoDatabaseLoader
{
public:
    VideoDatabaseLoader();
    void loadVideoPaths(string, string, string);
    vector<string>  getVideoPaths(){return video_names;}
    vector<string>  getAudioPaths(){return audio_names;}

private:
    int num_videos;
    vector<string> video_names;
    vector<string> audio_names;
};

#endif // VIDEODATABASELOADER_H
