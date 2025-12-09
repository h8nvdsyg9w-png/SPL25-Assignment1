#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}

//DESTRUCTOR
DJLibraryService::~DJLibraryService() {
    for (AudioTrack* &track : library) {
        if (track) {
            delete track;
        }
    }
    library.clear();
}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    
    for(const SessionConfig::TrackInfo& curr : library_tracks){
        if(curr.type == "MP3"){
            MP3Track* track = new MP3Track(curr.title , curr.artists , curr.duration_seconds ,
                curr.bpm , curr.extra_param1 , curr.extra_param2);
            library.push_back(track);           
        }else{
            WAVTrack* track = new WAVTrack(curr.title , curr.artists , curr.duration_seconds ,
                curr.bpm , curr.extra_param1 , curr.extra_param2);
            library.push_back(track);  
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << "\n" ;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";
    this->playlist = Playlist(playlist_name);
    for(int curr : track_indices){
        if(curr < 1 || static_cast<size_t>(curr) > library.size()){
            std::cout << "[WARNING] Invalid track index: " << curr << "\n";
        }else{
            auto clone = library[curr-1]->clone();
            AudioTrack* ptr = clone.release();
            if(!ptr){
                std::cout << "[ERROR] Track contains a nullptr. track: " << curr << "\n";
            }else{
                ptr->load();
                ptr->analyze_beatgrid();
                playlist.add_track(ptr);
            }
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << playlist.get_track_count() << " tracks) \n" ; 
}

/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> toRet;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for(const AudioTrack* track : tracks){
        toRet.push_back(track->get_title());
    }
    return toRet;
}
