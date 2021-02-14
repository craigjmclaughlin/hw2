
/*  This program is a modified version of the program "image.c" 
 *  appearing in _The OpenGL Programming Guide_ (red book).
 *  It demonstrates drawing pixels and shows the effect
 *  of glDrawPixels() and glCopyPixels().
 *  Interaction: moving the mouse while pressing the mouse button
 *  will copy the image in the lower-left corner of the window
 *  to the mouse position.
 *  There is no attempt to prevent you from drawing over the original
 *  image.
 *
 *  IMPORTANT: The window with the image must be the active window, i.e.,
 *  it must be clicked or the mouse must be over it, for input to occur.
 *  The result will be shown in the window you started the program from, 
 *  but don't make the mistake of typing in this window: it won't work.
 *
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "CLI.h"

/*	Create checkerboard image	*/
#define	checkImageWidth 1024
#define	checkImageHeight 1024
GLubyte checkImage[checkImageHeight][checkImageWidth][3];

static GLint height;

void 
makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
      }
   }
}

/*
 * Function name: init
 * Description:   Operations that only need be performed once for rendering
 *                are done here.
 * Arguments:	  none
 * Globals:       none
 * Returns:	  void
 */
void 
init(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   makeCheckImage();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*
 * Function name: display
 * Description:   The glut display callback, operations to render and 
 *                re-render are here.
 * Arguments:	  none
 * Globals:       none
 * Returns:	  void
 */
void 
display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glRasterPos2i(0, 0);
   glDrawPixels(checkImageWidth, checkImageHeight, GL_RGB, 
                GL_UNSIGNED_BYTE, checkImage);
   glFlush();
}

/*
 * Function name: reshape
 * Description:   glut reshape callback, what actions to take when the 
 *                window is reshaped.
 * Arguments:	  w - the new width
 *                h - the new height
 * Globals:       none
 * Returns:	  void
 */
void 
reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   height = (GLint) h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/*
 * Function name: motion
 * Description:   glut mouse motion callback, copies the image.
 * Arguments:	  x - x position of mouse
 *                y - y positions of mouse
 * Globals:	  none
 * Returns:	  void
 */
void 
motion(int x, int y)
{
   static GLint screeny;
   
   screeny = height - (GLint) y;
   glRasterPos2i (x, screeny);
   glCopyPixels (0, 0, checkImageWidth, checkImageHeight, GL_COLOR);
   glFlush ();
}

/*
 * Function name: mouse
 * Description:   glut mouse callback, draws lines on the image.
                  First left mouse click puts red dot at mouse location
                  Subsequent right mouse clicks draw a red line from that
                  location to current mouse location. Second left mouse
                  click resets routine and redraws the screen.
 * Arguments:	  button - which button (left, right, middle) is being pressed
                  press  - is it down?
                  x - x position of mouse
 *                y - y position of mouse (note this is from upper left corner)
 * Globals:	  none
 * Returns:	  void
 */
void
mouse(int button, int press, int x, int y)
{
  static int state, start_x, start_y;

  GLint viewport[4];
  int height;

  printf("Mouse: %d %d\n", x, y);
  switch(button)
  {
      case GLUT_LEFT_BUTTON:
	if(press == GLUT_DOWN)
        {
	   if(!state)    
           {
	     glGetIntegerv(GL_VIEWPORT, viewport);
	     height = abs(viewport[3]-viewport[1]);
             glColor3f(1.0, 0.0, 0.0);
             glBegin(GL_POINTS);
             glVertex2i(x, height - y);
	     glEnd();
	     glFlush();
             state = 1;
             start_x = x;
	     start_y = y;
	   }
	   else
           {
	     state = 0;
	     display();
	   }
	}
	break;
     case GLUT_RIGHT_BUTTON:
	if (press == GLUT_DOWN)
        {
	  glGetIntegerv(GL_VIEWPORT, viewport);
	  height = abs(viewport[3]-viewport[1]);
	  glColor3f(1.0, 0.0, 0.0);
	  glBegin(GL_LINES);
	  glVertex2i(start_x, height - start_y);
	  glVertex2i(x, height - y);
	  glEnd();
	  glFlush();
	}
	break;
  }
}

/*
 * Function name: main_loop
 * Description:   Process the line of input
 * Arguments:	  line - the line of input
 * Globals:       none
 * Returns:	  void
 */
void
main_loop(char line[])
{
   /* PUT YOUR CLI CODE HERE! */
   std::stack<char*> emptyStack;
   CLI::CLI_ProcessLine(line, emptyStack);

   std::cout << "Main loop complete.\n";

}

#define MAXLINE 1024

/*  ASCII Values for return, backspace, delete, escape and ctrl-d */
#define CR   13
#define BS    8
#define DEL 127
#define ESC  27
#define EOT   4

/*
 * Function name: read_next_command
 * Description:   The glut callback function which is called when a key
 *                is pressed. ESC redraws the screen, ctrl-d exits.
 * Arguments:	  key - The ASCII value of the key pressed.
 *                x   - x position of the mouse (window-relative)
 *                y   - y position of the mouse (window-relative)
 * Globals:       none
 * Returns:	  void
 */
void
read_next_command(unsigned char key, int x, int y)
{
   static char line[MAXLINE];
   static int count;
  
   if(count >= MAXLINE - 1)
   {
      printf("Error: Maximum line length exceeded. Discarded input.\n");
      count = 0;
      return;
   }

   putchar(key);
   fflush(stdout);  /* Immediate output (gratification) */
   
   if (key != CR && key != BS && key != DEL && key != ESC && key != EOT)
   {
      line[count]=key;
      count++;
   }
   else if (key == BS || key == DEL)
   {
      if(count == 0)
         return;
      
      fflush(stdout);
      count--;
   }
   else if (key == CR)
   {
      printf("\n");
      line[count] = '\0';
      count = 0;
      main_loop(line);
   }
   else if (key == ESC)
   {
      glutPostRedisplay();
   }
   else
   {
      printf("Exiting...\n");
      exit(0);
   }
}

/*
 * Function name: main
 * Description:   Set up GL window and render
 * Arguments:	  command line arguments.
 * Globals:       none
 * Returns:	  void
 */
int 
main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(250, 250);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(read_next_command);
   glutMouseFunc(mouse);
   //   glutMotionFunc(motion);


    std::stack<char*> empty;
    std::cout << printf("CLI> ");
    char input[MAXLINE];
    std::cin.getline(input, MAXLINE);
    main_loop(input);

   glutMainLoop();
   return 0; 
}
