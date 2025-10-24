#include "session.h"

#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/system.h>
#include <string.h>

static void autoSave();

static void autoSave() {
    AppStatus* as = getAppStatus();
    if (as->timerRunning) {
        Timer_Stop(as->timerID);
        Timer_Deinstall(as->timerID);
        as->timerRunning = false;
    }
    unsigned short pathToFile[64];
    unsigned char* fileName = "\\\\fls0\\irSession.sex";
    Bfile_StrToName_ncpy(pathToFile, fileName, 21);
    int file_handle = Bfile_OpenFile_OS(pathToFile, WRITE, 0);
    if (file_handle < 0) {
        int size = sizeof(CellData);
        file_handle = Bfile_CreateEntry_OS(pathToFile, CREATEMODE_FILE, &size);
        if (file_handle < 0) {
            return;
        }
    }
    Bfile_SeekFile_OS(file_handle, 0);
    char buff[sizeof(CellData)];
    memcpy(buff, as->cd, sizeof(CellData));
    int res = Bfile_WriteFile_OS(file_handle, buff, sizeof(CellData));
    Bfile_CloseFile_OS(file_handle);
    if (!as->timerRunning) {
        as->timerID = Timer_Install(0, autoSave, 120000);  // salva ogni 2 min
        if (as->timerID < 0) {
            return;
        }
        Timer_Start(as->timerID);
        as->timerRunning = true;
    }
}

void testSave() {
    CellData cd = CREATE_CELL();
    cd.domanda_pezzi = 12000;
    cd.tempo_setup = 35644;
    unsigned short pathToFile[64];
    unsigned char* fileName = "\\\\fls0\\test.sex";
    Bfile_StrToName_ncpy(pathToFile, fileName, 16);
    int file_handle = Bfile_OpenFile_OS(pathToFile, WRITE, 0);
    if (file_handle < 0) {
        int size = sizeof(CellData);
        file_handle = Bfile_CreateEntry_OS(pathToFile, CREATEMODE_FILE, &size);
        if (file_handle < 0) {
            return;
        }
    }

    Bfile_SeekFile_OS(file_handle, 0);
    Bfile_WriteFile_OS(file_handle, &cd, sizeof(CellData));
    Bfile_CloseFile_OS(file_handle);
}

void loadFile(AppStatus* as) {
    if (as->timerRunning) {
        Timer_Stop(as->timerID);
        Timer_Deinstall(as->timerID);
        as->timerRunning = false;
    }
    unsigned short pathToFile[64];
    unsigned char* fileName = "\\\\fls0\\irSession.sex";
    Bfile_StrToName_ncpy(pathToFile, fileName, 21);
    int file_handle = Bfile_OpenFile_OS(pathToFile, READ, 0);
    if (file_handle < 0) {
        int size = sizeof(CellData);
        file_handle = Bfile_CreateEntry_OS(pathToFile, CREATEMODE_FILE, &size);
        if (file_handle < 0) {
            DefineStatusMessage("Err.Open File", 0, TEXT_COLOR_RED, 0);
            Bdisp_HeaderText();
            Bdisp_PutDisp_DD();
            return;
        }
        Bfile_WriteFile_OS(file_handle, as->cd, size);
        DefineStatusMessage("File created", 0, TEXT_COLOR_GREEN, 0);
        Bdisp_HeaderText();
        Bdisp_PutDisp_DD();
        Bfile_CloseFile_OS(file_handle);
        return;
    }
    Bfile_SeekFile_OS(file_handle, 0);
    int res = Bfile_ReadFile_OS(file_handle, as->cd, sizeof(CellData), 0);
    if (res < 0) {
        DefineStatusMessage("Err.Read File", 0, TEXT_COLOR_RED, 0);
    } else {
        DefineStatusMessage("File Read", 0, TEXT_COLOR_GREEN, 0);
    }
    Bdisp_HeaderText();
    Bdisp_PutDisp_DD();
    Bfile_CloseFile_OS(file_handle);

    if (!as->timerRunning) {
        as->timerID = Timer_Install(0, autoSave, 120000);  // salva ogni 2 min
        if (as->timerID < 0) {
            return;
        }
        Timer_Start(as->timerID);
        as->timerRunning = true;
    }
}

void saveToFile(AppStatus* as) {
    if (as->timerRunning) {
        Timer_Stop(as->timerID);
        Timer_Deinstall(as->timerID);
        as->timerRunning = false;
    }
    unsigned short pathToFile[64];
    unsigned char* fileName = "\\\\fls0\\irSession.sex";
    Bfile_StrToName_ncpy(pathToFile, fileName, 21);
    int file_handle = Bfile_OpenFile_OS(pathToFile, WRITE, 0);
    int size = sizeof(CellData);
    if (file_handle < 0) {
        file_handle = Bfile_CreateEntry_OS(pathToFile, CREATEMODE_FILE, &size);
        if (file_handle < 0) {
            DefineStatusMessage("Err.Create File", 0, TEXT_COLOR_RED, 0);
            Bdisp_HeaderText();
            Bdisp_PutDisp_DD();
            return;
        }
    }
    Bfile_SeekFile_OS(file_handle, 0);
    char buff[sizeof(CellData)];
    memcpy(buff, as->cd, size);
    int res = Bfile_WriteFile_OS(file_handle, buff, size);
    if (res < 0) {
        DefineStatusMessage("Err.Write File", 0, TEXT_COLOR_RED, 0);
    } else {
        DefineStatusMessage("File Written", 0, TEXT_COLOR_GREEN, 0);
    }
    Bdisp_HeaderText();
    Bdisp_PutDisp_DD();
    Bfile_CloseFile_OS(file_handle);
    if (!as->timerRunning) {
        as->timerID = Timer_Install(0, autoSave, 120000);  // salva ogni 2 min
        if (as->timerID < 0) {
            return;
        }
        Timer_Start(as->timerID);
        as->timerRunning = true;
    }
}

void deleteFile(AppStatus* as) {
    if (as->timerRunning) {
        Timer_Stop(as->timerID);
        Timer_Deinstall(as->timerID);
        as->timerRunning = false;
    }
    unsigned short pathToFile[64];
    unsigned char* fileName = "\\\\fls0\\irSession.sex";
    Bfile_StrToName_ncpy(pathToFile, fileName, 21);
    Bfile_DeleteEntry(pathToFile);
    if (!as->timerRunning) {
        as->timerID = Timer_Install(0, autoSave, 120000);  // salva ogni 2 min
        if (as->timerID < 0) {
            return;
        }
        Timer_Start(as->timerID);
        as->timerRunning = true;
    }
}
