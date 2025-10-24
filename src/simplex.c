#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>

#include "commons.h"
#include "datatypes.h"
#include "gui.h"
#include "messages.h"
#include "programflow.h"
#include "session.h"
#include "solver.h"

int main(void) {
    initMenus();
    AppStatus* as = getAppStatus();
    as->currentMenuHandler = handleMenu_CellData;

    testSave();

    int key = 0;
    Bdisp_AllClr_VRAM();
    // gestione file
    loadFile(as);
    while (1) {
        handleMenu_CellData(as);
        GetKey(&key);
        if (key == KEY_CTRL_DEL) {
            deleteFile(as);
        } else if (key == KEY_CTRL_EXE) {
            saveToFile(as);
        }
    }

    return 0;
}
