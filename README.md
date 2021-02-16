#   Collaborative Audio Interface

Standalone audio app made using the JUCE library and the Tracktion Engine JUCE module. Software utilizes the WebSokcet protocol to integrate communication between different users.

This project is an exploration of real-time collaborative music making programs as part of my final project for my undergraduate degree at Goldsmiths, University of London. Research around designing multi-user and music production interfaces was used to inform and plan the development of a collaborative music-making platform using the WebSocket protocol (through Node.js) and the JUCE framework.

he proposed program is a software sequencer/sampler that models its functionality after hardware samplers but allows for any number of users to control the program from remote locations at the same time. The project got as far as to develop a proof of concept sequencer in C++ using the Juce framework and Tracktion engine along with a user-tested functional prototype in Max/MSP, laying down the groundwork for developing a more robust iteration of the program in C++.

The prototype (made in Max/MSP) is comprised of a four track sample player/sequencer with a selection of samples that were packaged into the program. Each track can be toggled between the looping or sequencer mode. The looping mode loops the selected sample and allows for control of the sample playback speed and pitch. The sequencer mode only allows you to edit the audio file by changing the start and end points.

The project's research report detailing the entire process and how the program works in more detail can be found here: https://drive.google.com/file/d/1rNJjAfrRJtVbXLQz8HR7S-VcVanZpPJ6/view?usp=sharing