/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2020 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

#include <QWidget>

namespace Ui {
    class PauseScreen;
}

class GameEngine;
struct PauseScreenPrivate;
class PauseScreen : public QWidget {
        Q_OBJECT

    public:
        explicit PauseScreen(GameEngine* engine, QWidget* parent = nullptr);
        ~PauseScreen();

        void previousScreen();
        void nextScreen();

    signals:
        void resume();
        void mainMenu();

    private slots:
        void on_resumeButton_clicked();

        void on_saveButton_clicked();

        void on_mainMenuButton_clicked();

        void on_stackedWidget_currentChanged(int arg1);

        void on_leftButton_clicked();

        void on_rightButton_clicked();

    private:
        Ui::PauseScreen* ui;
        PauseScreenPrivate* d;
};

#endif // PAUSESCREEN_H
