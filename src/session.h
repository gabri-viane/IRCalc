#pragma once
#ifndef H_SESSION_INCLUDED
#define H_SESSION_INCLUDED 1

#include "commons.h"
#include "datatypes.h"
#include "programflow.h"

#include <fxcg/file.h>

void testSave();

void loadFile(AppStatus* as);

void saveToFile(AppStatus *as);

void deleteFile(AppStatus* as);

#endif