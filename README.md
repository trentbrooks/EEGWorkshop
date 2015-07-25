## Visualising Brainwaves with EEG Workshop ##

Code / examples from the Creative Coding Sydney workshop - 26th July 2015. http://codeoncanvas.cc/events/cc_syd_2/. All code examples are for either the MyndPlay Brainband / Mindwave Mobile, or Muse EEG headsets.

Examples have been tested on OSX with Openframeworks 0.8.4 http://www.openframeworks.cc/versions/v0.8.4/of_v0.8.4_osx_release.zip

## Headsets ##
- MyndPlay Brainband / Mindwave Mobile http://neurosky.com/
- Muse http://www.choosemuse.com/

## Utilities ##
- Included the exported OSX app BrainWaveOSC for Brainband + Mindwave Mobile, which can also be found here: https://github.com/trentbrooks/BrainwaveOSC. Also saved a 3 minute EEG recording (BrainWaveOSC_OSX_0.93/data/exports/brainband-sample.csv), which can be used for playback instead of an EEG device.
- Included the command line scripts for Muse, which can also be found here https://github.com/trentbrooks/MuseEEG. Also saved a 3 minute EEG recording (MuseEEG/muse_recording.muse), which can be used for playback instead of an EEG device.
- Included the ofxThinkgear addon for example 2, which can also be downloaded here https://github.com/trentbrooks/ofxThinkgear

## Examples ##
#### 01_brainband_osc ####
- basic example displays 'attention' and 'meditation' values
- reads in EEG data via OSC from a MyndPlay Brainband or Mindwave Mobile using the BrainwaveOSC application 

#### 02_brainband_addon ####
- same display as example 1
- reads in EEG data directly from the ofxThinkgear addon

#### 03_muse_osc ####
- example displays the 5 EEG band frequencies (delta 1-3htz, theta 4-7htz, alpha 8-15htz, beta 16-31htz, gamma 32-50htz)
- reads in EEG data via OSC from Muse headset which requires one of the command line scripts to be run. eg. run-muse-preset-D.command

#### 04_muse_moire ####
- example displays how to modify a simple 2D arc/ring shape to produce a moire pattern effect using theta and alpha waves
- option for using a 'Wireframe shader' to increase performance based on this example: https://github.com/trentbrooks/WireframeShader
- reads in EEG data same as example 3

#### 05_brainband_fft ####
- a modified version of Lukasz's audioMesh Sound Sculpture example to work with EEG instead of audio https://github.com/julapy/SoundSculptures
- uses the raw EEG signal (make sure the 'send raw data with osc' checkbox is enabled in BrainwaveOSC)
- the data is processed by ofxFFT using the ofxFFTBase class https://github.com/julapy/ofxFFT
- reads in EEG data same as example 2
