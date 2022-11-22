/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo group class
///
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT_GROUP_H
#define BEEFTEXT_GROUP_H


class Group;


typedef std::shared_ptr<Group> SpGroup; ///< Type definition for shared pointer to SPComboGroup
typedef std::vector<SpGroup> VecSpGroup; ///< Type definition for vector of SPComboGroup


//****************************************************************************************************************************************************
/// \brief Combo group class
//****************************************************************************************************************************************************
class Group {
public: // member functions
    explicit Group(QString name, QString description = QString()); ///< Default constructor
    Group(QJsonObject const &object, qint32 formatVersion); ///< Constructor from JSON object
    Group(Group const &) = delete; ///< Disabled copy-constructor
    Group(Group &&) = delete; ///< Disabled assignment copy-constructor
    ~Group() = default; ///< Destructor
    Group &operator=(Group const &) = delete; ///< Disabled assignment operator
    Group &operator=(Group &&) = delete; ///< Disabled move assignment operator
    bool isValid() const; ///< Check if the group is valid
    QUuid uuid() const; ///< Get the UUID of the group
    QString name() const; ///< Get the name of the group
    void setName(QString const &name); ///< Set the name of the group
    QString description() const; ///< Get the description of the group
    void setDescription(QString const &description); ///< Set the description of the group
    bool enabled() const; ///< Set the enabled/disabled state of the group.
    void setEnabled(bool enable); ///< Get the enabled/disabled state of the group.
    QJsonObject toJsonObject() const; ///< Serialize the group in a JSon object

public: // static functions
    static SpGroup create(QString const &name, QString const &description = QString()); ///< Create a SpGroup
    static SpGroup create(QJsonObject const &object, qint32 formatVersion); ///< Create a SpGroup from a JSON object

private: // member functions
    void touch(); ///< Set the modification date/time to the current date/time

private: // data members
    QUuid uuid_; ///< The UUID for the group
    QString name_; ///< The name of the group
    QString description_; ///< The description of the group
    QDateTime creationDateTime_; ///< The creation date/time of the group
    QDateTime modificationDateTime_; ///< The last modification date/time of the group
    bool enabled_ { true }; ///< Is the group enabled.
};


Q_DECLARE_METATYPE(SpGroup)


#endif // #ifndef BEEFTEXT_GROUP_H