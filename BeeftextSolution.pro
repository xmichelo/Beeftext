TEMPLATE = subdirs

SUBDIRS += \
    XMiLib \
    Beeftext

XMiLib.subdir = Submodules\XMiLib\XMiLib
Beeftext.subdir = Beeftext


Beeftext.depends = XMiLib
