#include "ctest.h"
#include "msc.h"
#include "term.h"

CTEST (clear_screen, valid)
{
  int result = mt_clrscr ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (go_to_XY, valid)
{
  int result = mt_gotoXY (2, 26);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (get_screen_size, valid)
{
  int rows, cols;
  int result = mt_getscreensize (&rows, &cols);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (set_background, valid)
{
  int result = mt_setbgcolor (BLUE);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (set_foreground, valid)
{
  int result = mt_setfgcolor (BLUE);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (memory_init, valid)
{
  int result = sc_memoryInit ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (memory_get, valid)
{
  sc_memoryInit ();
  int val;
  int result = sc_memoryGet (0, &val);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (memory_set, valid)
{
  sc_memoryInit ();
  int result = sc_memorySet (0, 1);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (memory_save, valid)
{
  sc_memoryInit ();
  int result = sc_memorySave ("obj/example.mem");
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (memory_load, valid)
{
  sc_memoryInit ();
  int result = sc_memoryLoad ("obj/example.mem");
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (reg_init, valid)
{
  int result = sc_regInit ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (reg_get, valid)
{
  sc_regInit ();
  int val;
  int result = sc_regGet (FLAG_OVERFLOW, &val);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (reg_set, valid)
{
  sc_regInit ();
  int result = sc_regSet (FLAG_DIV_BY_ZERO, 1);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (accum_get, valid)
{
  int val;
  int result = sc_accumGet (&val);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (accum_set, valid)
{
  int result = sc_accumSet (5);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (count_get, valid)
{
  int val;
  int result = sc_countGet (&val);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (count_set, valid)
{
  int result = sc_countSet (5);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (cmd_encode, valid)
{
  int val;
  int result = sc_commandEncode (0x11, 12, &val);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (cmd_decode, valid)
{
  int cmd, operand;
  int result = sc_commandDecode (2188, &cmd, &operand);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}