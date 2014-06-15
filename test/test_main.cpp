#include "nwa_test.h"
#include "file_test.h"
#include "../src/FileHelper.h"

int main(int argc, char** argv)
{
   int status = 0;
   {
      FileHelper fh;
      TestFile testFile(fh);
      status |= QTest::qExec(&testFile, argc, argv);
   }
   {
      FileHelper fh;
      TestNWA testNWA(fh);
      status |= QTest::qExec(&testNWA, argc, argv);
   }
   return status;
}
