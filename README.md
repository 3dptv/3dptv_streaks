Streaks-based particle tracking velocimetry software
----------------------------------------------------

License 
--------


Copyright (c) 1995-2012 Matthias Machacek 

Permission is hereby granted, free of charge, to any person obtaining a
copy of the THE PARTICLE TRACKING VELOCIMETRY SOFTWARE and associated
documentation files, to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



Getting started
----------------
**Read** the Ph.D. thesis of Matthias Machacek attached as [PDF](https://github.com/downloads/3dptv/3dptv_streaks/diss_Matthias_Machacek.pdf)


Calibration:
-----------

The calibration code ```CamCal```, version 7.0 determines the inner (f, x0, y0, sx, θ, k1, k2, p1, p2) and outer parameters 
(X0, Y0, Z0, ω, φ, κ) of a pinhole camera model including a lens distortion compensation. The code is designed to 
process images from planar calibration patterns. 




Camera control and acquisition:
-------------------------------
The image recording program pulnix, version 2.0  enables to control the Pulnix TM6710 camera, 
the NI frame grabber PCI-1422 and the NI timer card PCI-6601



Pathline reconstruction:
------------------------
The three-dimensional path line reconstruction code Stereo2, version 9.3 processes the image pairs recorded 
with a two camera set-up 



Matlab Calibration:
-------------------

The calibration code startcal.m (MATLAB code) calculates the external parameters (X0, Y0, Z0, ω, φ, κ) of 
a stereo camera set-up

