/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of application constants
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "BeeftextConstants.h"


namespace constants {


QString const kApplicationName = "Beeftext";
QString const kOrganizationName = "beeftext.org";
qint32 const kVersionMajor = 1;
qint32 const kVersionMinor = 8;
QString const kBeeftextWikiHomeUrl = "https://github.com/xmichelo/Beeftext/wiki";
QString const kGettingStartedUrl = "https://github.com/xmichelo/Beeftext/wiki/Getting-started";
QString const kBeeftextWikiVariablesUrl = "https://github.com/xmichelo/Beeftext/wiki/Variables";
QString const kJsonFileDialogFilter = QObject::tr("JSON files (*.json);;All files (*.*)");
Qt::DateFormat const kJsonExportDateFormat = Qt::ISODateWithMs;
QChar const kEmojiDelimiter = '|';

QString const kStyleSheet = R"(

* {
outline: none; /* Disable the focus dotted rectangle nearly everywhere */
}

QWidget {
   font-family: "Segoe UI";
   font-size: 10pt;
   color: #464646;
   background-color: #eee;
}


/*****************************
* QPushButton
*****************************/
QPushButton {
   min-width: 70px;
   padding: 3px 5px 4px 5px;
   border-radius: 3px;
   background-color: #258fc0;
   border: none;
   color: #fff;
   font-weight: 600;
}

QPushButton:hover {
   background-color: #2e9fd1;	
}

QPushButton:pressed {
   background-color: #3cb3e7;
}

QPushButton:disabled {
   background-color: #ccc;
   color: #aaa;
}

QPushButton:focus {
   background-color: #006897;
}  

QPushButton:focus:hover {
   background-color: #2a79a9;
} 

QPushButton:focus:pressed {
   background-color: #3783b4;
} 

QPushButton::menu-indicator {
   width: 0px;
   height: 0px;
   padding: 0px;
   margin: 0px;
}


/*****************************
* All Edits
*****************************/	
QRadioButton:disabled {
   color: #aaa;
}


/*****************************
* All Edits
*****************************/	
QLineEdit, QPlainTextEdit, QTextEdit {
   background-color: #fff;
   border: 1px solid #ccc;
   selection-color: #fff;
   selection-background-color: #258fc0; 
}

QLineEdit:disabled {
   color: #aaa;
}

QAbstractScrollArea {
background-color: #fff;
}

QPlainTextEdit {
   font-family: Consolas;
}

QLineEdit:read-only {
   background-color: #eee;
}

/*****************************
* QComboBox
*****************************/
QComboBox {
   background-color: #fff;
   border: 1px solid #ccc;
   padding: 1px 5px 1px 5px;
}

QComboBox::drop-down {
   width: 24px;
   background-color: #eee;
   border: none;
   border-left: 1px solid #ccc;
}

QComboBox QAbstractItemView {
    border: 1px solid #ccc;
    selection-color: #fff;
    selection-background-color: #258fc0;
}

QComboBox::down-arrow {
image: url(:/MainWindow/Resources/ArrowDown.png);
}

/*****************************
* QTableView
*****************************/
QTableView {
   border: 1px solid #ccc;
}
QHeaderView:section { 
   background: #ddd; 
   border: none;
   padding: 5px;
   font-weight: bold;
}

QHeaderView:section:!last {
   border-right: 1px solid #fff;
}

QTableView::item {
   border: 0px; /* required in order for the padding to work. Don't ask why */
   padding-right: 10px;
}


QTableView::item:selected {
   background-color: #258fc0;
   color: #fff;
}


/*****************************
* QTableView
*****************************/
QListView {
   border: 1px solid #ccc;
}


QListView::item {
   border: 0px; /* required in order for the padding to work. Don't ask why */
   padding-right: 10px;
   padding: 3px 5px 3px 5px;
}

QListView::item:selected {
   background-color: #258fc0;
   color: #fff;
}

/*****************************
* QLineEdit
*****************************/
QLineEdit#editSearch {
   border-radius: 3px;
   border: 1px solid #ccc;
   padding: 2px;
}

/*****************************
* QScrollbar
*****************************/

QScrollBar {
   border: none;
   background: #eee;
   margin: 0;
}

QScrollBar:vertical {
   border-left: 1px solid #ccc;
   width: 13px; 
}

QScrollBar:horizontal {
   border-top: 1px solid #ccc;
   height: 13px;  
}


QScrollBar::handle {
   background: #c6c6c6;
   border-radius: 4px;
   margin: 1px;
}


QScrollBar::add-line:vertical {
height: 0px;
subcontrol-position: bottom;
subcontrol-origin: margin;
        }

QScrollBar::sub-line:vertical {
   height: 0px;
   subcontrol-position: top;
   subcontrol-origin: margin;
}

QScrollBar::add-page, QScrollBar::sub-page {
   background: transparent;
}

QScrollBar::add-line:horizontal {
   width: 0px;
   subcontrol-position: left;
   subcontrol-origin: margin;
}

QScrollBar::sub-line:horizontal {
   width: 0px;
   subcontrol-position: right;
   subcontrol-origin: margin;
}

/*****************************
* QGroupbox
*****************************/
QGroupBox {
   background-color: transparent;
   border: 1px solid #ccc;
   border-radius: 0px;
   margin-top: 9px; 
   font-weight: bold;
}

QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left; 
    padding: 0px 3px;
    background-color: #eee;
    left: 10px
}

/*****************************
* QTabWidget
*****************************/
QTabWidget:pane {
   border: 1px solid #ccc;

}

QTabWidget::tab-bar {
  subcontrol-position: top left;

}

QTabBar::tab {
  min-width: 100px;
  border: 1px solid #ccc;
  padding: 5px;
  margin-top: 3px;
  margin-bottom: -1px;
  border-top-left-radius: 5px;
  border-top-right-radius: 5px;
  font-weight: 600;
}

QTabBar::tab:selected {
   background-color: #fff;
   margin: 0px -1px -1px -1px;
}

QTabBar::tab:selected:first {
   margin-left: 0;
}

QTabBar::tab:selected:last {
   margin-right: 0;
}

QTabBar::tab:!selected:!first {
  margin-left: -1px
}

/*****************************
* Menu
*****************************/
QMenuBar {
   border-bottom: 1px solid #ccc;
   font-weight: bold;
}

QMenuBar::item:selected {
   border: none;
   background-color: #258fc0;
   color: #fff;
}

QMenu {
   border: 1px solid #ccc;
}

QMenu::item {
   padding: 4px 20px 4px 10px;
}

QMenu::item:selected {
   border: none;
   background-color: #258fc0;
   color: #fff;
}

QMenu::item:default {
   font-weight: bold;
}

QMenu::item:disabled {
   color: #aaa;
}

QMenu::item:disabled:selected {
   background: none;
}

QMenu::right-arrow {
   width: 6px;
   height: 6px;
   top: 2px;
   right: 2px;
}

QMenu::left-arrow {
   width: 6px;
   height: 6px;
   top: 2px;
   left: 2px;
}

QMenu::separator {
   background: #ccc;
   height: 1px;
   margin: 5px 2px 5px 2px;

}

QProgressBar {
   border: 1px solid #ccc;
   text-align: center;
   color: #fff;
   background-color: #ddd;   
}


QProgressBar::chunk {
  background-color: #258fc0;
}


)";


} // namespace constants
