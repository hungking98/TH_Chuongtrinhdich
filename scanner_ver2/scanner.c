/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // thêm cái này cho đỡ cảnh báo cái hàm toupper
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"

extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

void skipBlank()
{
  // TODO
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_SPACE))
    readChar(); //ham readChar return currentChar
}

void skipComment()
{
  // TODO
  // TODO

  do
  {
    readChar();
  } while (currentChar != EOF && charCodes[currentChar] != CHAR_TIMES);

  if (currentChar == EOF)
  {
    error(ERR_ENDOFCOMMENT, lineNo, colNo);
  }
  readChar();
  if (charCodes[currentChar] == CHAR_RPAR)
  {
    readChar();
  }
  else
  {
    skipComment();
  }
}

void skipCommentLine()
{
  while (currentChar != '\n')
  {
    readChar();
  }
  readChar();
}

Token *readIdentKeyword(void) //return Type là Ident hay Keyword
{
  // TODO
  Token *token = makeToken(TK_NONE, lineNo, colNo);
  int count = 0;

  //token->string[count] = toupper((char)currentChar);
  //readChar();

  while ((currentChar != EOF) &&
         ((charCodes[currentChar] == CHAR_LETTER) || (charCodes[currentChar] == CHAR_DIGIT)))
  { //ta đọc nếu gặp toàn kí tự letter hoặc number
    if (count < MAX_IDENT_LEN)
      token->string[count++] = ((char)currentChar); //trong khuôn khổ  mong muốn nạp kí tự vào token.string
    readChar();
  }

  token->string[count] = '\0';                    //khi khong doc duoc ki tu chu hoac so thi ket thuc lap. Dua ra ident.
  token->tokenType = checkKeyword(token->string); //kiem tra xem token co phai la tu khoa Keyword khong
  if (token->tokenType == TK_NONE)                //neu token khong phai keyword  thi tra ve Ident thôi
    token->tokenType = TK_IDENT;
  return token;
}

Token *readNumber(void)
{
  // TODO
  Token *token = makeToken(TK_NUMBER, lineNo, colNo);
  int count = 0;

  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_DIGIT))
  {
    if (count <= MAX_NUMBER)
      token->string[count++] = (char)currentChar;
    readChar();
  }
  if (count > MAX_NUMBER)
  {
    error(ERR_NUMBERTOOLONG, token->lineNo, token->colNo);

    return token;
  }

  token->string[count] = '\0';
  token->value = atoi(token->string); //chuyen ki tu ve so nguyen
  return token;
}

Token *readConstChar(void)
{
  // TODO
  // TODO
  Token *token = makeToken(TK_CHAR, lineNo, colNo);

  readChar();
  if (currentChar == EOF)
  {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }

  if (currentChar == '\\')
  {
    readChar();
    if (currentChar == 'n')
    {
      token->string[0] = '\\';
      token->string[1] = 'n';
      token->string[2] = '\0';
      readChar();
    }
    else if (currentChar != 'n')
    {
      error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
      return token;
    }
  }
  if((charCodes[currentChar]==CHAR_LETTER)||charCodes[currentChar]==CHAR_DIGIT){
    token->string[0] =currentChar;
    token->string[1]= '\0';
    readChar();
  }

  if (charCodes[currentChar] == CHAR_SINGLEQUOTE)
  {
    readChar();
    return token;
  }
  else
  {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }
}

Token *readConstString(void){
  Token *token = makeToken(TK_CONSTSTRING, lineNo, colNo);
  int count = 0;
  readChar();
 
  while((currentChar!=EOF)&&(charCodes[currentChar]!=CHAR_NHAYKEP)){
    token->string[count++] = (char)currentChar;
    readChar();

  }
  token->string[count] ='\0';
                 

  if(currentChar==EOF){
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }

  if(charCodes[currentChar]==CHAR_NHAYKEP){
    readChar();
    
  }
   return token;
}


