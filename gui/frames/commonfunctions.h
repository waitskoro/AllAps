#pragma once

#include <QPushButton>
#include "graph/plotter.h"

static void handleStopButton(QPushButton *button, Plotter *plotter)
{
    plotter->setRescale();
    bool isRescale = plotter->getRescale();
    button->setText(isRescale ? "Стоп" : "Запуск");
}
