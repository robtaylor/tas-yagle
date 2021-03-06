/*------------------------------------------------------------\
|                                                             |
| Tool    :                   XYAG                            |
|                                                             |
| File    :                  panel.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                        Lester Anthony                       |
|                                                             |
\------------------------------------------------------------*/

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/PushB.h>
#include <Xm/DialogS.h>

#include MUT_H
#include MLO_H
#include XTB_H
#include XSB_H

#include "XTB_panel.h"
#include "XTB_dialog.h"

static char    *XyagPanelButtonDisable = "xxxx";

/*------------------------------------------------------------\
|                       XyagBuildPanel                       |
\------------------------------------------------------------*/

void 
XyagBuildPanel(Father, Panel)
    Widget          Father;
    XyagPanelItem  *Panel;
{
    Arg             Args[13];
    XyagPanelButtonItem *Button;
    char           *Label;
    int             Counter;
    int             Fraction;
    int             X1;
    int             Y1;
    int             X2;
    int             Y2;
    Pixmap          ButtonPixmap;

    Fraction = Panel->ROW * Panel->COLUMN;

    XtSetArg(Args[0], XmNshadowType, XmSHADOW_ETCHED_IN);
    XtSetArg(Args[1], XmNdeleteResponse, XmDO_NOTHING);
    XtSetArg(Args[2], XmNtitle, Panel->TITLE);

    Panel->PANEL = XmCreateFormDialog(Father, Panel->TITLE, Args, 3);

    XtAddCallback(Panel->PANEL, XmNdestroyCallback,
                  XyagDestroyDialogCallback, NULL);

    Panel->PANEL_FORM = XtVaCreateManagedWidget("",
                                                xmFormWidgetClass,
                                                Panel->PANEL,
                                                XmNtopAttachment, XmATTACH_FORM,
                                                XmNbottomAttachment, XmATTACH_FORM,
                                                XmNleftAttachment, XmATTACH_FORM,
                                                XmNrightAttachment, XmATTACH_FORM,
                                                XmNfractionBase, 100,
                                                NULL);

    Panel->FRAME = XtVaCreateManagedWidget("",
                                           xmFrameWidgetClass,
                                           Panel->PANEL_FORM,
                                           XmNtopAttachment, XmATTACH_FORM,
                                           XmNbottomAttachment, XmATTACH_FORM,
                                           XmNleftAttachment, XmATTACH_FORM,
                                           XmNrightAttachment, XmATTACH_FORM,
                                           NULL);

    Panel->FORM = XtVaCreateManagedWidget("",
                                          xmFormWidgetClass,
                                          Panel->FRAME,
                                          XmNtopAttachment, XmATTACH_FORM,
                                          XmNbottomAttachment, XmATTACH_FORM,
                                          XmNleftAttachment, XmATTACH_FORM,
                                          XmNrightAttachment, XmATTACH_FORM,
                                          XmNfractionBase, Fraction,
                                          NULL);
    Button = Panel->LIST;

    for (Counter = 0; Button[Counter].LABEL != NULL; Counter++) {
        X1 = Button[Counter].X * Panel->ROW;
        Y1 = Button[Counter].Y * Panel->COLUMN;
        X2 = X1 + Button[Counter].DX * Panel->ROW;
        Y2 = Y1 + Button[Counter].DY * Panel->COLUMN;

        if (*Button[Counter].LABEL[0] != '\0') {
            Label = *Button[Counter].LABEL;
        }
        else {
            Label = XyagPanelButtonDisable;
        }

        if ((Button[Counter].TEXT != (char *) NULL) &&
            (Button[Counter].BITMAP != (char *) NULL)) {
            XtSetArg(Args[0], XmNtopAttachment, XmATTACH_POSITION);
            XtSetArg(Args[1], XmNtopPosition, Y1);
            XtSetArg(Args[2], XmNbottomAttachment, XmATTACH_POSITION);
            XtSetArg(Args[3], XmNbottomPosition, Y2);
            XtSetArg(Args[4], XmNrightAttachment, XmATTACH_POSITION);
            XtSetArg(Args[5], XmNrightPosition, X2);
            XtSetArg(Args[6], XmNleftAttachment, XmATTACH_POSITION);
            XtSetArg(Args[7], XmNleftPosition, X1);
            XtSetArg(Args[8], XmNscrollBarDisplayPolicy, XmSTATIC);
            XtSetArg(Args[9], XmNscrollHorizontal, True);
            XtSetArg(Args[10], XmNscrollVertical, True);
            XtSetArg(Args[11], XmNlistSizePolicy, XmCONSTANT);
            XtSetArg(Args[12], XmNselectionPolicy, XmMULTIPLE_SELECT);

            Button[Counter].BUTTON = XmCreateScrolledList(Panel->FORM, Button[Counter].TEXT, Args, 13);

            XtManageChild(Button[Counter].BUTTON);
        }
        else if (Button[Counter].TEXT != (char *) NULL) {
            XtSetArg(Args[0], XmNrows, 10);
            XtSetArg(Args[1], XmNeditable, False);
            XtSetArg(Args[2], XmNeditMode, XmMULTI_LINE_EDIT);
            XtSetArg(Args[3], XmNcursorPositionVisible, False);
            XtSetArg(Args[4], XmNtopAttachment, XmATTACH_POSITION);
            XtSetArg(Args[5], XmNtopPosition, Y1);
            XtSetArg(Args[6], XmNleftAttachment, XmATTACH_POSITION);
            XtSetArg(Args[7], XmNleftPosition, X1);
            XtSetArg(Args[8], XmNbottomAttachment, XmATTACH_POSITION);
            XtSetArg(Args[9], XmNbottomPosition, Y2);
            XtSetArg(Args[10], XmNrightAttachment, XmATTACH_POSITION);
            XtSetArg(Args[11], XmNrightPosition, X2);

            Button[Counter].BUTTON = XmCreateScrolledText(Panel->FORM, Button[Counter].TEXT, Args, 12);

            XtManageChild(Button[Counter].BUTTON);
        }
        else if (Button[Counter].BITMAP != (char *) NULL) {
            ButtonPixmap = XyagCreatePixmap(Father,
                                            Button[Counter].BITMAP,
                                            Button[Counter].WIDTH,
                                            Button[Counter].HEIGHT);

            Button[Counter].BUTTON = XtVaCreateManagedWidget(Label,
                                                             xmPushButtonWidgetClass,
                                                             Panel->FORM,
                                                             XmNlabelType, XmPIXMAP,
                                                             XmNlabelPixmap, ButtonPixmap,
                                                             XmNleftAttachment, XmATTACH_POSITION,
                                                             XmNleftPosition, X1,
                                                             XmNrightAttachment, XmATTACH_POSITION,
                                                             XmNrightPosition, X2,
                                                             XmNtopAttachment, XmATTACH_POSITION,
                                                             XmNtopPosition, Y1,
                                                             XmNbottomAttachment, XmATTACH_POSITION,
                                                             XmNbottomPosition, Y2,
                                                             NULL);
        }
        else {
            Button[Counter].BUTTON = XtVaCreateManagedWidget(Label,
                                                             xmPushButtonWidgetClass,
                                                             Panel->FORM,
                                                             XmNleftAttachment, XmATTACH_POSITION,
                                                             XmNleftPosition, X1,
                                                             XmNrightAttachment, XmATTACH_POSITION,
                                                             XmNrightPosition, X2,
                                                             XmNtopAttachment, XmATTACH_POSITION,
                                                             XmNtopPosition, Y1,
                                                             XmNbottomAttachment, XmATTACH_POSITION,
                                                             XmNbottomPosition, Y2,
                                                             NULL);
        }


        if (Label != XyagPanelButtonDisable) {
            if (Button[Counter].FOREGROUND != NULL) {
                XtVaSetValues(Button[Counter].BUTTON,
                              XtVaTypedArg, XmNforeground,
                              XmRString,
                              *Button[Counter].FOREGROUND,
                              strlen(*Button[Counter].FOREGROUND) + 1,
                              NULL);
            }

            if (Button[Counter].BACKGROUND != NULL) {
                XtVaSetValues(Button[Counter].BUTTON,
                              XtVaTypedArg, XmNbackground,
                              XmRString,
                              *Button[Counter].BACKGROUND,
                              strlen(*Button[Counter].BACKGROUND) + 1,
                              NULL
                    );
            }

            if (Button[Counter].CALLBACK != NULL) {
                if ((Button[Counter].TEXT != (char *) NULL) &&
                    (Button[Counter].BITMAP != (char *) NULL)) {
                    XtAddCallback(Button[Counter].BUTTON,
                                  XmNdefaultActionCallback,
                                  Button[Counter].CALLBACK,
                                  (XtPointer) NULL);

                    XtAddCallback(Button[Counter].BUTTON,
                                  XmNmultipleSelectionCallback,
                                  Button[Counter].CALLBACK,
                                  Button[Counter].CALLDATA);

                }
                else {
                    XtAddCallback(Button[Counter].BUTTON,
                                  XmNactivateCallback,
                                  Button[Counter].CALLBACK,
                                  Button[Counter].CALLDATA);
                }
            }
        }
        else {
            Button[Counter].BUTTON = NULL;
        }
    }

    XtVaSetValues(Panel->PANEL,
                  XmNresizable, False,
                  XmNx, Panel->X,
                  XmNy, Panel->Y,
                  XmNheight, Panel->HEIGHT,
                  XmNwidth, Panel->WIDTH,
                  NULL);
}

/*------------------------------------------------------------\
|                       XyagEnterPanel                        |
\------------------------------------------------------------*/

void 
XyagEnterPanel(Panel)
    XyagPanelItem  *Panel;
{
    XtManageChild(Panel->PANEL);
    XtPopup(XtParent(Panel->PANEL), XtGrabNone);

    if (Panel->COMPUTE == 1) {
//        XtVaSetValues(Panel->PANEL,
//                      XmNresizable, False,
//                      XmNx, Panel->X,
//                      XmNy, Panel->Y,
//                      XmNheight, Panel->HEIGHT,
//                      XmNwidth, Panel->WIDTH,
//                      NULL);
//
        Panel->COMPUTE = 0;
    }

    Panel->MANAGED = 1;
}

/*------------------------------------------------------------\
|                        XyagExitPanel                        |
\------------------------------------------------------------*/

void 
XyagExitPanel(Panel)
    XyagPanelItem  *Panel;
{
    XtUnmanageChild(Panel->PANEL);
    Panel->MANAGED = 0;
}
