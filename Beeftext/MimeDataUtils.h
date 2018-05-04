/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of MIME data related functions
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__MIME__DATA__UTILS__H
#define BEEFTEXT__MIME__DATA__UTILS__H


extern QString const kUuuidListMimeType; ///< The mime type for a serialized uuid list
extern QString const kGroupIndexMimeType; ///< The mime type for a serialized group index


QMimeData* uuidListToMimeData(QList<QUuid> const& uuids); ///< Create a QMimeData instance() of a specific MIME type containing list of UUID
QList<QUuid> mimeDataToUuidList(QMimeData const& mimeData); ///< Parse a UUID list from a piece of MIME data 
QMimeData* groupIndexToMimeData(qint32 index); ///< Create a QMimeData instance() of a specific MIME type containing a group index
qint32 mimeDataToGroupIndex(QMimeData const& mimeData); ///< Parse a group index from a MIME data of time



#endif // #ifndef BEEFTEXT__MIME__DATA__UTILS__H