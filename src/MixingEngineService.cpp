#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...";
    if(decks[0]){
        delete decks[0];
        decks[0] =nullptr;
    }
    if(decks[1]){
        delete decks[1];
        decks[1] =nullptr;
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout<< "\n=== Loading Track to Deck ===\n" ; 
    PointerWrapper<AudioTrack> pw(track.clone());
    if(!pw){
        std::cout<< "[ERROR] Track: " << track.get_title() << " failed to clone\n";
        return -1;
    }
    if(!decks[0] && !decks[1]) // first track
    {
        std::cout << "[Deck Switch] Target deck: 0\n";
        AudioTrack* audio = pw.release();
        decks[0] = audio;
        audio->load();
        audio->analyze_beatgrid();
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty.\n";
        std::cout << "[Load Complete] '" << audio->get_title() << "' is now loaded on deck 0\n";
        active_deck = 0;
        std::cout << "[Active Deck] Switched to deck 0\n";
        return active_deck;
    }
    else{

    size_t targetDeck = 1 - active_deck;
    std::cout<< "[Deck Switch] Target deck: "<< targetDeck << "\n";
    if(decks[targetDeck]){
        delete decks[targetDeck];
        decks[targetDeck] = nullptr;
    }
    pw->load();
    pw->analyze_beatgrid();
    if(decks[active_deck] && auto_sync && !can_mix_tracks(pw)){
        sync_bpm(pw);
    }
    decks[targetDeck] = pw.release();
    std::cout << "[Load Complete] '" << track.get_title() << "' is now loaded on deck " << targetDeck <<"\n" ; 
    active_deck = targetDeck;
    std::cout << "[Active Deck] Switched to deck " << targetDeck << "\n";
    return targetDeck;
}
    
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(!decks[active_deck]){
        return false;
    }
    if(!track){
        return false;
    }
    if(std::abs(track->get_bpm() - decks[active_deck]->get_bpm()) <= bpm_tolerance){
        return true;
    }
    return false;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] && track){
        int originalBPM (track->get_bpm());
        int avgBPM ((originalBPM + decks[active_deck]->get_bpm()) /2);
        track->set_bpm(avgBPM);
        std::cout << "[Sync BPM] Syncing BPM from " << originalBPM << " to " << avgBPM << "\n";
    }
}
