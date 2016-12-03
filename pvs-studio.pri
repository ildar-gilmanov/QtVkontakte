pvs_studio.target = pvs
pvs_studio.output = true
pvs_studio.cxxflags = -std=c++11
pvs_studio.sources = $${SOURCES}

SUPPRESS_WARNINGS=V122
QT_INCLUDE=$$absolute_path($$dirname(QMAKE_QMAKE)/../include)

PVS_CONFIG="\
exclude-path = /usr/include/\n\
exclude-path = /usr/lib64/\n\
exclude-path = /usr/lib/gcc/x86_64-linux-gnu/5/include/\n\
exclude-path = $$QT_INCLUDE/../mkspecs/linux-g++\n\
exclude-path = $$QT_INCLUDE\n\
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
                    && plog-converter -t $$pvs_studio.format -d \'$$SUPPRESS_WARNINGS\' -o \'$$pvs_studio.log\' \'$$pvs_studio.log\'.pvs.raw
    }
    !isEmpty(pvs_studio.output) {
        commands += && cat \'$$pvs_studio.log\' 1>&2
    }
    $${pvs_target}.commands = $${commands}
}

QMAKE_CLEAN += "$$pvs_studio.log"
QMAKE_EXTRA_TARGETS += "$${pvs_target}"
