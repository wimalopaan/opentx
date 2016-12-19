/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef SIMULATORMAINWINDOW_H
#define SIMULATORMAINWINDOW_H

#include "simulator.h"

#include <QDockWidget>
#include <QMainWindow>
#include <QPointer>

class DebugOutput;
class RadioData;
class RadioOutputsWidget;
class SimulatorDialog;
class SimulatorInterface;
class TrainerSimulator;
class TelemetrySimulator;

class QKeySequence;

namespace Ui {
class SimulatorMainWindow;
}

using namespace Simulator;

class SimulatorMainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit SimulatorMainWindow(QWidget * parent, SimulatorInterface * simulator, quint8 flags=0, Qt::WindowFlags wflags = Qt::WindowFlags());
    ~SimulatorMainWindow();

    bool setRadioData(RadioData * radioData);
    bool useTempDataPath(bool deleteOnClose = true, bool saveOnClose = false);
    bool setOptions(SimulatorOptions & options, bool withSave = true);
    QMenu * createPopupMenu();

  public slots:
    void start();
    void showRadioTitlebar(bool show);
    void toggleMenuBar(bool show);

  protected slots:
    virtual void closeEvent(QCloseEvent *);
    virtual void changeEvent(QEvent *e);
    void restoreUiState();
    void saveUiState();
    void luaReload(bool);
    void openJoystickDialog(bool);
    void showHelp(bool show);

  protected:
    void createDockWidgets();
    void addTool(QDockWidget * widget, Qt::DockWidgetArea area, QIcon icon = QIcon(), QKeySequence shortcut = QKeySequence());

    SimulatorInterface  * m_simulator;

    Ui::SimulatorMainWindow * ui;
    SimulatorDialog * m_simulatorWidget;
    DebugOutput * m_consoleWidget;
    RadioOutputsWidget * m_outputsWidget;

    QDockWidget * m_simulatorDockWidget;
    QDockWidget * m_consoleDockWidget;
    QDockWidget * m_telemetryDockWidget;
    QDockWidget * m_trainerDockWidget;
    QDockWidget * m_outputsDockWidget;

    QVector<keymapHelp_t> m_keymapHelp;
    int m_radioProfileId;
    bool m_firstShow;
    bool m_showRadioTitlebar;
    bool m_showMenubar;

    const static quint16 m_savedUiStateVersion;
};

#endif // SIMULATORMAINWINDOW_H