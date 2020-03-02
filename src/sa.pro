TEMPLATE = subdirs
SUBDIRS = \
          signAProtocol\
          signALib\
          signAScience\
          signAServe\
          signAProcess\
          signAChart\
          signACommonUI\
          signACoreFun\
          signAPlugin/FunPlugin \
          signAPlugin/TextImport \
          signAPlugin/DsfFileImport \
          SARibbonBar/SARibbon/src/SARibbonBar \
          signADataProc\
          signA


TRANSLATIONS+= \
            translations/zh.ts\#chinese
            translations/en.ts #English

OTHER_FILES += \
    readme.md\
    schedule.md\
    $$PWD/../doc/dev_records.md
