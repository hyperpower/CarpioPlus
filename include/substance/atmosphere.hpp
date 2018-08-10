#ifndef _ATMOSPHERE_HPP_
#define _ATMOSPHERE_HPP_
#pragma once

#include "type_define.hpp"
#include <array>

namespace carpio {
struct Atmosphere{
protected:
	Vt _altitude;
	Vt _altitude_geopotential;
	Vt _density;
	Vt _temperature;
	Vt _pressure;
	Vt _speed_of_sound;
	Vt _mean_molecular_weight;
};

//forward declaration
Atmosphere AtmosphereAtAltitude(Vt z);

Vt DensityAtAltitude(Vt z);
Vt TemperatureAtAltitude(Vt z);
Vt PressureAtAltitude(Vt z);
Vt SpeedOfSoundAtAltitude(Vt z);
Vt MeanMolecularWeightAtAltitude(Vt z);

// define constant
const double r0 = 6356766.0;    // (m), effective Earth radius at 45 deg N latitude
const double g0 = 9.80665;      // (m / s²) or (m² / s² m')
const double M0 = 28.9644;      // (kg / kmol)
const double Rstar = 8.31432e3; // (N m / kmol K)
const double r = 1.4;           // air constant

Vt GeopotentialAltitude(Vt Z){
	return r0 * Z / (r0 + Z);
}
// table 4 The defined reference levels and gradients of
//         the linearly segmented temperature-height from
//         the surface to 86 geometric kilometers
//      b = 1        2       3       4       5      6        7
const std::array<Vt, 7> Hb  =  // Geopotential height
           {0,       11000,  20000,  32000,  47000, 51000,   71000}; // (m')
const std::array<Vt, 7> Lmb =  // Molecular-scale temperature gradient
           {-6.5e-3, 0.0e-3, 1.0e-3, 2.8e-3, 0.0,   -2.8e-3, -2.0e-3}; // (K / m')

St _Findb(Vt H){
    St i = 1;
    while(i < 7 && H > Hb[i + 1]){
        i += 1;
    }
    return i - 1;
}
Vt _Tm(Vt H){
    auto b = _Findb(H);
    auto i = b + 1;
    return Tmb[i] + Lmb[i] * (H - Hb[i]);
}

bool _CheckAltitude(Vt Z){
    return Z < -5000 && Z > 1000000;
}

Atmosphere AtmosphereAtAltitude(Vt Z){
    ASSERT(_CheckAltitude(Z));
    if (Z < 8.6e4) {
        auto H = GeopotentialAltitude(Z);
//        auto M = MeanMolecular_weight_lower(Z)
//        auto T = temperature_lower(H, M)
//        auto P = pressure_lower(H)
//        auto c = SpeedOfSound_lower(T, M)
    } else {
//        auto T = temperature_upper(Z)
//        auto P = pressure_upper(Z)
//        auto M = mean_molecular_weight_upper(Z)
//        auto c = speed_of_sound_86km
    }
//    State(M, T, P, c)
}


}

#endif