Token *getToken(void)
{
  Token *token;
  int ln, cn;

  if (currentChar == EOF)
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar])
  {
  case CHAR_PLUS: //+
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar();
    return token;
  case CHAR_MINUS: //-
    token = makeToken(SB_MINUS, lineNo, colNo);
    readChar();
    return token;
  case CHAR_TIMES: //*
    token = makeToken(SB_TIMES, lineNo, colNo);
    readChar();
    return token;
  case CHAR_SLASH: // ki tu '/'
    readChar();
    if (charCodes[currentChar] == CHAR_SLASH)
    {
      skipCommentLine();
      return getToken();
    }
    else
    {
      token = makeToken(SB_SLASH, lineNo, colNo);
      readChar();
    }
    return token;
  case CHAR_SPACE:
    skipBlank();
    return getToken(); // gap dau cach thi skip,de quy de thuc hien tiep
  case CHAR_LETTER:
    return readIdentKeyword(); //gặp kí tự alpha hoặc gạch dưới _
  case CHAR_DIGIT:
    return readNumber(); //gap number

  case CHAR_LT: //<
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
    { // doc them '=' tuc la token '<='
      readChar();
      return makeToken(SB_LE, ln, cn);
    }
    else if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_GT))
    {
      readChar();
      return makeToken(SB_NEQ, ln, cn);
    }
    else
      return makeToken(SB_LT, ln, cn);
  case CHAR_GT:
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
    { // >=
      readChar();
      return makeToken(SB_GE, ln, cn);
    }
    else
      return makeToken(SB_GT, ln, cn);
  case CHAR_EQ: // ki tu '='
    token = makeToken(SB_EQ, lineNo, colNo);
    readChar();
    return token;
  case CHAR_EXCLAIMATION: // ki tu '!'
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
    { // !=
      readChar();
      return makeToken(SB_NEQ, ln, cn);
    }
    else
    {
      token = makeToken(TK_NONE, ln, cn);
      error(ERR_INVALIDSYMBOL, ln, cn); //viec tao token '!=' khong thanh cong, thong bao loi. KLP khong cho '!' dung mot minh
      return token;
    }
  case CHAR_COMMA: // ','
    token = makeToken(SB_COMMA, lineNo, colNo);
    readChar();
    return token;
  case CHAR_PERIOD: // '.'
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_RPAR))
    {
      readChar();
      return makeToken(SB_RSEL, ln, cn);
    }
    else
      return makeToken(SB_PERIOD, ln, cn);
  case CHAR_SEMICOLON: //';'
    token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar();
    return token;
  case CHAR_COLON: //':'
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
    { //phep gan ':='
      readChar();
      return makeToken(SB_ASSIGN, ln, cn);
    }
    else
      return makeToken(SB_COLON, ln, cn);
  case CHAR_SINGLEQUOTE:
    return readConstChar(); //gap dau mo ngoac nhay thi dung ham readConstChar doc hang ki tu
  case CHAR_LPAR:           //gap mo ngoac don '('
    ln = lineNo;
    cn = colNo;
    readChar();

    if (currentChar == EOF) //sau khi gap '(', neu gap ki tu ket thuc file thi tra ve type=SB_LPAR
      return makeToken(SB_LPAR, ln, cn);

    switch (charCodes[currentChar])
    { //mot so truong hop nhu '(.' tra ve SB_LSEL,
    case CHAR_PERIOD:
      readChar();
      return makeToken(SB_LSEL, ln, cn);
    case CHAR_TIMES: // gap '(*', ta se dung ham skipComment, neu khong doc duoc cap '*)' thi bao loi
      readChar();
      skipComment();
      return getToken();
    default:
      return makeToken(SB_LPAR, ln, cn);
    }
  case CHAR_RPAR: //gap dong ngoac don ')'
    token = makeToken(SB_RPAR, lineNo, colNo);
    readChar();
    return token;
  case CHAR_VUONGTRAI:
    token = makeToken(SB_LSEL, lineNo, colNo);
    readChar();
    return token;
  case CHAR_VUONGPHAI:
    token = makeToken(SB_RSEL, lineNo, colNo);
    readChar();
    return token;
  case CHAR_NHAYKEP:
    return readConstString();
  default: //cac truong hop con lai thong bao khong tao duoc token
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar();
    return token;
  }
}

