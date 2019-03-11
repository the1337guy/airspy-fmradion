#!/usr/bin/env python3
from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

# calculated by Twitter lambdaprog 
# /8 decimator LPF
# as the first-stage filter of /32 decimator

b = [
 25.80054878626173220E-6,
 53.19138439356930800E-6,
 93.94341652418872000E-6,
 137.4568420541721420E-6,
 169.2646452746862450E-6,
 170.1197918995168830E-6,
 120.3731788804096250E-6,
 6.878460766453898140E-6,
-169.4350070856139040E-6,
-387.1268307956495390E-6,
-601.9520128968730430E-6,
-750.9945347851355560E-6,
-764.1694200258054930E-6,
-582.4066841884884980E-6,
-179.1367006559430310E-6,
 420.3200334844814280E-6,
 0.001127808158433643,
 0.001795763137990068,
 0.002238432727721262,
 0.002270231316328288,
 0.001755404070146396,
 658.5584995804709930E-6,
-917.3481836964912190E-6,
-0.002719126654856985,
-0.004367409954619228,
-0.005419305236609882,
-0.005462897158401273,
-0.004227121411698297,
-0.001682964159551947,
 0.001891085255069228,
 0.005903114706431550,
 0.009515491121584566,
 0.011785386645285657,
 0.011859963864035114,
 0.009192120706559108,
 0.003732172131668413,
-0.003950989874477156,
-0.012657237836959346,
-0.020670880611145914,
-0.025999624008481784,
-0.026705066970119724,
-0.021270079736126192,
-0.008934816873859127,
 0.010067313152961412,
 0.034428573152584428,
 0.061869380606893967,
 0.089419158024582307,
 0.113835495432123027,
 0.132092972947044474,
 0.141856849092363069,
 0.141856849092363069,
 0.132092972947044474,
 0.113835495432123027,
 0.089419158024582307,
 0.061869380606893967,
 0.034428573152584428,
 0.010067313152961412,
-0.008934816873859127,
-0.021270079736126192,
-0.026705066970119724,
-0.025999624008481784,
-0.020670880611145914,
-0.012657237836959346,
-0.003950989874477156,
 0.003732172131668413,
 0.009192120706559108,
 0.011859963864035114,
 0.011785386645285657,
 0.009515491121584566,
 0.005903114706431550,
 0.001891085255069228,
-0.001682964159551947,
-0.004227121411698297,
-0.005462897158401273,
-0.005419305236609882,
-0.004367409954619228,
-0.002719126654856985,
-917.3481836964912190E-6,
 658.5584995804709930E-6,
 0.001755404070146396,
 0.002270231316328288,
 0.002238432727721262,
 0.001795763137990068,
 0.001127808158433643,
 420.3200334844814280E-6,
-179.1367006559430310E-6,
-582.4066841884884980E-6,
-764.1694200258054930E-6,
-750.9945347851355560E-6,
-601.9520128968730430E-6,
-387.1268307956495390E-6,
-169.4350070856139040E-6,
 6.878460766453898140E-6,
 120.3731788804096250E-6,
 170.1197918995168830E-6,
 169.2646452746862450E-6,
 137.4568420541721420E-6,
 93.94341652418872000E-6,
 53.19138439356930800E-6,
 25.80054878626173220E-6
 ]

f = 312500
w, h = signal.freqz(b)
fig, ax1 = plt.subplots()
fs = f / (np.pi * 2.0)
ax1.set_title('Digital filter frequency response')
ax1.grid(True)
ax1.plot(w * fs, 20 * np.log10(abs(h)), 'b')
ax1.set_ylabel('Amplitude [dB]', color='b')
ax1.set_xlabel('Frequency [Hz]')
plt.show()