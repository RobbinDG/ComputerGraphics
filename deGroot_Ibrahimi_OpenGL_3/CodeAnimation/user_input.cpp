#include "mainview.h"

#include <QDebug>

// Triggered by pressing a key
void MainView::keyPressEvent(QKeyEvent *ev) {
    switch(ev->key()) {
    case 'A': qDebug() << "A pressed"; break;
    default:
        // ev->key() is an integer. For alpha numeric characters keys it equivalent with the char value ('A' == 65, '1' == 49)
        // Alternatively, you could use Qt Key enums, see http://doc.qt.io/qt-5/qt.html#Key-enum
        qDebug() << ev->key() << "pressed";
        break;
    }

    // Used to update the screen after changes
    update();
}

// Triggered by releasing a key
void MainView::keyReleaseEvent(QKeyEvent *ev) {
    QVector3D movement;
    switch(ev->key()) {
    case 'W':
        movement.setZ(-1.0f);
        scene.move(movement * WASDspeed);
        break;
    case 'A':
        movement.setX(-1.0f);
        scene.move(movement * WASDspeed);
        break;
    case 'S':
        movement.setZ(1.0f);
        scene.move(movement * WASDspeed);
        break;
    case 'D':
        movement.setX(1.0f);
        scene.move(movement * WASDspeed);
        break;
    case Qt::Key_Up:
        movement.setY(1.0f);
        scene.move(movement * WASDspeed);
        break;
    case Qt::Key_Down:
        movement.setY(-1.0f);
        scene.move(movement * WASDspeed);
        break;
    default:
        qDebug() << ev->key() << "released";
        break;
    }

    update();
}

// Triggered by clicking two subsequent times on any mouse button
// It also fires two mousePress and mouseRelease events!
void MainView::mouseDoubleClickEvent(QMouseEvent *ev) {
    qDebug() << "Mouse double clicked:" << ev->button();

    update();
}

// Triggered when moving the mouse inside the window (only when the mouse is clicked!)
void MainView::mouseMoveEvent(QMouseEvent *ev) {
    qDebug() << "x" << ev->x() << "y" << ev->y();

    update();
}

// Triggered when pressing any mouse button
void MainView::mousePressEvent(QMouseEvent *ev) {
    qDebug() << "Mouse button pressed:" << ev->button();

    update();
    // Do not remove the line below, clicking must focus on this widget!
    this->setFocus();
}

// Triggered when releasing any mouse button
void MainView::mouseReleaseEvent(QMouseEvent *ev) {
    qDebug() << "Mouse button released" << ev->button();

    update();
}

// Triggered when clicking scrolling with the scroll wheel on the mouse
void MainView::wheelEvent(QWheelEvent *ev) {
    qDebug() << "Mouse wheel:" << ev->delta();

    update();
}
