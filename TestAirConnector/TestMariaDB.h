#pragma once

#ifndef MARIADBN_H
#define MARIADBN_H

#include <mariadb/conncpp.hpp>
#include <PlaneEvent.h>
//#include "PlaneEvent.h"

void testConn();
void insertDataPlane(std::shared_ptr<PlaneData> ptr_planedata);

#endif