/******************************************************************/

void printToken(Token *token)
{

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType)
  {
  case TK_NONE:
    printf("TK_NONE\n");
    break;
  case TK_IDENT:
    printf("TK_IDENT(%s)\n", token->string);
    break;
  case TK_NUMBER:
    printf("TK_NUMBER(%s)\n", token->string);
    break;
  case TK_CHAR:
    printf("TK_CHAR(\'%s\')\n", token->string);
    break;
  case TK_EOF:
    printf("TK_EOF\n");
    break;
  case KW_PROGRAM:
    printf("KW_PROGRAM\n");
    break;
  case KW_CONST:
    printf("KW_CONST\n");
    break;
  case KW_TYPE:
    printf("KW_TYPE\n");
    break;
  case KW_VAR:
    printf("KW_VAR\n");
    break;
  case KW_INTEGER:
    printf("KW_INTEGER\n");
    break;
  case KW_CHAR:
    printf("KW_CHAR\n");
    break;
  case KW_ARRAY:
    printf("KW_ARRAY\n");
    break;
  case KW_OF:
    printf("KW_OF\n");
    break;
  case KW_FUNCTION:
    printf("KW_FUNCTION\n");
    break;
  case KW_PROCEDURE:
    printf("KW_PROCEDURE\n");
    break;
  case KW_BEGIN:
    printf("KW_BEGIN\n");
    break;
  case KW_END:
    printf("KW_END\n");
    break;
  case KW_CALL:
    printf("KW_CALL\n");
    break;
  case KW_IF:
    printf("KW_IF\n");
    break;
  case KW_THEN:
    printf("KW_THEN\n");
    break;
  case KW_ELSE:
    printf("KW_ELSE\n");
    break;
  case KW_WHILE:
    printf("KW_WHILE\n");
    break;
  case KW_DO:
    printf("KW_DO\n");
    break;
  case KW_FOR:
    printf("KW_FOR\n");
    break;
  case KW_TO:
    printf("KW_TO\n");
    break;

  case SB_SEMICOLON:
    printf("SB_SEMICOLON\n");
    break;
  case SB_COLON:
    printf("SB_COLON\n");
    break;
  case SB_PERIOD:
    printf("SB_PERIOD\n");
    break;
  case SB_COMMA:
    printf("SB_COMMA\n");
    break;
  case SB_ASSIGN:
    printf("SB_ASSIGN\n");
    break;
  case SB_EQ:
    printf("SB_EQ\n");
    break;
  case SB_NEQ:
    printf("SB_NEQ\n");
    break;
  case SB_LT:
    printf("SB_LT\n");
    break;
  case SB_LE:
    printf("SB_LE\n");
    break;
  case SB_GT:
    printf("SB_GT\n");
    break;
  case SB_GE:
    printf("SB_GE\n");
    break;
  case SB_PLUS:
    printf("SB_PLUS\n");
    break;
  case SB_MINUS:
    printf("SB_MINUS\n");
    break;
  case SB_TIMES:
    printf("SB_TIMES\n");
    break;
  case SB_SLASH:
    printf("SB_SLASH\n");
    break;
  case SB_LPAR:
    printf("SB_LPAR\n");
    break;
  case SB_RPAR:
    printf("SB_RPAR\n");
    break;
  case SB_LSEL:
    printf("SB_LSEL\n");
    break;
  case SB_RSEL:
    printf("SB_RSEL\n");
    break;
  case TK_CONSTSTRING:
    printf("TK_CONSTSTRING(\"%s\")\n",token->string);
    break;
  }
}

int scan(char *fileName)
{
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF)
  {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR)
  {
    printf("Can\'t read input file!\n");
    return -1;
  }

  return 0;
}
