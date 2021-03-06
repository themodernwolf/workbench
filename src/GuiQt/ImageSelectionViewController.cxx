
/*LICENSE_START*/
/*
 *  Copyright (C) 2014 Washington University School of Medicine
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

#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QRadioButton>
#include <QScrollArea>

#define __IMAGE_SELECTION_VIEW_CONTROLLER_DECLARE__
#include "ImageSelectionViewController.h"
#undef __IMAGE_SELECTION_VIEW_CONTROLLER_DECLARE__

#include "Brain.h"
#include "BrowserTabContent.h"
#include "CaretAssert.h"
#include "DisplayGroupEnumComboBox.h"
#include "DisplayPropertiesImages.h"
#include "EventGraphicsUpdateAllWindows.h"
#include "EventManager.h"
#include "EventUserInterfaceUpdate.h"
#include "GuiManager.h"
#include "ImageFile.h"
#include "SceneClass.h"
#include "SceneClassAssistant.h"
#include "WuQtUtilities.h"

using namespace caret;


    
/**
 * \class caret::ImageSelectionViewController 
 * \brief View controller for image selection
 * \ingroup GuiQt
 */

/**
 * Constructor.
 */
ImageSelectionViewController::ImageSelectionViewController(const int32_t browserWindowIndex,
                                                           QWidget* parent)
: QWidget(parent),
m_browserWindowIndex(browserWindowIndex)
{
    setWindowTitle("Images");
    
    m_sceneAssistant = new SceneClassAssistant();
    
    QLabel* groupLabel = new QLabel("Group");
    m_imagesDisplayGroupComboBox = new DisplayGroupEnumComboBox(this);
    QObject::connect(m_imagesDisplayGroupComboBox, SIGNAL(displayGroupSelected(const DisplayGroupEnum::Enum)),
                     this, SLOT(imageDisplayGroupSelected(const DisplayGroupEnum::Enum)));
    
    QHBoxLayout* groupLayout = new QHBoxLayout();
    groupLayout->addWidget(groupLabel);
    groupLayout->addWidget(m_imagesDisplayGroupComboBox->getWidget());
    groupLayout->addStretch();
    
    m_imageDisplayCheckBox = new QCheckBox("Display Image");
    QObject::connect(m_imageDisplayCheckBox, SIGNAL(clicked(bool)),
                     this, SLOT(processSelectionChanges()));
    
    m_imageRadioButtonGroup = new QButtonGroup(this);
    QObject::connect(m_imageRadioButtonGroup, SIGNAL(buttonClicked(int)),
                     this, SLOT(imageRadioButtonClicked(int)));
    
    QWidget* imageRadioButtonWidget = new QWidget();
    imageRadioButtonWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_imageRadioButtonLayout = new QVBoxLayout(imageRadioButtonWidget);
    
    QScrollArea* imageRadioButtonScrollArea = new QScrollArea();
    imageRadioButtonScrollArea->setWidget(imageRadioButtonWidget);
    imageRadioButtonScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    imageRadioButtonScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    imageRadioButtonScrollArea->setWidgetResizable(true);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
//    WuQtUtilities::setLayoutSpacingAndMargins(layout, 2, 2);
    layout->addLayout(groupLayout, 0);
    layout->addWidget(m_imageDisplayCheckBox, 0);
    layout->addSpacing(10);
    layout->addWidget(imageRadioButtonScrollArea, 100);
    layout->addStretch();
    
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_USER_INTERFACE_UPDATE);
    
    s_allImageSelectionViewControllers.insert(this);
}

/**
 * Destructor.
 */
ImageSelectionViewController::~ImageSelectionViewController()
{
    EventManager::get()->removeAllEventsFromListener(this);
    
    s_allImageSelectionViewControllers.erase(this);
    
    delete m_sceneAssistant;
}

/**
 * Receive an event.
 *
 * @param event
 *    An event for which this instance is listening.
 */
