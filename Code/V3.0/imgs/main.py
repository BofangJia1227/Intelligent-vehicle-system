import os
import sys
import time
import urllib, json
import PyQt5.QtQml
import threading
from PyQt5.Qt import *
from PyQt5.QtQuick import QQuickView
from PyQt5.QtCore import QAbstractListModel, Qt, QUrl, QStringListModel
from PyQt5.QtGui import QGuiApplication

from multiprocessing import Process

''''''
def qmlview(view):
    view.show()


if __name__ == '__main__':
    app = QGuiApplication(sys.argv)
    view = QQuickView()
    view.setResizeMode(QQuickView.SizeRootObjectToView)

    qml_file = os.path.join(os.path.dirname(__file__), 'view.qml')
    view.setSource(QUrl.fromLocalFile(os.path.abspath(qml_file)))

    if view.status() == QQuickView.Error:
        print(QQuickView.Error)
        sys.exit(-1)

    view.show()
    while True:
        app.processEvents()
        time.sleep(0.02)
    del view

