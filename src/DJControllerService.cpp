#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if(cache.contains(track.get_title())){
        cache.get(track.get_title());
        return 1;
    }else{
        PointerWrapper<AudioTrack> clone(track.clone());
        AudioTrack* toInsert = clone.release();
        if(!toInsert){
            std::cout<< "[ERROR] Track:" << track.get_title() << "failed to insert \n" ;
            return 0; //dont know what to return we need to ask lotem
        }else{
            toInsert->load();
            toInsert->analyze_beatgrid();
            PointerWrapper<AudioTrack> pw(toInsert);
            if(cache.put(std::move(pw))){
                return -1;
            }else{
                return 0;
            }
        }
    }
    return 0; // Placeholder
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title);
}