void
ImageSelectionViewController::receiveEvent(Event* event)
{
    bool doUpdate = false;
    
    if (event->getEventType() == EventTypeEnum::EVENT_USER_INTERFACE_UPDATE) {
        EventUserInterfaceUpdate* uiEvent = dynamic_cast<EventUserInterfaceUpdate*>(event);
        CaretAssert(uiEvent);
        
        if (uiEvent->isUpdateForWindow(m_browserWindowIndex)) {
            if (uiEvent->isToolBoxUpdate()) {
                doUpdate = true;
                uiEvent->setEventProcessed();
            }
        }
    }
    
    if (doUpdate) {
        updateImageViewController();
    }
}

/**
 * Called when a selection changes.
 */
void
ImageSelectionViewController::processSelectionChanges()
{
    DisplayPropertiesImages* dpi = GuiManager::get()->getBrain()->getDisplayPropertiesImages();
    
    BrowserTabContent* browserTabContent =
    GuiManager::get()->getBrowserTabContentForBrowserWindow(m_browserWindowIndex, true);
    if (browserTabContent == NULL) {
        return;
    }
    const int32_t browserTabIndex = browserTabContent->getTabNumber();
    const DisplayGroupEnum::Enum displayGroup = dpi->getDisplayGroupForTab(browserTabIndex);
    dpi->setDisplayed(displayGroup,
                      browserTabIndex,
                      m_imageDisplayCheckBox->isChecked());
    
    EventManager::get()->sendEvent(EventGraphicsUpdateAllWindows().getPointer());
    
    updateOtherImageViewControllers();
}

/**
 * Called when the display group is changed.
 *
 * @param displayGroup
 *    Display group selected.
 */
void
ImageSelectionViewController::imageDisplayGroupSelected(const DisplayGroupEnum::Enum displayGroup)
{
    /*
     * Update selected display group in model.
     */
    BrowserTabContent* browserTabContent =
    GuiManager::get()->getBrowserTabContentForBrowserWindow(m_browserWindowIndex, false);
    if (browserTabContent == NULL) {
        return;
    }
    
    const int32_t browserTabIndex = browserTabContent->getTabNumber();
    Brain* brain = GuiManager::get()->getBrain();
    DisplayPropertiesImages* dsb = brain->getDisplayPropertiesImages();
    dsb->setDisplayGroupForTab(browserTabIndex,
                               displayGroup);
    
    /*
     * Since display group has changed, need to update controls
     */
    updateImageViewController();
}

/**
 * Called when an image radio button is clicked.
 *
 * @param buttonID
 *    ID of button that was clicked.
 */
void
ImageSelectionViewController::imageRadioButtonClicked(int buttonID)
{
    BrowserTabContent* browserTabContent =
    GuiManager::get()->getBrowserTabContentForBrowserWindow(m_browserWindowIndex, true);
    if (browserTabContent == NULL) {
        return;
    }
    const int32_t browserTabIndex = browserTabContent->getTabNumber();
    Brain* brain = GuiManager::get()->getBrain();
    DisplayPropertiesImages* dpi = brain->getDisplayPropertiesImages();
    const DisplayGroupEnum::Enum displayGroup = dpi->getDisplayGroupForTab(browserTabIndex);
    std::vector<ImageFile*> allImageFiles = brain->getAllImagesFiles();
    CaretAssertVectorIndex(allImageFiles, buttonID);
    
    dpi->setSelectedImageFile(displayGroup,
                              browserTabIndex,
                              allImageFiles[buttonID]);

    updateOtherImageViewControllers();
}

/**
 * Update the image selection widget.
 */
