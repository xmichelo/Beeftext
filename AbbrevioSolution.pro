TEMPLATE = subdirs

SUBDIRS += \
    XMiLib \
    Abbrevio

XMiLib.subdir = Submodules\XMiLib\XMiLib
Abbrevio.subdir = Abbrevio


Abbrevio.depends = XMiLib
