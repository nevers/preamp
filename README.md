The Preamp
==========

The aim of this project is to create a yet high fidelity stereo pre-amplifier. It is designed to provide comfort features such as IR remote control and an LCD display without compromising the audio quality.

The IO of the device is very simple:
* Six independenly selectable stereo inputs.
* Two low noise and low distortion identical stereo outputs.
* Digitally-controlled analog volume control using a rotary encoder. 
* Wide gain and attenuation range from +31.5dB to -95.5dB with 0.5dB steps.
* An LCD showing the currently selected channel and its volume in dB.

All electronics and hardware, including PCB and eclosure is costum design. At the heart of system, the PIC 18F4520 microcontroller is used. It controls the whole preamp: from listening to the rotary encoder and the IR inputs to selecting the active output channels and setting the output volume.

The main audio path is build from high-end components such as the OPA2134, an ultra-low distortion, low noise operational amplifier. More specifically, the OPA2134 is used at the input lines of the preamp. It's high impedance input stage draws as little as 5pA (excluding the filtering stage in front of it), providing superior sound capturing for the next stage. The audio is then sent to the PGA2311, a high-quality volume controller capable of completely muting the audio to aplifying it as much as +31.5dB. It is controlled by the microcontroller using SPI. The final component in the audio path is the NE5532, a capable operational amplifier, selected for its excellent audio performance. It also provides a high current output, which is indespensible for driving the outputs of the preamp. 
