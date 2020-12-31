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
#include "mainscreen.h"
#include "ui_mainscreen.h"

#include "game/gameengine.h"
#include "game/MoveEngines/humanmoveengine.h"
#include <musicengine.h>
#include <loadoverlay.h>
#include <questionoverlay.h>
#include <the-libs_global.h>
#include <online/logindialog.h>
#include <online/onlineapi.h>
#include "online/onlinecontroller.h"
#include <QPainter>
#include <QSvgRenderer>

struct MainScreenPrivate {
    QPixmap backgroundImage;
};

MainScreen::MainScreen(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MainScreen) {
    ui->setupUi(this);
    d = new MainScreenPrivate();

    ui->spacer1->changeSize(0, SC_DPI(200), QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->topBarrier->setBounceWidget(ui->playButton);
    ui->bottomBarrier->setBounceWidget(ui->exitButton);
    ui->exitButton->setProperty("type", "destructive");

    ui->gamepadHud->setButtonText(QGamepadManager::ButtonA, tr("Select"));
    ui->gamepadHud->setButtonText(QGamepadManager::ButtonB, tr("Exit"));

    ui->gamepadHud->setButtonAction(QGamepadManager::ButtonA, GamepadHud::standardAction(GamepadHud::SelectAction));
    ui->gamepadHud->setButtonAction(QGamepadManager::ButtonB, [ = ] {
        ui->exitButton->click();
    });
}

MainScreen::~MainScreen() {
    delete d;
    delete ui;
}

void MainScreen::on_exitButton_clicked() {
    QApplication::exit();
}

void MainScreen::on_loadButton_clicked() {
    MusicEngine::playSoundEffect(MusicEngine::Selection);

    LoadOverlay* load = new LoadOverlay(this);
    connect(load, &LoadOverlay::loadData, this, [ = ](QDataStream * stream) {
        GameEnginePtr engine(new GameEngine(new HumanMoveEngine(), new HumanMoveEngine()));
        if (!engine->loadGame(stream)) {
            QuestionOverlay* question = new QuestionOverlay(this);
            question->setStandardDialog(QuestionOverlay::FileCorrupt);
            connect(question, &QuestionOverlay::accepted, question, &QuestionOverlay::deleteLater);
            connect(question, &QuestionOverlay::rejected, question, &QuestionOverlay::deleteLater);
        } else {
            emit startGame(engine);
        }
    });
    load->load();
}

void MainScreen::on_playButton_clicked() {
    GameEnginePtr engine(new GameEngine(new HumanMoveEngine(), new HumanMoveEngine()));
    engine->startGame();
    emit startGame(engine);
}

void MainScreen::on_playOnlineButton_clicked() {
    LoginDialog* login = new LoginDialog(this);
    if (login->exec()) {
//        ui->stackedWidget->setCurrentWidget(ui->onlineScreen);
//        ui->onlineScreen->connectToOnline();

//        ReportController::setAutomaticReportingEnabled(this, true);
        OnlineController::instance()->connectToOnline();
    } else {

    }
}


void MainScreen::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
//    QSvgRenderer renderer(QStringLiteral(":/assets/background.svg"));

    QRect geometry;
    geometry.setSize(d->backgroundImage.size());
    geometry.moveCenter(QPoint(this->width() / 2, this->height() / 2));
//    renderer.render(&painter, geometry);
    painter.drawPixmap(geometry, d->backgroundImage);

    QLinearGradient grad(QPoint(0, this->height()), QPoint(0, this->height() - SC_DPI(50)));
    grad.setColorAt(0, QColor(0, 0, 0, 127));
    grad.setColorAt(1, QColor(0, 0, 0, 0));

    painter.setBrush(grad);
    painter.setPen(Qt::transparent);
    painter.drawRect(0, 0, this->width(), this->height());
}


void MainScreen::resizeEvent(QResizeEvent* event) {
    QPixmap image(":/assets/background.png");

    QSize size = image.size();
    size.scale(this->size(), Qt::KeepAspectRatioByExpanding);

    d->backgroundImage = image.scaled(size);
}
