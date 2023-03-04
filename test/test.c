#include "bc.h"
#include "ctest.h"
#include "msc.h"
#include "readkey.h"
#include "term.h"
#include "tui.h"

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

CTEST (print_A, valid)
{
  int result = bc_printA ("l");
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (bc_box, valid)
{
  int result = bc_box (6, 35, 7, 4);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (print_big_char, valid)
{
  int digit[2] = { 0x4242423C, 0x3C424242 };
  int result = bc_printbigchar (digit, 14, 60, GREEN, LIGHT_GREY);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (set_bc_pos, valid)
{
  int digit[2] = { 0x4242423C, 0x3C424242 };
  int result = bc_setbigcharpos (digit, 2, 6, 1);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (get_bc_pos, valid)
{
  short int val;
  int digit[2] = { 0x4242423C, 0x3C424242 };
  int result = bc_getbigcharpos (digit, 2, 6, &val);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (bc_write, valid)
{
  int big[18][2] = {
    { 0x4242423C, 0x3C424242 }, // 0
    { 0x48506040, 0x40404040 }, // 1
    { 0x2042423C, 0x7E020418 }, // 2
    { 0x3840423C, 0x3C424040 }, // 3
    { 0x7E424478, 0x40404040 }, // 4
    { 0x3E02027E, 0x3C424040 }, // 5
    { 0x3E02423C, 0x3C424242 }, // 6
    { 0x2040407E, 0x10101010 }, // 7
    { 0x3C42423C, 0x3C424242 }, // 8
    { 0x7C42423C, 0x3C424040 }, // 9
    { 0x66663C18, 0x66667E7E }, // A
    { 0x3E66663E, 0x3E666666 }, // B
    { 0x0202423C, 0x3C420202 }, // C
    { 0x4444443E, 0x3E444444 }, // D
    { 0x3E02027E, 0x7E020202 }, // E
    { 0x1E02027E, 0x02020202 }, // F
    { 0x7E181800, 0x0018187E }, // +
    { 0x00181800, 0x00181800 }  // :
  };
  int file = open ("obj/digit", O_RDWR | O_CREAT);
  int result = bc_bigcharwrite (file, big, 18);
  close (file);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (bc_read, valid)
{
  int count, big[36];
  int file = open ("obj/digit", O_RDONLY);
  int result = bc_bigcharread (file, big, 18, &count);
  close (file);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (tui_static, valid)
{
  mt_clrscr ();
  int result = g_static ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (tui_memorybox, valid)
{
  int result = g_memorybox ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (tui_accumbox, valid)
{
  int result = g_accumbox ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (tui_counterbox, valid)
{
  int result = g_counterbox ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (tui_operationbox, valid)
{
  int result = g_operationbox ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (tui_flagbox, valid)
{
  int result = g_flagbox ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (tui_bcbox, valid)
{
  int big[18][2] = {
    { 0x4242423C, 0x3C424242 }, // 0
    { 0x48506040, 0x40404040 }, // 1
    { 0x2042423C, 0x7E020418 }, // 2
    { 0x3840423C, 0x3C424040 }, // 3
    { 0x7E424478, 0x40404040 }, // 4
    { 0x3E02027E, 0x3C424040 }, // 5
    { 0x3E02423C, 0x3C424242 }, // 6
    { 0x2040407E, 0x10101010 }, // 7
    { 0x3C42423C, 0x3C424242 }, // 8
    { 0x7C42423C, 0x3C424040 }, // 9
    { 0x66663C18, 0x66667E7E }, // A
    { 0x3E66663E, 0x3E666666 }, // B
    { 0x0202423C, 0x3C420202 }, // C
    { 0x4444443E, 0x3E444444 }, // D
    { 0x3E02027E, 0x7E020202 }, // E
    { 0x1E02027E, 0x02020202 }, // F
    { 0x7E181800, 0x0018187E }, // +
    { 0x00181800, 0x00181800 }  // :
  };
  int result = g_bcbox (big);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (readkey, valid)
{
  enum keys key = KEY_DEFAULT;
  int result = rk_readkey (&key);
  printf("| key is %d ", key);
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (save_terminal, valid)
{
  int result = rk_mytermsave ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (restore_terminal, valid)
{
  int result = rk_mytermrestore ();
  int expected = 0;
  ASSERT_EQUAL (expected, result);
}

CTEST (terminal_regime, valid)
{
  int result = rk_mytermregime (0, 0, 1, 0, 1);
  int expected = 0;
  rk_mytermrestore ();
  ASSERT_EQUAL (expected, result);
}