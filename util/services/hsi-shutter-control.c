/*
Shutter control

purpose: close the shutter when in sun, open if not in sun

approach: read ADC, if value is above threshold, singal to close shutter.
runs in a loop forever

Executiuon controlled by hsi_shutter_control.service

*/