void
ImageSelectionViewController::updateImageViewController()
{
    BrowserTabContent* browserTabContent =
    GuiManager::get()->getBrowserTabContentForBrowserWindow(m_browserWindowIndex, true);
    if (browserTabContent == NULL) {
        return;
    }
    
    const int32_t browserTabIndex = browserTabContent->getTabNumber();
    Brain* brain = GuiManager::get()->getBrain();
    DisplayPropertiesImages* dpi = brain->getDisplayPropertiesImages();
    const DisplayGroupEnum::Enum displayGroup = dpi->getDisplayGroupForTab(browserTabIndex);
    
    m_imagesDisplayGroupComboBox->setSelectedDisplayGroup(dpi->getDisplayGroupForTab(browserTabIndex));
    
    std::vector<ImageFile*> allImageFiles = brain->getAllImagesFiles();
    
    
    m_imageDisplayCheckBox->setChecked(dpi->isDisplayed(displayGroup,
                                                          browserTabIndex));
    
    
    const int32_t numImageFiles = static_cast<int32_t>(allImageFiles.size());
    int32_t numRadioButtons = static_cast<int32_t>(m_imageRadioButtons.size());

    if (numImageFiles > numRadioButtons) {
        const int32_t numToAdd = numImageFiles - numRadioButtons;
        for (int32_t i = 0; i < numToAdd; i++) {
            const int buttonID = static_cast<int>(m_imageRadioButtons.size());
            QRadioButton* rb = new QRadioButton("");
            m_imageRadioButtons.push_back(rb);
            m_imageRadioButtonLayout->addWidget(rb);
            
            m_imageRadioButtonGroup->addButton(rb,
                                               buttonID);
        }
        
        numRadioButtons = static_cast<int32_t>(m_imageRadioButtons.size());
    }
    
    const ImageFile* selectedImageFile = dpi->getSelectedImageFile(displayGroup,
                                                                   browserTabIndex);
    for (int32_t i = 0; i < numRadioButtons; i++) {
        QRadioButton* rb = m_imageRadioButtons[i];
        
        if (i < numImageFiles) {
            rb->setText(allImageFiles[i]->getFileNameNoPath());
            if (allImageFiles[i] == selectedImageFile) {
                rb->setChecked(true);
            }
            rb->setVisible(true);
        }
        else {
            rb->setVisible(false);
        }
    }
}

/**
 * Update other selection toolbox since they should all be the same.
 */
void
ImageSelectionViewController::updateOtherImageViewControllers()
{
    for (std::set<ImageSelectionViewController*>::iterator iter = s_allImageSelectionViewControllers.begin();
         iter != s_allImageSelectionViewControllers.end();
         iter++) {
        ImageSelectionViewController* ivc = *iter;
        if (ivc != this) {
            ivc->updateImageViewController();
        }
    }
}


/**
 * Save information specific to this type of model to the scene.
 *
 * @param sceneAttributes
 *    Attributes for the scene.  Scenes may be of different types
 *    (full, generic, etc) and the attributes should be checked when
 *    saving the scene.
 *
 * @param instanceName
 *    Name of instance in the scene.
 */
SceneClass*
ImageSelectionViewController::saveToScene(const SceneAttributes* sceneAttributes,
                                 const AString& instanceName)
{
    SceneClass* sceneClass = new SceneClass(instanceName,
                                            "ImageSelectionViewController",
                                            1);
    m_sceneAssistant->saveMembers(sceneAttributes,
                                  sceneClass);
    
    // Uncomment if sub-classes must save to scene
    //saveSubClassDataToScene(sceneAttributes,
    //                        sceneClass);
    
    return sceneClass;
}

/**
 * Restore information specific to the type of model from the scene.
 *
 * @param sceneAttributes
 *    Attributes for the scene.  Scenes may be of different types
 *    (full, generic, etc) and the attributes should be checked when
 *    restoring the scene.
 *
 * @param sceneClass
 *     sceneClass from which model specific information is obtained.
 */
void
ImageSelectionViewController::restoreFromScene(const SceneAttributes* sceneAttributes,
                                      const SceneClass* sceneClass)
{
    if (sceneClass == NULL) {
        return;
    }
    
    m_sceneAssistant->restoreMembers(sceneAttributes,
                                     sceneClass);    
    
    //Uncomment if sub-classes must restore from scene
    //restoreSubClassDataFromScene(sceneAttributes,
    //                             sceneClass);
    
}

