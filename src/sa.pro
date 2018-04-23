TEMPLATE = subdirs
SUBDIRS = \
          signALib\
          signAChart\
          signACommonUI\
          signACoreFun\
          signAPlugin/FunPlugin \
          signAPlugin/textImport \
          signAPlugin/DsfFileImport \
          SARibbonBar/SARibbon/src/SARibbonBar \
          signADataProc\
          signA


TRANSLATIONS+= \
            translations/zh.ts\#chinese
            translations/en.ts #English

