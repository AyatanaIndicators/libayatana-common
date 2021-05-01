# libayatana-common - Common functions for Ayatana System Indicators  [![Build Status](https://travis-ci.com/AyatanaIndicators/libayatana-common.svg)](https://travis-ci.com/AyatanaIndicators/libayatana-common)

## About Ayatana Indicators

The Ayatana Indicators project is the continuation of Application
Indicators and System Indicators, two technologies developed by Canonical
Ltd. for the Unity7 desktop.

Application Indicators are a GTK implementation of the StatusNotifierItem
Specification (SNI) that was originally submitted to freedesktop.org by
KDE.

System Indicators are an extensions to the Application Indicators idea.
System Indicators allow for far more widgets to be displayed in the
indicator's menu.

The Ayatana Indicators project is the new upstream for application
indicators, system indicators and associated projects with a focus on
making Ayatana Indicators a desktop agnostic technology.

On GNU/Linux, Ayatana Indicators are currently available for desktop
envinronments like MATE (used by default in [Ubuntu
MATE](https://ubuntu-mate.com)), XFCE (used by default in
[Xubuntu](https://bluesabre.org/2021/02/25/xubuntu-21-04-progress-update/),
LXDE, and the Budgie Desktop.

The Lomiri Operating Environment (UI of the Ubuntu Touch OS, formerly
known as Unity8) uses Ayatana Indicators for rendering its notification
area and the [UBports](https://ubports.com) project is a core contributor
to the Ayatana Indicators project.

For further info, please visit:
https://ayatana-indicators.org

## About this Software Component

This shared library contains functions that two or more system indicator
components share.

This shared library was originally created to reduce code duplication
across various system indicator components.

## License and Copyright

See COPYING and AUTHORS file in this project.
