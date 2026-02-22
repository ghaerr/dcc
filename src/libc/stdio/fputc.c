#include <stdio.h>

int
fputc(int ch, FILE *fp)
{
   register int v;

   /* If last op was a read ... note fflush may change fp->mode and ret OK */
   if ((fp->mode & __MODE_READING) && fflush(fp))
      return EOF;

   v = fp->mode;
   /* Can't write if there's been an EOF or error then return EOF */
   if ((v & (__MODE_WRITE | __MODE_EOF | __MODE_ERR)) != __MODE_WRITE)
      return EOF;

   /* Buffer is full */
   if (fp->bufpos >= fp->bufend && fflush(fp))
      return EOF;

   /* Right! Do it! */
   *(fp->bufpos++) = ch;
   fp->mode |= __MODE_WRITING;

   /* Unbuffered or Line buffered and end of line */
   if (((ch == '\n' && (v & _IOLBF)) || (v & _IONBF)) && fflush(fp))
      return EOF;

   /* Can the macro handle this by itself ? */
   if (v & (__MODE_IOTRAN | _IOLBF | _IONBF))
      fp->bufwrite = fp->bufstart;	/* Nope */
   else
      fp->bufwrite = fp->bufend;	/* Yup */

   /* Correct return val */
   return (unsigned char) ch;
}
