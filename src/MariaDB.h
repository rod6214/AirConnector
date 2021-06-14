#pragma once

#ifndef MARIADBN_H
#define MARIADBN_H

#include <mariadb/conncpp.hpp>
#include "PlaneEvent.h"

void testConn();
void DLLTEMPLATE_API insertDataPlane(std::shared_ptr<PlaneData> ptr_planedata);
bool DLLTEMPLATE_API positionHasChanged(std::shared_ptr<PlaneData> ptr_planedata);
#endif