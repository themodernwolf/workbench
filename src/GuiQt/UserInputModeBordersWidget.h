#ifndef __USER_INPUT_MODE_BORDERS_WIDGET__H_
#define __USER_INPUT_MODE_BORDERS_WIDGET__H_

/*LICENSE_START*/
/*
 *  Copyright (C) 2014  Washington University School of Medicine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*LICENSE_END*/


#include <QWidget>

class QAction;
class QActionGroup;
class QComboBox;
class QStackedWidget;
class QToolButton;

namespace caret {

    class BorderFile;
    class Border;
    class Brain;
    class Surface;
    class UserInputModeBorders;
    
    class UserInputModeBordersWidget : public QWidget {
        
        Q_OBJECT

    public:
        UserInputModeBordersWidget(UserInputModeBorders* inputModeBorders,
                                   QWidget* parent = 0);
        
        virtual ~UserInputModeBordersWidget();
        
        void updateWidget();
        
        void executeFinishOperation();
        
        void executeRoiInsideSelectedBorderOperation(Brain* brain,
                                                     Surface* surface,
                                                     Border* border);
        
    private slots:
        void adjustViewActionTriggered();
        void drawOperationActionTriggered(QAction*);
        void editOperationActionTriggered(QAction*);
        
        void modeComboBoxSelection(int);
        
        void drawResetButtonClicked();
        void drawUndoButtonClicked();
        void drawUndoLastEditButtonClicked();
        void drawFinishButtonClicked();
        
    private:
        class BorderFileAndBorderMemento {
        public:
            BorderFileAndBorderMemento(BorderFile* borderFile,
                                       Border* border) {
                m_borderFile = borderFile;
                m_border     = border;
            }
            
            BorderFile* m_borderFile;
            Border*     m_border;
        };
        
        UserInputModeBordersWidget(const UserInputModeBordersWidget&);

        UserInputModeBordersWidget& operator=(const UserInputModeBordersWidget&);
        
        void setActionGroupByActionData(QActionGroup* actionGroup, 
                                        const int dataInteger);
        
        QActionGroup* drawOperationActionGroup;
        
        QActionGroup* editOperationActionGroup;
        
        QWidget* createModeWidget();
        
        QWidget* createDrawOperationWidget();
        
        QWidget* createEditOperationWidget();
        
        QWidget* createRoiOperationWidget();
        
        void setLastEditedBorder(std::vector<BorderFileAndBorderMemento>& undoFinishBorders);
        
        void resetLastEditedBorder();
        
        QComboBox* modeComboBox;
        
        QWidget* widgetMode;
        
        QWidget* widgetDrawOperation;
        
        QWidget* widgetEditOperation;
        
        QWidget* widgetRoiOperation;
        
        QStackedWidget* operationStackedWidget;
        
        UserInputModeBorders* inputModeBorders;
        
        QString m_transformToolTipText;
        
        QToolButton* m_undoFinishToolButton;
        
        std::vector<BorderFileAndBorderMemento> m_undoFinishBorders;
    };
    
#ifdef __USER_INPUT_MODE_BORDERS_WIDGET_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __USER_INPUT_MODE_BORDERS_WIDGET_DECLARE__

} // namespace
#endif  //__USER_INPUT_MODE_BORDERS_WIDGET__H_
