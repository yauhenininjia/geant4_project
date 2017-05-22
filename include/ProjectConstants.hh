#ifndef PROJECT_CONSTANTS_H
#define PROJECT_CONSTANTS_H 1

#include "G4SystemOfUnits.hh"

namespace PROJECT_CONSTANTS {
  const G4double GAP_1 = 50 * cm; // расстояние от источника до мишени
  const G4double GAP_2 = 1.170 * m; // расстояние от мишени до детектора
  const G4double CHAMBER_WIDTH = 3 * cm; // width of the chamber
  const G4double CHAMBER_LENGTH = 22 * cm; // length of the chamber
  const G4double CHAMBER_HEIGHT = 3 * cm; // high of the chamber 
  const G4double WORLD_LENGTH = 5 * m;
  const G4double TARGET_LENGTH = 10.0 * cm;
}

#endif
