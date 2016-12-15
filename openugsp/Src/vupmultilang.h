#ifndef _VUPMULTILANG_H_
#define _VUPMULTILANG_H_

typedef const char[60][2] VUPMULTISTRING;


VUPMULTISTRING VUP_MAIN_SUCCESS = "SUCCEEDED\n" , "ПОЛУЧИЛОСЬ\n";
VUPMULTISTRING VUP_MAIN_FAILED = "FAILED (Error:%u)\n" , "НЕ ВЫПОЛНЕННО(Ошибка:%u)\n";

VUPMULTISTRING VUP_MAIN_LOADINGIM = "Loading file %s into memory ...  " , "Загружаю файл %s в память ... ";
VUPMULTISTRING VUP_MAIN_ANALYZEFF = "Analyzing file format ... " , "Анализирую формат файла ... ");
VUPMULTISTRING VUP_MAIN_UNKNOWNFF = "Unknown file format. Operation halted\n" , "Неизвестный формат файла.Операция закончена\n";
VUPMULTISTRING VUP_MAIN_DESTWIN32 = "Destination Operating System: Microsoft Windows\n" , "Целевая ОС: Мастдай(т.е Винда)";
VUPMULTISTRING VUP_MAIN_DESTUNIX = "Destination Operating System: UNIX-Like Operating System\n" , "Целевая ОС: UNIX подобная операционная система";
VUPMULTISTRING VUP_MAIN_TRYINGTODETECTGT = "Trying to detect game type from binary ...\n" , "Пытаюсь обнаружить тип игры из двоичных данных файла ...\n";
VUPMULTISTRING VUP_MAIN_SAVINGCHANGES = "Saving changes to destination file ... " , "Сохраняю изменения в целевой файл ...";
VUPMULTISTRING VUP_MAIN_OPERATIONSCOMPLETED = "\nAll operations completed. Have fun!!!\n" , "\n Все операции завершены. Приятного вам администрирования ;)\n"

VUPMULTISTRING VUP_SHARED_FOUNDAT = "Found at" , "Найдено на";
VUPMULTISTRING VUP_SHARED_NOTFOUND = "Not found" , "Не найдено";
VUPMULTISTRING VUP_SHARED_PATCHING = "Patching ..." , "Модифицирую ...";
VUPMULTISTRING VUP_SHARED_DONE = "Done" , "Готово";
VUPMULTISTRING VUP_SHARED_FAILED = "Failed" , "Не выполненно";

VUPMULTISTRING VUP_ETQW_DETECTEDGT = "%s has been detected\n" , "Обнаружен %s\n";


VUPMULTISTRING VUP_VLV_DETECTEDGT = "%s\nHas been detected\n" , "Обнаружен:\n%s\n";

#endif
