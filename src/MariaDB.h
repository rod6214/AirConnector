#pragma once

#ifndef MARIADBN_H
#define MARIADBN_H


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string.h>
#include <regex>
#include "PlaneEvent.h"

void testConn();
void DLLTEMPLATE_API insertDataPlane(std::shared_ptr<PlaneData> ptr_planedata);
bool DLLTEMPLATE_API positionHasChanged(std::shared_ptr<PlaneData> ptr_planedata);
std::filesystem::path DLLTEMPLATE_API getParentPath();
std::filesystem::path DLLTEMPLATE_API getConfigurationPath();
#endif