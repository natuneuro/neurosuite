/***************************************************************************
                          itemcolors.cpp  -  description
                             -------------------
    begin                : Tue Sep 16 2004
    copyright            : (C) 2003 by Lynn Hazan
    email                : lynn.hazan@myrealbox.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
// application specific includes
#include "itemcolors.h"

//Added by qt3to4:
#include <QList>
#include <QDebug>

ItemColors::ItemColors()
    :itemList(),
      colorChanged(false)
{
}

ItemColors::~ItemColors(){
    qDebug() << "~ItemColors()";
    qDeleteAll(itemList);
    itemList.clear();
}


ItemColors::ItemColors(const ItemColors& origin):colorChanged(origin.colorChanged){
    //Insert into itemList a deep copy of all the elements of origin.itemList
    //ItemColor* itemColor;
    QList<ItemColor*> originItemList =  origin.itemList;

    for (int i = 0; i < originItemList.size(); ++i) {
        itemList.append(new ItemColor(*originItemList.at(i)));
    }
}

QColor ItemColors::color(int identifier, SelectionMode mode){
    ItemColors::ItemColor* theItemColor = 0L;

    if(mode == BY_INDEX)
      theItemColor = itemList.at(static_cast<uint>(identifier));
    else if (mode == BY_ITEM_NUMBER)
      theItemColor = itemColor(identifier);

    //In case no ItemColor have been find (should not happen), return black.
    if(theItemColor == NULL)
      return QColor(Qt::black);
    else
      return theItemColor->color;
}

void ItemColors::setColor(int identifier, const QColor &color, SelectionMode mode){
    ItemColors::ItemColor* theItemColor = 0L;

    if(mode == BY_INDEX)
      theItemColor = itemList.at(static_cast<uint>(identifier));
    else if (mode == BY_ITEM_NUMBER)
      theItemColor = itemColor(identifier);

    theItemColor->color = color;
}

int ItemColors::itemId(int index){
    return (itemList.at(static_cast<uint>(index)))->itemId;
}

QString ItemColors::itemLabel(int index){
    return (itemList.at(static_cast<uint>(index)))->label;
}

QString ItemColors::itemLabelById(int id){
    if(itemColorIndex(id) == -1)
        return QString();
    else
        return (itemList.at(static_cast<uint>(itemColorIndex(id))))->label;
}


bool ItemColors::contains(int itemId){
    if(itemColorIndex(itemId) == -1)
        return false;
    else
        return true;
}


bool ItemColors::isColorChanged(int identifier, SelectionMode mode){
    ItemColors::ItemColor* theItemColor = 0L;

    if(mode == BY_INDEX)
      theItemColor = itemList.at(static_cast<uint>(identifier));
    else if (mode == BY_ITEM_NUMBER)
      theItemColor = itemColor(identifier);

    return theItemColor->isChanged;
}

void ItemColors::setColorChanged(int identifier, bool changed, SelectionMode mode){
    ItemColors::ItemColor* theItemColor = 0L;

    if(mode == BY_INDEX)
      theItemColor = itemList.at(static_cast<uint>(identifier));
    else if (mode == BY_ITEM_NUMBER)
      theItemColor = itemColor(identifier);

    theItemColor->isChanged = changed;
    //Update colorChanged if necessary (the change status is true and colorChanged is not already true)
    if(changed && !colorChanged) colorChanged = true;
}

uint ItemColors::append(int itemId, const QColor &color){
    itemList.append(new ItemColor(itemId,color));
    return itemList.count();
}

uint ItemColors::append(int itemId,const QString& label,const QColor& color){
    itemList.append(new ItemColor(itemId,color,label));
    return itemList.count();
}


void ItemColors::insert(int itemId, const QColor& color,int index){
    itemList.insert(index, new ItemColor(itemId,color));
}

void ItemColors::insert(int itemId, const QString &label, const QColor& color, int index){
    itemList.insert(index, new ItemColor(itemId,color,label));
}

bool ItemColors::remove(int identifier, SelectionMode mode){
  if(mode == BY_INDEX) {
      if(identifier>=0 && identifier<itemList.count()) {
        delete itemList.takeAt(identifier);
        return true;
        } else {
          return false;
        }
    } else if(mode == BY_ITEM_NUMBER){
        const int index = itemColorIndex(identifier);
        if(index == -1)
          return false;
        delete itemList.takeAt(index);
        return true;
    }
    else return false;//unknow mode
}

ItemColors::ItemColor* ItemColors::itemColor(int itemId) const{

  for (int i = 0; i < itemList.size(); ++i) {
      if (itemList.at(i)->itemId == itemId)
        return itemList.at(i);
    }
  return 0;
}

int ItemColors::itemColorIndex(int itemId) const{
    //Iterate on the list until the item is find

  for (int i = 0; i < itemList.size(); ++i) {
      if (itemList.at(i)->itemId == itemId)
        return i;
  }
  return -1;
}

QList<int> ItemColors::colorChangedItemList(){
    QList<int> changedList;
    for (int i = 0; i < itemList.size(); ++i) {
        if (itemList.at(i)->isChanged)
          changedList << itemList.at(i)->itemId;
    }

    return changedList;
}

void ItemColors::resetAllColorStatus(){
    setColorChanged(false);

    for (int i = 0; i < itemList.size(); ++i) {
        if (itemList.at(i)->isChanged)
          itemList.at(i)->isChanged = false;
    }
}

void ItemColors::changeItemId(int index, int newItemId){
    ItemColors::ItemColor* theItemColor = itemList.at(static_cast<uint>(index));

    theItemColor->itemId = newItemId;
}

void ItemColors::changeItemLabel(int index, const QString &newItemLabel){
    ItemColors::ItemColor* theItemColor = itemList.at(static_cast<uint>(index));

    theItemColor->label = newItemLabel;
}



