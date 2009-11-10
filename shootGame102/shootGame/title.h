#include "structs.h"
#include "dispflgs.h"

#ifndef _TITLE_H
#define _TITLE_H

    int DrawTitle(Screen *);
    int DrawSelect(Screen *);
    int DrawScore(Screen *);
    int DrawNowLoad(Screen *);

    int InitTitle(Screen *);
    int InitSelect(Screen *);
    int InitScore(Screen *);
    int InitNowLoad(Screen *);
#endif
