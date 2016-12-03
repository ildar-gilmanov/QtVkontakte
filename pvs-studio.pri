pvs_studio.target = pvs
pvs_studio.output = true
pvs_studio.cxxflags = -std=c++11
pvs_studio.sources = $${SOURCES}

QT_INCLUDE=$$absolute_path($$dirname(QMAKE_QMAKE)/../include)

PVS_CONFIG="\
exclude-path = /usr/include/\n\
exclude-path = /usr/lib64/\n\
exclude-path = /usr/lib/gcc/x86_64-linux-gnu/5/include/\n\
\n\
exclude-path = $$QT_INCLUDE/../mkspecs/linux-g++\n\
exclude-path = $$QT_INCLUDE/Qt3DCore\n\
exclude-path = $$QT_INCLUDE/Qt3DExtras\n\
exclude-path = $$QT_INCLUDE/Qt3DInput\n\
exclude-path = $$QT_INCLUDE/Qt3DLogic\n\
exclude-path = $$QT_INCLUDE/Qt3DQuick\n\
exclude-path = $$QT_INCLUDE/Qt3DQuickExtras\n\
exclude-path = $$QT_INCLUDE/Qt3DQuickInput\n\
exclude-path = $$QT_INCLUDE/Qt3DQuickRender\n\
exclude-path = $$QT_INCLUDE/Qt3DRender\n\
exclude-path = $$QT_INCLUDE/QtBluetooth\n\
exclude-path = $$QT_INCLUDE/QtCharts\n\
exclude-path = $$QT_INCLUDE/QtCLucene\n\
exclude-path = $$QT_INCLUDE/QtConcurrent\n\
exclude-path = $$QT_INCLUDE/QtCore\n\
exclude-path = $$QT_INCLUDE/QtDataVisualization\n\
exclude-path = $$QT_INCLUDE/QtDBus\n\
exclude-path = $$QT_INCLUDE/QtDesigner\n\
exclude-path = $$QT_INCLUDE/QtDesignerComponents\n\
exclude-path = $$QT_INCLUDE/QtGamepad\n\
exclude-path = $$QT_INCLUDE/QtGui\n\
exclude-path = $$QT_INCLUDE/QtHelp\n\
exclude-path = $$QT_INCLUDE/QtLocation\n\
exclude-path = $$QT_INCLUDE/QtMultimedia\n\
exclude-path = $$QT_INCLUDE/QtMultimediaQuick_p\n\
exclude-path = $$QT_INCLUDE/QtMultimediaWidgets\n\
exclude-path = $$QT_INCLUDE/QtNetwork\n\
exclude-path = $$QT_INCLUDE/QtNfc\n\
exclude-path = $$QT_INCLUDE/QtOpenGL\n\
exclude-path = $$QT_INCLUDE/QtOpenGLExtensions\n\
exclude-path = $$QT_INCLUDE/QtPacketProtocol\n\
exclude-path = $$QT_INCLUDE/QtPlatformHeaders\n\
exclude-path = $$QT_INCLUDE/QtPlatformSupport\n\
exclude-path = $$QT_INCLUDE/QtPositioning\n\
exclude-path = $$QT_INCLUDE/QtPrintSupport\n\
exclude-path = $$QT_INCLUDE/QtPurchasing\n\
exclude-path = $$QT_INCLUDE/QtQml\n\
exclude-path = $$QT_INCLUDE/QtQmlDebug\n\
exclude-path = $$QT_INCLUDE/QtQmlDevTools\n\
exclude-path = $$QT_INCLUDE/QtQuick\n\
exclude-path = $$QT_INCLUDE/QtQuickControls2\n\
exclude-path = $$QT_INCLUDE/QtQuickParticles\n\
exclude-path = $$QT_INCLUDE/QtQuickTemplates2\n\
exclude-path = $$QT_INCLUDE/QtQuickTest\n\
exclude-path = $$QT_INCLUDE/QtQuickWidgets\n\
exclude-path = $$QT_INCLUDE/QtScxml\n\
exclude-path = $$QT_INCLUDE/QtSensors\n\
exclude-path = $$QT_INCLUDE/QtSerialBus\n\
exclude-path = $$QT_INCLUDE/QtSerialPort\n\
exclude-path = $$QT_INCLUDE/QtSql\n\
exclude-path = $$QT_INCLUDE/QtSvg\n\
exclude-path = $$QT_INCLUDE/QtTest\n\
exclude-path = $$QT_INCLUDE/QtUiPlugin\n\
exclude-path = $$QT_INCLUDE/QtUiTools\n\
exclude-path = $$QT_INCLUDE/QtWebChannel\n\
exclude-path = $$QT_INCLUDE/QtWebEngine\n\
exclude-path = $$QT_INCLUDE/QtWebEngineCore\n\
exclude-path = $$QT_INCLUDE/QtWebEngineWidgets\n\
exclude-path = $$QT_INCLUDE/QtWebSockets\n\
exclude-path = $$QT_INCLUDE/QtWebView\n\
exclude-path = $$QT_INCLUDE/QtWidgets\n\
exclude-path = $$QT_INCLUDE/QtX11Extras\n\
exclude-path = $$QT_INCLUDE/QtXml\n\
exclude-path = $$QT_INCLUDE/QtXmlPatterns\n\
"

