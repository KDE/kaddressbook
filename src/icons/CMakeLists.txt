########### install files ###############
file(GLOB icons "*-apps-kaddressbook.*")
ecm_install_icons(
    ICONS ${icons}
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)
