/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of MIME data related functions
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "MimeDataUtils.h"


QString const kUuuidListMimeType = "application/x-beeftextuuidlist"; ///< the mime type for a serialized uuid list
QString const kGroupIndexMimeType = "application/x-beeftextgroupindex"; ///< the mime type for a serialized group index


//****************************************************************************************************************************************************
/// \param[in] uuids The list of UUID
/// \return The MIME data for 
//****************************************************************************************************************************************************
QMimeData *uuidListToMimeData(QList<QUuid> const &uuids) {
    QByteArray array;
    for (QUuid const &uuid: uuids)
        array.push_back(uuid.toRfc4122());
    QMimeData *result = new QMimeData;
    result->setData(kUuuidListMimeType, array);
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] mimeData the MIME data
//****************************************************************************************************************************************************
QList<QUuid> mimeDataToUuidList(QMimeData const &mimeData) {
    if (!mimeData.hasFormat(kUuuidListMimeType))
        return QList<QUuid>();
    QByteArray const array = mimeData.data(kUuuidListMimeType);
    qsizetype const arraySize = array.size();
    if ((0 == arraySize) || (0 != array.size() % 16))
        return QList<QUuid>();
    QList<QUuid> result;
    for (qsizetype i = 0; i < arraySize; i += 16)
        result.append(QUuid::fromRfc4122(array.mid(i, 16)));
    return result;
}


//****************************************************************************************************************************************************
/// \param[in] index The index.
/// \return A newly allocated QMimeData instance() containing the index of a group in a Beeftext specific MIME type
//****************************************************************************************************************************************************
QMimeData *groupIndexToMimeData(qint32 index) {
    QMimeData *data = new QMimeData();
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream << index;
    data->setData(kGroupIndexMimeType, array);
    return data;
}


//****************************************************************************************************************************************************
/// \param[in] mimeData The MIME data
/// \return The index contained in the MIME data
/// \return -1 if a problem occurs
//****************************************************************************************************************************************************
qint32 mimeDataToGroupIndex(QMimeData const &mimeData) {
    if (!mimeData.hasFormat(kGroupIndexMimeType))
        return -1;
    QByteArray array = mimeData.data(kGroupIndexMimeType);
    if (array.size() != sizeof(qint32))
        return -1;
    QDataStream stream(&array, QIODevice::ReadOnly);
    qint32 result;
    stream >> result;
    return result;
}