defineReplace(pvs_studio_filter_sources) {
    variable = $$1
    in = $$eval($$variable)
    out =
    for(source, in) {
        contains(source, "^.+\\.(cpp|cpp|cc|cx|cxx|cp|c\\+\\+)$") {
            out += "$${source}"
        }
        contains(source, "^.+\\.c$") {
            out += "$${source}"
        }
    }
    return($${out})
}

isEmpty(pvs_studio.target) {
    pvs_studio.target = pvs_studio
}
isEmpty(pvs_studio.log) {
    contains(pvs_studio.format, "^tasklist$") {
        pvs_studio.log = "$$pvs_studio.target".tasks
    } else {
        pvs_studio.log = "$$pvs_studio.target".log
    }
}
isEmpty(pvs_studio.cfg) {
    pvs_studio.cfg = "pvs-studio.cfg"
}

pvs_sources = $$pvs_studio.sources
pvs_sources = $$pvs_studio_filter_sources(pvs_sources)
pvs_plogs =
pvs_targets =
for(source, pvs_sources) {
    contains(source, "^/.*$") {
        relpath = $$relative_path("$$source", "$$_PRO_FILE_PWD_")
        !contains(relpath, "^../") {
            source = "$$relpath"
        }
    }
    dir = $$dirname(source)
    isEmpty(dir) {
        dir = "."
    }
    args = $$pvs_studio.args
    dir = $$clean_path("$$OUT_PWD/PVS-Studio/$${dir}")
    log = $$clean_path("$${dir}/$${source}.plog")
    dir = $$dirname(log)
    !contains(source, "^/.*$") {
        source = "$$_PRO_FILE_PWD_/$${source}"
    }
    source = $$clean_path("$$source")
    contains(source, "^.+\\.(cpp|cpp|cc|cx|cxx|cp|c\\+\\+)$") {
        lang_flags=$$pvs_studio.cxxflags $(CXXFLAGS) $(DEFINES) -DPVS_STUDIO
    }
    contains(source, "^.+\\.c$") {
        lang_flags=$$pvs_studio.cflags $(CFLAGS) $(DEFINES) -DPVS_STUDIO
    }
    !isEmpty(pvs_studio.license) {
        args += --lic-file \'$$pvs_studio.license\'
    }
    args += --cfg \'$$pvs_studio.cfg\'
    args += --output-file \'$${log}\'
    args += --source-file \'$${source}\'
    args += --platform linux64
    args += --preprocessor gcc
    args += --cl-params $${lang_flags} $(INCPATH)
    target = $${source}_pvs
    $${target}.target = "$$log"
    $${target}.commands = echo \"$$PVS_CONFIG\" > \'$$pvs_studio.cfg\' && \
                          mkdir -p \'$${dir}\' && \
                          rm -f \'$${log}\' && \
                          pvs-studio $${args} \'$${source}\'
    $${target}.depends = "$${source}"
    pvs_plogs += "$${log}"
    pvs_targets += $${target}
    QMAKE_CLEAN += "$${log}"
    QMAKE_EXTRA_TARGETS += "$${target}"
}

pvs_target = $$pvs_studio.target
$${pvs_target}.depends = $${pvs_targets}
isEmpty(pvs_plogs) {
    $${pvs_target}.commands = echo \"\" > "$$pvs_studio.log"
} else {
    commands = cat $${pvs_plogs} > "$$pvs_studio.log"
    !isEmpty(pvs_studio.output) {
        isEmpty(pvs_studio.format) {
            pvs_studio.format = errorfile
        }
    }
    !isEmpty(pvs_studio.format) {
        commands += && mv \'$$pvs_studio.log\' \'$$pvs_studio.log\'.pvs.raw \
                    && plog-converter -t $$pvs_studio.format -o \'$$pvs_studio.log\' \'$$pvs_studio.log\'.pvs.raw
    }
    !isEmpty(pvs_studio.output) {
        commands += && cat \'$$pvs_studio.log\' 1>&2
    }
    $${pvs_target}.commands = $${commands}
}

QMAKE_CLEAN += "$$pvs_studio.log"
QMAKE_EXTRA_TARGETS += "$${pvs_target}"
