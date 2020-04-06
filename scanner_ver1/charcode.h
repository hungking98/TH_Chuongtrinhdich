/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __CHARCODE_H__
#define __CHARCODE_H__

typedef enum {
  CHAR_SPACE,                  //dấu cách
  CHAR_LETTER,                 // chữ số và dấu _
  CHAR_DIGIT,                  //chữ số
  CHAR_PLUS,                   //+
  CHAR_MINUS,                   //-
  CHAR_TIMES,                   // nhân *
  CHAR_SLASH,                   // chia /
  CHAR_LT,                       // <
  CHAR_GT,                       // >
  CHAR_EXCLAIMATION,              //!
  CHAR_EQ,                        // <=
  CHAR_COMMA,                     // ,
  CHAR_PERIOD,                    // .
  CHAR_COLON,                     // :
  CHAR_SEMICOLON,                 // ;
  CHAR_SINGLEQUOTE,               // nháy đơn '
  CHAR_LPAR,                      // (
  CHAR_RPAR,                      // )
  CHAR_UNKNOWN                    // không nhận dạng
} CharCode;

#endif
