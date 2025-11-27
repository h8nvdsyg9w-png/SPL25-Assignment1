#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
                      int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm), 
       waveform_data(nullptr), waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    #ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto& artist : artists) {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
    #endif
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

AudioTrack::~AudioTrack() {

    #ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
    #endif

    //making sure there wont be a memory leak with waveform_data
    delete[] waveform_data;
}

AudioTrack::AudioTrack(const AudioTrack& other)
    :title(other.title),
    artists(other.artists),
    duration_seconds(other.duration_seconds),
    bpm(other.bpm),
    waveform_data(nullptr),
    waveform_size(other.waveform_size)
{
    //allocate a new array for the new object's values,
    //and deep copying the values of the original array
    waveform_data = new double[waveform_size];
    for (size_t i = 0; i < waveform_size; ++i)
        waveform_data[i] = other.waveform_data[i];

    #ifdef DEBUG
    std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;
    #endif

}

AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    
    //making sure that we dont implement the same object
    if (this == &other) {
    return *this;
    }

    //deleting the previous array to prevent memory leaks before allocating the new one
    delete[] waveform_data;

    //preforming a shallow copy of the values
    title = other.title;
    artists = other.artists;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    //alocating new array and deep copying the original
    waveform_data = new double[waveform_size];
    for (size_t i = 0; i < waveform_size; ++i){
        waveform_data[i] = other.waveform_data[i]; 
    }

    #ifdef DEBUG
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    #endif

    return *this;
}

AudioTrack::AudioTrack(AudioTrack&& other) noexcept 
    :title(other.title),
    artists(other.artists),
    duration_seconds(other.duration_seconds),
    bpm(other.bpm),
    waveform_data(nullptr),
    waveform_size(other.waveform_size)
{
    #ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
    #endif

    waveform_data = other.waveform_data;

    //making sure that we are not deleting the heap memory,
    //instead we remove the access of the former owner of the memory
    other.waveform_data = nullptr;
}

AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {
    
    //making sure that we dont implement the same object
    if (this == &other) {
    return *this;
    }

    //deleting the previous array to prevent memory leaks before allocating the new one
    delete[] waveform_data;

    //preforming a shallow copy of the values
    title = other.title;
    artists = other.artists;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    waveform_data = other.waveform_data;

    //making sure that we are not deleting the heap memory,
    //instead we remove the access of the former owner of the memory
    other.waveform_data = nullptr;

    #ifdef DEBUG
    std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    #endif

    return *this;
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}