#pragma once

#ifndef MARIADBN_H
#define MARIADBN_H

#include <mariadb/conncpp.hpp>
#include <PlaneEvent.h>
//#include "PlaneEvent.h"

void testConn();
void insertDataPlane(AirPlane* airplane);

#endif