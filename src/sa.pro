TEMPLATE = subdirs
SUBDIRS = \
          signALib\
          signAProtocol\
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

